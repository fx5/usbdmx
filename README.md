usbdmx - Linux/Mac/Windows-Driver for [FX5 USB DMX Interface](http://www.fx5.de/)
and [Digital Enlightenment](http://www.digital-enlightenment.de).

BSD-License. API compatible with
original Windows-Drivers usbdmx.dll and (!) usbdmxsi.dll

To use the interface without root-privileges:
```
$ sudo cp 50-usbdmx.rules /etc/udev/rules.d/
```

To use the non-static-version "usbdmx_example" or "simple_example":
```
$ LD_LIBRARY_PATH=. ./usbdmx_example
$ LD_LIBRARY_PATH=. ./simple_example
```

or try:
```
$ python demo.py
```

```
Copyright (c) 2011-2013, Frank Sievertsen

Based upon hidapi Copyright (c) 2010 Alan Ott.

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
```
