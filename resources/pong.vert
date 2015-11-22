#version 120

attribute vec3 currVert;

varying vec2 fragCoord;

void main() {
    fragCoord = currVert.xy;
    fragCoord = fragCoord + vec2(1.0, 1.0);
    fragCoord = fragCoord / 2.0;

    gl_Position = vec4(currVert, 1.0);
}
