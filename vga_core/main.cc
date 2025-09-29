#include <stdio.h>
#include "xil_types.h"
#include "xparameters.h"
#include "xil_io.h"
#include <stdlib.h>
#include "xil_printf.h"
#include "sleep.h"

#include "interrupt.h"
#include "vga.h"
#include "sprite_ctrl.h"
#include "game_logic.h"

// Parameter definitions
#define INTC_DEVICE_ID      XPAR_PS7_SCUGIC_0_DEVICE_ID
#define BTNS_DEVICE_ID      XPAR_AXI_GPIO_0_DEVICE_ID
#define LEDS_DEVICE_ID      XPAR_AXI_GPIO_1_DEVICE_ID
#define INTC_GPIO_INTERRUPT_ID	XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR

volatile bool TIMER_INTR_FLG;

XGpio BTNInst;
XScuGic InterruptController; /* Instance of the Interrupt Controller */
XScuGic_Config *GicConfig;/* The configuration parameters of the controller */

// Global Variables
int btn_value = 0;
int prev_btn_value = -1;
int highScore = 0;
int gameScore = 0;
int imgBG_Frame = 0;

// Flags
bool game_init_flag = false;
bool menu_init_flag = false;
bool over_init_flag = false;
bool pause_init_flag = false;
bool game_over_flag = false;

enum GameState {
    MAIN_MENU,
    GAME_LOOP,
    GAME_OVER,
    PAUSE
};

// initial game state is main menu
enum GameState currentState = MAIN_MENU;

// BUTTON MAPPING
// LEFT:		4
// RIGHT:		8
// UP: 			16
// DOWN: 		2
// CENTER: 		1
// DEFAULT: 	0

int main()
{
    XTmrCtr TimerInstancePtr;
    int xStatus;
    // Initialise Push Buttons
    xStatus = XGpio_Initialize(&BTNInst, BTNS_DEVICE_ID);
    if (xStatus != XST_SUCCESS)
        return XST_FAILURE;

    // Set all buttons direction to inputs
    XGpio_SetDataDirection(&BTNInst, 1, 0xFF);

    //-----------Setup Timer Interrupt---------------------------------------

    xStatus = XTmrCtr_Initialize(&TimerInstancePtr, XPAR_AXI_TIMER_0_DEVICE_ID);

    XTmrCtr_SetHandler(&TimerInstancePtr,
                       (XTmrCtr_Handler)Timer_InterruptHandler,
                       &TimerInstancePtr);

    // Reset Values
    XTmrCtr_SetResetValue(&TimerInstancePtr, 0, 0xFFFFFFFF);
    //XTmrCtr_SetResetValue(&TimerInstancePtr, 0, 0xFA0A1F00);

    // Interrupt Mode and Auto reload
    XTmrCtr_SetOptions(&TimerInstancePtr,
                       XPAR_AXI_TIMER_0_DEVICE_ID,
                       (XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION));

    xStatus = ScuGicInterrupt_Init(XPAR_PS7_SCUGIC_0_DEVICE_ID, &TimerInstancePtr);

    /*Enable the interrupt for the device and then cause (simulate) an interrupt so the handlers will be called*/
    XScuGic_Enable(&InterruptController, 61);
    XScuGic_SetPriorityTriggerType(&InterruptController, 61, 0xa0, 3);

    // Button interrupt setup
    XGpio_InterruptEnable(&BTNInst, BTN_INT);
    XGpio_InterruptGlobalEnable(&BTNInst);

    XScuGic_Connect(&InterruptController, INTC_GPIO_INTERRUPT_ID,
                     (Xil_ExceptionHandler)BTN_Intr_Handler,
                     (void *)&BTNInst);

    XScuGic_Enable(&InterruptController, INTC_GPIO_INTERRUPT_ID);

	while (1)
	{
		XTmrCtr_Start(&TimerInstancePtr, 0);
		while (TIMER_INTR_FLG == false)
		{
		}

		TIMER_INTR_FLG = false;

		// Start of program logic
		switch (currentState)
		{
			case MAIN_MENU:
			{
				// Check if menu was already printed
				if (!menu_init_flag)
				{
					// Print main menu
					vga_raw_print(*img_menu_data, img_menu_width, img_menu_height, 0, 0, 1, 0);
					
                    // If a highScore was previously set, print that too
                    imgBG_Frame = 0;
					gameScore = highScore;
					drawScore(*img_menu_data, 0);

					menu_init_flag = true;
				}

				// Should a button be pressed, enter the GAME_LOOP state
				if (btn_value > 0)
				{
					currentState = GAME_LOOP;
					game_init_flag = false;
				}
				break;
			}
			case GAME_LOOP:
			{
				// Initialize Game if not done previously
				if (!game_init_flag)
				{
					game_init_flag = game_initialize();
				}

				// Loop the Game
				game_loop();

				// Animation delay
				usleep(10000);

				// If game is over
				if (game_over_flag)
				{
					currentState = GAME_OVER;
					over_init_flag = false;
					break;
				}

				// If PAUSE was asserted
				if (btn_value == 1)
				{
					currentState = PAUSE;
					pause_init_flag = false;
				}
				break;
			}
			case PAUSE:
			{
				// Print the pause menu
				vga_raw_print(*img_pause_data, img_pause_width, img_pause_height, 0, 0, 1, 0);
				// Should the up button be pressed, enter the GAME_LOOP state
				if (btn_value == 16)
				{
					vga_clear(*img_pause_data, img_pause_width, img_pause_height, 0, 0, *img_bg_data, 1, 0, imgBG_Frame);
					currentState = GAME_LOOP;
				}
				// Should the down button be pressed, enter the GAME_OVER state
				else if (btn_value == 2)
				{
					currentState = GAME_OVER;
					over_init_flag = false;
				}
				break;
			}
			case GAME_OVER:
			{
				// If Game Over screen was not drawn previously
				if (!over_init_flag)
				{
					// Print game over screen
					vga_raw_print(*img_over_data, img_over_width, img_over_height, 0, 0, 1, 0);

					// Print the score set during the game
                    imgBG_Frame = 0;
					drawScore(*img_over_data, 160);
					
					// save the high score
					if (highScore < gameScore)
					{
						highScore = gameScore;
					}

					over_init_flag = true;
				}

				// Should any button be pressed, enter the MAIN_MENU state
				if (btn_value > 0)
				{
					currentState = MAIN_MENU;
					menu_init_flag = false;
				}
				break;
			}
			default:
			{
				break;
			}
		}
	}
   return 0;
}
