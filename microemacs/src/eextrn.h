/* -*- c -*-
 *
 * JASSPA MicroEmacs - www.jasspa.com
 * eextrn.h - External function definitions.
 *
 * Copyright (C) 1988-2002 JASSPA (www.jasspa.com)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */
/*
 * Created:     Unknown
 * Synopsis:    External function definitions.
 * Authors:     Jon Green & Steven Phillips
 * Notes:
 *     The external command function declarations were part of efunc.h,
 *     these have all been moved to this header file and all other
 *     exeternally used functions have also been added.
 */

#if	!(defined __ABBREVC) || (defined _ANSI_C)		/* abbrev.c externals */
#if MEOPT_ABBREV
extern	int	bufferAbbrev APRAM((int f, int n));
extern	int	globalAbbrev APRAM((int f, int n));
extern	int	expandAbbrev APRAM((int f, int n));
#else
#define bufferAbbrev notAvailable
#define globalAbbrev notAvailable
#define expandAbbrev notAvailable
#endif
#endif
/*
 * basic.c
 */
#if	!(defined __BASICC) || (defined _ANSI_C)		/* basic.c externals */
extern	int	windowGotoBol APRAM((int f, int n));
extern	int	windowGotoEol APRAM((int f, int n));
extern	int	meErrorEob APRAM((void)) ;
extern	int	meErrorBob APRAM((void)) ;
extern	int	meWindowBackwardChar APRAM((register meWindow *wp, register int n)) ;
extern	int	meWindowForwardChar APRAM((register meWindow *wp, register int n)) ;
extern	int	windowBackwardChar APRAM((int f, int n));
extern	int	windowForwardChar APRAM((int f, int n));
extern	meUByte meWindowGetChar APRAM((meWindow *wp)) ;
extern	int	windowGotoLine APRAM((int f, int n));
#if MEOPT_NARROW
extern	int	windowGotoAbsLine APRAM((meInt line)) ;
#else
#define windowGotoAbsLine(l) windowGotoLine(1,l)
#endif
extern	int	windowGotoBob APRAM((int f, int n));
extern	int	windowGotoEob APRAM((int f, int n));
extern	int	windowForwardLine APRAM((int f, int n));
extern	int	windowBackwardLine APRAM((int f, int n));
#if	MEOPT_WORDPRO
extern	int	windowBackwardParagraph APRAM((int f, int n));
extern	int	windowForwardParagraph APRAM((int f, int n));
#else
#define windowBackwardParagraph notAvailable
#define windowForwardParagraph notAvailable
#endif
extern	int	windowSetMark APRAM((int f, int n));
extern	int	windowSwapDotAndMark APRAM((int f, int n));
extern	void	meBufferStoreLocation APRAM((meLine *lp, meUShort lo, meInt ln)) ;
extern	void	meBufferUpdateLocation APRAM((meBuffer *bp, meUInt noLines, meUShort newOff)) ;
#endif
/*
 * bind.c
 */
#if !(defined __BINDC) || (defined _ANSI_C)		/* bind.c externals */
extern  meUInt  cmdHashFunc APRAM((register meUByte *cmdName)) ;
extern  meUShort  meGetKeyFromString APRAM((meUByte **tp)) ;
extern  meUShort  meGetKey APRAM((int flag)) ;
extern  int     meGetStringFromChar APRAM((meUShort cc, meUByte *d)) ;
extern	void	meGetStringFromKey APRAM((meUShort cc, meUByte *seq));
extern	int     decode_fncname APRAM((meUByte *fname, int silent));
extern	int	bindkey APRAM((meUByte *prom, int f, int n, meUShort *lclNoBinds,
                               meBind **lclBinds)) ;
extern	int	unbindkey APRAM((meUByte *prom, int n, meUShort *lclNoBinds,
                                 meBind **lclBinds)) ;
extern	int	globalBindKey APRAM((int f, int n));
extern	int	globalUnbindKey APRAM((int f, int n));
#if MEOPT_LOCALBIND
extern	int	bufferBindKey APRAM((int f, int n));
extern	int	bufferUnbindKey APRAM((int f, int n));
extern	int	mlBind APRAM((int f, int n));
extern	int	mlUnbind APRAM((int f, int n));
#else
#define bufferBindKey notAvailable
#define bufferUnbindKey notAvailable
#define mlBind notAvailable
#define mlUnbind notAvailable
#endif
extern	int	descBindings APRAM((int f, int n));
extern	int	descKey APRAM((int f,int n));
#if MEOPT_EXTENDED
extern	int	listCommands APRAM((int f, int n));
extern	int	commandApropos APRAM((int f, int n));
extern  void    charMaskTblInit APRAM((void));
extern	int	setCharMask APRAM((int f, int n));
#else
#define listCommands notAvailable
#define commandApropos notAvailable
#define setCharMask notAvailable
#endif
#endif
/*
 * buffer.c
 */
#if !(defined __BUFFERC) || (defined _ANSI_C)		/* buffer.c externals */
extern  int     getBufferName APRAM((meUByte *prompt, int opt, int defH, meUByte *buf)) ;
#if MEOPT_FILEHOOK
extern int      addFileHook APRAM((int f, int n)) ;
#else
#define addFileHook notAvailable
#endif
#if MEOPT_EXTENDED
extern	int	nextWndFindBuf APRAM((int f, int n));
#else
#define nextWndFindBuf notAvailable
#endif
extern	int	findBuffer APRAM((int f, int n));
extern	int	nextBuffer APRAM((int f, int n));
extern	int	swbuffer APRAM((meWindow *wp, meBuffer *bp));
extern	int	delBuffer APRAM((int f, int n));
extern	int	delSomeBuffers APRAM((int f, int n));
extern  meBuffer *replacebuffer APRAM((meBuffer *oldbuf)) ;
extern	int	HideBuffer APRAM((meBuffer *bp)) ;
extern  void    linkBuffer APRAM((meBuffer *bp)) ;
extern  void    unlinkBuffer APRAM((meBuffer *bp)) ;
extern	int	zotbuf APRAM((meBuffer *bp, int silent));
extern	int	changeBufName APRAM((int f,int n));
extern	int	adjustMode APRAM((meBuffer *bp, int nn));
extern	int	bufferMode APRAM((int f, int n));
extern	int	globalMode APRAM((int f, int n));
extern	int	namedBufferMode APRAM((int f, int n));
extern  int     addLine APRAM((register meLine *ilp, meUByte *text)) ;
#define addLineToBob(bp,text) (bp->lineCount += addLine(bp->dotLine,text))
#define addLineToEob(bp,text) (bp->lineCount += addLine(bp->baseLine,text))
extern	int	listBuffers APRAM((int f, int n));
extern  int     bufferNeedSaving APRAM((meBuffer *bp)) ;
extern	int	anyChangedBuffer APRAM((void));
extern  meBuffer *createBuffer APRAM((register meUByte *bname)) ;
#define BFND_CREAT  0x01
#define BFND_BINARY 0x02
#define BFND_CRYPT  0x04
#define BFND_RBIN   0x08
#define BFND_MKNAM  0x10
#define BFND_CLEAR  0x20

extern	meBuffer *bfind APRAM((meUByte *bname, int cflag));
extern	void    resetBufferWindows APRAM((meBuffer *bp)) ;
extern	int	bclear APRAM((meBuffer *bp));
extern  int     getBufferInfo APRAM((meInt *,meInt *,meInt *,meInt *)) ;
#endif
/*
 * crypt.c
 */
#if !(defined __CRYPTC) || (defined _ANSI_C)				/* crypt.c externals */
#if MEOPT_CRYPT
extern  int	setBufferCryptKey APRAM((meBuffer *bp, meUByte *key)) ;
extern	int	setCryptKey APRAM((int f, int n));
extern	int	meCrypt APRAM((meUByte *bptr, meUInt len));
#else
#define setCryptKey notAvailable
#define meCrypt notAvailable
#endif
#endif
/*
 * dir.c        - Directory handling
 */
#if !(defined __DIRLISTC) || (defined _ANSI_C)		/* dirlist.c externals */
#if MEOPT_DIRLIST
extern int directoryTree APRAM((int f, int n));
#else
#define directoryTree notAvailable
#endif
#endif
/*
 * display.c
 */
#if !(defined __DISPLAYC) || (defined _ANSI_C)		/* display.c externals */
#if MEOPT_COLOR
extern  int	addColor APRAM((int f, int n)) ;
extern	int	addColorScheme APRAM((int f, int n));
#else
#define addColor notAvailable
#define addColorScheme notAvailable
#endif
extern  int     convertUserScheme APRAM((int n, int defaultScheme));
extern  meUByte assessModeLine APRAM((meUByte *ml)) ;
extern  void    windCurLineOffsetEval APRAM((meWindow *wp)) ;
extern  void    reframe APRAM((meWindow *wp)) ;
extern  void    updCursor APRAM((register meWindow *wp)) ;
extern  int     renderLine APRAM((meUByte *s, int len, int wid));
extern	int	screenUpdate APRAM((int f, int n));
extern	int	update APRAM((int force));
extern	void	updone APRAM((meWindow *wp));
extern	void	updall APRAM((meWindow *wp));
#if MEOPT_EXTENDED
extern	int     showCursor APRAM((int f, int n)) ;
extern	int     showRegion APRAM((int f, int n)) ;
#else
#define showCursor notAvailable
#define showRegion notAvailable
#endif

