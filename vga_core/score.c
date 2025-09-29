#include "score.h"

// Global variables
int scoreCopy = 0; // Copy of gameScore for processing

// Draws the High Score component to the VGA display
//
// Variables:
// imgBG : Background image to be printed under the high score
// shiftY : Shifts the high score drawing based on chosen Y value
void drawScore(const unsigned int *imgBG, int shiftY)
{
    // Define position and scaleFactor for rendering high score
    int posX = 0;           // X position on the screen
    int posY = 10 + shiftY; // Y position on the screen
    float scaleFactor = 1.0;
    // Initialize variables
    scoreCopy = gameScore;
    int digitCount = 0;

    // Calculate the number of digits in the score
    while (scoreCopy != 0)
    {
        digitCount++;
        scoreCopy /= 10;
    }

    // 0 edge case
    if (gameScore == 0)
    {
        digitCount = 1; // If score is 0, set digit count to 1
    }

    // Account for "High Score: " text
    digitCount += 2;
    // Calculate the total width required to render all digits
    int totalWidth = img_num_width * digitCount;
    // Calculate the starting position to center the score horizontally
    int startX = posX + (VGA_WIDTH - totalWidth) / 2;
    // Loop through each digit of the score
    scoreCopy = gameScore; // Reset scoreCopy to the original gameScore

    // Handle the case for "High Score: " text
    if (highScore <= gameScore)
    {
        // Print H
        vga_print(*img_num_data, img_num_width, img_num_height, startX, posY, imgBG, scaleFactor, 10, imgBG_Frame);
        // Print I:
        vga_print(*img_num_data, img_num_width, img_num_height, startX + img_num_width, posY, imgBG, scaleFactor, 11, imgBG_Frame);
    }
    else
    {
        // Remove the "HI:"" text
        digitCount -= 2;
        // Recalculate
        totalWidth = img_num_width * digitCount;
        startX = posX + (VGA_WIDTH - totalWidth) / 2;
    }

    // Render the score digits
    while (scoreCopy >= 0)
    {
        // Extract the rightmost digit
        int digit = scoreCopy % 10;
        // Calculate the x position for this digit
        int digitPosX = startX + (digitCount - 1) * img_num_width;

        // Render the digit sprite
        vga_print(*img_num_data, img_num_width, img_num_height, digitPosX, posY, imgBG, scaleFactor, digit, imgBG_Frame);

        // Move to the next digit
        scoreCopy = scoreCopy / 10;
        digitCount--;

        // 0 edge case
        if (scoreCopy == 0)
        {
            return;
        }
    }
}
