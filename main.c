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


#define WINDOW_HEIGHT 512
#define WINDOW_WIDTH 512
#define NUM_VERTS 4


//Functions:
void initGLUT(void);
void initOpenGL(void);
void gameLoop(void);

//GLOBALS:
GLuint vBuffer, eBuffer, shaderProg;
GLuint vertPos;

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


/* Initializes the OpenGL environment for ray tracing.
Sets the OpenGL state variables
Sets the global vertex and element buffers for drawing
*/
void initOpenGL(void) {
    // OpenGL state variables
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Simple Square for ray tracing shaders
    GLushort elemData[] = {0, 1, 2, 3, 0, 2};
    //                       x,    y,    z,
    GLfloat vertData[] = {-1.0,  1.0,  0.0,
                          -1.0, -1.0,  0.0,
                           1.0, -1.0,  0.0,
                           1.0,  1.0,  0.0};
    int dataSize = NUM_VERTS * 3 * sizeof(GLfloat);
    glGenBuffers(1, &vBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, vertData, GL_STATIC_DRAW);

    glGenBuffers(1, &eBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(GLushort), elemData, GL_STATIC_DRAW);

    // compile shader programs here in Resources folder
    shaderProg = shaderSetup("resources/shader.vert", "resources/shader.frag");
    if (!shaderProg) {
        perror("ERROR Setting up default shadern");
        exit(1);
    }
    vertPos = glGetAttribLocation(shaderProg, "currVert");
    
    return;
}


void gameLoop(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Do game things

    // render screen
    glUseProgram(shaderProg);
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eBuffer);
    // pass info to shader
    glEnableVertexAttribArray(vertPos);
    glVertexAttribPointer(vertPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    // only display ever 1/60th of a second
    glutSwapBuffers();
    glutPostRedisplay();
}

