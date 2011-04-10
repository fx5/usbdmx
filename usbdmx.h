#include <stdint.h>

// types for library functions
typedef unsigned char TDMXArray[512];
typedef char TSERIAL[16];
typedef TSERIAL TSERIALLIST[32];
typedef void (THOSTDEVICECHANGEPROC) (void);
typedef void (THOSTINPUTCHANGEPROCBLOCK) (unsigned char blocknumber);

#define DWORD uint32_t

// define library functions
void GetAllConnectedInterfaces(TSERIALLIST* SerialList);
void GetAllOpenedInterfaces(TSERIALLIST* SerialList);
DWORD OpenLink(TSERIAL Serial, TDMXArray *DMXOutArray, TDMXArray *DMXInArray);
DWORD CloseLink (TSERIAL Serial);
DWORD CloseAllLinks (void);
DWORD RegisterInterfaceChangeNotification (THOSTDEVICECHANGEPROC Proc);
DWORD UnregisterInterfaceChangeNotification (void);
DWORD RegisterInputChangeNotification (THOSTDEVICECHANGEPROC Proc);
DWORD UnregisterInputChangeNotification (void);

DWORD SetInterfaceMode (TSERIAL Serial, unsigned char Mode);
  // Modes:
  // 0: Do nothing - Standby
  // 1: DMX In -> DMX Out
  // 2: PC Out -> DMX Out
  // 3: DMX In + PC Out -> DMX Out
  // 4: DMX In -> PC In
  // 5: DMX In -> DMX Out & DMX In -> PC In
  // 6: PC Out -> DMX Out & DMX In -> PC In
  // 7: DMX In + PC Out -> DMX Out & DMX In -> PC In

DWORD GetDeviceVersion(TSERIAL Serial);
DWORD SetInterfaceAdvTxConfig(
    TSERIAL Serial, unsigned char Control, uint16_t Breaktime, uint16_t Marktime,
    uint16_t Interbytetime, uint16_t Interframetime, uint16_t Channelcount, uint16_t Startbyte
);
DWORD StoreInterfaceAdvTxConfig(TSERIAL Serial);
DWORD RegisterInputChangeBlockNotification(THOSTINPUTCHANGEPROCBLOCK Proc);
DWORD UnregisterInputChangeBlockNotification(void);

/// And the Functions from usbdmxsi.dll also

DWORD OpenInterface(TDMXArray * DMXOutArray, TDMXArray * DMXInArray, unsigned char Mode);
  // Modes:
  // 0: Do nothing - Standby
  // 1: DMX In -> DMX Out
  // 2: PC Out -> DMX Out
  // 3: DMX In + PC Out -> DMX Out
  // 4: DMX In -> PC In
  // 5: DMX In -> DMX Out & DMX In -> PC In
  // 6: PC Out -> DMX Out & DMX In -> PC In
  // 7: DMX In + PC Out -> DMX Out & DMX In -> PC In

DWORD CloseInterface(void);



