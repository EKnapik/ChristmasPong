#version 120
// Author: Eric M. Knapik
// github: eknapik

varying vec2 fragCoord;

float fbm(vec2 p) {
    float ql = length( p );
    p.x += 0.05*sin(.81*iGlobalTime+ql*2.0);
    p.y += 0.05*sin(1.53*iGlobalTime+ql*6.0);
    
    float total = 0.0;
    float freq = 0.0152250;
    float lacunarity = 2.51;
    float gain = 0.15;
    float amp = gain;
    
    for(int i = 0; i < 5; i++) {
        total += texture2D(iChannel0, p*freq).r*amp;
        freq *= lacunarity;
        amp *= gain;
    }
    
    return total;
}

float fbm3(vec3 p) {
    float ql = length( p );
    
    float total = 0.0;
    float freq = .02250;
    float lacunarity = 0.151;
    float gain = 0.15;
    float amp = gain;
    
    for(int i = 0; i < 5; i++) {
        total += texture2D(iChannel1, p.xy*freq).r*amp;
        freq *= lacunarity;
        amp *= gain;
    }
    
    return total;
}

//-----------OBJECT OPERATIONS-----------
vec2 shapeMin(vec2 shape1, vec2 shape2) {
    return (shape1.x < shape2.x) ? shape1 : shape2;
}
float length2( vec2 p )
{
    return sqrt( p.x*p.x + p.y*p.y );
}

float length8( vec2 p )
{
    p = p*p; p = p*p; p = p*p;
    return pow( p.x + p.y, 1.0/8.0 );
}
vec3 opTx( vec3 p, mat4 m )
{
    vec4 q = m*vec4(p,1.0);
    return q.xyz;
}
//----------END OBJECT OPERATIONS----------

//----------DISTNACE FUNCTIONS----------
float sdPlane(vec3 p) {
    return p.y;
}

float sdCylinder( vec3 p, vec2 h ) {
  vec2 d = abs(vec2(length(p.xz),p.y)) - h;
  return min(max(d.x,d.y),0.0) + length(max(d,0.0));
}

float distSphere(vec3 pos, float r) {
    return length(pos)-r;
}

//---------END DISTANCE FUNCTIONS--------


//-----------OBJ MAP AND RAYMARCH-------------
vec2 map(vec3 pos) {
    vec2 shape; // the distance to this shape and the shape id
                // distance to shape is x, shape id is y
    shape = vec2(sdPlane(pos), 1.0);
    shape = shapeMin(shape, vec2(distSphere(pos, 1.0), 1.0));
    return shape;
}

vec2 rayMarch(in vec3 rayOrigin, in vec3 rayDir) {
    float tmin = 0.0;
    float tmax = 60.0;
    
    float t = tmin;
    float precis = 0.0002;
    float material = -1.0;
    
    // for more accuracy increase the amount of checks in the for loop
    for(int i = 0; i < 60; i++) {
        vec2 shapeObj = map(rayOrigin + t*rayDir);
        float dist = shapeObj.x;
        if(dist < precis || t > tmax) {
            break;
        }
        t += dist;
        material = shapeObj.y;
    }
    
    if( t>tmax ) {
        material = -1.0; // didn't hit anything so background;
    }
    return vec2( t, material ); // return distance and material hit for this ray
}


// ----- LIGHTING --------
// Inigo Quilez's soft shadow
float softshadow(vec3 rayOrigin, vec3 rayDir, float mint, float maxt) {
    float k = 8.0; // how soft the shadow is (a constant)
    float res = 1.0;
    float t = mint;
    for(int i=0; i<10; i++) {
        float h = map(rayOrigin + t*rayDir).x;
        res = min(res, k*h/t);
        t += h; // can clamp how much t increases by for more precision
        if( h < 0.001 ) {
            break;
        }
    }
    return clamp(res, 0.0, 1.0);
}

// Inigo Quilez's fast normal adjusted slightly becuase objects were shading themselves
vec3 calcNormal(vec3 pos) {
    vec3 epsilon = vec3(0.023, 0.0, 0.0);
    vec3 nor = vec3(
        map(pos+epsilon.xyy).x - map(pos-epsilon.xyy).x,
        map(pos+epsilon.yxy).x - map(pos-epsilon.yxy).x,
        map(pos+epsilon.yyx).x - map(pos-epsilon.yyx).x);
    return normalize(nor);
}


// --- COMBINE EVERYTHING TO GET PIXEL COLOR
vec3 calColor(vec3 rayOrigin, vec3 rayDir) {
    vec3 matCol; // material color
    // finds the t of intersect and what is it intersected with
    vec2 result = rayMarch(rayOrigin, rayDir);
    float t = result.x;
    
    vec3 pos = rayOrigin + t*rayDir;
    vec3 nor = calcNormal( pos );
    vec3 reflectEye = reflect(normalize(-rayDir), nor); // rayDir is the eye to position
    vec3 posToLight; // define vector that is dependant per light
    float ambCoeff = 0.1;
    float shadow, attenuation, spotCos, spotCoeff = 0.0;           // how much in the light
    float diff, spec;
    
    // define spotlight illuminating moon and then the moonlight
    
    bool background = false;
    // set the material coefficients
    if(result.y > 0.5 && result.y < 1.5) { //plane
        matCol = vec3(0.20,0.35,0.55);
        float fo=pow(0.023*result.x, 1.1);
            matCol=mix(matCol,vec3(0.91,0.88,0.98),fo);
    } else if(result.y > 1.5 && result.y < 2.5) { // sphere
        matCol = vec3(0.8);
    } else {                                      // background
        background = true;
    }
    
    // calculate light addition per spotlight
    // Bidirectional reflectance distribution function
    vec3 brdf = vec3(0.0);

    // set the brackground
    if(background) {
        brdf = 0.2*vec3(0.0, 0.2, 0.45);
        result.x = 20.0;
    }
    
    return vec3(clamp(brdf, 0.0, 1.0));  
}

// CAMERA SETTING
mat3 mkCamMat(in vec3 rayOrigin, in vec3 lookAtPoint, float roll) {
    vec3 cw = normalize(lookAtPoint - rayOrigin);
    vec3 cp = vec3(sin(roll), cos(roll), 0.0); //this is a temp right vec for cross determination
    vec3 cu = normalize(cross(cw, cp));
    vec3 cv = normalize(cross(cu, cw));

    return mat3(cu, cv, cw);
}


void mainImage() {
    vec2 iResolution = vec2(512.0, 512.0);
    vec2 q = fragCoord.xy / iResolution.xy;
    vec2 p = -1.0 + 2.0*q;
    p.x *= iResolution.x / iResolution.y;
    
    // camera or eye (where rays start)
    vec3 rayOrigin = vec3(0.0, 1.5, 0.0);
    vec3 lookAtPoint = vec3(0.0, -1.0, 0.0);
    float focalLen = 1.0; // how far camera is from image plane
    mat3 camMat = mkCamMat(rayOrigin, lookAtPoint, 0.0);

    // ray direction into image plane
    vec3 rayDir = camMat * normalize(vec3(p.xy, focalLen));
    
    //render the scene with ray marching
    vec3 col = calColor(rayOrigin, rayDir);

    gl_FragColor = vec4(col, 1.0); 
    //fragColor = vec4(.9); // that off white
}

