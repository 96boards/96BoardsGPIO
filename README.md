#96BoardGPIO library  
##Overview  

This is the rudimentary beginnings of the 96BoardGPIO library
to control real world hardware via the GPIO on the 96Boards family of
boards that confirm to the CE spec.

One of the cool things about the 96Boards CE project is that all of the
boards us the same pins for the Low Speed Expansion Connector so you can
plug any expansion board into any 96Board and it will connect electrically
BUT there is a issue where GPIO is concerned.  Different SoC's have
different GPIO pins.  So even though electrically the pins are in the same
place it takes differnt code to enable and use the GPIO on pins 23 - 34. 
Not so fun.

The 96BoardGPIO library trys to abstract the info so that you can just
tell it what board you are using and what pins you want to use and the
library does the rest.

##Work Needed  
Early days, there are lots of improvements that can and I expect will be
made.  The library can now look in /etc/96boards_gpio.conf for config data
about this board.  It would be nice to add lookup tables that are external
and converted into XML so that the library will not requre recomplation to
add a new 96Boards board.  We need to identify how we know what board is
what at run time so we only need to call an init library call and not have
to input the board name.  The entire read side needs work to setup and
control the input settings.  Lots of work remaining in the kernel for
96Boards.

##Example code  
In the mean time the blink.c file is a demo of blinking an LED on pin 23. 
There is a blink.py file that uses the Python bindings to the C shared
library.

##Install the source code  
So to install the library and test it with the example blink code you need
to do the following:

**git clone https://github.com/96boards/96BoardsGPIO.git**  

cd 96BoardsGPIO/lib  
make  
sudo make install  

cd ../examples  
make  
sudo ./bink  
sudo ./plink.py  

The code so far has been tested on a Dragonboard 410 c where it is known to
work very well.

The gpio.c file is a shared library that will be the under pinnings
of a Python, Perl, Java and C++ librarys.

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

