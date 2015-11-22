// main.cpp
// 
// Raytraced Pong in the style of Christmas
// Copyright Eric Knapik 2015


// some include probably
#include <GLUT/GLUT.h> // setup for MacOSX
#include <OpenGL/gl.h> // setup for MacOSX
#include <stdio.h>
#include <stdlib.h>


#define WINDOW_HEIGHT 512
#define WINDOW_WIDTH 512


//Functions:
void initGLUT(void);
void initOpenGL(void);

//GLOBALS:
GLuint vbuffer, ebuffer, shaderProg;
int numVert = 4;
GLushort elemData[] = {0, 1, 2, 3, 0, 2};
//                       x,    y,    z,
GLfloat vertData[] = {-1.0,  1.0,  0.0,
                      -1.0, -1.0,  0.0,
                       1.0, -1.0,  0.0,
                       1.0,  1.0,  0.0};


int main(int argc, char *argv[]) {

    // initialize game
    // setup variable
    // enter glut main loop which is game loop

    initGLUT();
    initOpenGL();

    glutMainLoop();
    return 0;
}


void initGLUT(void) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_HEIGHT, WINDOW_WIDTH);
    glutCreateWindow("Christmas Pong");
    glutDisplayFunc(gameLoop);
}


void 