#define POKE_NOMARK   0x01      /* Don't mark the poke */
#define POKE_NOFLUSH  0x02      /* Don't flush the poke */
#if MEOPT_POKE
#define POKE_COLORS   0x04      /* The fore & back args are color strings */
#endif
extern  void    pokeScreen APRAM((int flags, int row, int col, meUByte *scheme, meUByte *str)) ;
#if MEOPT_POKE
extern	int	screenPoke APRAM((int f, int n)) ;
#else
#define screenPoke notAvailable
#endif
extern  void    menuline APRAM((int flag));

extern  int     renderLine APRAM((meUByte *s1, int len, int wid)) ;

/* Virtual video interfaces */
extern  int     meVideoAttach  APRAM ((meVideo *vvptr, meWindow *wp));
extern  void    meVideoDetach  APRAM ((meWindow *wp));

extern  int     doRedrawEvent APRAM ((void));

#define MWCURSOR   0x001
#define MWSPEC     0x002
#define MWABORT    0x004
#define MWPAUSE    0x008
#define MWWAIT     0x010
#define MWCLEXEC   0x020
#define MWCLWAIT   0x040
#define MWUSEMLCOL 0x080
#define MWERASE    0x100

extern  int     mlwrite APRAM((int flags, meUByte *fmt, ...)) ;
#endif
/*
 * eval.c
 */
#if !(defined __EVALC) || (defined _ANSI_C)		/* eval.c externals */
extern  int     regexStrCmp APRAM((meUByte *str, meUByte *reg, int exact)) ;
extern	meUByte  *gtfun APRAM((meUByte *fname));
extern  meUByte  *getUsrLclCmdVar APRAM((meUByte *vname, register meVarList *varList)) ;
#define getUsrVar(vname) getUsrLclCmdVar(vname,&usrVarList)
extern	meVariable *SetUsrLclCmdVar APRAM((meUByte *vname, meUByte *vvalue,
                                           register meVarList *varList)) ;
extern	int	setVar APRAM((meUByte *vname, meUByte *vvalue, meRegister *regs)) ;
extern	int	setVariable APRAM((int f, int n));
extern	int	unsetVariable APRAM((int f, int n));
extern	meUByte *meItoa APRAM((int i));
extern	meUByte *getval APRAM((meUByte *token));
extern	int	descVariable APRAM((int f, int n));
#if MEOPT_EXTENDED
extern	int	listVariables APRAM((int f, int n));
#else
#define listVariables notAvailable
#endif
#endif
/*
 * exec.c
 */
#if !(defined __EXECC) || (defined _ANSI_C)		/* exec.c externals */
extern	int	mePushRegisters APRAM((int flags));
extern	int	mePopRegisters APRAM((int flags));
extern  int     biChopFindString APRAM((register meUByte *ss, register int len, 
                                        register meUByte **tbl, register int size)) ;
extern	int	execFunc APRAM((int index, int f, int n)) ;
extern  void    execFuncHidden APRAM((int keyCode, int index, meUInt arg)) ;
#define meEBF_ARG_GIVEN   0x01
#define meEBF_HIDDEN      0x02
extern  void    execBufferFunc APRAM((meBuffer *bp, int index, int flags, int n)) ;
extern  int     lineExec APRAM((int f, int n, meUByte *cmdstr));
/* Note  tok (the destination token string) must be meTOKENBUF_SIZE_MAX in size, 
 * returning a string no bigger than meBUF_SIZE_MAX with the \0 */
extern	meUByte  *token APRAM((meUByte *src, meUByte *tok));
extern	int	macarg APRAM((meUByte *tok));
extern  int     meGetString APRAM((meUByte *prompt, int option, int defnum,
                               meUByte *buffer, int size));
extern	int	storemac APRAM((int f, int n));
extern	int	dofile APRAM((meUByte *fname, int f, int n));
extern	int	execCommand APRAM((int f, int n));
extern	int	execLine APRAM((int f, int n));
extern	int	execBuffer APRAM((int f, int n));
extern	int	execFile APRAM((int f, int n));
extern	int	startup APRAM((meUByte *sfname));
#endif
/*
 * file.c
 */
#if !(defined __FILEC) || (defined _ANSI_C)		/* file.c externals */
extern  int fnamecmp APRAM((meUByte *f1, meUByte *f2)) ;
#define meFILETYPE_NASTY      0
#define meFILETYPE_REGULAR    1
#define meFILETYPE_DIRECTORY  2
#define meFILETYPE_NOTEXIST   3
#define meFILETYPE_HTTP       4
#define meFILETYPE_FTP        5
extern  int getFileStats APRAM((meUByte *file, int flag, meStat *stats, meUByte *lname)) ;
extern  void set_dirs APRAM((void)) ;
#define meFL_CHECKDOT    0x01
#define meFL_USESRCHPATH 0x02
#define meFL_USEPATH     0x04
#define meFL_EXEC        0x08
extern	int fileLookup APRAM((meUByte *fname, meUByte *ext, meUByte flags, meUByte *outName)) ;
extern	int executableLookup APRAM((meUByte *fname, meUByte *outName)) ;
extern  int bufferOutOfDate APRAM((meBuffer *bp)) ;
extern	meUByte *gwd APRAM((meUByte drive));
extern  meUByte *getFileBaseName APRAM((meUByte *fname)) ;
extern  void  getFilePath APRAM((meUByte *fname, meUByte *path)) ;
extern  int inputFileName APRAM((meUByte *prompt, meUByte *fn, int corFlag));
extern	int ifile APRAM((meBuffer *bp, meUByte *fname, meUInt flags));
extern	int insFile APRAM((int f, int n));
extern  int findFileList APRAM((meUByte *seed, int bflag, meInt lineno)) ;
extern  int findSwapFileList APRAM((meUByte *seed, int bflag, meInt lineno)) ;
extern	int findFile APRAM((int f, int n));
extern	int readFile APRAM((int f, int n));
extern	int viewFile APRAM((int f, int n));
extern  void freeFileList APRAM((int noStr, meUByte **files)) ;
#if MEOPT_CRYPT
extern	int	resetkey APRAM((meBuffer *bp));
#endif
extern	int	readin APRAM((meBuffer *, meUByte *fname));
extern	meUByte   makename APRAM((meUByte *bname, meUByte *fname));
extern  void    autowriteout APRAM((register meBuffer *bp)) ;
extern  void    autowriteremove APRAM((meBuffer *bp)) ;
#if MEOPT_EXTENDED
extern	int     nextWndFindFile APRAM((int f, int n));
extern	int     fileOp APRAM((int f, int n));
extern	int	appendBuffer APRAM((int f, int n));
#else
#define nextWndFindFile notAvailable
#define fileOp notAvailable
#define appendBuffer notAvailable
#endif
extern	int	writeBuffer APRAM((int f, int n));
extern	int	saveBuffer APRAM((int f, int n));
extern  int     writeOut APRAM((register meBuffer *bp, meUInt flags, meUByte *fn)) ;
extern	int	writeout APRAM((meBuffer *bp, int flags, meUByte *fn));
extern	int     saveSomeBuffers APRAM((int f, int n)) ;
extern	void	resetBufferNames APRAM((meBuffer *bp, meUByte *fname));
extern	int	changeFileName APRAM((int f, int n));
extern	int	changeDir APRAM((int f, int n));
#ifdef _CONVDIR_CHAR
extern  void    fileNameConvertDirChar APRAM((meUByte *fname)) ;
#else
#define fileNameConvertDirChar(ff)
#endif
#define PATHNAME_COMPLETE 0
#define PATHNAME_PARTIAL  1
extern  void    pathNameCorrect APRAM((meUByte *oldName, int nameType, 
                                       meUByte *newName, meUByte **baseName)) ;
#ifdef _WIN32
extern  void    fileNameCorrect APRAM((meUByte *oldName, meUByte *newName, meUByte **baseName)) ;
#else
#define fileNameCorrect(o,n,b) pathNameCorrect(o,PATHNAME_COMPLETE,n,b)
#endif
extern  void    getDirectoryList APRAM((meUByte *pathName, meDirList *dirList)) ;

#endif

/*
 * fileio.c
 */
#if !(defined __FILEIOC) || (defined _ANSI_C)		/* fileio.c externals */
#define meRWFLAG_FMOD      0x0000ffff
#define meRWFLAG_SILENT    0x00010000
#define meRWFLAG_READ      0x00020000
#define meRWFLAG_INSERT    0x00040000
#define meRWFLAG_WRITE     0x00080000
#define meRWFLAG_BACKUP    0x00100000
#define meRWFLAG_OPENEND   0x00200000
#define meRWFLAG_OPENTRUNC 0x00400000
#define meRWFLAG_AUTOSAVE  0x00800000
#define meRWFLAG_CHKBREAK  0x01000000
#define meRWFLAG_IGNRNRRW  0x02000000
/* following using in ffCopyFile to remove the source file & create a dir */
#define meRWFLAG_DELETE    0x04000000
#define meRWFLAG_MKDIR     0x08000000
#define meRWFLAG_FTPCLOSE  0x10000000
#define meRWFLAG_NOCONSOLE 0x20000000

