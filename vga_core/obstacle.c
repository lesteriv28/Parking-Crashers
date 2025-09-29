#include "obstacle.h"

int obstacle_visible_positionY = 222;
int coin_animation_speed = 3;

// Random number generator
int rngBlock()
{
	return Xil_In32(RNG_ADDR);
}

// Function to initialize a new obstacle
Obstacle createObstacle(Obstacle obstacles[], int numObstacles)
{
	Obstacle obstacle;

	// Set scale default to 1
	obstacle.scale = 1;
	// Choose what type of obstacle randomly
	int obstacleSelect = abs(rngBlock()) % 3;

	switch (obstacleSelect)
	{
	case (0):
	{
		obstacle.type = RACCOON;
		break;
	}
	case (1):
	{
		obstacle.type = CONCRETE;
		break;
	}
	case (2):
	{
		obstacle.type = COIN;
		break;
	}
	default:
	{
		obstacle.type = RACCOON;
		break;
	}
	}

	// Choose path of obstacle
	// Obstacle pathing lanes are fixed to 5
	obstacle.path = abs(rngBlock()) % 5;

	// Obstacle overlap flag
	bool overlap_flag = true;
	// Introduce a counter
	int loopCounter = 0;

	// Prove that the created obstacle does not overlap
	while (overlap_flag)
	{
		// Set obstacle Y position between 0 and obstacle_visible_positionY setting
		obstacle.y = abs(rngBlock()) % obstacle_visible_positionY;
		// Calculate the obstacle X position
		obstacle.x = linearPath(obstacle.y, obstacle.path);

		// Check for overlap with existing obstacles
		overlap_flag = false;
		for (int i = 0; i < numObstacles; i++)
		{
			// 100 pixel width is fixed as the minimum obstacle overlap distance
			// If an existing obstacle is in the same path as the created obstacle
			// AND the Y position between the two obstacles is less than 100 pixels
			if (obstacles[i].path == obstacle.path && abs(obstacles[i].y - obstacle.y) < 100)
			{
				overlap_flag = true;
				break;
			}
		}

		// Increment the loop counter
		loopCounter++;

		// Check if the loop has run too many times
		if (loopCounter >= 3)
		{
			// Change the obstacle path to the next one
			obstacle.path = (obstacle.path + 1) % 5;
		}
	}
	return obstacle;
}

// Input in obstacle Y-Position and the obstacle set path
// Returns obstacle X-Position based on its obstacle set path
// Note: integers in the cases are hand calculated for 640x480 resolution
int linearPath(int posY, int obstacle_path)
{
	int posX;
	switch (obstacle_path)
	{
	// Left
	case (0):
	{
		posX = (posY - 492) / (-1.22);
		break;
	}
	// Middle Left
	case (1):
	{
		posX = (posY - 875) / (-2.47);
		break;
	}
	// Middle
	case (2):
	{
		posX = (posY - 3900) / (-12);
		break;
	}
	// Middle Right
	case (3):
	{
		posX = (posY + 1153) / (3.93);
		break;
	}
	// Right
	case (4):
	{
		posX = (posY + 430) / (1.66);
		break;
	}
	default:
		break;
	}

	// Prevent out of bounds drawing
	if (posX < 0)
	{
		posX = 0;
	}
	return posX;
}

// Function to update the position of an obstacle
void updateObstacle(Obstacle *obstacle)
{
	obstacle->y += 1;
	obstacle->x = linearPath(obstacle->y, obstacle->path);

	// Calculate the dynamic scale based on the obstacle Y position
	obstacle->scale = 0.2 + ((float)(obstacle->y - 100) / 380.0) * 1.8;
}

// Function to render obstacles on the screen
void obstacle_print(Obstacle obstacles[], int numObstacles)
{
	int posX = 0;
	int posY = 0;
	float scale = 0;

	// Iterate through all obstacles
	for (int i = 0; i < numObstacles; i++)
	{
		// If the obstacle is visible
		if (obstacles[i].y >= obstacle_visible_positionY)
		{
			posX = obstacles[i].x;
			posY = obstacles[i].y;
			scale = obstacles[i].scale;

			if (obstacles[i].type == RACCOON)
			{
				vga_print(*img_racc_data, img_racc_width, img_racc_height, posX, posY, *img_bg_data, scale, 0, imgBG_Frame);
			}
			else if (obstacles[i].type == CONCRETE)
			{
				vga_print(*img_conc_data, img_conc_width, img_conc_height, posX, posY, *img_bg_data, scale, 0, imgBG_Frame);
			}
			else if (obstacles[i].type == COIN)
			{
				// Calculate coin_animation_speed
				// NOTE: modulus 8 is due to coin having 8 animation frames
				int coin_animation_frame = (obstacles[i].y / coin_animation_speed) % 8;
				vga_print(*img_coin_data, img_coin_width, img_coin_height, posX, posY, *img_bg_data, scale, coin_animation_frame, imgBG_Frame);
			}
		}
	}
}

// Function to clear the obstacles on the screen
void obstacle_clear(Obstacle obstacle)
{
	int posX = obstacle.x;
	int posY = obstacle.y;
	float scale = obstacle.scale;

	if (obstacle.type == RACCOON)
	{
		vga_clear(*img_racc_data, img_racc_width, img_racc_height, posX, posY, *img_bg_data, scale, 0, imgBG_Frame);
	}
	else if (obstacle.type == CONCRETE)
	{
		vga_clear(*img_conc_data, img_conc_width, img_conc_height, posX, posY, *img_bg_data, scale, 0, imgBG_Frame);
	}
	else if (obstacle.type == COIN)
	{
		vga_clear(*img_coin_data, img_coin_width, img_coin_height, posX, posY, *img_bg_data, scale, 0, imgBG_Frame);
	}
}
