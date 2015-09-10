# Makefile template for shared library

CC = gcc # C compiler
CFLAGS = -fPIC -Wall -Wextra -O2 -g # C flags
LDFLAGS = -shared  # linking flags
RM = rm -f  # rm command
CP = cp -f  # cp command
LD = ldconfig
TARGET_LIB = lib96BoardsGPIO.so # target lib
SL = ln -sf

SRCS = gpio.c # source files
OBJS = $(SRCS:.c=.o)

.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@.0.1 $^
	$(SL) $@.0.1 $@.1
	$(SL) $@.0.1 $@

$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@

include $(SRCS:.c=.d)

.PHONY: clean install

install:
	-${CP} lib* /usr/local/lib/.
	-${CP} gpio.h /usr/local/include/.
	-${CP} *.py /usr/local/lib/python2.7/dist-packages/
	-${LD}
	
clean:
	-${RM} lib* ${OBJS} $(SRCS:.c=.d)