extern int      ffReadFile APRAM((meUByte *fname, meUInt flags, meBuffer *bp, meLine *hlp)) ;
#define meBACKUP_CREATE_PATH 0x0001
extern int      createBackupName APRAM((meUByte *filename, meUByte *fn, meUByte backl, int flag)) ;


extern int      ffWriteFileOpen APRAM((meUByte *fname, meUInt flags, meBuffer *bp)) ;
extern int      ffWriteFileWrite APRAM((register int len, 
                                        register meUByte *buff, int eolFlag)) ;
extern int      ffWriteFileClose APRAM((meUByte *fname, meUInt flags, meBuffer *bp)) ;
extern int      ffWriteFile APRAM((meUByte *fname, meUInt flags, meBuffer *bp)) ;
extern int	ffFileOp APRAM((meUByte *sfname, meUByte *dfname, meUInt dFlags)) ;
#endif
/*
 * frame.c
 */
#if !(defined __FRAMEC) || (defined _ANSI_C)		/* frame.c externals */
extern	int     meFrameChangeWidth APRAM((meFrame *frame, int ww));
extern	int     meFrameChangeDepth APRAM((meFrame *frame, int hh));
extern	int	frameChangeDepth APRAM((int f, int n));
extern	int	frameChangeWidth APRAM((int f, int n));
extern	meFrame *meFrameInit APRAM((meFrame *sibling)) ;
extern	int     meFrameInitWindow APRAM((meFrame *frame, meBuffer *buffer)) ;
#if MEOPT_FRAME
extern	int	meFrameDelete APRAM((meFrame *frame, int flags));
extern	int	frameCreate APRAM((int f, int n));
extern	int	frameDelete APRAM((int f, int n));
extern	int	frameNext APRAM((int f, int n));

#define meFrameLoopBegin()                                                   \
do {                                                                         \
    meFrame *loopFrame=frameList;                                            \
    for(loopFrame=frameList ; loopFrame!=NULL ; loopFrame=loopFrame->next)   \
    {

#define meFrameLoopContinue(cond)                                            \
if( cond ) continue

#define meFrameLoopBreak(cond)                                               \
if( cond ) break

#define meFrameLoopEnd()                                                     \
    }                                                                        \
} while(0)

#else
#define frameCreate notAvailable
#define frameDelete notAvailable
#define frameNext notAvailable

#define loopFrame frameCur
#define meFrameLoopBegin()
#define meFrameLoopContinue(cond)
#define meFrameLoopBreak(cond)
#define meFrameLoopEnd()

#endif

#endif
/*
 * input.c
 */
#if !(defined __INPUTC) || (defined _ANSI_C)		/* input.c externals */
#define mlCR_LOWER_CASE     0x01
#define mlCR_QUIT_ON_USER   0x02
#define mlCR_UPDATE_ON_USER 0x04
#define mlCR_CURSOR_IN_MAIN 0x08
#define mlCR_QUOTE_CHAR     0x10
#define mlCR_ALPHANUM_CHAR  0x20
extern	int     mlCharReply APRAM((meUByte *prompt, int mask, meUByte *validList, meUByte *helpStr)) ;
extern	int	mlyesno APRAM((meUByte *prompt));
extern	void	mlDisp APRAM((meUByte *prompt, meUByte *buf, meUByte *cont, int cpos)) ;
extern	int	getexecCommand APRAM((void));
#define meGETKEY_SILENT     0x01
#define meGETKEY_SINGLE     0x02
#define meGETKEY_COMMAND    0x04
extern	meUShort  meGetKeyFromUser APRAM((int f, int n, int flag));
extern  int     createBuffList APRAM((meUByte ***listPtr, int noHidden)) ;
extern  int     createCommList APRAM((meUByte ***listPtr, int noHidden)) ;
extern  int     createVarList  APRAM((meUByte ***listPtr)) ;
#define MLBUFFER     0x00001		/* entering a buffer name	     */
#define MLCOMMAND    0x00002		/* entering a command		     */
#define MLFILE       0x00004		/* entering a filename		     */
#define MLSEARCH     0x00008		/* entering a search string          */
#define	MLVARBL      0x00010		/* entering a variable               */
#define	MLUSER       0x00020		/* entering a mode                   */
#define MLNOHIST     0x00040            /* Don't use the history (wont store */
#define MLUPPER      0x00080		/* convert chars to uppercase	     */
#define MLLOWER      0x00100		/* convert chars to lowercase	     */
#define	MLNOSPACE    0x00200		/* no spaces allowed		     */
#define	MLNOSTORE    0x00400		/* no history storing		     */
#define	MLISEARCH    0x00800		/* Special incremental search mode   */
#define	MLNORESET    0x01000		/* don't reset the buffer            */
#define	MLMACHIST    0x02000		/* Add output to history even if mac */
#define	MLMACNORT    0x04000		/* dont reset the buffer even if mac */
#define MLINSENSCASE 0x08000            /* ignore letter case in completion  */
#define MLFFZERO     0x10000            /* en/disable \x00 -> \xff encode    */
#define MLHIDEVAL    0x20000            /* Hide the value using '*'s         */

/* setup a #define with the correct flags for getting a filename, this must
 * consider where the platform file system only has one case and where it
 * is case insensitive
 */
#ifdef _SINGLE_CASE
/* Typically dos */
#define MLFILECASE   (MLFILE|MLLOWER)
#define MLBUFFERCASE MLBUFFER
#else
#ifdef _INSENSE_CASE
/* Typically windows */
#define MLFILECASE   (MLFILE|MLINSENSCASE)
#define MLBUFFERCASE (MLBUFFER|MLINSENSCASE)
#else
/* Typically unix */
#define MLFILECASE   MLFILE
#define MLBUFFERCASE MLBUFFER
#endif
#endif
/* Following must be setup when using MLUSER, mlgsStrList must be an
 * array of completion strings, and mlgsStrListSize must be a count
 * of the number of completions. Not the completion array is sorted!
 */
extern meUByte **mlgsStrList ;
extern int mlgsStrListSize ;
extern int meGetStringFromUser APRAM((meUByte *prompt, int option, int defnum, meUByte *buf, int nbuf)) ;
#endif
/*
 * hilight.c
 */
#if !(defined __HILIGHTC) || (defined _ANSI_C)		/* hilight.c externals */
extern	void	mlerase APRAM((int flag));
#if MEOPT_HILIGHT
extern	int	indent APRAM((int f, int n));
extern	int	hilight APRAM((int f, int n));
extern  void    hilightCurLineOffsetEval APRAM((meWindow *wp)) ;
extern  int     indentLine APRAM((void)) ;
extern  meUShort hilightLine APRAM((meVideoLine *vp1)) ;
extern  void    hilightLookBack APRAM((meWindow *)) ;
#else
#define indent notAvailable
#define hilight notAvailable
#endif
#endif
/*
 * history.c
 */
#if !(defined __HISTORYC) || (defined _ANSI_C)		/* history.c externals */
extern  void    initHistory APRAM((void)) ;
extern  int     setupHistory APRAM((int option, meUByte **numPtr, meUByte ***list)) ;
extern  void    addHistory APRAM((int option, meUByte *str)) ;
#if MEOPT_REGISTRY
extern  int     readHistory APRAM((int f, int n)) ;
extern  int     saveHistory APRAM((int f, int n)) ;
#else
#define readHistory notAvailable
#define saveHistory notAvailable
#endif
#endif
/*
 * isearch.c
 */
#if !(defined __ISEARCHC) || (defined _ANSI_C)		/* isearch.c externals */
#if MEOPT_ISEARCH
extern	int	isearchBack APRAM((int f, int n));
extern	int	isearchForw APRAM((int f, int n));
#else
#define isearchBack notAvailable
#define isearchForw notAvailable
#endif
#endif
/*
** key.c
*/
#if !(defined __KEYC) || (defined _ANSI_C)			/* key.c externals */
extern  void    count_key_table APRAM((void)) ;
extern  int     decode_key APRAM((register meUShort code, meUInt *arg)) ;
extern  int     delete_key APRAM((register meUShort code)) ;
extern  int	insert_key APRAM((register meUShort code, meUShort index, meUInt arg)) ;
#endif
/*
 * line.c
 */
#if !(defined __LINEC) || (defined _ANSI_C)		/* line.c externals */
extern	meLine    *lalloc APRAM((int used));
extern	int	bchange APRAM((void));
extern	void	lchange APRAM((int flag));
extern	int	insSpace APRAM((int f, int n));
extern	int	insTab APRAM((int f, int n));
extern  void	lunmarkBuffer APRAM((meBuffer *bp, meLine *lp, meLine *nlp));
extern  meUByte  *lmakespace APRAM((int n));
extern	int	linsert APRAM((int n, int c));
extern	int	lsinsert  APRAM((int n, meUByte *cp));
extern	int	lnewline APRAM((void));
extern	int	mldelete APRAM((meInt n, meUByte *kstring));
extern	int	ldelete APRAM((meInt n, int kflag));
extern	int	ldelnewline APRAM((void));
extern	int	ksave APRAM((void));
extern	meUByte  *kaddblock APRAM((meInt count));
extern	int	yankfrom APRAM((struct meKill *pklist));
extern	int	yank APRAM((int f, int n));
extern	int	reyank APRAM((int f, int n));
extern  void    freeLineLoop APRAM((meLine *lp, int flag)) ;
#endif
/*
 * maro.c
 */
