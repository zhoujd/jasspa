# -!- makefile -!-
#
# JASSPA MicroEmacs - www.jasspa.com
# sunos.mak - Makefile for Sun Solaris 9
#
# Copyright (C) 1997-2004 JASSPA (www.jasspa.com)
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
# Author:	Jon Green
# Created:	Mon Jan 5 20:41:16 2004
# Synopsis:	Make file for Sun Solaris
# Notes:
#
#               Build from the command line using nmake.
#
#               make              - builds locally.
#		make clean        - cleans emacs directory
#		make spotless     - cleans some more !!
#		make install	   - builds and copies to c:/emacs.
#
##############################################################################
#
MEVER=		04

RM	=	rm -f
RMDIR   =       rm -rf
CD	=	cd
MKDIR	=	mkdir
CP	=	cp
INSTALL	=	cp
TAR	=	tar
GZIP	=	gzip
ZIP	=	zip -9
RELDIR	=	../release
MEEXE	=	/opt/jasspa/bin/me -n
#
# Documentation components
#
DOCBIN	=
DOCDIR	=
MERELDIR= ../release

SUPERMAN=	$(DOCBIN)superman -b
DROFF	=	$(DOCBIN)droff
NR2RTF	=	$(DOCBIN)nr2rtf
NRAR	=	$(DOCBIN)nrar
NR2HTML	=	$(DOCBIN)nr2html
NR2EHF	=	$(DOCBIN)nr2ehf
NRORDER	=	$(DOCBIN)nrorder
NRSEARCH=	$(DOCBIN)nrsearch
HTS	=	$(DOCBIN)hts
HTS2HTML=	$(DOCBIN)hts2html
HC31	=	hc31
#hc31

# Standard File Extensions
I	=	-I
O	=	o
EXE	=	.exe

##############################################################################
#
# Documentation Build
#
##############################################################################

HTSS	=	me.hts
HLPS	=	me.rtf
SMS	=	me.sm
LIBS	=	me.lbn
PSOS	=	me.pso
EHFS	=	meehf.hts
MANLOG	=	man.log

READTXT	=	readme.txt
FAQTXT	=	faq.txt
BUILDTXT=	build.txt
PATCHTXT=	patch.txt

TXTS	=	$(READTXT) $(FAQTXT) $(BUILDTXT) $(PATCHTXT)

MAN1	=	$(DOCDIR)m1cmd000.1 $(DOCDIR)m1cmd001.1

CMD2_000=	$(DOCDIR)m2cmd000.2 $(DOCDIR)m2cmd001.2 $(DOCDIR)m2cmd002.2\
		$(DOCDIR)m2cmd003.2 $(DOCDIR)m2cmd004.2 $(DOCDIR)m2cmd005.2\
		$(DOCDIR)m2cmd006.2 $(DOCDIR)m2cmd007.2 $(DOCDIR)m2cmd008.2\
		$(DOCDIR)m2cmd009.2
CMD2_010=	$(DOCDIR)m2cmd010.2 $(DOCDIR)m2cmd011.2 $(DOCDIR)m2cmd012.2\
		$(DOCDIR)m2cmd013.2 $(DOCDIR)m2cmd014.2 $(DOCDIR)m2cmd015.2\
		$(DOCDIR)m2cmd016.2 $(DOCDIR)m2cmd017.2 $(DOCDIR)m2cmd018.2\
		$(DOCDIR)m2cmd019.2
CMD2_020=	$(DOCDIR)m2cmd020.2 $(DOCDIR)m2cmd021.2 $(DOCDIR)m2cmd022.2\
		$(DOCDIR)m2cmd023.2 $(DOCDIR)m2cmd024.2 $(DOCDIR)m2cmd025.2\
		$(DOCDIR)m2cmd026.2 $(DOCDIR)m2cmd027.2 $(DOCDIR)m2cmd028.2\
		$(DOCDIR)m2cmd029.2
CMD2_030=	$(DOCDIR)m2cmd030.2 $(DOCDIR)m2cmd031.2 $(DOCDIR)m2cmd032.2\
		$(DOCDIR)m2cmd033.2 $(DOCDIR)m2cmd034.2 $(DOCDIR)m2cmd035.2\
		$(DOCDIR)m2cmd036.2 $(DOCDIR)m2cmd037.2 $(DOCDIR)m2cmd038.2\
		$(DOCDIR)m2cmd039.2
CMD2_040=	$(DOCDIR)m2cmd040.2 $(DOCDIR)m2cmd041.2 $(DOCDIR)m2cmd042.2\
		$(DOCDIR)m2cmd043.2 $(DOCDIR)m2cmd044.2 $(DOCDIR)m2cmd045.2\
		$(DOCDIR)m2cmd046.2 $(DOCDIR)m2cmd047.2 $(DOCDIR)m2cmd048.2\
		$(DOCDIR)m2cmd049.2
