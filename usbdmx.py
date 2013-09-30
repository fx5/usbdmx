"""
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
"""

import ctypes

try:
    _usbdmx = ctypes.cdll.LoadLibrary("./libusbdmx.so")
except OSError:
    _usbdmx = ctypes.cdll.LoadLibrary("./libusbdmx.dylib")


def scan():
    """Returns a list of all connected Interface-objects."""
    val=ctypes.create_string_buffer("\0"*(32*16))
    _usbdmx.GetAllConnectedInterfaces(val)

    result = []
    for i in xrange(32):
        serial = val[i*16:(i+1)*16]
        if serial == "0" * 16:
            break
        result.append(FX5Interface(serial))

    return result


class FX5Interface(object):
    def __init__(self, serial):
        self.serial = serial

    def __repr__(self):
        return "<Interface %s>" % self.serial

    def open(self):
        """Open link to Interface and create attributes dmx_in and dmx_out"""
        self.dmx_in = ctypes.create_string_buffer("\0"*512)
        self.dmx_out = ctypes.create_string_buffer("\0"*512)
        if _usbdmx.OpenLink(self.serial, self.dmx_out, self.dmx_in) == 0:
            raise IOError("Unable to open interface")

    def close(self):
        """Close link"""
        if _usbdmx.CloseLink(self.serial) == 0:
            raise IOError("Unable to close link")

    def mode(self, mode):
        """ Set Mode """
        if _usbdmx.SetInterfaceMode(self.serial, mode) == 0:
            raise IOError("Unable to set mode")

    def set_dmx(self, num, val):
        """ Set DMX-Out Adress num to value val """
        assert(0 <= num <= 511)
        assert(0 <= val <= 255)
        self.dmx_out[num] = chr(val)

    def get_dmx(self, num):
        """ Get DMX-In Adress num """
        assert(0 <= num <= 511)
        return ord(self.dmx_in[num])


if __name__ == "__main__":
    import time
    fx5s = scan()
    fx5 = fx5s[0]
    print "using:", fx5
    fx5.open()
    fx5.mode(0)
    fx5.mode(6)
    for k in xrange(512):
        fx5.set_dmx(k, 255)
    time.sleep(0.1)

    for j in xrange(256):
        print "Testing",j
        for k in xrange(512):
            fx5.set_dmx(k, j)
        time.sleep(0.1)
        for k in xrange(512):
            assert(j == fx5.get_dmx(k))
    fx5.close()

