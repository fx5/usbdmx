#ifndef USBDMX_API_H
#define USBDMX_API_H

#ifdef __cplusplus
extern "C" { 
#endif

#include <stdint.h>

#define DWORD uint32_t
#ifdef _WIN32 // holds true for win32 and win64
#ifdef USB_DMX_IMPLEMENTATION
    // compile the usbdmx implementation -> export the dll functions
    #define USB_DMX_DLL __declspec(dllexport)
#else
    // use the usbdmx header in a client -> import the dll functions
    #define USB_DMX_DLL __declspec(dllimport)
#endif
    #define USB_DMX_API_CALLING_CONVENTION        __stdcall
    #define USB_DMX_CALLBACK_CALLING_CONVENTION   __stdcall
    // compatibility with legacy callback calling convention
    // The macro USB_DMX_CALLBACK has been renamed to USB_DMX_CALLBACK_CALLING_CONVENTION
    // in order to make clear that it's only the calling convention not possibly any other
    // e.g. __declspec attribute. But that might break backwards compatibility of existing
    // projects. So the old macro is here for legacy reasons. It should be marked as
    // deprecated in future releases and finally be removed.
    #define USB_DMX_CALLBACK USB_DMX_CALLBACK_CALLING_CONVENTION
#else
    // not windows, remove all the dll magic
    #define USB_DMX_DLL
    #define USB_DMX_API_CALLING_CONVENTION
    #define USB_DMX_CALLBACK_CALLING_CONVENTION
    // compatibility with legacy callback calling convention, see above comment
    #define USB_DMX_CALLBACK
#endif


// types for library functions
typedef unsigned char TDMXArray[512];
typedef char TSERIAL[16];
typedef TSERIAL TSERIALLIST[32];
typedef void (USB_DMX_CALLBACK_CALLING_CONVENTION THOSTDEVICECHANGEPROC) (void);
typedef void (USB_DMX_CALLBACK_CALLING_CONVENTION THOSTINPUTCHANGEPROCBLOCK) (unsigned char blocknumber);


// define library functions
USB_DMX_DLL void USB_DMX_API_CALLING_CONVENTION GetAllConnectedInterfaces(TSERIALLIST* SerialList);
USB_DMX_DLL void USB_DMX_API_CALLING_CONVENTION GetAllOpenedInterfaces(TSERIALLIST* SerialList);
USB_DMX_DLL DWORD USB_DMX_API_CALLING_CONVENTION OpenLink(TSERIAL Serial, TDMXArray *DMXOutArray, TDMXArray *DMXInArray);
USB_DMX_DLL DWORD USB_DMX_API_CALLING_CONVENTION CloseLink (TSERIAL Serial);
USB_DMX_DLL DWORD USB_DMX_API_CALLING_CONVENTION CloseAllLinks (void);
USB_DMX_DLL DWORD USB_DMX_API_CALLING_CONVENTION RegisterInterfaceChangeNotification (THOSTDEVICECHANGEPROC Proc);
USB_DMX_DLL DWORD USB_DMX_API_CALLING_CONVENTION UnregisterInterfaceChangeNotification (void);
USB_DMX_DLL DWORD USB_DMX_API_CALLING_CONVENTION RegisterInputChangeNotification (THOSTDEVICECHANGEPROC Proc);
USB_DMX_DLL DWORD USB_DMX_API_CALLING_CONVENTION UnregisterInputChangeNotification (void);

USB_DMX_DLL DWORD USB_DMX_API_CALLING_CONVENTION SetInterfaceMode (TSERIAL Serial, unsigned char Mode);
  // Modes:
  // 0: Do nothing - Standby
  // 1: DMX In -> DMX Out
  // 2: PC Out -> DMX Out
  // 3: DMX In + PC Out -> DMX Out
  // 4: DMX In -> PC In
  // 5: DMX In -> DMX Out & DMX In -> PC In
  // 6: PC Out -> DMX Out & DMX In -> PC In
  // 7: DMX In + PC Out -> DMX Out & DMX In -> PC In

USB_DMX_DLL DWORD USB_DMX_API_CALLING_CONVENTION GetDeviceVersion(TSERIAL Serial);
USB_DMX_DLL DWORD USB_DMX_API_CALLING_CONVENTION SetInterfaceAdvTxConfig(
    TSERIAL Serial, unsigned char Control, uint16_t Breaktime, uint16_t Marktime,
    uint16_t Interbytetime, uint16_t Interframetime, uint16_t Channelcount, uint16_t Startbyte
);
USB_DMX_DLL DWORD USB_DMX_API_CALLING_CONVENTION StoreInterfaceAdvTxConfig(TSERIAL Serial);
USB_DMX_DLL DWORD USB_DMX_API_CALLING_CONVENTION RegisterInputChangeBlockNotification(THOSTINPUTCHANGEPROCBLOCK Proc);
USB_DMX_DLL DWORD USB_DMX_API_CALLING_CONVENTION UnregisterInputChangeBlockNotification(void);

/// And the Functions from usbdmxsi.USB_DMX_DLL also

USB_DMX_DLL DWORD USB_DMX_API_CALLING_CONVENTION OpenInterface(TDMXArray * DMXOutArray, TDMXArray * DMXInArray, unsigned char Mode);
  // Modes:
  // 0: Do nothing - Standby
  // 1: DMX In -> DMX Out
  // 2: PC Out -> DMX Out
  // 3: DMX In + PC Out -> DMX Out
  // 4: DMX In -> PC In
  // 5: DMX In -> DMX Out & DMX In -> PC In
  // 6: PC Out -> DMX Out & DMX In -> PC In
  // 7: DMX In + PC Out -> DMX Out & DMX In -> PC In

USB_DMX_DLL DWORD USB_DMX_API_CALLING_CONVENTION CloseInterface(void);


#ifdef __cplusplus
}
#endif

#endif // USBDMX_API_H
