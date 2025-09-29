#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <xil_types.h>
#include <xil_io.h>
#include <stdlib.h>
#include <stdio.h>
#include <sleep.h>
#include <stdbool.h>

#include "vga.h"

// Random Number Generator Address is fixed
// This address is not defined in xparameters.h and must be explictly defined here
#define RNG_ADDR 0x43C00000

// Obstacle types
typedef enum
{
    RACCOON,
    CONCRETE,
    COIN
} ObstacleType;

// Obstacle object
typedef struct
{
    ObstacleType type;
    int path;
    int x;
    int y;
    float scale;
} Obstacle;

extern int imgBG_Frame;

void obstacle_clear(Obstacle obstacle);
void obstacle_print(Obstacle obstacles[], int numObstacles);
Obstacle createObstacle(Obstacle obstacles[], int numObstacles);
void updateObstacle(Obstacle *obstacle);
int linearPath(int y_pos, int obstacle_path);
int rngBlock();

#endif
