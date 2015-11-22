#version 120
// Author: Eric M. Knapik
// github: eknapik

varying vec2 fragCoord;
uniform vec2 ballPos;
uniform vec2 player;
uniform vec2 comp;

vec2 ballPosT;


struct Sphere {
    float radius;
    vec3 pos;
};

// uses two points to define the capsule
struct Capsule {
    float radius;
    vec3 a;
    vec3 b;
};

// ----- DISTANCE FUNCTIONS -----
float distPlane(vec3 pos) {
    return pos.y;
}

float distSphere(vec3 pos, Sphere sphere) {
    return length(pos-sphere.pos) - sphere.radius;
}

float distCapsule(vec3 pos, Capsule capsule) {
    vec3 pa = pos-capsule.a, ba = capsule.b-capsule.a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h ) - capsule.radius;
}

// ---- OBJECT FUNCTIONS --------
vec2 objMin(vec2 obj1, vec2 obj2) {
    return (obj1.x < obj2.x) ? obj1 : obj2;
}


// map of the scene 
vec2 map(vec3 pos) {

    Sphere sphere = Sphere(0.15, vec3(ballPosT, 0.0));
    Capsule capsule = Capsule(0.15, vec3(-1.5, 0.5, 0.0), vec3(-1.5, 2.5, 0.0));
    vec2 result = objMin(vec2(distPlane(pos-vec3(0.0, -20.0, 0.0)), 1.0), vec2(distSphere(pos, sphere), 2.0));
    result = objMin(result, vec2(distCapsule(pos, capsule), 3.0));
    return result;
}

// raymarch
vec2 rayMarch(vec3 rayOr, vec3 rayDir) {
    float tMin = 0.0;
    float tMax = 30.0;
    
    float t = tMin;
    float precis = 0.0002;
    float material = -1.0;
    float dist;
    
    for(int i = 0; i < 100; i++) {
        vec2 obj = map(rayOr + rayDir*t);
        dist = obj.x; // distance to nearest object
        
        if(dist < precis || t > tMax) {
            break;
        }
        t += dist;
        material = obj.y;
    }
    
    if(t > tMax) {
        material = -1.0;
    }
    return vec2(t, material);
}


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

// fast normal
vec3 calcNormal(vec3 pos) {
    vec3 epsilon = vec3(0.003, 0.0, 0.0);
    vec3 nor = vec3(
        map(pos+epsilon.xyy).x - map(pos-epsilon.xyy).x,
        map(pos+epsilon.yxy).x - map(pos-epsilon.yxy).x,
        map(pos+epsilon.yyx).x - map(pos-epsilon.yyx).x);
    return normalize(nor);
}



vec3 calColor(vec3 rayOr, vec3 rayDir) {
    vec2 obj = rayMarch(rayOr, rayDir);
    
    vec3 pos = rayOr + rayDir*obj.x;
    vec3 nor = calcNormal( pos );
    vec3 reflectEye = reflect(normalize(-rayDir), nor); // rayDir is the eye to position
    float time = 3.0;
    vec3 lightPos = vec3(3.0*sin(time), 3.0, 3.0*cos(time));
    vec3 lightDir = normalize(lightPos - pos);
    vec3 lightCol = vec3(1.0, 0.9, 0.7);
    float specCoeff, diffCoeff, ambCoeff;
    float spec, diff, shadow;
    vec3 amb;
    
    // set material of object
    vec3 material;
    if(obj.y > 0.0 && obj.y < 1.5) { // hit the plane
        float tile = 0.4*mod(floor(5.0*pos.x) + floor(5.0*pos.z), 2.0);
        tile = tile + 0.5;
        material = vec3(tile);
    } else if(obj.y > 1.5 && obj.y < 2.5) { // hit the sphere
        material = vec3(0.9, 0.1, 0.3);
    } else if(obj.y > 2.5 && obj.y < 3.5) { // hit capsule
        material = vec3(0.1, 0.1, 0.9);
    } else {
        material = vec3(0.0);
    }

    // calculate lighting
    vec3 brdf;
    ambCoeff = 0.1;
    diffCoeff = 1.0;
    specCoeff = 1.2;
    shadow = softshadow(pos, lightDir, 0.025, 2.5);
    amb = ambCoeff*vec3(1.0, 1.0, 1.0);
    diff = shadow*diffCoeff*clamp(dot(nor,lightDir), 0.0, 1.0);
    spec = shadow*specCoeff*pow(clamp(dot(reflectEye,lightDir), 0.0, 1.0), 15.0);
    brdf = material*lightCol*(diff+spec);
    brdf += amb;
    return brdf;
}


// CAMERA SETTING
mat3 mkCamMat(in vec3 rayOrigin, in vec3 lookAtPoint, float roll) {
    vec3 cw = normalize(lookAtPoint - rayOrigin);
    vec3 cp = vec3(sin(roll), cos(roll), 0.0); //this is a temp right vec for cross determination
    vec3 cu = normalize(cross(cw, cp));
    vec3 cv = normalize(cross(cu, cw));

    return mat3(cu, cv, cw);
}


void main() {
    // Transform Ball
    ballPosT.x = ((ballPos.x * 2) - 100) / 17;
    ballPosT.y = ((ballPos.y * 2) - 100) / 17;


    vec2 res = vec2(1.0, 1.0);
    vec2 q = fragCoord.xy / res.xy;
    vec2 p = -1.0 + 2.0*q;
    p.x *= res.x / res.y;
    
    // camera or eye (where rays start)
    vec3 rayOrigin = vec3(0.0, 0.5, -6.0);
    vec3 lookAtPoint = vec3(0.0, 0.5, 100.0);
    float focalLen = 1.0; // how far camera is from image plane
    mat3 camMat = mkCamMat(rayOrigin, lookAtPoint, 0.0);

    // ray direction into image plane
    vec3 rayDir = camMat * normalize(vec3(p.xy, focalLen));
    
    //render the scene with ray marching
    vec3 col = calColor(rayOrigin, rayDir);

    gl_FragColor = vec4(col, 1.0); 
}