#if !(defined __MACROC) || (defined _ANSI_C)		/* macro.c externals */
extern	int	macroDefine APRAM((int f, int n));
extern	int	macroFileDefine APRAM((int f, int n));
#if MEOPT_EXTENDED
extern	int	macroHelpDefine APRAM((int f, int n));
extern  void    helpBufferReset APRAM((meBuffer *bp));
extern	int	help APRAM((int f, int n));
extern	int	helpItem APRAM((int f, int n));
extern  int	helpCommand APRAM((int f, int n)) ;
extern  int	helpVariable APRAM((int f, int n)) ;
extern  int     nameKbdMacro APRAM((int f, int n)) ;
extern	int	macroQuery APRAM((int f, int n));
extern  meMacro *userGetMacro APRAM((meUByte *buf, int len)) ;
extern  int     insMacro APRAM((int f, int n)) ;
#else
#define macroHelpDefine notAvailable
#define help notAvailable
#define helpItem notAvailable
#define helpCommand notAvailable
#define helpVariable notAvailable
#define nameKbdMacro notAvailable
#define macroQuery notAvailable
#define insMacro notAvailable
#endif
extern	int	startKbdMacro APRAM((int f, int n));
extern	int	endKbdMacro APRAM((int f, int n));
extern	int	execKbdMacro APRAM((int f, int n));
extern	int	stringExec APRAM((int f, int n, meUByte *macro)) ;
extern	int	execString APRAM((int f, int n));
#endif /* __MACROC */
/*
 * main.c
 */
#if !(defined __MAINC) || (defined _ANSI_C)		/* main.c externals */
/*extern	void	edinit APRAM((char *bname)); */
extern  int     insertChar APRAM((register int c, register int n)) ;
extern  void    doOneKey APRAM((void)) ;
extern	int	execute APRAM((int c, int f, int n));
extern	int	meAbout APRAM((int f, int n));
extern	int	exitEmacs APRAM((int f, int n));
extern	int	quickExit APRAM((int f, int n));
extern  int     promptSaveAll APRAM((int f, int n)) ;
extern	int	saveExitEmacs APRAM((int f, int n));
extern	int	ctrlg APRAM((int f, int n));
extern	int	notAvailable APRAM((int f, int n));
extern  int     noMarkSet APRAM((void)) ;
extern	int	rdonly APRAM((void));
extern	int     voidFunc APRAM((int f, int n)) ;
extern	int	resterr APRAM((void));
extern	int	prefixFunc APRAM((int f, int n));
extern	int	uniArgument APRAM((int f, int n));
extern  void    mesetup APRAM((int argc, char *argv[]));
#if (defined _UNIX) || (defined _WIN32)
extern  int     meDie APRAM((void)) ;
#endif
extern  void    autoSaveHandler APRAM((void)) ;
#if MEOPT_EXTENDED
extern	int     commandWait APRAM((int f, int n)) ;
#else
#define commandWait notAvailable
#endif

#ifndef NDEBUG
extern  void    _meAssert APRAM((char *file, int line));
#define meAssert(x) while(!(x)){_meAssert(__FILE__,__LINE__);break;}
#else
#define meAssert(x) /* Nothing (x) */
#endif
#endif
/*
 * narrow.c
 */
#if !(defined __NARROWC) || (defined _ANSI_C)		/* narrow.c externals */
#if MEOPT_NARROW
extern void createNarrow APRAM((meBuffer *bp, meLine *slp, meLine *elp, meInt sln, meInt eln, meUShort name)) ;
extern void delSingleNarrow APRAM((meBuffer *bp, int useDot)) ;
extern void removeNarrow APRAM((meBuffer *bp, register meNarrow *nrrw, int useDot)) ;
extern void unnarrowBuffer APRAM((meBuffer *bp)) ;
extern void redoNarrowInfo APRAM((meBuffer *bp)) ;
extern int  narrowBuffer APRAM((int f, int n)) ;
#else
#define narrowBuffer notAvailable
#endif
#endif
/*
 * next.c
 */
#if !(defined __NEXTC) || (defined _ANSI_C)		/* next.c externals */
#if MEOPT_FILENEXT
extern int      getNextLine APRAM((int f, int n)) ;
extern int      addNextLine APRAM((int f, int n)) ;
#else
#define getNextLine notAvailable
#define addNextLine notAvailable
#endif
#if MEOPT_RCS
extern int      rcsFilePresent APRAM((meUByte *fname)) ;
extern int      doRcsCommand APRAM((meUByte *fname, register meUByte *comStr)) ;
extern int      rcsCiCoFile APRAM((int f, int n)) ;
#else
#define rcsCiCoFile notAvailable
#endif
#if (defined _XTERM) || (defined _DOS) || (defined _WIN32)
extern int      changeFont APRAM((int f, int n));
#else
#define changeFont notAvailable
#endif
#endif

/*
 * osd.c
 */
#if !(defined __OSDC) || (defined _ANSI_C)              /* osd.c externals */
#if MEOPT_OSD
extern void     osdStoreAll APRAM((void)) ;
extern void     osdRestoreAll APRAM((int)) ;
extern void     osdDisp APRAM((meUByte *buf, meUByte *cont, int cpos)) ;
extern int      osdMouseContextChange APRAM((int leftPick)) ;
extern int      osdMainMenuCheckKey APRAM((int cc)) ;
extern int      osd APRAM((int f, int n));
extern void     osdMainMenuUpdate APRAM((int force)) ;
#if MEOPT_LOCALBIND
extern	int	osdBindKey APRAM((int f, int n));
extern	int	osdUnbindKey APRAM((int f, int n));
#else
#define osdBindKey notAvailable
#define osdUnbindKey notAvailable
#endif
#else
#define osd notAvailable
#define osdBindKey notAvailable
#define osdUnbindKey notAvailable
#endif
#endif
/*
 * print.c
 */
#if !(defined __PRINTC) || (defined _ANSI_C)              /* print.c externals */
#if MEOPT_PRINT
extern  int     printColor APRAM ((int f, int n));
extern  int     printScheme APRAM ((int f, int n));
extern	int	printBuffer APRAM((int f, int n)) ;
extern	int	printRegion APRAM((int f, int n)) ;
#ifdef _WIN32
extern  int     WinPrint APRAM((meUByte *name, meLine *phead));
#endif /* _WIN32 */
#else
#define printColor notAvailable
#define printScheme notAvailable
#define printBuffer notAvailable
#define printRegion notAvailable
#endif /* PRINT */
#endif
/*
 * random.c
 */
#if !(defined __RANDOMC) || (defined _ANSI_C)		/* random.c externals */
extern  void   *meMalloc APRAM((size_t s)) ;
extern  void   *meRealloc APRAM((void *, size_t s)) ;
extern  int     stricmp APRAM((const char *str1, const char *str2)) ;
extern  int     strnicmp APRAM((const char *str1, const char *str2, size_t)) ;
extern  int     stridif APRAM((const char *str1, const char *str2)) ;
extern  void    sortStrings APRAM((int noStr, meUByte **strs, int offset, meIFuncSS cmpFunc)) ;
extern  int     sortLines APRAM((int f, int n)) ;
extern	int	bufferInfo APRAM((int f, int n));
extern	int	getcline APRAM((meWindow *wp));
extern	int	getcol APRAM((meUByte *ss, int off));
#define getccol() getcol(frameCur->windowCur->dotLine->text,frameCur->windowCur->dotOffset)
extern	int	setccol APRAM((int pos));
extern	int	getcwcol APRAM((void));
extern	int	setcwcol APRAM((int pos));
extern	int	transChars APRAM((int f, int n));
extern	int	transLines APRAM((int f, int n));
extern	int	quoteKeyToChar APRAM((meUShort c)) ;
extern	int	quote APRAM((int f, int n));
extern	int	meTab APRAM((int f, int n));
extern	int	meBacktab APRAM((int f, int n));
extern	int	insLine APRAM((int f, int n));
extern	int	meLineSetIndent APRAM((int curInd, int newInd, int undo));
extern	int	meNewline APRAM((int f, int n));
extern	int	deblank APRAM((int f, int n));
extern	int	forwDelChar APRAM((int f, int n));
extern	int	backDelChar APRAM((int f, int n));
extern	int	killLine APRAM((int f, int n));
extern	int	mlClear APRAM((int f, int n));
extern	int	mlWrite APRAM((int f, int n));
extern  void    makestrlow APRAM((meUByte *str));
#if MEOPT_CFENCE
extern  meUByte   gotoFrstNonWhite APRAM((void)) ;
extern  int     doCindent APRAM((int *inComment)) ;
extern	int	cinsert APRAM((void));
extern	int	gotoFence APRAM((int f, int n));
#else
#define gotoFence notAvailable
#endif
#if MEOPT_WORDPRO
extern  int	winsert APRAM((void)) ;
#endif
extern	int	insString APRAM((int f, int n));

