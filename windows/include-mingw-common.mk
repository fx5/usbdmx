
all: usbdmx${ID}.dll usbdmx_example${ID}.exe
clean:
	rm -f usbdmx${ID}.dll windows/hid${ID}.o usbdmx${ID}.o windows/pthread${ID}.o usbdmx_example${ID}.exe

windows/hid${ID}.o:
	${CC} -c windows/hid.c -o windows/hid${ID}.o

windows/pthread${ID}.o:
	${CC} -c windows/pthread.c -o windows/pthread${ID}.o

usbdmx${ID}.o:
	${CC} -Wall -I windows/ -c usbdmx.c -o usbdmx${ID}.o

usbdmx${ID}.dll: usbdmx${ID}.o windows/hid${ID}.o windows/pthread${ID}.o
	${CC} -shared -s -I windows/ usbdmx.c windows/pthread${ID}.o windows/hid${ID}.o -l setupapi -o usbdmx${ID}.dll

usbdmx_example${ID}.exe: usbdmx${ID}.o windows/hid${ID}.o windows/pthread${ID}.o
	${CC} usbdmx${ID}.o windows/pthread${ID}.o windows/hid${ID}.o windows/sleep.cpp windows/usbdmx_example.cpp -l setupapi -o usbdmx_example${ID}.exe