CMD2_050=	$(DOCDIR)m2cmd050.2 $(DOCDIR)m2cmd051.2 $(DOCDIR)m2cmd052.2\
		$(DOCDIR)m2cmd053.2 \
		$(DOCDIR)m2cmd056.2 $(DOCDIR)m2cmd057.2 $(DOCDIR)m2cmd058.2\
		$(DOCDIR)m2cmd059.2
CMD2_060=	$(DOCDIR)m2cmd060.2 $(DOCDIR)m2cmd061.2 $(DOCDIR)m2cmd062.2\
		$(DOCDIR)m2cmd063.2 $(DOCDIR)m2cmd064.2 $(DOCDIR)m2cmd065.2\
		$(DOCDIR)m2cmd066.2 $(DOCDIR)m2cmd067.2 $(DOCDIR)m2cmd068.2\
		$(DOCDIR)m2cmd069.2
CMD2_070=	$(DOCDIR)m2cmd070.2 $(DOCDIR)m2cmd071.2 $(DOCDIR)m2cmd072.2\
		$(DOCDIR)m2cmd073.2 $(DOCDIR)m2cmd074.2 $(DOCDIR)m2cmd075.2\
		$(DOCDIR)m2cmd076.2 $(DOCDIR)m2cmd077.2 $(DOCDIR)m2cmd078.2\
		$(DOCDIR)m2cmd079.2
CMD2_080=	$(DOCDIR)m2cmd080.2 $(DOCDIR)m2cmd081.2 $(DOCDIR)m2cmd082.2\
		$(DOCDIR)m2cmd083.2 $(DOCDIR)m2cmd084.2 $(DOCDIR)m2cmd085.2\
		$(DOCDIR)m2cmd086.2 $(DOCDIR)m2cmd087.2 $(DOCDIR)m2cmd088.2\
		$(DOCDIR)m2cmd089.2
CMD2_090=	$(DOCDIR)m2cmd090.2 $(DOCDIR)m2cmd091.2 $(DOCDIR)m2cmd092.2\
		$(DOCDIR)m2cmd093.2 $(DOCDIR)m2cmd094.2 $(DOCDIR)m2cmd095.2\
		$(DOCDIR)m2cmd096.2 $(DOCDIR)m2cmd097.2 $(DOCDIR)m2cmd098.2\
		$(DOCDIR)m2cmd099.2
CMD2_100=	$(DOCDIR)m2cmd100.2 $(DOCDIR)m2cmd101.2 $(DOCDIR)m2cmd102.2\
		$(DOCDIR)m2cmd103.2 $(DOCDIR)m2cmd104.2 $(DOCDIR)m2cmd105.2\
		$(DOCDIR)m2cmd106.2 $(DOCDIR)m2cmd107.2 $(DOCDIR)m2cmd108.2\
		$(DOCDIR)m2cmd109.2
CMD2_110=	$(DOCDIR)m2cmd110.2 $(DOCDIR)m2cmd111.2 $(DOCDIR)m2cmd112.2\
		$(DOCDIR)m2cmd113.2 $(DOCDIR)m2cmd114.2 $(DOCDIR)m2cmd115.2\
		$(DOCDIR)m2cmd116.2 $(DOCDIR)m2cmd117.2 $(DOCDIR)m2cmd118.2\
		$(DOCDIR)m2cmd119.2
CMD2_120=	$(DOCDIR)m2cmd120.2 $(DOCDIR)m2cmd121.2 $(DOCDIR)m2cmd122.2\
		$(DOCDIR)m2cmd123.2 $(DOCDIR)m2cmd124.2 $(DOCDIR)m2cmd125.2\
		$(DOCDIR)m2cmd126.2 $(DOCDIR)m2cmd127.2 $(DOCDIR)m2cmd128.2\
		$(DOCDIR)m2cmd129.2
CMD2_130=	$(DOCDIR)m2cmd130.2 $(DOCDIR)m2cmd131.2 $(DOCDIR)m2cmd132.2\
		$(DOCDIR)m2cmd133.2 $(DOCDIR)m2cmd134.2 $(DOCDIR)m2cmd135.2\
		$(DOCDIR)m2cmd136.2 $(DOCDIR)m2cmd137.2 $(DOCDIR)m2cmd138.2\
		$(DOCDIR)m2cmd139.2
CMD2_140=	$(DOCDIR)m2cmd140.2 $(DOCDIR)m2cmd141.2 $(DOCDIR)m2cmd142.2\
		$(DOCDIR)m2cmd143.2 $(DOCDIR)m2cmd144.2 $(DOCDIR)m2cmd145.2\
		$(DOCDIR)m2cmd146.2 $(DOCDIR)m2cmd147.2 $(DOCDIR)m2cmd148.2\
		$(DOCDIR)m2cmd149.2
CMD2_150=	$(DOCDIR)m2cmd150.2 $(DOCDIR)m2cmd151.2 $(DOCDIR)m2cmd152.2\
		$(DOCDIR)m2cmd153.2 $(DOCDIR)m2cmd154.2 $(DOCDIR)m2cmd155.2\
		$(DOCDIR)m2cmd156.2 $(DOCDIR)m2cmd157.2 $(DOCDIR)m2cmd158.2\
		$(DOCDIR)m2cmd159.2
