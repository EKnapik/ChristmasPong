// main.cpp
// 
// Raytraced Pong in the style of Christmas
// Copyright Eric Knapik 2015


// some include probably
#include <GLUT/GLUT.h> // setup for MacOSX
#include <OpenGL/gl.h> // setup for MacOSX
#include <stdio.h>
#include <stdlib.h>
#include "shaderSetup.h"
#include "render.h"
#include "pong.h"


#define WINDOW_HEIGHT 512
#define WINDOW_WIDTH 512

//Functions:
void initGLUT(int argc, char *argv[]);
void gameLoop(void);

// GLOBALS:
int gameState;

int main(int argc, char *argv[]) {

    // initialize game
    // setup variable
    // enter glut main loop which is game loop

    initGLUT(argc, argv);
    initOpenGL();

    glutMainLoop();
    return 0;
}


void initGLUT(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_HEIGHT, WINDOW_WIDTH);
    glutCreateWindow("Christmas Pong");
    glutDisplayFunc(gameLoop);
}

void gameLoop(void) {
    // paddle1 pos
    // paddle2 pos
    // ball


    render();
    // only display ever 1/60th of a second
    glutPostRedisplay();
}




