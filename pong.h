// pong.h
// 
// Copyright Eric Knapik 2015
// The game logic for the game pong


#include <stdio.h>
#include <stdlib.h>


void initGame(float **ballPos, float **ballVel, float **player, float **comp);
void resetGame(float **ballPos, float **ballVel, float **player, float **comp);
void updateGame(int mouse[], float **ballPos, float **ballVel, float **player, float **comp);