CMD2_160=	$(DOCDIR)m2cmd160.2 $(DOCDIR)m2cmd161.2 $(DOCDIR)m2cmd162.2\
		$(DOCDIR)m2cmd163.2

CMD2_200=	$(DOCDIR)m2cmd200.2 $(DOCDIR)m2cmd201.2 $(DOCDIR)m2cmd202.2\
		$(DOCDIR)m2cmd203.2

NAR2_000=	$(DOCDIR)m2nar000.2 $(DOCDIR)m2nar001.2	$(DOCDIR)m2nar002.2 \
		$(DOCDIR)m2nar003.2 $(DOCDIR)m2nar004.2 $(DOCDIR)m2nar005.2 \
		$(DOCDIR)m2nar006.2 $(DOCDIR)m2nar007.2 $(DOCDIR)m2nar008.2 \
		$(DOCDIR)m2nar009.2 

MOD2_000=	$(DOCDIR)m2mod000.2 $(DOCDIR)m2mod001.2 $(DOCDIR)m2mod002.2\
		$(DOCDIR)m2mod003.2 $(DOCDIR)m2mod004.2 $(DOCDIR)m2mod005.2\
		$(DOCDIR)m2mod006.2 $(DOCDIR)m2mod007.2 $(DOCDIR)m2mod008.2\
		$(DOCDIR)m2mod009.2
MOD2_010=	$(DOCDIR)m2mod010.2 $(DOCDIR)m2mod011.2 $(DOCDIR)m2mod012.2\
		$(DOCDIR)m2mod013.2 $(DOCDIR)m2mod014.2 $(DOCDIR)m2mod015.2\
		$(DOCDIR)m2mod016.2 $(DOCDIR)m2mod017.2 $(DOCDIR)m2mod018.2\
		$(DOCDIR)m2mod019.2
MOD2_020=	$(DOCDIR)m2mod020.2 $(DOCDIR)m2mod021.2 $(DOCDIR)m2mod022.2\
		$(DOCDIR)m2mod023.2 $(DOCDIR)m2mod024.2 $(DOCDIR)m2mod025.2\
		$(DOCDIR)m2mod026.2 $(DOCDIR)m2mod027.2 $(DOCDIR)m2mod028.2\
		$(DOCDIR)m2mod029.2
MOD2_030=	$(DOCDIR)m2mod030.2 $(DOCDIR)m2mod031.2 $(DOCDIR)m2mod032.2

MAC3_000=	$(DOCDIR)m3mac000.3 $(DOCDIR)m3mac001.3 $(DOCDIR)m3mac002.3\
		$(DOCDIR)m3mac003.3 $(DOCDIR)m3mac004.3 $(DOCDIR)m3mac005.3\
		$(DOCDIR)m3mac006.3 $(DOCDIR)m3mac007.3 $(DOCDIR)m3mac008.3\
		$(DOCDIR)m3mac009.3
MAC3_010=	$(DOCDIR)m3mac010.3 $(DOCDIR)m3mac011.3 $(DOCDIR)m3mac012.3\
		$(DOCDIR)m3mac013.3 $(DOCDIR)m3mac014.3 $(DOCDIR)m3mac015.3\
		$(DOCDIR)m3mac016.3 $(DOCDIR)m3mac017.3 $(DOCDIR)m3mac018.3\
		$(DOCDIR)m3mac019.3
MAC3_020=	$(DOCDIR)m3mac020.3 $(DOCDIR)m3mac021.3 $(DOCDIR)m3mac022.3\
		$(DOCDIR)m3mac023.3 $(DOCDIR)m3mac024.3 $(DOCDIR)m3mac025.3\
		$(DOCDIR)m3mac026.3 $(DOCDIR)m3mac027.3 $(DOCDIR)m3mac028.3\
		$(DOCDIR)m3mac029.3
MAC3_030=	$(DOCDIR)m3mac030.3 $(DOCDIR)m3mac031.3 $(DOCDIR)m3mac032.3\
		$(DOCDIR)m3mac033.3 $(DOCDIR)m3mac034.3 $(DOCDIR)m3mac035.3\
		$(DOCDIR)m3mac036.3 $(DOCDIR)m3mac037.3 $(DOCDIR)m3mac038.3\
		$(DOCDIR)m3mac039.3
MAC3_040=	$(DOCDIR)m3mac040.3 $(DOCDIR)m3mac041.3 $(DOCDIR)m3mac042.3\
		$(DOCDIR)m3mac043.3 $(DOCDIR)m3mac044.3 $(DOCDIR)m3mac045.3\
		$(DOCDIR)m3mac046.3 $(DOCDIR)m3mac047.3 $(DOCDIR)m3mac048.3\
		$(DOCDIR)m3mac049.3
