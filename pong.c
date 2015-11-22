// pong.c


#include "pong.h"

void initGame(int *ballPos, int *ballVel, int *player, int *comp) {
    resetGame(ballPos, ballVel, player, comp);
}


void resetGame(int *ballPos, int *ballVel, int *player, int *comp) {
    ballPos[0] = 50;
    ballPos[1] = 50;

    srand(time(NULL));
    ballVel[0] = (rand() % 10) -5;
    ballVel[1] = (rand() % 10) -5;

    player[0] = 5;
    player[1] = 50;

    comp[0] = 95;
    comp[1] = 50;
}


void updateGame(int *mouse, int *ballPos, int *ballVel, int *player, int *comp) {
    // set positions
    // check collisions
    // check for win

    // player
    player[1] = mouse[1];
    // ball
    ballPos[0] += ballVel[0];
    ballPos[1] += ballVel[1];
    // comp
}
