lib_LTLIBRARIES = lib96BoardsGPIO.la
include_HEADERS = gpio.h
lib96BoardsGPIO_la_SOURCES = gpio.c
lib96BoardsGPIO_la_CPPFLAGS = -I${top_srcdir}/lib
python_PYTHON = gpio_96boards.py

# This must be /etc and not $sysconfig to get picked up by shells
profiledir = /etc/profile.d
profile_DATA = pin_mappings.sh

xsessiondir = /etc/X11/Xsession.d
xsession_DATA = pin_mappings.sh
