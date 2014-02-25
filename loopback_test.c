#include "usbdmx.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char ** argv) {
    int result;
    int set_value;
    int count = 0;

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

    while(1) {
        set_value = count % 256;
        dmx_out[0] = set_value;
        dmx_out[1] = set_value;
        dmx_out[2] = set_value;

        usleep(50000);  // Wait 50 ms

        if (  (dmx_in[0] != set_value) ||
              (dmx_in[1] != set_value) ||
              (dmx_in[2] != set_value)) {
            printf("FAIL %d\n", count);
        } else {
            count ++;
            printf("Still working %d\n", count);
        }
    }
    return 0;
}
