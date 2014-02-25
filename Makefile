CC=gcc
CXX=g++
CPPOBJS=
OBJS=$(COBJS) $(CPPOBJS)
CFLAGS+=-Wall -g -c -fpic -pthread

ifeq ($(shell uname -s), Darwin)
    PLATFORM=mac
    COBJS=$(PLATFORM)/hid.o usbdmx.o
    LIBS=-pthread -framework IOKit -framework CoreFoundation
    LIB_NAMES=libusbdmx.dylib
    LD_SHARED=-dynamic -bundle
else
    COBJS=linux/hid.o usbdmx.o
    LIBS=-pthread `pkg-config libudev --libs`
    LIB_NAMES=libusbdmx.so
    LD_SHARED=-shared
endif

all: usbdmx_example usbdmx_example_static simple_example simple_example_static loopback_test $(LIB_NAMES)
	echo "" ; head -n 17 README.md

clean:
	rm -f $(OBJS) usbdmx_example usbdmx_example_static libusbdmx.a simple_example simple_example_static loopback_test $(LIB_NAMES)
	make -f Makefile-mingw32 clean
	make -f Makefile-mingw64 clean

windows-all:
	make -f Makefile-mingw32
	make -f Makefile-mingw64


usbdmx_example: usbdmx_example.cpp libusbdmx.a $(LIB_NAMES)
	g++ -L. -Wall -o usbdmx_example usbdmx_example.cpp -lusbdmx

usbdmx_example_static: usbdmx_example.cpp libusbdmx.a $(LIB_NAMES)
	g++ $(OBJS) usbdmx_example.cpp -o usbdmx_example_static $(LIBS)

simple_example: simple_example.c libusbdmx.a $(LIB_NAMES)
	gcc -L. -Wall -o simple_example simple_example.c -l usbdmx

simple_example_static: simple_example.c libusbdmx.a $(LIB_NAMES)
	gcc $(OBJS) $(LIBS) simple_example.c -o simple_example_static

loopback_test: loopback_test.c libusbdmx.a $(LIB_NAMES)
	gcc $(OBJS) $(LIBS) loopback_test.c -o loopback_test

libusbdmx.so: $(OBJS)
	$(CC) $(OBJS) $(LD_SHARED) -s -o libusbdmx.so $(LIBS)

libusbdmx.dylib: $(OBJS)
	$(CC) $(OBJS) -dynamiclib -s -o libusbdmx.dylib $(LIBS)

libusbdmx.a: usbdmx.o
	ar rcs libusbdmx.a usbdmx.o

$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(CPPOBJS): %.o: %.cpp
	$(CXX) $(CFLAGS) $< -o $@


.PHONY: clean
