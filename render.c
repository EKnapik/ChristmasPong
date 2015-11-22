#include "render.h"

/* Initializes the OpenGL environment for ray tracing.
Sets the OpenGL state variables
Sets the global vertex and element buffers for drawing
*/
void initOpenGL(void) {
    // OpenGL state variables
    glEnable(GL_DEPTH_TEST | GL_CULL_FACE);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glCullFace(GL_FRONT);
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
    shaderProg = shaderSetup("resources/pong.vert", "resources/pong.frag");
    if (!shaderProg) {
        perror("ERROR Setting up default shadern");
        exit(1);
    }
    vertPos = glGetAttribLocation(shaderProg, "currVert");
    ball_glPos = glGetUniformLocation(shaderProg, "ballPos");
    player_glPos = glGetUniformLocation(shaderProg, "playerPos");
    comp_glPos = glGetUniformLocation(shaderProg, "compPos");
    
    return;
}



// optionally can be given the game state to render a something
void render(float *ballPos, float *player, float *comp) {
    // render screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProg);
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eBuffer);
    // pass info to shader
    glEnableVertexAttribArray(vertPos);
    glVertexAttribPointer(vertPos, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glUniform2fv(ball_glPos, 1, ballPos);
    glUniform2fv(player_glPos, 1, player);
    glUniform2fv(comp_glPos, 1, comp);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    glutSwapBuffers();
}
