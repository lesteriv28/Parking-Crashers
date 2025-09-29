#ifndef VGA_H
#define VGA_H

#include <xil_cache.h> // cache flush

#include "sprite_ctrl.h" // sprite assets

// VGA resolution is fixed
#define VGA_WIDTH 640
#define VGA_HEIGHT 480

void vga_print(const unsigned int *imgIN, int imgWIDTH, int imgHEIGHT, int shiftX, int shiftY, const unsigned int *imgBG, float scaleFactor, int selectFrame, int selectFrameBG);
void vga_raw_print(const unsigned int *imgIN, int imgWIDTH, int imgHEIGHT, int shiftX, int shiftY, float scaleFactor, int selectFrame);
void vga_clear(const unsigned int *imgIN, int imgWIDTH, int imgHEIGHT, int shiftX, int shiftY, const unsigned int *imgBG, float scaleFactor, int selectFrame, int selectFrameBG);

#endif
