#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <sleep.h>
#include <stdio.h>

#include "obstacle.h"
#include "vga.h"
#include "score.h"

extern volatile bool TIMER_INTR_FLG;

extern int btn_value;
extern int prev_btn_value;
extern bool game_init_flag;
extern bool game_over_flag;
extern int gameScore;
extern int imgBG_Frame;

void drawCar();
void obstacle_logic();
void collision_logic();
bool game_initialize();
void game_loop();
void swapBackground();

#endif
