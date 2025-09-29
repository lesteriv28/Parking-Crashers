#include "vga.h"

// VGA display pointer
int *VGA_OUT = (int *)0x02EAA000;

// Main VGA display print function
//
// Variables
// imgIN : image to print
// imgWIDTH : width of image to print
// imgHEIGHT : height of image to print
// shiftX : shift image to print by the specified pixels in x direction
// shiftY : shift image to print by the specified pixels in y direction
// imgBG : background that is displayed under the image to print
// scaleFactor : scale image to print by specified value
// selectFrame : selects the frame of the imgIN (frame is animation of imgIN)
// selectFrameBG : selects the frame of the imgBG (frame is variation of imgBG)
void vga_print(const unsigned int *imgIN, int imgWIDTH, int imgHEIGHT, int shiftX, int shiftY, const unsigned int *imgBG, float scaleFactor, int selectFrame, int selectFrameBG)
{
    // Calculate new dimensions after scaling
    int scaledWidth = (int)(imgWIDTH * scaleFactor);
    int scaledHeight = (int)(imgHEIGHT * scaleFactor);

    // Print scaled image
    for (int i = 0; i < scaledHeight; i++)
    {
        for (int j = 0; j < scaledWidth; j++)
        {
            // Calculate the corresponding pixel in the original imgIN
            int origX = (int)(j / scaleFactor);
            int origY = (int)(i / scaleFactor);

            // Disregard out of bounds printing
            if (((j + shiftX) + (i + shiftY) * VGA_WIDTH) <= (VGA_WIDTH * VGA_HEIGHT))
            {
                // image indexs
                int VGA_OUT_index = (j + shiftX) + (i + shiftY) * VGA_WIDTH;
                int imgIN_index = origX + (origY + selectFrame * imgHEIGHT) * imgWIDTH;
                // background image index
				int imgBG_index = (j + shiftX) + (i + shiftY + VGA_HEIGHT * selectFrameBG) * VGA_WIDTH;

                // if imgIN pixel is not empty AND it does not match the VGA display
                if (imgIN[imgIN_index] != 0x00 && imgIN[imgIN_index] != VGA_OUT[VGA_OUT_index])
                {
                    // print to VGA display
                    VGA_OUT[VGA_OUT_index] = imgIN[imgIN_index];
                }
                // if imgIN is empty and imgBG is not on display
                else if (imgIN[imgIN_index] == 0x00 && imgBG[imgBG_index] != VGA_OUT[VGA_OUT_index])
                {
                    // print BG to VGA display
                    VGA_OUT[VGA_OUT_index] = imgBG[imgBG_index];
                }
            }
        }
    }
    // Flush cache to ensure clean drawing
    Xil_DCacheFlush();
}

// Variation of the Main VGA display print function without the background print check
//
// Variables
// imgIN : image to print
// imgWIDTH : width of image to print
// imgHEIGHT : height of image to print
// shiftX : shift image to print by the specified pixels in x direction
// shiftY : shift image to print by the specified pixels in y direction
// scaleFactor : scale image to print by specified value
// selectFrame : selects the frame of the imgIN (frame is animation of imgIN)
//
// NOTE: If printing background (passing background as imgIN), remember to select the proper frame!
void vga_raw_print(const unsigned int *imgIN, int imgWIDTH, int imgHEIGHT, int shiftX, int shiftY, float scaleFactor, int selectFrame)
{
    // Calculate new dimensions after scaling
    int scaledWidth = (int)(imgWIDTH * scaleFactor);
    int scaledHeight = (int)(imgHEIGHT * scaleFactor);

    // Print scaled image
    for (int i = 0; i < scaledHeight; i++)
    {
        for (int j = 0; j < scaledWidth; j++)
        {
            // Calculate the corresponding pixel in the original imgIN
            int origX = (int)(j / scaleFactor);
            int origY = (int)(i / scaleFactor);

            // Disregard out of bounds printing
            if (((j + shiftX) + (i + shiftY) * VGA_WIDTH) <= (VGA_WIDTH * VGA_HEIGHT))
            {
                // image indexs
                int VGA_OUT_index = (j + shiftX) + (i + shiftY) * VGA_WIDTH;
                int imgIN_index = origX + (origY + selectFrame * imgHEIGHT) * imgWIDTH;

                // if imgIN pixel is not empty AND it does not match the VGA display
                if (imgIN[imgIN_index] != 0x00 && imgIN[imgIN_index] != VGA_OUT[VGA_OUT_index])
                {
                    // print to VGA display
                    VGA_OUT[VGA_OUT_index] = imgIN[imgIN_index];
                }
            }
        }
    }
    // Flush cache to ensure clean drawing
    Xil_DCacheFlush();
}

// Variation of the Main VGA display print function that prints the background over the image to print
//
// Variables
// imgIN : image to print
// imgWIDTH : width of image to print
// imgHEIGHT : height of image to print
// shiftX : shift image to print by the specified pixels in x direction
// shiftY : shift image to print by the specified pixels in y direction
// imgBG : background that is displayed under the image to print
// scaleFactor : scale image to print by specified value
// selectFrame : selects the frame of the imgIN (frame is animation of imgIN)
// selectFrameBG : selects the frame of the imgBG (frame is variation of imgBG)
void vga_clear(const unsigned int *imgIN, int imgWIDTH, int imgHEIGHT, int shiftX, int shiftY, const unsigned int *imgBG, float scaleFactor, int selectFrame, int selectFrameBG)
{
    // Calculate new dimensions after scaling
    int scaledWidth = (int)(imgWIDTH * scaleFactor);
    int scaledHeight = (int)(imgHEIGHT * scaleFactor);

    // Print scaled image
    for (int i = 0; i < scaledHeight; i++)
    {
        for (int j = 0; j < scaledWidth; j++)
        {
            // Calculate the corresponding pixel in the original imgIN
            int origX = (int)(j / scaleFactor);
            int origY = (int)(i / scaleFactor);

            // Disregard out of bounds printing
            if (((j + shiftX) + (i + shiftY) * VGA_WIDTH) <= (VGA_WIDTH * VGA_HEIGHT))
            {
                // image indexes
                int VGA_OUT_index = (j + shiftX) + (i + shiftY) * VGA_WIDTH;
                int imgIN_index = origX + (origY + selectFrame * imgHEIGHT) * imgWIDTH;

                // if imgIN pixel is not empty
                if (imgIN[imgIN_index] != 0x00)
                {
                    // background image index
                    int imgBG_index = (j + shiftX) + (i + shiftY + VGA_HEIGHT * selectFrameBG) * VGA_WIDTH;
                    // print BG to VGA display
                    VGA_OUT[VGA_OUT_index] = imgBG[imgBG_index];
                }
            }
        }
    }
    // Flush cache to ensure clean drawing
    Xil_DCacheFlush();
}
