// render.h
//
// Handles rendering a square to the screne and the propper shader to use
// Copyright Eric Knapik 2015


#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#include "shaderSetup.h"

#define NUM_VERTS 4

//GLOBALS:
GLuint vBuffer, eBuffer, shaderProg;
GLuint vertPos;

void initOpenGL(void);
// renders a square to the screen using shaderProg
void render(void);

