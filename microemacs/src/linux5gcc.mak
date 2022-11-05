# -!- makefile -!-
#
# JASSPA MicroEmacs - www.jasspa.com
# linux26gcc.mak - Make file for Linux v2 Kernel 2.6 using gcc
#
# Copyright (C) 2001-2009 JASSPA (www.jasspa.com)
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 675 Mass Ave, Cambridge, MA 02139, USA.
#
##############################################################################
#
# Created:     Wed Jul 28 2004
# Synopsis:    Make file for Linux v2 Kernel 2.6 using gcc
# Notes:
#	Run "make -f linux26gcc.mak"      for optimised build produces ./me
#	Run "make -f linux26gcc.mak med"  for debug build produces     ./med
#
#	Run "make -f linux26gcc.mak clean"      to clean source directory
#	Run "make -f linux26gcc.mak spotless"   to clean source directory even more
#
##############################################################################
#
# Installation Directory
INSTDIR	      = /usr/local/bin
INSTPROGFLAGS = -s -o root -g root -m 0775
#
# Local Definitions
A        = .a
EXE      = 

CC       = cc
MK       = make
LD       = $(CC)
STRIP    = strip
AR       = ar
RM       = rm -f
RMDIR    = rm -r -f

BUILDID  = linux5gcc
OUTDIRR  = .$(BUILDID)-release
OUTDIRD  = .$(BUILDID)-debug
TRDPARTY = ../3rdparty

CCDEFS   = -D_LINUX -D_LINUX5 -D_64BIT -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -I. -I$(TRDPARTY)/mesock -I$(TRDPARTY)/tfs -I$(TRDPARTY)/zlib
CCFLAGSR = -O3 -flto -DNDEBUG=1 -Wall -Wno-uninitialized -Wno-unused-result
CCFLAGSD = -g -Wall
LDDEFS   = 
LDFLAGSR = -O3 -flto
LDFLAGSD = -g
LDLIBS   = -lm

ifeq "$(BCFG)" "debug"
BOUTDIR  = $(OUTDIRD)
CCFLAGS  = $(CCFLAGSD)
LDFLAGS  = $(LDFLAGSD)
STRIP    = - echo No strip - debug 
else
BOUTDIR  = $(OUTDIRR)
CCFLAGS  = $(CCFLAGSR)
LDFLAGS  = $(LDFLAGSR)
endif

ifeq "$(BCOR)" "ne"
BCOR_CDF = -D_NANOEMACS
PRGLIBS  = 
else
BCOR     = me
BCOR_CDF = -D_SOCKET
PRGLIBS  = $(TRDPARTY)/mesock/$(BOUTDIR)/mesock$(A) $(TRDPARTY)/tfs/$(BOUTDIR)/tfs$(A) $(TRDPARTY)/zlib/$(BOUTDIR)/zlib$(A)
endif

#
# The Xlib are located in /usr/X11R6/lib64 on some Linux distributions like RHEL4.* .
#
ifeq "$(strip $(shell uname -m))" "x86_64"
WINDOW_LIBS = $(MAKEWINLIBS) -L/usr/X11R6/lib64 -lX11
else
WINDOW_LIBS = $(MAKEWINLIBS) -L/usr/X11R6/lib -lX11
endif
#
# figure out if termcap is avaiable or if ncurses should be tried to
# accomplish this try to compile test.c and see if it can link termcap. For
# Linux 2.6 then preference would appear to be "ncurses" rather than "termcap".
#
test = $(shell echo "\#include <stdio.h>" > _t.c ; echo "main() { printf(\"HW\n\"); }" >> _t.c ; $(LD) $(LDFLAGS) -o /dev/null -lncurses _t.c 2>&1 ; rm -f _t.c)
ifneq "$(strip $(test))" ""
CONSOLE_LIBS  = -ltermcap
#CONSOLE_LIBS  = -lncurses
else
CONSOLE_LIBS  = -lncurses
endif

ifeq "$(BTYP)" "c"
BTYP_CDF = -D_ME_CONSOLE
BTYP_LIB = $(CONSOLE_LIBS)
else ifeq "$(BTYP)" "w"
BTYP_CDF = $(MAKEWINDEFS) -D_ME_WINDOW -I/opt/X11/include
BTYP_LIB = $(WINDOW_LIBS)
else
BTYP_CDF = $(MAKEWINDEFS) -D_ME_CONSOLE -D_ME_WINDOW -I/opt/X11/include
BTYP_LIB = $(WINDOW_LIBS) $(CONSOLE_LIBS)
BTYP     = cw
endif

