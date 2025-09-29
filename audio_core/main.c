#include "adventures_with_ip.h"


int main(void) {
    // Initial setup functions...
    IicConfig(XPAR_XIICPS_0_DEVICE_ID);
    AudioPllConfig();
    AudioConfigureJacks();

    while(1) {
    	//play_background();
    	//play_getLucky();
    	//play_seeU();
    	play_hit();
    	play_coin();
    	play_die();
        usleep(10000); // Sleep for 10 milliseconds
    }

    return 1;
}






