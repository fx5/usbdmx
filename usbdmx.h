#include <stdint.h>

// types for library functions
typedef unsigned char TDMXArray[512];
typedef char TSERIAL[16];
typedef TSERIAL TSERIALLIST[32];
typedef void (THOSTDEVICECHANGEPROC) (void);

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
  // 0: Do nothing - Standby
  // 1: DMX In -> DMX Out
  // 2: PC Out -> DMX Out
  // 3: DMX In + PC Out -> DMX Out
  // 4: DMX In -> PC In
  // 5: DMX In -> DMX Out & DMX In -> PC In
  // 6: PC Out -> DMX Out & DMX In -> PC In
  // 7: DMX In + PC Out -> DMX Out & DMX In -> PC In

