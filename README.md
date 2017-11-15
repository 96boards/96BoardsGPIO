#96BoardGPIO library
##Overview

This is the rudimentary beginnings of the 96BoardGPIO library
to control real world hardware via the GPIO on the 96Boards family of
boards that conform to the CE spec.

One of the cool things about the 96Boards CE project is that all of the
boards use the same pins for the Low Speed Expansion Connector so you can
plug any expansion board into any 96Board and it will connect electrically
BUT there is an issue where GPIO is concerned.  Different SoC's have
different GPIO pins.  So even though electrically the pins are in the same
place it takes different code to enable and use the GPIO on pins 23 - 34.
Not so fun.

The 96BoardGPIO library tries to abstract the info so that you can just
tell it what board you are using and what pins you want to use and the
library does the rest.

##Work Needed
The entire read side needs work to setup and control the input settings.
Lots of work remaining in the kernel for 96Boards.

##Install the source code
So to install the library and test it with the example blink code you need
to do the following:
```
git clone https://github.com/jackmitch/libsoc.git
cd libsoc
./autogen.sh && ./configure --enable-board="<board name>"
make && sudo make install
# <board name>" is dragonboard410c or hikey or bubblegum.

git clone https://github.com/96boards/96BoardsGPIO.git
cd 96BoardsGPIO
./autogen.sh && ./configure
make && sudo make install

sudo ldconfig
```

##Example code
The examples directory contains a blink.c and blink.py that demo how to blink
an LED hooked up to pin 23(A).
```
cd ./examples
make
sudo ./blink
sudo python blink.py
```

The code so far has been tested on a Dragonboard 410 c where it is known to
work very well.

##License

This library (96BoardsGPIO) is free software; you can redistribute it
and/or modify it under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation; version 2.1 of the
License.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