MAC3_050=	$(DOCDIR)m3mac050.3 $(DOCDIR)m3mac051.3 $(DOCDIR)m3mac052.3\
		$(DOCDIR)m3mac053.3 $(DOCDIR)m3mac054.3 $(DOCDIR)m3mac055.3\
		$(DOCDIR)m3mac056.3 $(DOCDIR)m3mac057.3 $(DOCDIR)m3mac058.3\
		$(DOCDIR)m3mac059.3
MAC3_060=	$(DOCDIR)m3mac060.3 $(DOCDIR)m3mac061.3 $(DOCDIR)m3mac062.3\
		$(DOCDIR)m3mac063.3 $(DOCDIR)m3mac064.3

OSD3_000=	$(DOCDIR)m3osd000.3 $(DOCDIR)m3osd001.3 $(DOCDIR)m3osd002.3\
		$(DOCDIR)m3osd003.3 $(DOCDIR)m3osd004.3 $(DOCDIR)m3osd005.3\
		$(DOCDIR)m3osd006.3 $(DOCDIR)m3osd007.3 $(DOCDIR)m3osd008.3\
		$(DOCDIR)m3osd009.3
OSD3_010=	$(DOCDIR)m3osd010.3 $(DOCDIR)m3osd011.3 $(DOCDIR)m3osd012.3

FCL3_000=	$(DOCDIR)m3fcl001.3 $(DOCDIR)m3fcl002.3 $(DOCDIR)m3fcl003.3\
		$(DOCDIR)m3fcl004.3 $(DOCDIR)m3fcl005.3 $(DOCDIR)m3fcl006.3\
		$(DOCDIR)m3fcl007.3 $(DOCDIR)m3fcl008.3 $(DOCDIR)m3fcl009.3

FCL3_010=	$(DOCDIR)m3fcl010.3 $(DOCDIR)m3fcl011.3

FIL4_000=	$(DOCDIR)m4fil000.4 $(DOCDIR)m4fil001.4 $(DOCDIR)m4fil002.4\
		$(DOCDIR)m4fil003.4 $(DOCDIR)m4fil004.4 $(DOCDIR)m4fil005.4\
		$(DOCDIR)m4fil006.4 $(DOCDIR)m4fil007.4 $(DOCDIR)m4fil008.4\
		$(DOCDIR)m4fil009.4
FIL4_010=	$(DOCDIR)m4fil010.4 $(DOCDIR)m4fil011.4 $(DOCDIR)m4fil012.4\
		$(DOCDIR)m4fil013.4 $(DOCDIR)m4fil014.4 $(DOCDIR)m4fil015.4\
		$(DOCDIR)m4fil016.4 $(DOCDIR)m4fil017.4 $(DOCDIR)m4fil018.4\
		$(DOCDIR)m4fil019.4

FIL4_020=	$(DOCDIR)m4fil020.4 $(DOCDIR)m4fil021.4 $(DOCDIR)m4fil022.4\
		$(DOCDIR)m4fil023.4 $(DOCDIR)m4fil024.4 $(DOCDIR)m4fil025.4\
		$(DOCDIR)m4fil026.4 $(DOCDIR)m4fil027.4 $(DOCDIR)m4fil028.4\
		$(DOCDIR)m4fil029.4
FIL4_030=	$(DOCDIR)m4fil030.4 $(DOCDIR)m4fil031.4 $(DOCDIR)m4fil032.4\
		$(DOCDIR)m4fil033.4 $(DOCDIR)m4fil034.4 $(DOCDIR)m4fil035.4\
		$(DOCDIR)m4fil036.4

VAR5_000=	$(DOCDIR)m5var000.5 $(DOCDIR)m5var001.5 $(DOCDIR)m5var002.5\
		$(DOCDIR)m5var003.5 $(DOCDIR)m5var004.5 $(DOCDIR)m5var005.5\
		$(DOCDIR)m5var006.5 $(DOCDIR)m5var007.5 $(DOCDIR)m5var008.5\
		$(DOCDIR)m5var009.5
VAR5_010=	$(DOCDIR)m5var010.5 $(DOCDIR)m5var011.5 $(DOCDIR)m5var012.5\
		$(DOCDIR)m5var013.5 $(DOCDIR)m5var014.5 $(DOCDIR)m5var015.5\
		$(DOCDIR)m5var016.5 $(DOCDIR)m5var017.5 $(DOCDIR)m5var018.5\
		$(DOCDIR)m5var019.5
VAR5_020=	$(DOCDIR)m5var020.5 $(DOCDIR)m5var021.5 $(DOCDIR)m5var022.5\
		$(DOCDIR)m5var023.5 $(DOCDIR)m5var024.5 $(DOCDIR)m5var025.5\
		$(DOCDIR)m5var026.5 $(DOCDIR)m5var027.5 $(DOCDIR)m5var028.5\
		$(DOCDIR)m5var029.5
VAR5_030=	$(DOCDIR)m5var030.5 $(DOCDIR)m5var031.5 $(DOCDIR)m5var032.5\
		$(DOCDIR)m5var033.5 $(DOCDIR)m5var034.5 $(DOCDIR)m5var035.5\
		$(DOCDIR)m5var036.5 $(DOCDIR)m5var037.5 $(DOCDIR)m5var038.5\
		$(DOCDIR)m5var039.5