#define meAM_ABSLINE  0x0100
#define meAM_EXECBUFF 0x0101
#define meAM_FRSTNRRW 0x0102
#define meAM_FRSTPOS  0x4000
extern  int     alphaMarkGet APRAM((meBuffer *bp, meUShort name)) ;
extern  int     alphaMarkSet APRAM((meBuffer *bp, meUShort name, meLine *lp,
                                    meUShort off, int silent)) ;
extern	int	setAlphaMark APRAM((int f, int n));
extern	int	gotoAlphaMark APRAM((int f, int n));
extern  int     insFileName APRAM ((int f, int n)) ;
#if MEOPT_EXTENDED
extern  int     cmpBuffers APRAM ((int f, int n)) ;
#else
#define cmpBuffers notAvailable
#endif
#if MEOPT_CALLBACK
extern  int     createCallback APRAM ((int f, int n)) ;
extern  void    callBackHandler APRAM((void)) ;
#else
#define createCallback notAvailable
#endif
#if MEOPT_MOUSE
extern  int     setCursorToMouse APRAM ((int f, int n)) ;
#else
#define setCursorToMouse notAvailable
#endif
#endif
/*
 * region.c
 */
#if !(defined __REGIONC) || (defined _ANSI_C)		/* region.c externals */
extern	int	getregion APRAM((meRegion *rp));
extern	int	killRegion APRAM((int f, int n));
extern	int	copyRegion APRAM((int f, int n));
extern	int	lowerRegion APRAM((int f, int n));
extern	int	upperRegion APRAM((int f, int n));
#if MEOPT_EXTENDED
extern	int	killRectangle APRAM((int f, int n));
extern	int	yankRectangle APRAM((int f, int n));
#else
#define killRectangle notAvailable
#define yankRectangle notAvailable
#endif
#endif
/*
 * registry.c
 */
#if !(defined __REGISTRYC) || (defined _ANSI_C)
/* Command line prototyps */
#if MEOPT_REGISTRY
extern int deleteRegistry APRAM((int f, int n));
extern int findRegistry APRAM((int f, int n));
extern int getRegistry APRAM((int f, int n));
extern int listRegistry APRAM((int f, int n));
extern int markRegistry APRAM((int f, int n));
extern int readRegistry APRAM((int f, int n));
extern int saveRegistry APRAM((int f, int n));
extern int setRegistry APRAM((int f, int n));

/* API registry prototypes made available for export */
extern meRegNode *regFind APRAM((meRegNode *root, meUByte *subkey));
#define regGetChild(reg) (reg->child)
#define regGetNext(reg)  (reg->next)
extern meRegNode *regRead APRAM((meUByte *rname, meUByte *fname, int mode));
extern meRegNode *regSet APRAM((meRegNode *root, meUByte *subkey, meUByte *value));
extern meRegNode *vregFind APRAM((meRegNode *root, meUByte *fmt, ...));
extern int  regDelete APRAM((meRegNode *root));
extern int  regSave APRAM((meRegNode *root, meUByte *fname));
extern int  anyChangedRegistry APRAM((void));
#define regGetName(reg)       (reg->name)
#define regGetValue(reg)      (reg->value)
#define regGetValueLen(reg)   (reg->len)
#define regGetLong(reg,def)   ((reg->value==NULL) ? def:meAtoi(reg->value))
#define regGetString(reg,def) ((reg->value==NULL) ? def:reg->value)

#else
#define deleteRegistry notAvailable
#define findRegistry notAvailable
#define getRegistry notAvailable
#define listRegistry notAvailable
#define markRegistry notAvailable
#define readRegistry notAvailable
#define saveRegistry notAvailable
#define setRegistry notAvailable
#endif
#endif
/*
 * search.c
 */
#if !(defined __SEARCHC) || (defined _ANSI_C)		/* search.c externals */
#define meEXPAND_BACKSLASH 0x01
#define meEXPAND_FFZERO    0x02
#define meEXPAND_PRINTABLE 0x04
extern  int     expandchar APRAM((int c, meUByte *d, int flags)) ;
extern  int     expandexp APRAM((int slen, meUByte *s, int dlen, int doff,
                                 meUByte *d, int cpos, int *opos, int flags)) ;
extern	int	eq APRAM((int bc, int pc));
extern	int	searchForw APRAM((int f, int n));
extern	int	huntForw APRAM((int f, int n));
extern	int	searchBack APRAM((int f, int n));
extern	int	huntBack APRAM((int f, int n));
extern	int	replaceStr APRAM((int f, int n));
extern	int	queryReplaceStr APRAM((int f, int n));
#endif
/*
 * spawn.c
 */
#if !(defined __SPAWNC) || (defined _ANSI_C)		/* spawn.c externals */
#ifdef _UNIX
extern  void    __mkTempName APRAM((meUByte *buf, meUByte *name));
#define mkTempName(bb,nn,ee) __mkTempName((bb),(nn))
#else
extern  void    __mkTempName APRAM((meUByte *buf, meUByte *name, meUByte *ext));
#define mkTempName(bb,nn,ee) __mkTempName((bb),(nn),(ee))
#endif
#ifdef _WIN32
extern void mkTempCommName(meUByte *filename, meUByte *basename) ;
#else
#define mkTempCommName(filename,basename) mkTempName(filename,basename,NULL)
#endif
#if MEOPT_SPAWN

#define COMMAND_FILE         "stdout.~~~"
#define DUMMY_STDIN_FILE     "stdin.~~~" 

#define LAUNCH_BUFFERNM      0x0001      /* Do not use the comspec    */
#define LAUNCH_SILENT        0x0002      /* Do not use the comspec    */
#define LAUNCH_NOCOMSPEC     0x0004      /* Do not use the comspec    */
#define LAUNCH_DETACHED      0x0008      /* Detached process launch   */
#define LAUNCH_LEAVENAMES    0x0010      /* Leave the names untouched */
#define LAUNCH_SHOWWINDOW    0x0020      /* Dont hide the new window  */
#define LAUNCH_USER_FLAGS    0x002F      /* User flags bitmask        */
#define LAUNCH_SHELL         0x0100
#define LAUNCH_SYSTEM        0x0200
#define LAUNCH_FILTER        0x0400
#define LAUNCH_PIPE          0x0800
#define LAUNCH_IPIPE         0x1000
extern	int	meShell APRAM((int f, int n));
extern	int	doShellCommand APRAM((meUByte *cmdstr)) ;
extern	int	meShellCommand APRAM((int f, int n));
extern  int     doPipeCommand APRAM((meUByte *comStr, meUByte *path, meUByte *bufName, 
                                 int silent)) ;
extern	int	pipeCommand APRAM((int f, int n));
#if MEOPT_IPIPES
extern  int     doIpipeCommand APRAM((meUByte *comStr, meUByte *path, meUByte *bufName, 
                                  int silent)) ;
extern	int	ipipeCommand  APRAM((int f, int n)) ;
extern	int	ipipeWrite APRAM((int f, int n)) ;
extern  void    ipipeRead APRAM((meIPipe *ipipe)) ;
extern  int     ipipeSetSize APRAM((meWindow *wp, meBuffer *bp)) ;
extern	void    ipipeRemove APRAM((meIPipe *ipipe)) ;
#ifdef _UNIX
extern  void    ipipeCheck APRAM((void)) ;
#endif
extern	int	ipipeKill APRAM((int f, int n)) ;
extern  int     anyActiveIpipe APRAM((void)) ;
#else
#define ipipeWriteString voidFunc
#define ipipeCommand  pipeCommand
#define ipipeWrite voidFunc
#define ipipeKill voidFunc
#endif

#if MEOPT_EXTENDED
#define FILTER_IN_FILE    "fltinp.~~~"
#define FILTER_OUT_FILE   "fltout.~~~"
extern	int	meFilter APRAM((int f, int n));
#ifdef _UNIX
extern	int	suspendEmacs APRAM((int f, int n));
#else
#define suspendEmacs notAvailable
#endif
#else
#define meFilter notAvailable
#define suspendEmacs notAvailable
#endif

#else

#define meShell notAvailable
#define meShellCommand notAvailable
#define pipeCommand notAvailable
#define ipipeWriteString notAvailable
#define ipipeCommand  notAvailable
#define ipipeWrite notAvailable
#define ipipeKill notAvailable
#define meFilter notAvailable
#define suspendEmacs notAvailable

#endif /* MEOPT_SPAWN */
#endif

/*
 * spell.c
 */
