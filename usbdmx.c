/*
Copyright (c) 2010, Frank Sievertsen
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Signal 11 Software nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include "usbdmx.h"
#include "hidapi.h"
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

#define MAX_OPENED 32

typedef struct  {
    TSERIAL serial;
    hid_device * handle;
    pthread_t thread;
    TDMXArray * dmx_in;
    TDMXArray * dmx_out;
    TDMXArray dmx_cmp;
    volatile int stop;
    volatile unsigned char mode;
    volatile unsigned char old_mode;
} device_t;


#define DMX_INTERFACE_VENDOR_ID 0x4B4
#define DMX_INTERFACE_PRODUCT_ID 0xF1F
#define DMX_INTERFACE_VENDOR_ID_2 0x16C0
#define DMX_INTERFACE_PRODUCT_ID_2 0x88B


void wserial_to_serial(const wchar_t * s1, char * s2) {
    int i;
    for(i=0;i<16;i++) {
        s2[i] = s1[i];
    }
}
void wserial_from_serial(wchar_t * s1, const char * s2) {
    int i;
    for(i=0;i<16;i++) {
        s1[i] = s2[i];
    }
    s1[i] = 0;
}

void set_mode(hid_device * handle, unsigned char mode) {
    unsigned char buffer[34];

    memset(buffer, 0, 34);
    buffer[1]=16;
    buffer[2]=mode;
    hid_write(handle, buffer, 34);
}

THOSTDEVICECHANGEPROC * callback_func;

char NO_DEV[] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

device_t open_devices[MAX_OPENED];

int find_dev(TSERIAL serial) {
    int i;
    for(i=0; i<MAX_OPENED; i++) {
        if(memcmp(serial, open_devices[i].serial, 16) == 0) {
            return i;
        }
    }
    return -1;
}
void set_dev(int pos, char * serial) {
    memcpy(open_devices[pos].serial, serial, 16);
}

void GetAllConnectedInterfaces(TSERIALLIST* SerialList) {
    struct hid_device_info *devs, *cur_dev;
    int pos = 0;

    devs = hid_enumerate(0x0, 0x0);
    cur_dev = devs;

    while (cur_dev) {
        if(
            ((cur_dev->vendor_id == DMX_INTERFACE_VENDOR_ID)&&(cur_dev->product_id == DMX_INTERFACE_PRODUCT_ID)) ||
            ((cur_dev->vendor_id == DMX_INTERFACE_VENDOR_ID_2)&&(cur_dev->product_id == DMX_INTERFACE_PRODUCT_ID_2))
        ) {
            wserial_to_serial(cur_dev->serial_number, SerialList[0][pos]);
            pos ++;
            cur_dev = cur_dev->next;
        }
    }
    wserial_to_serial(L"0000000000000000", SerialList[0][pos]);
}

void GetAllOpenedInterfaces(TSERIALLIST* SerialList) {
    int pos;
    int i;
    pos = 0;
    for(i=0; i<MAX_OPENED; i++) {
        if(memcmp(open_devices[i].serial,NO_DEV,16)!=0) {
            memcpy(SerialList[0][pos], open_devices[i].serial, 16);
            pos ++;
        }
    }
    wserial_to_serial(L"0000000000000000", SerialList[0][pos]);
}

DWORD SetInterfaceMode (TSERIAL Serial, unsigned char Mode) {
    int pos;

    pos = find_dev(Serial);
    if (pos == -1) {
        return 0;
    }
    open_devices[pos].mode = Mode;
    while(open_devices[pos].mode != open_devices[pos].old_mode) {
        usleep(10000);
    }
    return 1;
}


void *read_write_thread(void *pointer)
{
    device_t * device = (device_t *) pointer;
    unsigned char buffer[35];
    int size;
    int i;
    int res;
    int changed;
    THOSTDEVICECHANGEPROC * tmp;

    while(!device->stop) {
        if(device->old_mode != device->mode) {
            device->old_mode = device->mode;
            set_mode(device->handle, device->old_mode);
        }
        if(device->dmx_out) {
            for(i=0; i<16; i++) {
                if(memcmp(device->dmx_cmp + (i * 32), (* device->dmx_out) + (i * 32), 32) != 0) {
                    memcpy(device->dmx_cmp + (i * 32), (* device->dmx_out) + (i * 32), 32);
                    memcpy(buffer + 2, device->dmx_cmp + (i * 32), 32);
                    buffer[1] = i;
                    buffer[0] = 0;
                    res = hid_write(device->handle, buffer, 34);
                }
            }
        }
        if(device->dmx_in) {
            changed = 0;
            while(1) {
                size = hid_read(device->handle, buffer, 33);
                if(size == 0) {
                    break;
                }
                if(size < 0) {
                    break;
                }
                if(size == 33) {
                    memcpy(device->dmx_in[0]+buffer[0]*32, buffer+1, 32);
                    changed = 1;
                }
            }
            tmp = callback_func;
            if(changed && tmp) {
                (*tmp)();
            }
        }
        usleep(10000);
    }
    device->stop = 2;

    return NULL;
}

DWORD OpenLink(TSERIAL Serial, TDMXArray *DMXOutArray, TDMXArray *DMXInArray) {
    hid_device *handle;
    int pos;

    wchar_t wserial[17];
    wserial_from_serial(wserial, Serial);

    pos = find_dev(Serial);
    if(pos != -1) {
        return 1;
    }

    pos = find_dev(NO_DEV);
    if(pos == -1) {
        return 0;
    }
    handle = hid_open(DMX_INTERFACE_VENDOR_ID, DMX_INTERFACE_PRODUCT_ID, wserial);
    if(!handle) {
        handle = hid_open(DMX_INTERFACE_VENDOR_ID_2, DMX_INTERFACE_PRODUCT_ID_2, wserial);
    }
    if (!handle) {
        return 0;
    }
    hid_set_nonblocking(handle, 1);

    memset(&open_devices[pos], 0, sizeof(device_t));
    set_dev(pos, Serial);
    open_devices[pos].handle = handle;
    open_devices[pos].dmx_out = DMXOutArray;
    open_devices[pos].dmx_in = DMXInArray;

    set_mode(handle, 0);

    pthread_create(&open_devices[pos].thread, NULL, read_write_thread, &open_devices[pos]);
    return 1;
}

DWORD CloseLink (TSERIAL Serial) {
    int pos;

    pos = find_dev(Serial);
    if(pos == -1) {
        return 0;
    }
    open_devices[pos].stop = 1;
    while(open_devices[pos].stop != 2) {
        usleep(10000);
    }
    hid_close(open_devices[pos].handle);
    set_dev(pos, NO_DEV);
    return 1;
}

DWORD CloseAllLinks (void) {
    int i;
    int result = 1;
    for(i=0; i<MAX_OPENED; i++) {
        if(memcmp(open_devices[i].serial, NO_DEV, 16) != 0) {
            if(0 == CloseLink(open_devices[i].serial)) {
                result = 0;
            }
        }
    }
    return result;
}

DWORD RegisterInterfaceChangeNotification (THOSTDEVICECHANGEPROC Proc) {
    // FIXME
    return 1;
}

DWORD UnregisterInterfaceChangeNotification (void) {
    // FIXME too
    return 1;
}

DWORD RegisterInputChangeNotification (THOSTDEVICECHANGEPROC Proc) {
    callback_func = Proc;
    return 1;
}

DWORD UnregisterInputChangeNotification (void) {
    callback_func = NULL;
    return 1;
}