VAR5_040=	$(DOCDIR)m5var040.5 $(DOCDIR)m5var041.5 $(DOCDIR)m5var042.5\
		$(DOCDIR)m5var043.5 $(DOCDIR)m5var044.5 $(DOCDIR)m5var045.5\
		$(DOCDIR)m5var046.5 $(DOCDIR)m5var047.5 $(DOCDIR)m5var048.5\
		$(DOCDIR)m5var049.5
VAR5_050=	$(DOCDIR)m5var050.5 $(DOCDIR)m5var051.5 $(DOCDIR)m5var052.5\
		$(DOCDIR)m5var053.5 $(DOCDIR)m5var054.5 $(DOCDIR)m5var055.5\
		$(DOCDIR)m5var056.5 $(DOCDIR)m5var057.5 $(DOCDIR)m5var058.5\
		$(DOCDIR)m5var059.5
VAR5_060=	$(DOCDIR)m5var060.5 $(DOCDIR)m5var061.5 $(DOCDIR)m5var062.5\
		$(DOCDIR)m5var063.5 $(DOCDIR)m5var064.5 $(DOCDIR)m5var065.5\
		$(DOCDIR)m5var066.5 $(DOCDIR)m5var067.5 $(DOCDIR)m5var068.5\
		$(DOCDIR)m5var069.5
VAR5_070=	$(DOCDIR)m5var070.5 $(DOCDIR)m5var071.5 $(DOCDIR)m5var072.5\
		$(DOCDIR)m5var073.5 $(DOCDIR)m5var074.5 $(DOCDIR)m5var075.5\
		$(DOCDIR)m5var076.5 $(DOCDIR)m5var077.5 $(DOCDIR)m5var078.5\
		$(DOCDIR)m5var079.5
VAR5_080=	$(DOCDIR)m5var080.5 $(DOCDIR)m5var081.5 $(DOCDIR)m5var082.5\
		$(DOCDIR)m5var083.5 $(DOCDIR)m5var084.5

FIL8_000=	$(DOCDIR)m8fil000.8 $(DOCDIR)m8fil001.8 $(DOCDIR)m8fil002.8\
		$(DOCDIR)m8fil003.8 $(DOCDIR)m8fil004.8 $(DOCDIR)m8fil005.8\
		$(DOCDIR)m8fil006.8

TYP9_000=	$(DOCDIR)m9typ000.9 $(DOCDIR)m9typ001.9 $(DOCDIR)m9typ002.9\
		$(DOCDIR)m9typ003.9 $(DOCDIR)m9typ004.9 $(DOCDIR)m9typ005.9\
		$(DOCDIR)m9typ006.9 $(DOCDIR)m9typ007.9 $(DOCDIR)m9typ008.9\
		$(DOCDIR)m9typ009.9

TYP9_010=	$(DOCDIR)m9typ010.9 $(DOCDIR)m9typ011.9 $(DOCDIR)m9typ012.9\
		$(DOCDIR)m9typ013.9 $(DOCDIR)m9typ014.9 $(DOCDIR)m9typ015.9\
		$(DOCDIR)m9typ016.9 $(DOCDIR)m9typ017.9 $(DOCDIR)m9typ018.9\
		$(DOCDIR)m9typ019.9

TYP9_020=	$(DOCDIR)m9typ020.9 $(DOCDIR)m9typ021.9 $(DOCDIR)m9typ022.9\
		$(DOCDIR)m9typ023.9 $(DOCDIR)m9typ024.9 $(DOCDIR)m9typ025.9\
		$(DOCDIR)m9typ026.9 $(DOCDIR)m9typ027.9 $(DOCDIR)m9typ028.9\
		$(DOCDIR)m9typ029.9

TYP9_030=	$(DOCDIR)m9typ030.9 $(DOCDIR)m9typ031.9 $(DOCDIR)m9typ032.9\
		$(DOCDIR)m9typ033.9 $(DOCDIR)m9typ034.9 $(DOCDIR)m9typ035.9\
		$(DOCDIR)m9typ036.9

TNIS	=	me.tni
TNIS	=

TEMPSMS	=	cmd2_000.sm	cmd2_010.sm	cmd2_020.sm	cmd2_030.sm \
		cmd2_040.sm	cmd2_050.sm	cmd2_060.sm	cmd2_070.sm \
		cmd2_080.sm	cmd2_090.sm	cmd2_100.sm	cmd2_110.sm \
		cmd2_120.sm	cmd2_130.sm	cmd2_140.sm	cmd2_150.sm \
		cmd2_160.sm	cmd2_200.sm \
		nar2_000.sm \
		mod2_000.sm	mod2_010.sm	mod2_020.sm	mod2_030.sm \
		mac3_000.sm	mac3_010.sm	mac3_020.sm	mac3_030.sm \
		mac3_040.sm	mac3_050.sm	mac3_060.sm \
		osd3_000.sm	osd3_010.sm \
		fcl3_000.sm	fcl3_010.sm \
		fil4_000.sm	fil4_010.sm	fil4_020.sm	fil4_030.sm \
		var5_000.sm	var5_010.sm	var5_020.sm	var5_030.sm \
		var5_040.sm	var5_050.sm	var5_060.sm	var5_070.sm \
		var5_080.sm	fil8_000.sm \
		typ9_000.sm	typ9_010.sm	typ9_020.sm	typ9_030.sm \
		faq.2

