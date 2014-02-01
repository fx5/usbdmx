#!/usr/bin/env python

import sys
import gtk
import usbdmx
import threading
import time

gtk.gdk.threads_init()

interfaces = usbdmx.scan()

if not interfaces:
    print "No interfaces found"
    sys.exit()

dmx = interfaces[0]
dmx.open()
dmx.mode(6)

main_win = gtk.Window()


table = gtk.Table()
main_win.add(table)


class DMXButton(gtk.SpinButton):

    def __init__(self, number):
        gtk.SpinButton.__init__(self, climb_rate=1, digits=0)
        self._number = number
        self.set_range(0, 255)
        self.set_increments(1, 20)
        self.connect("value-changed", self._on_value_changed)

    def _on_value_changed(self, *a, **b):
        dmx.set_dmx(self._number, int(self.get_value()))


class DMXEntry(gtk.SpinButton):

    def __init__(self, number):
        gtk.SpinButton.__init__(self)
        self.set_range(0, 255)
        self.set_increments(1, 20)
        self.set_sensitive(False)
        self._number = number

dmx_ins = []
per_row = 15

for i in xrange(150):
    col = i % per_row
    row = int(i / per_row) * 3
    table.attach(gtk.Label(str(i)), col, col+1, row, row + 1)
    table.attach(DMXButton(i), col, col+1, row+1, row+2)
    w = DMXEntry(i)
    table.attach(w, col, col+1, row+2, row+3)
    dmx_ins.append(w)


class DMXInUpdater(threading.Thread):

    def __init__(self):
        threading.Thread.__init__(self)
        self.setDaemon(True)

    def run(self):
        while True:
            time.sleep(0.1)
            gtk.gdk.threads_enter()
            try:
                for i, w in enumerate(dmx_ins):
                    w.set_value(dmx.get_dmx(i))
            finally:
                gtk.gdk.threads_leave()

DMXInUpdater().start()

main_win.show_all()

main_win.connect("destroy", lambda a: gtk.main_quit())

gtk.main()
