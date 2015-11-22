// pong.c


#include "pong.h"

// in a rush to check pointers
void initGame(float **ballPos, float **ballVel, float **player, float **comp) {
    *ballPos = malloc(sizeof(float) * 2);
    *ballVel = malloc(sizeof(float) * 2);
    *player = malloc(sizeof(float) * 2);
    *comp = malloc(sizeof(float) * 2);
    resetGame(ballPos, ballVel, player, comp);
}


void resetGame(float **ballPos, float **ballVel, float **player, float **comp) {
    (*ballPos)[0] = 50;
    (*ballPos)[1] = 50;

    // srand(3423094);
    (*ballVel)[0] = 1.0; // (rand() % 4) -2;
    (*ballVel)[1] = -2.0; // (rand() % 4) -2;

    (*player)[0] = 5;
    (*player)[1] = 50;

    (*comp)[0] = 95;
    (*comp)[1] = 50;
}


void updateGame(int mouse[], float **ballPos, float **ballVel, float **player, float **comp) {
    // set positions
    // check collisions
    // check for win

    // player
    (*player)[1] = mouse[1];
    // ball
    (*ballPos)[0] += (*ballVel)[0];
    (*ballPos)[1] += (*ballVel)[1];
    if((*ballPos)[0] <= 0) { // check x boundings
        (*ballPos)[0] = 0;
        (*ballVel)[0] *= -1;
    } else if((*ballPos)[0] >= 100) {
        (*ballPos)[0] = 100;
        (*ballVel)[0] *= -1;
    }
    if((*ballPos)[1] <= 0) { // check y boundings
        (*ballPos)[1] = 0;
        (*ballVel)[1] *= -1;
    } else if((*ballPos)[1] >= 100) {
        (*ballPos)[1] = 100;
        (*ballVel)[1] *= -1;
    }
    // comp
    int compSpeed = 3;
    if((*comp)[1] < ((*ballPos)[1] - 3)) {
        (*comp)[1] += compSpeed;
    } else if((*comp)[1] > ((*ballPos)[1] + 3)) {
        (*comp)[1] -= compSpeed;
    }
    
}