#if !(defined __SPELLC) || (defined _ANSI_C)			/* spell.c externals */
#if MEOPT_SPELL
extern	int	dictionaryAdd APRAM((int f, int n));
extern	int	spellRuleAdd APRAM((int f, int n));
extern	int	dictionaryDelete APRAM((int f, int n));
extern	int	dictionarySave APRAM((int f, int n));
extern	int	spellWord APRAM((int f, int n));
extern  int     anyChangedDictionary APRAM((void)) ;
#else
#define dictionaryAdd notAvailable
#define spellRuleAdd notAvailable
#define dictionaryDelete notAvailable
#define dictionarySave notAvailable
#define spellWord notAvailable
#endif
extern	void	findWordsInit APRAM((meUByte *mask));
extern	meUByte  *findWordsNext APRAM((void));
#endif
/*
 * tag.c
 */
#if !(defined __TAGC) || (defined _ANSI_C)			/* tag.c externals */
#if MEOPT_TAGS
extern	int	findTag APRAM((int f, int n));
#else
#define findTag notAvailable
#endif
#endif
/*
 * termio.c
 */
#if !(defined __TERMIOC) || (defined _ANSI_C)		/* termio.c externals */
extern	void    TTdoBell APRAM((int)) ;
extern	void    TTbell APRAM((void));
extern  int     charListToShorts APRAM((meUShort *sl, meUByte *cl)) ;
extern  int     keyListToShorts APRAM((meUShort *sl, meUByte *kl)) ;
extern  void    translateKeyAdd APRAM((meTRANSKEY *tcapKeys, int count, int time,
                                       meUShort *key, meUShort map)) ;
extern	int	translateKey APRAM((int f, int n));
extern  char   *meTParm APRAM((char *str, ...)) ;
#endif
/*
 * time.c
 */
#if !(defined __TIMEC) || (defined _ANSI_C)		/* time.c externals */
extern	int	set_timestamp APRAM((meBuffer *bp));
#endif
/*
 * undo.c
 */
#if !(defined __UNDOC) || (defined _ANSI_C)		/* undo.c externals */
#if MEOPT_UNDO
extern  meUndoNode *meUndoCreateNode     APRAM((size_t size)) ;
extern	void	meUndoAddInsChar     APRAM((void));
extern	void	meUndoAddDelChar     APRAM((void));
extern  void    meUndoAddRepChar     APRAM((void));
extern	void	meUndoAddInsChars    APRAM((meInt numChars));
extern	void	meUndoAddDelChars    APRAM((meInt numChars));
extern	void	meUndoAddReplaceBgn  APRAM((meLine *elinep, meUShort elineo));
extern	void	meUndoAddReplaceEnd  APRAM((meInt numChars));
extern	void    meUndoAddReplace     APRAM((meUByte *dstr, meInt count)) ;
#if MEOPT_NARROW
extern  void    meUndoAddUnnarrow    APRAM((meInt sln, meInt eln, meUShort name)) ;
extern  void    meUndoAddNarrow      APRAM((meInt sln, meUShort name)) ;
#endif
extern	void	meUndoRemove         APRAM((meBuffer *bp)) ;
extern	int	meUndo               APRAM((int f, int n));
#else
#define meUndo notAvailable
#endif
#endif
/*
 * window.c
 */
#if !(defined __WINDOWC) || (defined _ANSI_C)		/* window.c externals */
extern  void    meWindowMakeCurrent APRAM((meWindow *wp)) ;
extern  void    frameAddModeToWindows APRAM((int mode)) ;
extern  void    meBufferAddModeToWindows APRAM((meBuffer *bp, int mode)) ;
extern  void    meWindowFixTextSize APRAM((meWindow *wp));
extern  void    meWindowFixScrollBars APRAM((meWindow *wp));
extern  void    meWindowFixScrollBox APRAM((meWindow *wp));
extern  int     frameSetupMenuLine  APRAM((int n));
extern	int	windowRecenter APRAM((int f, int n));
extern	int	windowGotoNext APRAM((int f, int n));
extern	int	windowGotoPrevious APRAM((int f, int n));
extern	int	windowScrollDown APRAM((int f, int n));
extern  int     windowScrollLeft APRAM((int f, int n));
extern  int     windowScrollRight APRAM((int f, int n));
extern	int	windowScrollUp APRAM((int f, int n));
extern	int	windowDeleteOthers APRAM((int f, int n));
extern	int	windowDelete APRAM((int f, int n));
extern	int	windowGrowDepth APRAM((int f, int n));
extern	int	windowResizeDepth APRAM((int f, int n));
extern	int	windowShrinkDepth APRAM((int f, int n));
extern	int	windowSplitDepth APRAM((int f, int n));
extern  int     frameResizeWindows APRAM((int f, int n));
#if MEOPT_HSPLIT
extern	int	windowGrowWidth APRAM((int f, int n));
extern	int	windowResizeWidth APRAM((int f, int n));
extern	int	windowShrinkWidth APRAM((int f, int n));
extern	int	windowSplitWidth APRAM((int f, int n));
#else
#define windowGrowWidth notAvailable
#define windowResizeWidth notAvailable
#define windowShrinkWidth notAvailable
#define windowSplitWidth notAvailable
#endif
#if MEOPT_POSITION
extern	int	positionSet APRAM((int f, int n));
extern	int	positionGoto APRAM((int f, int n));
#else
#define positionSet notAvailable
#define positionGoto notAvailable
#endif
#if MEOPT_MOUSE
extern  int     windowSetScrollWithMouse APRAM((int f, int n));
#else
#define windowSetScrollWithMouse notAvailable
#endif
/* uses the bfind flags for finding buffer "name"
 * if not null. Also if WPOP_MKCURR set then 
 * makes the buffer current.
 */
#define WPOP_MKCURR 0x100
#define WPOP_USESTR 0x200
#define WPOP_EXIST  0x400
extern	meWindow *meWindowPopup APRAM((meUByte *name, int flags, meBuffer **bufferReplaced));
extern	int	windowPopup APRAM((int f, int n));
#if MEOPT_EXTENDED
extern	int	windowScrollNextUp APRAM((int f, int n));
extern	int	windowScrollNextDown APRAM((int f, int n));
#else
#define windowScrollNextUp notAvailable
#define windowScrollNextDown notAvailable
#endif
#endif
/*
 * word.c
 */
#if !(defined __WORDC) || (defined _ANSI_C)		/* word.c externals */
extern	int	backWord APRAM((int f, int n));
extern	int	forwWord APRAM((int f, int n));
extern	int	upperWord APRAM((int f, int n));
extern	int	lowerWord APRAM((int f, int n));
extern	int	capWord APRAM((int f, int n));
extern	int	forwDelWord APRAM((int f, int n));
extern	int	backDelWord APRAM((int f, int n));
#if MEOPT_WORDPRO
extern	int	wrapWord APRAM((int f, int n));
extern	int	justify	 APRAM((int leftMargin, int leftDoto));
extern	int	fillPara APRAM((int f, int n));
extern	int	killPara APRAM((int f, int n));
extern	int	countWords APRAM((int f, int n));
#else
#define wrapWord notAvailable
#define fillPara notAvailable
#define killPara notAvailable
#define countWords notAvailable
#endif
#endif

/* gettimeofday - Retreives the time of day to millisecond resolution
 *                This is a 4.2 BSD function. IRIX 5.x and Solaris prior to
 *                2.5 provide a single argument version, for backward
 *                compatibility the 2nd argument is ignored. HP-UX 10.x and
 *                versions of Solaris above 2.5 provide a 2 argument version.
 *                Win32 does not provide this function (of course). 
 *                
 *                Jon Green 18th Dec '96.
 */
/* windows has name clashes with timeval, so define an meTimeval */
#ifdef _WIN32

struct meTimeval
{
    int tv_sec;                         /* Time in seconds */
    int tv_usec;                        /* Time in milliseconds */
};

struct meTimezone 
{
    int tz_minuteswest;                 /* Offset (+/-ve) from UTC in minutes */
    int tz_dsttime;                     /* Flag indicating the type of DST correction */
};

extern void gettimeofday (struct meTimeval *tp, struct meTimezone *tz);

#else
#define meTimeval  timeval
#define meTimezone timezone
#endif

#ifdef _WIN32
/* The current win32 rename() function does not cope with long filenames
 * Use the Win32 definition - note that these functions have an oposite
 * boolean sense that the ANSI 'C' definitions, explicitly test to convert
 * to the more familier invocation.
 *
 * Why the hell we need these functions and the ANSI 'C' functions are not
 * valid is beyond belief - typical bloody Microsoft !!
 */
extern int chdir(const char *name) ;
extern int _getdrive(void) ;
#define meChdir(dir)        chdir(dir)
#define meRename(src,dst)   (MoveFile(src,dst)==meFALSE)
#define meUnlink(fn)        (DeleteFile(fn)==meFALSE)
/* Doesn't exist if function returns -1 */
#define meTestExist(fn)     (((int) GetFileAttributes(fn)) < 0)
/* Can't read if doesn't exist or its a directory */
#define meTestRead(fn)      (GetFileAttributes(fn) & FILE_ATTRIBUTE_DIRECTORY)
/* Can't write if exists and its readonly or a directory */
#define meTestWrite(fn)     ((((int) GetFileAttributes(fn)) & 0xffff8011) > 0)
/* File is a directory */
#define meTestDir(fn)       ((GetFileAttributes(fn) & (0xf0000000|FILE_ATTRIBUTE_DIRECTORY)) != FILE_ATTRIBUTE_DIRECTORY)
extern int meTestExecutable(meUByte *fileName) ;
#define meStatTestRead(st)  (((st).stmode & FILE_ATTRIBUTE_DIRECTORY) == 0)
#define meStatTestWrite(st) (((st).stmode & (FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_READONLY)) == 0)
#define meStatTestSystem(st) (((st).stmode & FILE_ATTRIBUTE_SYSTEM) == 0)
#define meChmod(fn,mode)    (SetFileAttributes(fn,mode))
extern void WinShutdown (void);
#define meExit(status)      (WinShutdown(), ExitProcess(status))
#endif

