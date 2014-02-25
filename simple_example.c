/*

This is a simple example how to use the simple libusbdmx really easy.

All you need is the function OpenInterface(), it opens the first usbdmx-device with one of
the following modes:

  0: Do nothing - Standby
  1: DMX In -> DMX Out
  2: PC Out -> DMX Out
  3: DMX In + PC Out -> DMX Out
  4: DMX In -> PC In
  5: DMX In -> DMX Out & DMX In -> PC In
  6: PC Out -> DMX Out & DMX In -> PC In
  7: DMX In + PC Out -> DMX Out & DMX In -> PC In

*/

#include "usbdmx.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char ** argv) {
    int result;

    TDMXArray dmx_out;
    TDMXArray dmx_in;

    // Open Interface in Mode 6 (PC Out -> DMX Out & DMX In -> PC In)
    // See head of this file for details
    result = OpenInterface(&dmx_out, &dmx_in, 6);

    if(result == 0) {
        // Open failed
        printf("Unable to open Interface\n");
        exit(1);
    }

    printf("Setting dmx_out values\n");

    dmx_out[0] = 64;    // Set DMX-Channel 0 to 25%
    dmx_out[1] = 127;   // Set DMX-Channel 1 to 50%
    dmx_out[2] = 191;   // Set DMX-Channel 2 to 75%
    dmx_out[3] = 255;   // Set DMX-Channel 3 to 100%

    printf("Waiting one second\n");

    usleep(200000);  // Wait 200 ms

    printf("Received:\n");

    // Let's show the world what we received
    printf("DMX-Input 0 = %d\n", dmx_in[0]);
    printf("DMX-Input 1 = %d\n", dmx_in[1]);
    printf("DMX-Input 2 = %d\n", dmx_in[2]);
    printf("DMX-Input 3 = %d\n", dmx_in[3]);

    dmx_out[0] = 0;   // Set DMX-Channel 0 to 0%
    dmx_out[1] = 0;   // Set DMX-Channel 1 to 0%
    dmx_out[2] = 0;   // Set DMX-Channel 2 to 0%
    dmx_out[3] = 0;   // Set DMX-Channel 3 to 0%

    CloseInterface();

    return 0;
}
