#
# File: /Makefile
# Project: memory-management
# Created Date: Mon September 21st 202020
# Author: Julius Schmidt
# -----
# Last Modified:
# Modified By:
# -----
# Copyright (c) 2020 Julius Schmidt
# -----
# HISTORY:
# Date     	By	Comments
# ---------	---	-------------------------------------------------
#


# Version Number
MAJORVERSION := 1.0

C=.c
A=.a
O=.o
SO=.so
E=
CC=cc
I=include
STD?=c11
CFLAGS+=-I$I 
CFLAGS+=-Wall -Wextra 
CFLAGS+=-Wmissing-prototypes
CFLAGS+=-Wformat-security -Wswitch-enum
CFLAGS+=-pedantic-errors
CFLAGS+=-std=$(STD)
CFLAGS+=-O3
LDFLAGS=-O3
LD=cc
AS=as
AR=ar ruv
RANLIB=ranlib
DIFF=diff
RM=rm -f
CUSTOM=custom.mk
EXTRAS=
THREADS=
include $(CUSTOM)
B=$(BUILDDIR)
SRC=src
TARGET=bin/libmem
MKDIR=mkdir -p
DIRNAME=dirname

OBJS=         $Bassert$O \
							$Bexcept$O \
							$Bmem$O \
							$Bmemchk$O \
							$(THREADS)


all::         $B$(TARGET)$A 

$B$(TARGET)$A::	$(OBJS) $(EXTRAS)
							$(MKDIR) $(shell $(DIRNAME) $(TARGET))
							$(AR) $@ $(OBJS) $(EXTRAS); $(RANLIB) $@ || true

# Linux-specific rule for building a shared library
$B$(TARGET)$(SO).$(MAJORVERSION): $(OBJS) $(EXTRAS)
							$(CC) -shared -Wl,-soname,$(TARGET)$(SO).$(MAJORVERSION) -o $@ $(OBJS) $(EXTRAS)

$Bassert$O:   $(SRC)/assert$C;      $(CC) $(CFLAGS) -c -o $@ $(SRC)/assert$C
$Bexcept$O:   $(SRC)/except$C;      $(CC) $(CFLAGS) -c -o $@ $(SRC)/except$C
$Bmem$O:      $(SRC)/mem$C;         $(CC) $(CFLAGS) -c -o $@ $(SRC)/mem$C
$Bmemchk$O:   $(SRC)/memchk$C;      $(CC) $(CFLAGS) -c -o $@ $(SRC)/memchk$C

clean::
							$(RM) $B*$O
							$(RM) $(EXAMPLES)

clobber::     clean
							$(RM) $B$(TARGET)$A

version::
							$(CC) -v


# DO NOT DELETE THIS LINE -- make depend depends on it.

$Bassert$O:   $I/assert.h $I/except.h
$Bexcept$O:   $I/assert.h $I/except.h
$Bmem$O:      $I/assert.h $I/except.h $I/mem.h
$Bmemchk$O:   $I/assert.h $I/except.h $I/mem.h
