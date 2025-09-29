#include "game_logic.h"

// Global variables
int shift = 0;
int carOffset = (VGA_WIDTH - img_car_width)/2; // Centers the car
int numObstacles = 0;
int prevScore = -1;
int bg_score_change = 0;
int carSpeed = 0;
int prev_bg_frame = 0;

// Max of 4 obstacles at any given time
Obstacle obstacles[4];

// Game Loop Function
void game_loop()
{
	// Change background after score has went up by 10
	if (bg_score_change == gameScore)
	{
		// 4 different backgrounds to choose from
		imgBG_Frame = abs(rngBlock()) % 4;

		// Force a different background
		if (prev_bg_frame == imgBG_Frame)
		{
			imgBG_Frame = (imgBG_Frame + 1) % 4;
		}

		// Print the freshly loaded background
		vga_raw_print(*img_bg_data, VGA_WIDTH, VGA_HEIGHT, 0, 0, 1, imgBG_Frame);

		// Force draw the car and high score
		prev_btn_value = -1;
		prevScore = -1;

		// Increment the score that triggers the next background change
		bg_score_change += 10;

		prev_bg_frame = imgBG_Frame;
	}

	// Handle obstacle drawing and logic
	obstacle_logic();

	// Draw car
	drawCar();

	// Collision logic
	collision_logic();

	// Draw gameScore
	if (prevScore != gameScore)
	{
		drawScore(*img_bg_data, 0);
		prevScore = gameScore;
	}
}

// Function to initialize the game loop
bool game_initialize()
{
	// Variable initialization
	game_over_flag = false;
	shift = 0;
	gameScore = 0;
	prev_btn_value = -1;
	prevScore = -1;
	bg_score_change = 0;
	prev_bg_frame = imgBG_Frame;

	// Max number of obstacles on screen fixed at 4
	// NOTE: DO NOT CHANGE
	// Program becomes very slow and will introduce infinite loops due to overlap protection
	numObstacles = 4;

	// Create the initial obstacles
	for (int i = 0; i < numObstacles; i++)
	{
		obstacles[i] = createObstacle(obstacles, numObstacles);
	}

	return true;
}

// Draws the Car Sprite
void drawCar()
{
	int increment = 0;

	// If car is stationary, do not print car to save performance
	if (prev_btn_value == 0 && btn_value == 0)
	{
		return;
	}
	prev_btn_value = btn_value;

	// Default car speed
	carSpeed = 3;

	// Determine car speed based on background
	if (imgBG_Frame == 2)
	{
		// Rain
		carSpeed = 5;
	}
	else if (imgBG_Frame == 3)
	{
		// Snow
		carSpeed = 10;
	}
	// Default sprite frame
	int carSpriteDirection = 0;

	// Draw car turning left
	if (btn_value == 4)
	{
		increment = -carSpeed;
		carSpriteDirection = 2;
	}
	// Draw car turning right
	else if (btn_value == 8)
	{
		increment = carSpeed;
		carSpriteDirection = 1;
	}

	// Update shift
	shift += increment;

	// Restrict car movement to between 20 and 500 pixels accounting for car sprite offset
	if (shift + carOffset < 20)
	{
		shift = 20 - carOffset;
	}
	else if (shift + carOffset > 500)
	{
		shift = 500 - carOffset;
	}

	// You Wouldn't Print A Car
	vga_print(*img_car_data, img_car_width, img_car_height, shift + carOffset, 410, *img_bg_data, 1, carSpriteDirection, imgBG_Frame);
}

// Function to handle obstacle logic
void obstacle_logic()
{
	// Add new obstacles if there is space
	if (numObstacles < 4) {
	    obstacles[numObstacles] = createObstacle(obstacles, numObstacles);
	    numObstacles++;
	}

	// Update positions of obstacles
	for (int i = 0; i < numObstacles; i++)
	{
		updateObstacle(&obstacles[i]);
	}

	// Render obstacles
	obstacle_print(obstacles, numObstacles);

	// Check if any obstacle has reached the END
	for (int i = 0; i < numObstacles; i++)
	{
		// If the obstacle Y position is outside the visible screen
		if (obstacles[i].y >= 440)
		{
			// Clear the obstacle from the screen
			obstacle_clear(obstacles[i]);
			// Remove obstacle by shifting remaining obstacles and reducing numObstacles
			for (int j = i; j < numObstacles - 1; j++)
			{
				obstacles[j] = obstacles[j + 1];
			}
			numObstacles--;
		}
	}
}

// Function to handle collision logic
void collision_logic()
{
	// Check if a obstacle and car collided
	for (int i = 0; i < numObstacles; i++)
	{
		// If an obstacle is at the set collision height
		if (obstacles[i].y >= 360)
		{
			// Check if the obstacle is overlapping with the car x position
			int obstacle_start = obstacles[i].x;
			int obstacle_end = obstacles[i].x + img_racc_width * obstacles[i].scale;

			// Calculate the range of car positions
			// +- 20 to account for background masking sections
			int car_start = shift + carOffset + 20;
			int car_end = shift + carOffset + img_car_width - 20;

			// Check if the ranges overlap, if they do, collision detected
			if (obstacle_start <= car_end && obstacle_end >= car_start) {
				// logic based on obstacle type
				switch (obstacles[i].type)
				{
					case (COIN):
						// Clear coin from screen
						obstacle_clear(obstacles[i]);
						// increment score
						gameScore += 5;
						break;

					case (RACCOON):
						// Clear Raccoon
						obstacle_clear(obstacles[i]);
						// Print Splat
						vga_raw_print(*img_splat_data, img_splat_width, img_splat_height, shift + carOffset, 390, 1, 0);

						usleep(500000);

						// Game Over flag
						game_over_flag = true;
						break;

					case (CONCRETE):
						// Clear Concrete
						obstacle_clear(obstacles[i]);
						// Print Explosion
						vga_raw_print(*img_bomb_data, img_bomb_width, img_bomb_height, shift + carOffset + 10, 400, 0.5, 0);

						usleep(500000);

						// Game Over flag
						game_over_flag = true;
						break;

					default:
						break;
				}

				// Remove obstacle by shifting remaining obstacles and reducing numObstacles
				for (int j = i; j < numObstacles - 1; j++)
				{
					obstacles[j] = obstacles[j + 1];
				}
				numObstacles--;
			}
		}
	}
}
