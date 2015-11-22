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
#include "pong.h"
#include "render.h"


#define WINDOW_HEIGHT 512
#define WINDOW_WIDTH 1024

//Functions:
void initGLUT(int argc, char *argv[]);
void mouseCallBack(int x, int y);
void mouseEntryCallBack(int state);
void gameLoop(void);

// GLOBALS:
int gameState;
int mouseState;
int mouse[2];
float *ballPos;
float *ballVel;
float *player;
float *comp;

int main(int argc, char *argv[]) {

    // initialize game
    // setup variable
    // enter glut main loop which is game loop

    initGLUT(argc, argv);
    initOpenGL();
    initGame(&ballPos, &ballVel, &player, &comp);

    glutMainLoop();
    return 0;
}


void initGLUT(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Christmas Pong");
    glutDisplayFunc(gameLoop);
    glutPassiveMotionFunc(mouseCallBack);
    glutEntryFunc(mouseEntryCallBack);
}

void mouseCallBack(int x, int y) {
    if(mouseState == GLUT_ENTERED) {
            printf("X: %d, Y: %d\n", mouse[0], mouse[1]);
            mouse[0] = (x * 100 / WINDOW_WIDTH);
            mouse[1] = (y * 100 / WINDOW_HEIGHT);
    }
}

void mouseEntryCallBack(int state) {
    mouseState = state;
}




void gameLoop(void) {
    updateGame(mouse, &ballPos, &ballVel, &player, &comp);

    render(ballPos, player, comp);
    // only display ever 1/60th of a second
    glutPostRedisplay();
}




