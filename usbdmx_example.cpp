#include <stdio.h>
extern "C" {
    #include "usbdmx.h"
}
#include <unistd.h>
#include <string.h>

void InterfaceChange(void) {
  printf("Interface cnonfiguration has changed\n");
}

void InputChange(void) {
  printf("Input has changed\n");
}

int main(int argc, char *argv[])
{

  int i;

  // "InterfaceList" is filled by "GetAllConnectedInterfaces" or "GetAllOpenedInterfaces"
  // It contains the serials of up to 32 connected interfaces. A serial (16 characters) is
  // an array of char with a fixed length of 16, it is not a null terminated string (there is no null-termination)
  TSERIALLIST InterfaceList;

  // "DMX_Out" "DMX_In" are the DMX data buffers
  TDMXArray DMX_Out, DMX_In;

  // You can (optional) register a parameterless function which will be called when an interface is connected/disconnected
  RegisterInterfaceChangeNotification(&InterfaceChange);

  // You can (optional) register a parameterless function which will be called if at least one interface is configurated in input mode and some input channels have changed
  RegisterInputChangeNotification(&InputChange);

  // Here you get the list of all connected interfaces. If the value (serial) of "0000000000000000" occurs in the list then there are no more interfaces connected
  GetAllConnectedInterfaces(&InterfaceList);

  // With this function you open a link to the specified interface. Also you have to turn over the pointer to your DMX buffers.
  // The first pointer points to the DMX out buffer (PC -> Interface), the second to the DMX in buffer (Interface -> PC)
  // If you don't use either DMX output or DMX input you can set the corresponding pointer to zero.
  // (In this example simply the first interface of the list returned from "GetAllConnectedInterfaces" is opened without testing if there is connected any interface to the PC)
  printf("SERIAL: %s\n", InterfaceList[0]);

  memset(DMX_In,0,sizeof(TDMXArray));
  memset(DMX_Out,0,sizeof(TDMXArray));

  OpenLink(InterfaceList[0], &DMX_Out, &DMX_In);

  // If you would call "GetAllOpenedInterfaces" here you'd get a list that contains only one valid serial: InterfaceList[0]

  // Now you can set the mode of the specified interface (a link must be opened).
  // The following modes are allowed:
  // 0: Do nothing - Standby
  // 1: DMX In -> DMX Out
  // 2: PC Out -> DMX Out
  // 3: DMX In + PC Out -> DMX Out
  // 4: DMX In -> PC In
  // 5: DMX In -> DMX Out & DMX In -> PC In
  // 6: PC Out -> DMX Out & DMX In -> PC In
  // 7: DMX In + PC Out -> DMX Out & DMX In -> PC In
  // If you want simply write to the DMX output use mode 2, if you want also read the DMX input use mode 6
  SetInterfaceMode(InterfaceList[0], 6);


  while(0) {
    SetInterfaceMode(InterfaceList[0], 0);
    sleep(1);
    SetInterfaceMode(InterfaceList[0], 6);
    sleep(1);
  }

  sleep(1);
  // Your main programm loop
  for(i=0;i<512;i++) {
      DMX_Out[i]=i;
  }
  while (1) {

    // Now you can write to your DMX out buffer. The dll detects any change and the new values are transmitted to the interface
    for(i=0;i<512;i++) {
        DMX_Out[i] += 1;
    }

    // Or you can read from the DMX input. If you have registered a function with "RegisterInputChangeNotification" it will be called everytime a channel changes it's value.
    // Every time the function is called up to 32 channels may have changed their value. Please hold the registered function as short as possible (< 1ms execution time)
    unsigned char tmp;
    tmp = DMX_In[0];
    tmp = DMX_In[1];
    tmp = DMX_In[2];
    tmp = DMX_In[3];
    //... and so on ...

    usleep(100000);
    for(i=0;i<512;i++) {
        if(DMX_Out[i]!=DMX_In[i]) {
            printf("ARGH\n");
        }
    }
    printf("Testing... %d\n",DMX_In[i]);
  }

  // At the end of the programm you should clean up a little
  CloseLink(InterfaceList[0]);
  UnregisterInputChangeNotification();
  UnregisterInterfaceChangeNotification();

  return 0;
}
