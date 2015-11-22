#version 120

// this is an outgoing value
// later versions have this as "out vec4 color"
varying vec4 Color;

void main()
{

    // black color
    //gl_FragColor = vec4( 0.0, 0.0, 0.0, 1.0 );

    // amber monochrome effect
    // gl_FragColor = vec4( 1.0, 0.745, 0.0, 0.0 );

    // Shows interpolation which is kinda cool
    gl_FragColor = Color;

}