#ifdef _DOS
extern int  unlink(const char *file) ;
extern int  meGetFileAttributes(meUByte *fn) ;
#define meFILE_ATTRIB_READONLY  0x01
#define meFILE_ATTRIB_HIDDEN    0x02
#define meFILE_ATTRIB_SYSTEM    0x04
#define meFILE_ATTRIB_VOLLABEL  0x08
#define meFILE_ATTRIB_DIRECTORY 0x10
#define meFILE_ATTRIB_ARCHIVE   0x20
extern void _meChmod(meUByte *fn,meUShort attr) ;
extern int  _meChdir(meUByte *path) ;
/* Doesn't exist if function returns -1 */
#define meTestExist(fn)     (meGetFileAttributes(fn) < 0)
/* Can't read if doesn't exist or its a directory */
#define meTestRead(fn)      (meGetFileAttributes(fn) & 0x10)
/* Can't write if exists and its readonly or a directory */
#define meTestWrite(fn)     ((meGetFileAttributes(fn) & 0xffff8011) > 0)
/* File is a directory */
#define meTestDir(fn)       ((meGetFileAttributes(fn) & 0xf0000010) != 0x010)
extern int meTestExecutable(meUByte *fileName) ;
#define meStatTestRead(st)  (((st).stmode & 0x10) == 0)
#define meStatTestWrite(st) (((st).stmode & 0x11) == 0)
#define meChmod _meChmod
#define meChdir _meChdir
#endif

#ifdef _UNIX
/* Define the standard POSIX tests for the file stats */
#ifndef S_IROTH                         /* Other read permission  */
#define S_IROTH  0000004
#endif
#ifndef S_IRGRP                         /* Group read permission */
#define S_IRGRP  0000040
#endif
#ifndef S_IRUSR                         /* User read permission */
#define S_IRUSR  0000400                
#endif
#ifndef S_IWOTH                         /* Other write permission */
#define S_IWOTH  0000002        
#endif
#ifndef S_IWGRP                         /* Group write permission */
#define S_IWGRP  0000020        
#endif
#ifndef S_IWUSR                         /* User write permission */
#define S_IWUSR  0000200                
#endif
#ifndef S_IXOTH                         /* Other exeute permission */
#define S_IXOTH  0000001                
#endif
#ifndef S_IXGRP                         /* Group execute permission */
#define S_IXGRP  0000010                
#endif
#ifndef S_IXUSR                         /* User execute permission */
#define S_IXUSR  0000100
#endif
#ifndef S_IRWXO                         /* Other read/write/execute */
#define S_IRWXO  ((S_IROTH)|(S_IWOTH)|(S_IXOTH))
#endif 
#ifndef S_IRWXU                         /* User read/write/execute */
#define S_IRWXU  ((S_IRUSR)|(S_IWUSR)|(S_IXUSR))
#endif
#ifndef S_IRWXG                         /* Group read/write/exexute */
#define S_IRWXG  ((S_IRGRP)|(S_IWGRP)|(S_IXGRP))
#endif
#ifndef S_ISGID                         /* Set group id on exec */
#define S_ISGID  0002000               
#endif
#ifndef S_ISUID                         /* Set user id on exec */
#define S_ISUID  0004000  
#endif
#ifndef S_IFLNK                         /* Set if symbolic link -- NOT POSIX */
#define S_IFLNK  0120000
#endif
/* Posix test functions */
#ifndef S_ISDIR                         /* Test directory */
#define S_ISDIR(x)  (((x) & 0170000) == 0040000)
#endif
#ifndef S_ISCHR                         /* Test character special file */
#define S_ISCHR(x)  (((x) & 0170000) == 0020000)
#endif
#ifndef S_ISBLK                         /* Test block special file */
#define S_ISBLK(x)  (((x) & 0170000) == 0060000)
#endif
#ifndef S_ISREG                         /* Test regular file */
#define S_ISREG(x)  (((x) & 0170000) == 0100000)
#endif
#ifndef S_ISFIFO                        /* Test FIFO */
#define S_ISFIFO(x) (((x) & 0170000) == 0010000)
#endif
#ifndef S_ISLNK                         /* Test symbolic link -- NOT POSIX */
#define S_ISLNK(x)  (((x) & 0170000) == (S_IFLNK))
#endif

/* File modes defined in terms of POSIX tests */
extern int meGidInGidList(gid_t gid) ;
#define meStatTestRead(st)                                                   \
((((st).stuid == meUid) && ((st).stmode & S_IRUSR)) ||                       \
 ((st).stmode & S_IROTH) ||                                                  \
 (((st).stmode & S_IRGRP) &&                                                 \
  (((st).stgid == meGid) || (meGidSize && meGidInGidList((st).stgid))))) 
#define meStatTestWrite(st)                                                  \
((((st).stuid == meUid) && ((st).stmode & S_IWUSR)) ||                       \
 ((st).stmode & S_IWOTH) ||                                                  \
 (((st).stmode & S_IWGRP) &&                                                 \
  (((st).stgid == meGid) || (meGidSize && meGidInGidList((st).stgid))))) 
#define meStatTestExec(st)                                                   \
((((st).stuid == meUid) && ((st).stmode & S_IXUSR)) ||                       \
 ((st).stmode & S_IXOTH) ||                                                  \
 (((st).stmode & S_IXGRP) &&                                                 \
  (((st).stgid == meGid) || (meGidSize && meGidInGidList((st).stgid))))) 
#endif

/* Differentiate between different styles for waiting for a process to finish */
#ifdef _BSD
/* Use wait4 to enable us to wait on a PID. */
#define meWaitpid(pid,status,opt) wait4((pid),(status),(opt),NULL)
#define meWAIT_STATUS union wait
/* Use getwd() to get the current working directory */
#define meGetcwd(buf,size)        getwd((char *)(buf))
/* Use the lighter weight vfork() for forking */
#define meFork  vfork
#else
/* SVr4, POSIX.1 */
#define meWaitpid(pid,status,opt) waitpid((pid),(status),(opt))
#define meWAIT_STATUS int
/* POSIX.1 */
#define meGetcwd(buf,size)        getcwd((char *)(buf),(size))
/* Use fork() always */
#define meFork  fork
#endif /* _BSD */

#ifndef meRename
#define meRename(src,dst) rename((char *)(src),(char *)(dst))
#endif
#ifndef meUnlink
#define meUnlink(fn) unlink((char *)(fn))
#endif
#ifndef meChdir
#define meChdir(fn) chdir((char *)(fn))
#endif
#ifndef meTestExist
#define meTestExist(fn) access((char *)(fn),F_OK)
#endif
#ifndef meTestRead
#define meTestRead(fn) access((char *)(fn),R_OK)
#endif
#ifndef meTestWrite
#define meTestWrite(fn) access((char *)(fn),W_OK)
#endif
#ifndef meTestExec
#define meTestExec(fn) access((char *)(fn),X_OK)
#endif
#ifndef meTestDir
#define meTestDir(fn) (getFileStats(fn,0,NULL,NULL) != meFILETYPE_DIRECTORY)
#endif
#ifndef meChmod
#define meChmod(fn,attr) chmod((char *)(fn),attr)
#endif
#ifndef meExit
extern void exit(int i) ;
#if MEOPT_CLIENTSERVER
    /* Close & delete the client file */
#define meExit(n) (TTkillClientServer(),exit(n))
#else
#define meExit exit
#endif
#endif

/*
 * Terminal I/O file defintions
 */
extern	char   *getenv APRAM((const char *));

extern  void    free APRAM((void *block));
extern  void   *malloc APRAM((size_t s));
extern  void   *realloc APRAM((void *block, size_t s));
extern	void   *memcpy APRAM((void *, const void *, size_t));
extern	void   *memset APRAM((void *, int,    size_t));

extern	size_t	strlen APRAM((const char *s));
extern	int	strcmp APRAM((const char *s1, const char *s2));
extern	char   *strcpy APRAM((char *s1, const char *s2));
extern	char   *strcat APRAM((char *s1, const char *s2));
extern	char   *strdup APRAM((const char *s1));
extern	char   *strncpy APRAM((char *s1, const char *s2, size_t n));
extern	int	strncmp APRAM((const char *s1, const char *s2, size_t n));
extern	char   *strchr APRAM((const char *, int));
extern	char   *strrchr APRAM((const char *, int));
extern  long    strtol APRAM((const char*, char**, int));
extern  int     abs APRAM((int));
extern  int     system APRAM((const char*));