OUTDIR   = $(BOUTDIR)-$(BCOR)$(BTYP)
PRGNAME  = $(BCOR)$(BTYP)
PRGFILE  = $(PRGNAME)$(EXE)
PRGHDRS  = ebind.h edef.h eextrn.h efunc.h emain.h emode.h eprint.h esearch.h eskeys.h estruct.h eterm.h evar.h evers.h eopt.h \
	   ebind.def efunc.def eprint.def evar.def etermcap.def emode.def eskeys.def \
	   $(BUILDID).mak
PRGOBJS  = $(OUTDIR)/abbrev.o $(OUTDIR)/basic.o $(OUTDIR)/bind.o $(OUTDIR)/buffer.o $(OUTDIR)/crypt.o $(OUTDIR)/dirlist.o $(OUTDIR)/display.o \
	   $(OUTDIR)/eval.o $(OUTDIR)/exec.o $(OUTDIR)/file.o $(OUTDIR)/fileio.o $(OUTDIR)/frame.o $(OUTDIR)/hilight.o $(OUTDIR)/history.o $(OUTDIR)/input.o \
	   $(OUTDIR)/isearch.o $(OUTDIR)/key.o $(OUTDIR)/line.o $(OUTDIR)/macro.o $(OUTDIR)/main.o $(OUTDIR)/narrow.o $(OUTDIR)/next.o $(OUTDIR)/osd.o \
	   $(OUTDIR)/print.o $(OUTDIR)/random.o $(OUTDIR)/regex.o $(OUTDIR)/region.o $(OUTDIR)/registry.o $(OUTDIR)/search.o $(OUTDIR)/spawn.o \
	   $(OUTDIR)/spell.o $(OUTDIR)/tag.o $(OUTDIR)/termio.o $(OUTDIR)/time.o $(OUTDIR)/undo.o $(OUTDIR)/window.o $(OUTDIR)/word.o \
	   $(OUTDIR)/unixterm.o
#
# Rules
.SUFFIXES: .c .o

$(OUTDIR)/%.o : %.c
	$(CC) $(CCDEFS) $(BCOR_CDF) $(BTYP_CDF) $(CCFLAGS) -c -o $@ $<


all: $(PRGLIBS) $(OUTDIR)/$(PRGFILE)

$(OUTDIR)/$(PRGFILE): $(OUTDIR) $(PRGOBJS) $(PRGLIBS)
	$(RM) $@
	$(LD) $(LDDEFS) $(LDFLAGS) -o $@ $(PRGOBJS) $(PRGLIBS) $(BTYP_LIB) $(LDLIBS)
	$(STRIP) $@

$(PRGOBJS): $(PRGHDRS)

$(OUTDIR):
	[ -d $(OUTDIR) ] || mkdir $(OUTDIR)

$(TRDPARTY)/zlib/$(BOUTDIR)/zlib$(A):
	cd $(TRDPARTY)/zlib && $(MK) -f $(BUILDID).mak BCFG=$(BCFG)

$(TRDPARTY)/tfs/$(BOUTDIR)/tfs$(A):
	cd $(TRDPARTY)/tfs && $(MK) -f $(BUILDID).mak BCFG=$(BCFG)

$(TRDPARTY)/mesock/$(BOUTDIR)/mesock$(A):
	cd $(TRDPARTY)/mesock && $(MK) -f $(BUILDID).mak BCFG=$(BCFG)

clean:
	$(RMDIR) $(OUTDIR)
	cd $(TRDPARTY)/mesock && $(MK) -f $(BUILDID).mak clean
	cd $(TRDPARTY)/tfs && $(MK) -f $(BUILDID).mak clean
	cd $(TRDPARTY)/zlib && $(MK) -f $(BUILDID).mak clean

spotless: clean
	$(RM) *~
	$(RM) tags
	cd $(TRDPARTY)/mesock && $(MK) -f $(BUILDID).mak spotless
	cd $(TRDPARTY)/tfs && $(MK) -f $(BUILDID).mak spotless
	cd $(TRDPARTY)/zlib && $(MK) -f $(BUILDID).mak spotless
