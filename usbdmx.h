#ifdef __cplusplus
extern "C" { 
#endif

#include <stdint.h>

// types for library functions
typedef unsigned char TDMXArray[512];
typedef char TSERIAL[16];
typedef TSERIAL TSERIALLIST[32];
typedef void (THOSTDEVICECHANGEPROC) (void);
typedef void (THOSTINPUTCHANGEPROCBLOCK) (unsigned char blocknumber);

#define DWORD uint32_t
#ifdef WIN32
    #define DLL __declspec(dllexport)
#else
    #define DLL
#endif

// define library functions
DLL void GetAllConnectedInterfaces(TSERIALLIST* SerialList);
DLL void GetAllOpenedInterfaces(TSERIALLIST* SerialList);
DLL DWORD OpenLink(TSERIAL Serial, TDMXArray *DMXOutArray, TDMXArray *DMXInArray);
DLL DWORD CloseLink (TSERIAL Serial);
DLL DWORD CloseAllLinks (void);
DLL DWORD RegisterInterfaceChangeNotification (THOSTDEVICECHANGEPROC Proc);
DLL DWORD UnregisterInterfaceChangeNotification (void);
DLL DWORD RegisterInputChangeNotification (THOSTDEVICECHANGEPROC Proc);
DLL DWORD UnregisterInputChangeNotification (void);

DLL DWORD SetInterfaceMode (TSERIAL Serial, unsigned char Mode);
  // Modes:
  // 0: Do nothing - Standby
  // 1: DMX In -> DMX Out
  // 2: PC Out -> DMX Out
  // 3: DMX In + PC Out -> DMX Out
  // 4: DMX In -> PC In
  // 5: DMX In -> DMX Out & DMX In -> PC In
  // 6: PC Out -> DMX Out & DMX In -> PC In
  // 7: DMX In + PC Out -> DMX Out & DMX In -> PC In

DLL DWORD GetDeviceVersion(TSERIAL Serial);
DLL DWORD SetInterfaceAdvTxConfig(
    TSERIAL Serial, unsigned char Control, uint16_t Breaktime, uint16_t Marktime,
    uint16_t Interbytetime, uint16_t Interframetime, uint16_t Channelcount, uint16_t Startbyte
);
DLL DWORD StoreInterfaceAdvTxConfig(TSERIAL Serial);
DLL DWORD RegisterInputChangeBlockNotification(THOSTINPUTCHANGEPROCBLOCK Proc);
DLL DWORD UnregisterInputChangeBlockNotification(void);

/// And the Functions from usbdmxsi.dll also

DLL DWORD OpenInterface(TDMXArray * DMXOutArray, TDMXArray * DMXInArray, unsigned char Mode);
  // Modes:
  // 0: Do nothing - Standby
  // 1: DMX In -> DMX Out
  // 2: PC Out -> DMX Out
  // 3: DMX In + PC Out -> DMX Out
  // 4: DMX In -> PC In
  // 5: DMX In -> DMX Out & DMX In -> PC In
  // 6: PC Out -> DMX Out & DMX In -> PC In
  // 7: DMX In + PC Out -> DMX Out & DMX In -> PC In

DLL DWORD CloseInterface(void);


#ifdef __cplusplus
}
#endif