all:: $(HTSS) $(HLPS) $(SMS) $(EHFS) $(TXTS)
#
# Build the txt readme files.
$(READTXT): m2nar001.2 m2nar003.2 m2nar002.2
	    $(DROFF) -l10000 -c JASSPA -p -o $@ m2nar001.2 m2nar003.2 m2nar002.2
$(BUILDTXT): m2nar007.2
	    $(DROFF) -l10000 -c JASSPA -p -o $@ m2nar007.2
$(FAQTXT):  faq.2
	    $(DROFF) -l10000 -c JASSPA -p -o $@ faq.2
$(PATCHTXT): m2nar009.2
	    $(DROFF) -l10000 -c JASSPA -p -o $@ m2nar009.2

#
# Build the local index page.
#
me.0: me.sm me.nrs
	$(NRSEARCH) -E $(MANLOG) -d -o $@ -n me.nrs me.sm
me.lbn: me.sm me.0
	$(NRAR) -E $(MANLOG) -M me me.0 me.sm
clean::
	$(RM) me.0
	$(RM) me.lbn
	$(RM) me.err
	$(RM) me.rtf

#
# Build the main components.
#
cmd2_000.sm:	$(CMD2_000)
	$(SUPERMAN) -o $@ $(CMD2_000)
cmd2_010.sm:	$(CMD2_010)
	$(SUPERMAN) -o $@ $(CMD2_010)
cmd2_020.sm:	$(CMD2_020)
	$(SUPERMAN) -o $@ $(CMD2_020)
cmd2_030.sm:	$(CMD2_030)
	$(SUPERMAN) -o $@ $(CMD2_030)
cmd2_040.sm:	$(CMD2_040)
	$(SUPERMAN) -o $@ $(CMD2_040)
cmd2_050.sm:	$(CMD2_050)
	$(SUPERMAN) -o $@ $(CMD2_050)
cmd2_060.sm:	$(CMD2_060)
	$(SUPERMAN) -o $@ $(CMD2_060)
cmd2_070.sm:	$(CMD2_070)
	$(SUPERMAN) -o $@ $(CMD2_070)
cmd2_080.sm:	$(CMD2_080)
	$(SUPERMAN) -o $@ $(CMD2_080)
cmd2_090.sm:	$(CMD2_090)
	$(SUPERMAN) -o $@ $(CMD2_090)
cmd2_100.sm:	$(CMD2_100)
	$(SUPERMAN) -o $@ $(CMD2_100)
cmd2_110.sm:	$(CMD2_110)
	$(SUPERMAN) -o $@ $(CMD2_110)
cmd2_120.sm:	$(CMD2_120)
	$(SUPERMAN) -o $@ $(CMD2_120)
cmd2_130.sm:	$(CMD2_130)
	$(SUPERMAN) -o $@ $(CMD2_130)
cmd2_140.sm:	$(CMD2_140)
	$(SUPERMAN) -o $@ $(CMD2_140)
cmd2_150.sm:	$(CMD2_150)
	$(SUPERMAN) -o $@ $(CMD2_150)
cmd2_160.sm:	$(CMD2_160)
	$(SUPERMAN) -o $@ $(CMD2_160)
cmd2_200.sm:	$(CMD2_200)
	$(SUPERMAN) -o $@ $(CMD2_200)

mod2_000.sm:	$(MOD2_000)
	$(SUPERMAN) -o $@ $(MOD2_000)
mod2_010.sm:	$(MOD2_010)
	$(SUPERMAN) -o $@ $(MOD2_010)
mod2_020.sm:	$(MOD2_020)
	$(SUPERMAN) -o $@ $(MOD2_020)
mod2_030.sm:	$(MOD2_030)
	$(SUPERMAN) -o $@ $(MOD2_030)

mac3_000.sm:	$(MAC3_000)
	$(SUPERMAN) -o $@ $(MAC3_000)
mac3_010.sm:	$(MAC3_010)
	$(SUPERMAN) -o $@ $(MAC3_010)
mac3_020.sm:	$(MAC3_020)
	$(SUPERMAN) -o $@ $(MAC3_020)
mac3_030.sm:	$(MAC3_030)
	$(SUPERMAN) -o $@ $(MAC3_030)
mac3_040.sm:	$(MAC3_040)
	$(SUPERMAN) -o $@ $(MAC3_040)
mac3_050.sm:	$(MAC3_050)
	$(SUPERMAN) -o $@ $(MAC3_050)
mac3_060.sm:	$(MAC3_060)
	$(SUPERMAN) -o $@ $(MAC3_060)

