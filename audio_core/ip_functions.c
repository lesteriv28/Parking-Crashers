/*
 * ip_functions.c
 *
 * Contains all functions which pertain to setup and use of IP periperals.
 */

#include "adventures_with_ip.h"
#include "sleep.h"
/* ---------------------------------------------------------------------------- *
 * 								audio_stream()									*
 * ---------------------------------------------------------------------------- *
 * This function performs audio loopback streaming by sampling the input audio
 * from the codec and then immediately passing the sample to the output of the
 * codec.
 *
 * The main menu can be accessed by entering 'q' on the keyboard.
 * ---------------------------------------------------------------------------- */
unsigned int sample_rate = 44100; // Hz

void audio_stream(){
	u32  in_left, in_right;

	while (!XUartPs_IsReceiveData(UART_BASEADDR)){
		// Read audio input from codec
		in_left = Xil_In32(I2S_DATA_RX_L_REG);
		in_right = Xil_In32(I2S_DATA_RX_R_REG);
		// Write audio output to codec
		Xil_Out32(I2S_DATA_TX_L_REG, in_left);
		Xil_Out32(I2S_DATA_TX_R_REG, in_right);
	}

	/* If input from the terminal is 'q', then return to menu.
	 * Else, continue streaming. */
	//if(XUartPs_ReadReg(UART_BASEADDR, XUARTPS_FIFO_OFFSET) == 'q') menu();
	//else audio_stream();
} // audio_stream()


void play_hit(){
    // Calculate the number of 32-bit samples in the array
    unsigned int num_samples = hit_wav_len / 4;

    // Cast the byte array to a u32 pointer for direct 32-bit access
    u32* audio_data = (u32*)hit_wav;
    double sample_duration_us = 1.0 / sample_rate * 1e6;

    for (unsigned int i = 0; i < num_samples; i++) {
        // Directly use the 32-bit audio sample
        u32 sample = audio_data[i];

        // Assuming each 32-bit word contains two 16-bit samples (left and right channels)
        Xil_Out32(I2S_DATA_TX_L_REG, sample & 0xFFFF); // Lower 16 bits for the left channel
        Xil_Out32(I2S_DATA_TX_R_REG, sample >> 16);    // Upper 16 bits for the right channel

        // Implement delay if necessary
        usleep((useconds_t)sample_duration_us);
    }
    // Additional logic as needed
}
void play_coin(){
    // Calculate the number of 32-bit samples in the array
    unsigned int num_samples = coin_wav_len / 4;

    // Cast the byte array to a u32 pointer for direct 32-bit access
    u32* audio_data = (u32*)coin_wav;
    double sample_duration_us = 1.0 / sample_rate * 1e6;

    for (unsigned int i = 0; i < num_samples; i++) {
        // Directly use the 32-bit audio sample
        u32 sample = audio_data[i];

        // Assuming each 32-bit word contains two 16-bit samples (left and right channels)
        Xil_Out32(I2S_DATA_TX_L_REG, sample & 0xFFFF); // Lower 16 bits for the left channel
        Xil_Out32(I2S_DATA_TX_R_REG, sample >> 16);    // Upper 16 bits for the right channel

        // Implement delay if necessary
        usleep((useconds_t)sample_duration_us);
    }
    // Additional logic as needed
}
void play_die(){
    // Calculate the number of 32-bit samples in the array
    unsigned int num_samples = dyingsfx_wav_len / 4;

    // Cast the byte array to a u32 pointer for direct 32-bit access
    u32* audio_data = (u32*)dyingsfx_wav;
    double sample_duration_us = 1.0 / sample_rate * 1e6;

    for (unsigned int i = 0; i < num_samples; i++) {
        // Directly use the 32-bit audio sample
        u32 sample = audio_data[i];

        // Assuming each 32-bit word contains two 16-bit samples (left and right channels)
        Xil_Out32(I2S_DATA_TX_L_REG, sample & 0xFFFF); // Lower 16 bits for the left channel
        Xil_Out32(I2S_DATA_TX_R_REG, sample >> 16);    // Upper 16 bits for the right channel

        // Implement delay if necessary
        usleep((useconds_t)sample_duration_us);
    }
    // Additional logic as needed
}
