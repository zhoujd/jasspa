# -!- makefile -!- ###########################################################
#
#			MicroEmacs Jasspa Distribution 1999.
#
#  System        : MicroEmacs
#  Description	 : Make file for Openstep 4.2 on NeXT Motorola 040
#  Created       : Sat Jan 24 00:01:40 1998
#
#  Last Modified : <:00204.2347>
#
#  Notes
#	Run "make -f openstep.mak"      for optimised build produces ./me
#	Run "make -f openstep.mak med"  for debug build produces     ./med
#
#	Run "make -f openstep.mak clean"      to clean source directory
#	Run "make -f openstep.mak spotless"   to clean source directory even more
#
#  History
#
##############################################################################
#
# Installation Directory
INSTDIR	      = /usr/local/bin
INSTPROGFLAGS = -s -o root -g root -m 0775
#
# Local Definitions
RM            = rm -f
CC            = cc 
# -posix
LD            = $(CC)
STRIP         =	strip
CDEBUG        =	-g  -Wall
COPTIMISE     =	-O
CDEFS         = -D_NEXT -I.
LIBS          = -ltermcap
LDFLAGS       = 
INSTALL       =	install
#
# Rules
.SUFFIXES: .c .o .od

.c.o:	
	$(CC) $(COPTIMISE) $(CDEFS) $(MAKECDEFS) -c $<

.c.od:	
	$(CC) $(CDEBUG) $(CDEFS) $(MAKECDEFS) -o $@ -c $<
#
# Source files
STDHDR	= ebind.h edef.h eextrn.h efunc.h emain.h emode.h eprint.h \
	  esearch.h eskeys.h estruct.h eterm.h evar.h evers.h \
	  ebind.def efunc.def eprint.def evar.def etermcap.def emode.def eskeys.def
STDSRC	= abbrev.c basic.c bind.c buffer.c crypt.c dirlist.c display.c \
	  eval.c exec.c file.c fileio.c hilight.c history.c input.c \
	  isearch.c key.c line.c macro.c main.c narrow.c next.c osd.c \
	  print.c random.c regex.c region.c registry.c search.c spawn.c \
	  spell.c tag.c termio.c time.c undo.c window.c word.c

PLTHDR  = 
PLTSRC  = unixterm.c
#
# Object files
STDOBJ	= $(STDSRC:.c=.o)	
PLTOBJ  = $(PLTSRC:.c=.o)
OBJ	= $(STDOBJ) $(PLTOBJ)

DSTDOBJ	= $(STDSRC:.c=.od)	
DPLTOBJ = $(PLTSRC:.c=.od)
DOBJ	= $(DSTDOBJ) $(DPLTOBJ)
#
# Targets
all: me

install: me
	$(INSTALL) $(INSTPROGFLAGS) me $(INSTDIR)
	@echo "install done"

clean:
	$(RM) me med *.o *.od core

spotless: clean
	$(RM) tags *~

me:	$(OBJ)
	$(RM) $@
	$(LD) $(LDFLAGS) -o $@ $(OBJ) $(LIBS)
	$(STRIP) $@


med:	$(DOBJ)
	$(RM) $@
	$(LD) $(LDFLAGS) -o $@ $(DOBJ) $(LIBS)
#
# Dependancies
$(STDOBJ): $(STDHDR)
$(PLTOBJ): $(STDHDR) $(PLTHDR)

$(DSTDOBJ): $(STDHDR)
$(DPLTOBJ): $(STDHDR) $(PLTHDR)