osd3_000.sm:	$(OSD3_000)
	$(SUPERMAN) -o $@ $(OSD3_000)
osd3_010.sm:	$(OSD3_010)
	$(SUPERMAN) -o $@ $(OSD3_010)

fcl3_000.sm:	$(FCL_000)
	$(SUPERMAN) -o $@ $(FCL3_000)
fcl3_010.sm:	$(FCL_010)
	$(SUPERMAN) -o $@ $(FCL3_010)

fil4_000.sm:	$(FIL4_000)
	$(SUPERMAN) -o $@ $(FIL4_000)
fil4_010.sm:	$(FIL4_010)
	$(SUPERMAN) -o $@ $(FIL4_010)
fil4_020.sm:	$(FIL4_020)
	$(SUPERMAN) -o $@ $(FIL4_020)
fil4_030.sm:	$(FIL4_030)
	$(SUPERMAN) -o $@ $(FIL4_030)

var5_000.sm:	$(VAR5_000)
	$(SUPERMAN) -o $@ $(VAR5_000)
var5_010.sm:	$(VAR5_010)
	$(SUPERMAN) -o $@ $(VAR5_010)
var5_020.sm:	$(VAR5_020)
	$(SUPERMAN) -o $@ $(VAR5_020)
var5_030.sm:	$(VAR5_020)
	$(SUPERMAN) -o $@ $(VAR5_030)
var5_040.sm:	$(VAR5_040)
	$(SUPERMAN) -o $@ $(VAR5_040)
var5_050.sm:	$(VAR5_050)
	$(SUPERMAN) -o $@ $(VAR5_050)
var5_060.sm:	$(VAR5_060)
	$(SUPERMAN) -o $@ $(VAR5_060)
var5_070.sm:	$(VAR5_070)
	$(SUPERMAN) -o $@ $(VAR5_070)
var5_080.sm:	$(VAR5_080)
	$(SUPERMAN) -o $@ $(VAR5_080)

fil8_000.sm:	$(FIL8_000)
	$(SUPERMAN) -o $@ $(FIL8_000)

typ9_000.sm:	$(FIL9_000)
	$(SUPERMAN) -o $@ $(TYP9_000)
typ9_010.sm:	$(FIL9_010)
	$(SUPERMAN) -o $@ $(TYP9_010)
typ9_020.sm:	$(FIL9_020)
	$(SUPERMAN) -o $@ $(TYP9_020)
typ9_030.sm:	$(FIL9_030)
	$(SUPERMAN) -o $@ $(TYP9_030)

nar2_000.sm:	$(NAR2_000)
	$(SUPERMAN) -o $@ $(NAR2_000)

me.sm: $(TEMPSMS) $(TNIS) $(MAN1)
	$(SUPERMAN) -o $@ $(TNIS) $(MAN1) $(TEMPSMS)

me.hts: me.sm me.0 me.lbn me.tni forward.lbn logo.png
	$(NR2HTML) -E $(MANLOG)			\
		   -c JASSPA			\
		   -x -M me			\
		   -n me			\
		   -H me			\
		   -L. -lme -l$(DOCDIR)forward	\
		   -tCommands:CommandGlossary	\
		   -tVariables:VariableGlossary	\
		   -tMacro-Dev:MacroGlossary	\
		   -tGlossary:Glossary		\
		   me.sm me.0
	$(HTS) -a $@ -E $(MANLOG) -s me/logo -b logo.png
	- $(RM) *.so

me.rtf: me.sm me.0 me.lbn me.tni forward.lbn me.hpj logo.bmp
	$(NR2RTF)  -E $(MANLOG)			\
		   -c JASSPA			\
		   -x -M me			\
		   -n me			\
		   -H me			\
		   -L. -lme -l$(DOCDIR)forward	\
		   -tHome:me			\
		   -tCommands:CommandGlossary   \
		   -tVariables:VariableGlossary \
		   -tMacro-Dev:MacroGlossary    \
		   -tGlossary:Glossary		\
		    me.sm me.0
#	$(HC31) me.hpj
#	- $(RM) me.rtf
#	- $(RM) *.so
#me.ehf: me.sm me.tni
#	$(NR2EHF)  -E $(MANLOG)			\
#		   -o $@ me.sm

meehf.hts: me.sm me.0 me.lbn me.tni forward.lbn logo.png
	$(NR2HTML) -E $(MANLOG)				\
		   -b -c JASSPA				\
		   -x -M me				\
		   -n me -o meehf.hts			\
		   -H me				\
		   -L. -lme -l$(DOCDIR)forward		\
		   -t Commands:CommandGlossary		\
		   -t Variables:VariableGlossary	\
		   -t Macro-Dev:MacroGlossary		\
		   -t Glossary:Glossary			\
		    me.sm me.0
	$(HTS) -a $@ -E $(MANLOG) -s me/logo -b logo.png
	- $(RM) *.so

