#version 120

// This is our vector which is a point of our shape
// because of version 120 this is a attribute
// later versions have this as an "in" type

attribute vec4 currentVertex;

// Our Transformation matrix
uniform mat4 transform;

varying vec4 Color;


void main()
{
    vec3 temp = vec3(currentVertex.x, currentVertex.y, currentVertex.z);

    gl_Position = transform * currentVertex;
    Color = vec4( clamp( temp, 0.0, 1.0), 1.0 );
}

