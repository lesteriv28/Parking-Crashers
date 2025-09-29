#ifndef gameScore_H_
#define gameScore_H_

#include <stdio.h>
#include <stdbool.h>

#include "vga.h"

extern int highScore;
extern int gameScore;
extern int imgBG_Frame;

void drawScore(const unsigned int *imgBG, int shiftY);

#endif