#
# Build the postscript print job ordering files.
#
me.pso:	me.sm me.tni
		$(NRORDER) $(NROFLAGS) -z -o $@ me.sm
#		$(NRORDER) $(NROFLAGS) -z -o $@ -i me me.sm
RELDOCFILES=	$(MERELDIR)/me.rtf \
		$(MERELDIR)/me.prj \
		$(MERELDIR)/logo.bmp \
		$(MERELDIR)/readme.txt \
		$(RELDIR)/html.z

$(RELDIR)/html.z: me.hts
		$(MKDIR) html
		$(HTS2HTML) -p html -l ".html" me.hts
		$(RUNBINDIR)/icomp -i html/*.* $(@F)
		$(INSTALL) $(@F) $(@D)
		$(RMDIR) html

$(MERELDIR)/me.rtf: me.rtf
		$(INSTALL) $(@F) $(@D)
$(MERELDIR)/me.prj: me.prj
		$(INSTALL) $(@F) $(@D)
$(MERELDIR)/logo.bmp: logo.bmp
		$(INSTALL) $(@F) $(@D)
$(MERELDIR)/readme.txt: readme.txt
		$(INSTALL) $(@F) $(@D)
		crlf32 -po $@
clean::
	$(RM) *.sm
	$(RM) $(MANLOG)
	$(RM) *.hts
	$(RM) *.pso
	$(RM) ehf/me/*.htm
	$(RM) ehf/me/*.ehf
	$(RM) ehf/*.htm
	$(RM) ehf/*.gif
	$(RM) htm/me/*.htm
	$(RM) htm/me/*.gif
	$(RM) htm/*.htm
	$(RM) htm/*.zip
	$(RM) html/me/*.html
	$(RM) html/me/*.gif
	$(RM) html/*.html
	$(RM) html/*.tar.gz
	$(RM) me.lbn
	$(RM) me.fts
	$(RM) me.ehf
	$(RM) mehtm.zip
	$(RM) mehtml.tgz
	$(RM) me.rtf
	$(RM) me.gid
	$(RM) $(READTXT)
	$(RM) $(BUILDTXT)
	$(RM) $(FAQTXT)

spotless:: clean
	$(RM) *~
	$(RM) core
	$(RM) *.log		
	$(RM) tags
	$(RMDIR) ehf/me
	$(RMDIR) ehf
	$(RMDIR) htm/me
	$(RMDIR) htm
	$(RMDIR) html/me
	$(RMDIR) html

me.ehf: meehf.hts
	- $(MKDIR) ehf
	- $(MKDIR) ehf/me
	$(RM) ehf/me/*.htm
	$(RM) ehf/me/me.ehf
	cd ehf; $(HTS2HTML) -l .htm ../meehf.hts
	$(CP) ehf/me.htm ehf/me/1.htm
	cd ehf/me; $(MEEXE) "@ehftools" *.htm
	$(CP) ehf/me/me.ehf .

mehtm.zip: me.hts
	- $(MKDIR) htm
	- $(MKDIR) htm/me
	$(RM) htm/*.htm
	$(RM) htm/*.zip
	$(RM) htm/me/*.gif
	$(RM) htm/me/*.htm
	cd htm; $(HTS2HTML) -l .htm ../me.hts
	cd htm; $(ZIP) mehtm me.htm me/*
	mv htm/mehtm.zip .

mehtml.tgz: me.hts
	- $(MKDIR) html
	- $(MKDIR) html/me
	$(RM) html/*.html
	$(RM) html/*.tar.gz
	$(RM) html/me/*.gif
	$(RM) html/me/*.html
	cd html; $(HTS2HTML) -l .html ../me.hts
	cd html; $(TAR) cvf mehtml.tar me.html me
	cd html; $(GZIP) -9 mehtml.tar
	$(CP) html/mehtml.tar.gz mehtml.tgz

#mewinhlp.zip: me.hlp
#	$(ZIP) mewinhlp me.hlp

full:: mehtm.zip mehtml.tgz $(HLPS) me.ehf $(TXTS)

release:: all full
	- $(MKDIR) $(RELDIR)
	$(CP) readme.txt ../src
	$(CP) readme.txt ../macros
	$(CP) readme.txt $(RELDIR)
	$(CP) patch.txt ../src
	$(CP) patch.txt  $(RELDIR)
	$(CP) faq.txt ../src
	$(CP) faq.txt ../macros
	$(CP) faq.txt $(RELDIR)
	$(CP) build.txt ../src
	$(CP) build.txt ../macros
	$(CP) build.txt $(RELDIR)
	$(CP) me.ehf ../macros
	$(CP) mehtml.tgz $(RELDIR)/mehtml.tar.gz
	$(CP) mehtm.zip $(RELDIR)/mehtm.zip
	$(GZIP) -9 -c  me.ehf > me.ehf.gz
	$(ZIP) meehf.zip me.ehf
	$(CP) me.ehf.gz $(RELDIR)
	$(CP) meehf.zip $(RELDIR)

spotless::
	$(RM) *.zip
	$(RM) *.gz
	$(RM) *.tgz
