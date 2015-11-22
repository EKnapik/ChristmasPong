// pong.h
// 
// Copyright Eric Knapik 2015
// The game logic for the game pong


#include <stdio.h>
#include <stdlib.h>


void initGame(int *ballPos, int *ballVel, int *player, int *comp);
void resetGame(int *ballPos, int *ballVel, int *player, int *comp);
void updateGame(int *mouse, int *ballPos, int *ballVel, int *player, int *comp);