#ifdef _NOPUTENV
#define meGetenv(s1)        ((void *) megetenv((const char *)(s1)))
extern char    *megetenv APRAM((const char *s));
extern int      putenv APRAM((const char *s));
#else
#define meGetenv(s1)        ((void *) getenv((const char *)(s1)))
#endif
#define mePutenv(s1)        (putenv((const char *)(s1)))
#define meStrlen(s1)        strlen((const char *)(s1))
#define meStrcmp(s1,s2)     strcmp((const char *)(s1),(const char *)(s2))
#define meStrncmp(s1,s2,n)  strncmp((const char *)(s1),(const char *)(s2),(n))
#define meStrcat(s1,s2)     strcat((char *)(s1),(const char *)(s2))
#define meStrcpy(s1,s2)     strcpy((char *)(s1),(const char *)(s2))
#define meStrncpy(s1,s2,n)  strncpy((char *)(s1),(const char *)(s2),(n))
#define meStridif(s1,s2)    stridif((const char *)(s1),(const char *)(s2))
#define meStricmp(s1,s2)    stricmp((const char *)(s1),(const char *)(s2))
#define meStrnicmp(s1,s2,n) strnicmp((const char *)(s1),(const char *)(s2),(n))
#define meStrdup(s1)        ((void *) strdup((const char *)(s1)))
#define meStrstr(s1,s2)     ((void *) strstr((const char *)(s1),(const char *)(s2)))
#define meStrchr(s1,c1)     ((void *) strchr((const char *)(s1),(c1)))
#define meStrrchr(s1,c1)    ((void *) strrchr((const char *)(s1),(c1)))

#define CHRMSK_MACROTYPE   0x0f
#define CHRMSK_DISPLAYABLE 0x10
#define CHRMSK_POKABLE     0x20
#define CHRMSK_PRINTABLE   0x40
#define CHRMSK_SPACE       0x80

#define CHRMSK_LOWER       0x01
#define CHRMSK_UPPER       0x02
#define CHRMSK_ALPHA       0x03
#define CHRMSK_HEXDIGIT    0x04
#define CHRMSK_ALPHANUM    0x07
#define CHRMSK_SPLLEXT     0x08
#define CHRMSK_USER1       0x10
#define CHRMSK_USER2       0x20
#define CHRMSK_USER3       0x40
#define CHRMSK_USER4       0x80

#define CHRMSK_DEFWORDMSK  CHRMSK_ALPHANUM

#define getMacroType(c)  (charMaskTbl1[((meUByte) (c))] & CHRMSK_MACROTYPE)
#define getMacroTypeS(s) (charMaskTbl1[((meUByte) (*s))] & CHRMSK_MACROTYPE)
#define isDisplayable(c) (charMaskTbl1[((meUByte) (c))] & CHRMSK_DISPLAYABLE)
#define isPokable(c)     (charMaskTbl1[((meUByte) (c))] & CHRMSK_POKABLE)
#define isPrint(c)       (charMaskTbl1[((meUByte) (c))] & CHRMSK_PRINTABLE)
#define isSpace(c)       (charMaskTbl1[((meUByte) (c))] & CHRMSK_SPACE)

#define isDigit(c)       (((c) >= '0') && ((c) <= '9'))
#define isLower(c)       (charMaskTbl2[((meUByte) (c))] & CHRMSK_LOWER)
#define isUpper(c)       (charMaskTbl2[((meUByte) (c))] & CHRMSK_UPPER)
#define isAlpha(c)       (charMaskTbl2[((meUByte) (c))] & CHRMSK_ALPHA)
#define isXDigit(c)      (charMaskTbl2[((meUByte) (c))] & CHRMSK_HEXDIGIT)
#define isAlphaNum(c)    (charMaskTbl2[((meUByte) (c))] & CHRMSK_ALPHANUM)
#define isSpllExt(c)     (charMaskTbl2[((meUByte) (c))] & CHRMSK_SPLLEXT)
#define isWord(c)        (charMaskTbl2[((meUByte) (c))] & isWordMask)
#define isSpllWord(c)    (charMaskTbl2[((meUByte) (c))] & (CHRMSK_ALPHANUM|CHRMSK_SPLLEXT))
#define inWord()         (isWord(meLineGetChar(frameCur->windowCur->dotLine, frameCur->windowCur->dotOffset)))
#define inPWord()        ((meLineGetChar(frameCur->windowCur->dotLine, frameCur->windowCur->dotOffset)) > ' ')

#define toLower(c)       (isUpper(c) ? (charCaseTbl[((meUByte) (c))]):c)
#define toUpper(c)       (isLower(c) ? (charCaseTbl[((meUByte) (c))]):c)
#define toggleCase(c)    (charCaseTbl[((meUByte) (c))])

#define toUserFont(c)    (charLatinUserTbl[((meUByte) c)])
#define toLatinFont(c)   (charUserLatinTbl[((meUByte) c)])

#define	hexToNum(c)      ((c <= '9') ? (c^0x30)   : \
                          (c >= 'a') ? (c-'a'+10) : \
                                       (c-'A'+10))

/*	Macro argument token types					*/

#define TKNUL	0x00			/* end-of-string		*/
#define TKARG	0x01			/* interactive argument 	*/
#define TKREG	0x02			/* register variable		*/
#define TKVAR	0x03			/* user variables		*/
#define TKENV	0x04			/* environment variables	*/
#define TKFUN	0x05			/* function.... 		*/
#define TKDIR	0x06			/* directive			*/
#define TKLBL	0x07			/* line label			*/
#define TKLIT	0x08			/* numeric literal		*/
#define TKSTR	0x09			/* quoted string literal	*/
#define TKCMD	0x0A			/* command name 		*/
#define TKLVR	0x0B			/* Local variable 		*/
#define TKCVR	0x0C			/* Command variable 		*/

#define meAtoi(s) strtol((char *)(s),(char **)NULL,0)
#define meAtol(s) (meAtoi(s) != 0)
#define meLtoa(s) ((s) ? truem:falsem)

/* is url file? test */
#define isHttpLink(fn) (!strncmp((char *)fn,"http://",7))
#define isFtpLink(fn)  (!strncmp((char *)fn,"ftp://",6))
#define isUrlLink(fn)  (isHttpLink(fn) || isFtpLink(fn))
#define isUrlFile(fn)  (!strncmp((char *)fn,"file:",5))


/* use this with some care */
#define meFree(x) free(x)
#define meNullFree(p) (((p)!=NULL)?(free(p),1):0)

#if MEOPT_OSD
#define mlResetCursor() ((frameCur->mlStatus&MLSTATUS_POSOSD) ? TTmove(osdRow,osdCol):TTmove(frameCur->depth,frameCur->mlColumn))
#else
#define mlResetCursor() TTmove(frameCur->depth,frameCur->mlColumn)
#endif
#define mwResetCursor() TTmove(frameCur->mainRow,frameCur->mainColumn)
#define resetCursor()   ((frameCur->mlStatus&(MLSTATUS_POSOSD|MLSTATUS_POSML)) ? mlResetCursor():mwResetCursor())

#define get_tab_pos(c)      ((tabwidth-1) - (c)%tabwidth)       /* Get the number of characters to the next TAB position. */
#define next_tab_pos(c)     (tabwidth - ((c)%tabwidth))         /* */
#define	at_tab_pos(c)       ((c)%tabwidth)                      /* Get the number of characters to the next TAB position. */
#define get_no_tabs_pos(c)  ((c)/tabwidth)                      /* Get the number of TAB characters to the current position. */

#define restoreWindWSet(wp,ws)                                               \
((wp)->vertScroll=(ws)->vertScroll,(wp)->dotLine=(ws)->dotLine,              \
 (wp)->dotOffset=(ws)->dotOffset,(wp)->markLine=(ws)->markLine,              \
 (wp)->markOffset=(ws)->markOffset,(wp)->dotLineNo=(ws)->dotLineNo,          \
 (wp)->markLineNo=(ws)->markLineNo)
#define restoreWindBSet(wp,bp)                                               \
((wp)->vertScroll=(bp)->vertScroll,(wp)->dotLine=(bp)->dotLine,              \
 (wp)->dotOffset=(bp)->dotOffset,(wp)->markLine=(bp)->markLine,              \
 (wp)->markOffset=(bp)->markOffset,(wp)->dotLineNo=(bp)->dotLineNo,          \
 (wp)->markLineNo=(bp)->markLineNo)
#define storeWindBSet(bp,wp)                                                 \
((bp)->vertScroll=(wp)->vertScroll,(bp)->dotLine=(wp)->dotLine,              \
 (bp)->dotOffset=(wp)->dotOffset,(bp)->markLine=(wp)->markLine,              \
 (bp)->markOffset=(wp)->markOffset,(bp)->dotLineNo=(wp)->dotLineNo,          \
 (bp)->markLineNo=(wp)->markLineNo)

#define setShowRegion(sbp,sln,slo,eln,elo)                                   \
(selhilight.bp=sbp,selhilight.markLineNo=sln,selhilight.markOffset=slo,      \
 selhilight.dotLineNo=eln,selhilight.dotOffset=elo,                          \
 selhilight.flags = SELHIL_ACTIVE|SELHIL_FIXED|SELHIL_CHANGED)

