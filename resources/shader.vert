#version 120

attribute vec3 currVert;

void main()
{
    gl_Position = vec4(currVert, 1.0);
}

