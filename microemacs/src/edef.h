/* -*- c -*-
 *
 * JASSPA MicroEmacs - www.jasspa.com
 * edef.h - Global variable definitions.
 *
 * Originally written by Dave G. Conroy for MicroEmacs
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
 * Created:     For MicroEMACS 3.2
 * Synopsis:    Global variable definitions.
 * Authors:     Dave G. Conroy, Steve Wilhite, George Jones, Daniel Lawrence,
 *          Jon Green & Steven Phillips
 */

extern  meDIRLIST  curDirList ;
extern  struct osdDIALOG  *osdDialogHd; /* Root of the on screen displays */
extern  struct osdDISPLAY *osdDisplayHd;/* Menu display list */
extern  meREGISTERS *meRegHead;         /* The register Head            */
extern  meREGISTERS *meRegCurr;         /* The current register set     */
extern  SELHILIGHT selhilight;          /* Selection hilight            */
extern  HILBLOCK  *hilBlock;            /* Hilighting colour changes    */
extern  meCOLOR    noColors ;           /* No defined colours           */
extern  int        styleTableSize;      /* Size of the colour table     */
extern  meSTYLE   *styleTable;          /* Highlighting colour table    */
#if ABBREV
extern  meABBREV  *globalAbrev ;        /* Global Abreviation file      */
#endif
extern  mePOS     *mePosition ;         /* Position stack head          */
extern  meUShort   mePositionMark ;     /* Position next alpha mark name*/
extern  meUByte   *disLineBuff ;        /* interal display buffer array */
extern  meUShort   hilBlockS ;          /* Hilight - HilBlock array siz */
extern  int        disLineSize ;        /* interal display buffer size  */
extern  int        cursorState ;        /* Current state of cursor      */
extern  FRAMELINE *frameStore;          /* Frame store line pointers    */
extern  meUByte   *progName ;           /* the program name (argv[0])   */
extern  meUByte  **ModeLineStr ;        /* modeline line format         */
extern  meUByte    orgModeLineStr[] ;   /* original modeline line format*/
extern  meUByte   *modeLineStr ;        /* current modeline format      */
extern  meUByte    modeLineFlags ;      /* current modeline flags       */
extern  meInt      autotime;            /* auto save time in seconds    */
extern  int        keptVersions;        /* No. of kept backup versions  */
extern  meUInt     delaytime;           /* mouse-time delay time        */
extern  meUInt     repeattime;          /* mouse-time repeat time       */
extern  meUInt     idletime;            /* idle-time delay time         */
extern  meUByte    fillbullet[];        /* Fill bullet character class  */
extern  meShort    fillbulletlen;       /* Fill lookahead limit         */
extern  meShort    fillcol;             /* Fill column                  */
extern  meUByte    filleos[];           /* Fill E-O-S character class   */
extern  meShort    filleoslen;          /* Fill E-O-S ' ' insert len    */
extern  meUByte    fillignore[];        /* Fill Ignore character class  */
extern  meUShort   tabsize;             /* Virtual Tab size             */
extern  meUShort   tabwidth;            /* Real TAB size                */
extern  meShort    matchlen;            /* Fence matching delay length  */
extern  meUByte   *homedir;             /* Home directory               */
extern  meUByte   *searchPath;          /* emf search path              */
extern  meUByte   *curdir;              /* current working directory    */
extern  meUByte   *curFuncName ;        /* Current macro command name   */
extern  meUByte   *execstr;             /* pointer to string to execute */
extern  int        execlevel;           /* execution IF level           */
extern  int        bufHistNo;           /* inc buff hist numbering      */
extern  int        lastCommand ;        /* The last user executed key   */
extern  int        lastIndex ;          /* The last user executed comm  */
extern  int        thisCommand ;        /* The cur. user executed key   */
extern  int        thisIndex ;          /* The cur. user executed comm  */
extern  meUByte    hexdigits[];
extern  meUShort   keyTableSize;        /* The current size of the key table */
extern  KEYTAB     keytab[];            /* key bind to functions table  */
extern  meUByte    fillmode;            /* Justify mode                 */
extern  meUByte    scrollFlag ;         /* horiz/vert scrolling method  */
extern  meUByte    sgarbf;              /* State of screen unknown      */
extern  meUByte    clexec;              /* command line execution flag  */
extern  meUByte    mcStore;             /* storing text to macro flag   */
extern  meUByte    cmdstatus;           /* last command status          */
extern  meUByte    kbdmode;             /* current keyboard macro mode  */
extern  meByte     macbug;              /* macro debuging flag          */
extern  meUByte    thisflag;            /* Flags, this command          */
extern  meUByte    lastflag;            /* Flags, last command          */
extern  meUByte    lastReplace;         /* set to non-zero if last was a replace */

extern  meUInt  meSystemCfg;            /* ME system config variable    */
#define meSYSTEM_CONSOLE    0x000001    /* This is a console version    */
#define meSYSTEM_RGBCOLOR   0x000002    /* System has definable rgb color */
#define meSYSTEM_ANSICOLOR  0x000004    /* Ansi standard color (8 colors) */
#define meSYSTEM_XANSICOLOR 0x000008    /* Extended Ansi colors (16)    */
#define meSYSTEM_FONTS      0x000010    /* Use termcap fonts (bold etc) */
#define meSYSTEM_UNIXSYSTEM 0x000080    /* This is a unix sys           */
#define meSYSTEM_MSSYSTEM   0x000100    /* This is a dos or win32 sys   */
#define meSYSTEM_DRIVES     0x000200    /* This system has drives (C:\) */
#define meSYSTEM_DOSFNAMES  0x000400    /* Dos 8.3 file name restriction*/
#define meSYSTEM_IPIPES     0x000800    /* The system supports ipipes   */
#define meSYSTEM_TABINDANY  0x001000    /* Tab key indents from any pos */
#define meSYSTEM_ALTMENU    0x002000    /* Enable Alt as menu hot key   */
#define meSYSTEM_ALTPRFX1   0x004000    /* Enable Alt as prefix1        */
#define meSYSTEM_KEEPUNDO   0x008000    /* Keep undo history after save */
#define meSYSTEM_FONTFIX    0x010000    /* Enables ANSI 0-31 rendering  */
#define meSYSTEM_CLNTSRVR   0x020000    /* Enables the client server    */
#define meSYSTEM_CTCHASPC   0x040000    /* Enables win32 catch A-space  */
#define meSYSTEM_SHOWWHITE  0x080000    /* Display TAB, CR & space chars*/
#define meSYSTEM_HIDEBCKUP  0x100000    /* Hide backup files            */
#define meSYSTEM_TABINDFST  0x200000    /* Tab key indents first col pos*/
#define meSYSTEM_NOEMPTYANK 0x400000    /* Don't allow empty yank (ext) */

#ifdef _UNIX
#ifdef _CLIENTSERVER
#define meSYSTEM_MASK (meSYSTEM_ANSICOLOR|meSYSTEM_FONTS|meSYSTEM_DOSFNAMES|meSYSTEM_IPIPES|meSYSTEM_TABINDANY|meSYSTEM_ALTMENU|meSYSTEM_ALTPRFX1|meSYSTEM_KEEPUNDO|meSYSTEM_FONTFIX|meSYSTEM_CLNTSRVR|meSYSTEM_SHOWWHITE|meSYSTEM_HIDEBCKUP|meSYSTEM_TABINDFST|meSYSTEM_NOEMPTYANK)
#else
#define meSYSTEM_MASK (meSYSTEM_ANSICOLOR|meSYSTEM_FONTS|meSYSTEM_DOSFNAMES|meSYSTEM_IPIPES|meSYSTEM_TABINDANY|meSYSTEM_ALTMENU|meSYSTEM_ALTPRFX1|meSYSTEM_KEEPUNDO|meSYSTEM_FONTFIX|meSYSTEM_SHOWWHITE|meSYSTEM_HIDEBCKUP|meSYSTEM_TABINDFST|meSYSTEM_NOEMPTYANK)
#endif
#endif
#ifdef _DOS
#define meSYSTEM_MASK (meSYSTEM_TABINDANY|meSYSTEM_ALTMENU|meSYSTEM_ALTPRFX1|meSYSTEM_KEEPUNDO|meSYSTEM_SHOWWHITE|meSYSTEM_HIDEBCKUP|meSYSTEM_TABINDFST|meSYSTEM_NOEMPTYANK)
#endif
#ifdef _WIN32
extern  meUInt meSYSTEM_MASK;           /* ME system mask - dependant on win32 flavour */
#endif

/* the $mouse variables always exist so the C variables must */
#define meMOUSE_NOBUTTONS   0x0000f     /* # mouse buttons              */
#define meMOUSE_ENBLE       0x00010     /* mouse is enabled             */
#define meMOUSE_SWAPBUTTONS 0x00020     /* swap mouse buttons           */
#define meMOUSE_ICON        0xf0000     /* Mask to set the mouse pointer icon */
extern  meUInt meMouseCfg;              /* ME mouse config variable     */
extern  meUByte mouse_pos;              /* mouse virtual position       */
extern  meShort mouse_X;                /* mouse X pos at last event    */
extern  meShort mouse_Y;                /* mouse X pos at last event    */
extern  meShort mouse_dX;               /* mouse delta X last event pos */
extern  meShort mouse_dY;               /* mouse delta X last event pos */

#if MEUNDO
extern  meUInt undoContFlag;            /* continuation of an undo      */
#endif

/* File Modes */
#if (defined _DOS) || (defined _WIN32)
extern  meUShort meUmask ;              /* file creation umask          */
#endif
#ifdef _UNIX
extern  meUShort  meUmask ;             /* file creation umask          */
extern  mode_t    meXUmask ;            /* directory creation umask     */
extern  uid_t     meUid ;               /* current user id              */
extern  gid_t     meGid ;               /* current group id             */
extern  int       meGidSize ;           /* number of groups the user belongs to   */
extern  gid_t    *meGidList ;           /* list of group ids the user belongs to  */
#endif /* _UNIX */

/* Signals */
#ifdef _UNIX
#ifdef _POSIX_SIGNALS
extern  sigset_t  meSigHoldMask ;       /* signals held when spawning and reading */
#endif /* _POSIX_SIGNALS */
#ifdef _BSD_SIGNALS
extern int meSigHoldMask;               /* signals held when spawning and reading */
#endif /* _BSD_SIGNALS */
extern int meSigLevel ;                 /* signal level */
#endif /* _UNIX */

/* Environment Variables */
#if ((defined(_UNIX)) && (defined(_NOPUTENV)))
extern char    **meEnviron;             /* Our own environment */
#endif /* ((defined(_UNIX)) && (defined(_NOPUTENV))) */

#ifdef _IPIPES
/* Incremental pipe variables */
extern meIPIPE  *ipipes ;               /* list of all the current pipes*/
extern int       noIpipes ;             /* count of all the cur pipes   */
#endif

#define meALARM_DIE       0x01
#define meALARM_WINSIZE   0x02
#define meALARM_VARIABLE  0x04
#define meALARM_PIPED     0x10

extern  meUByte   alarmState;           /* Auto-save alarm time         */
extern  meInt     startTime;            /* me start time. used as offset*/
extern  LINE     *menuLine ;            /* Menu-poke line */
extern  LINE     *lpStore;              /* line off which to store macro*/
extern  BUFFER   *lpStoreBp ;           /* help is stored in a buffer   */
extern  meUShort  thiskey;              /* the current key              */
extern  int       taboff;               /* tab offset for display       */
extern  meUShort  prefixc[];            /* prefix chars           */
extern  meUShort  reptc;                /* current universal repeat char*/
extern  meUShort  breakc;               /* current abort-command char*/
#ifdef _CLIPBRD
/* When implementing the clipboard it is best if the clipboard is only
 * attempted to be got or set once per command. This is because of other
 * window interaction.
 *
 * If the command is a macro the the following nasty things can happen if this
 * is not so
 *
 * 1) The 1st yank in a macro may return a different value to the 2nd yank, if
 *    the user defines another region in a different window.
 *
 * 2) If the macro is looping round cutting and pasting then the user will now
 *    be able to reliably cut 'n' paste in another window.
 *
 * 3) A macro could cut a region and the paste a different one!
 *
 * Nasty. Try and ensure these don't happen.
 */

#define CLIP_TRY_GET 0x01               /* We can try and get the clipbd*/
#define CLIP_TRY_SET 0x02               /* We can try and set the clipbd*/
#define CLIP_OWNER   0x04               /* We are the owners of clipbrd */
#define CLIP_STALE   0x08               /* Clipboard is stale           */
#define CLIP_RECVED  0x10               /* Clipboard has been obtained  */
extern  meUByte   clipState;            /* clipboard status flag        */
#endif
extern  meUInt    cursorBlink;          /* cursor-blink blink time      */
extern  int       blinkState;           /* cursor blink state           */
extern  meCOLOR   cursorColor;          /* cursor-color scheme          */
extern  meSCHEME  osdScheme;            /* Menu line color scheme       */
extern  meSCHEME  mlScheme;             /* Message line color scheme    */
extern  meSCHEME  mdLnScheme;           /* Mode line color scheme       */
extern  meSCHEME  sbarScheme;           /* Scroll bar color scheme      */
extern  meSCHEME  globScheme;           /* Global color scheme          */
extern  meSCHEME  trncScheme;           /* truncate color scheme        */

extern  int       gsbarmode;            /* global scroll bar mode       */
extern  meUByte   boxChars [];          /* Array of box characters      */
extern  meUByte   windowChars [];       /* Array of window characters   */
extern  meUByte   displayTab;           /* tab \t display character */
extern  meUByte   displayNewLine;       /* new-line \n display character */
extern  meUByte   displaySpace;         /* space ' ' display character */

extern  meUByte  *envars[];             /* list of recognized env. vars */
extern  meUByte  *derNames[];           /* name list of directives      */
extern  meUByte   derTypes[];           /* type list of directives      */
extern  meUByte  *funcNames[];          /* name list of user funcs      */
extern  meUByte   funcTypes[];          /* type list of user funcs      */
extern  KILL     *kbufp;                /* current kill buffer chunk pointer */
extern  KILL     *kbufh;                /* kill buffer header pointer   */
extern  meUByte   lkbdptr[];            /* Holds last keybd macro data  */
extern  int       lkbdlen;              /* Holds length of last macro   */
extern  meUByte  *kbdptr;               /* start of current keyboard buf*/
extern  int       kbdlen;               /* len of current macro         */
extern  int       kbdoff;               /* current offset of macro      */
extern  int       kbdrep;               /* number of repetitions        */
extern  meUByte   emptym[];             /* empty literal                */
extern  meUByte   errorm[];             /* error literal                */
extern  meUByte   abortm[];             /* abort literal                */
extern  meUByte   truem[];              /* true literal         */
extern  meUByte   falsem[];             /* false litereal               */
extern  meVARLIST usrVarList;           /* user variables list          */

#if CFENCE
extern meShort    statementIndent ;
extern meShort    continueIndent ;
extern meShort    continueMax ;
extern meShort    commentIndent ;
extern meShort    switchIndent ;
extern meShort    braceIndent ;
extern meShort    caseIndent ;
extern meShort    commentMargin ;
extern meUByte    commentContOrg[] ;
extern meUByte   *commentCont ;
#endif

/* global buffer names */
extern meUByte    BvariablesN[];
extern meUByte    BbindingsN[];
extern meUByte    BtranskeyN[];
extern meUByte    BcompleteN[];
extern meUByte    BcommandsN[];
extern meUByte    BicommandN[];
extern meUByte    BregistryN[];
extern meUByte    BbuffersN[];
extern meUByte    BcommandN[];
extern meUByte    BolhelpN[];
extern meUByte    BserverN[];
extern meUByte    BaboutN[];
extern meUByte    BstdinN[];
extern meUByte    BmainN[];
extern meUByte    BhelpN[];

extern meUByte   *fileIgnore ;
#if FLNEXT
extern meUByte   *flNextFileTemp ;
extern meUByte   *flNextLineTemp ;

extern meUByte    noNextLine ;
extern meUByte  **nextName ;
extern meUByte   *nextLineCnt ;
extern meUByte ***nextLineStr ;
#endif

extern meUByte    fileHookCount ;
extern meUByte  **fileHookExt ;
extern meUByte  **fileHookFunc ;
extern meShort   *fileHookArg ;

#if DORCS
extern meUByte   *rcsFile ;
extern meUByte   *rcsCoStr ;
extern meUByte   *rcsCoUStr ;
extern meUByte   *rcsCiStr ;
extern meUByte   *rcsCiFStr ;
extern meUByte   *rcsUeStr ;
#endif

/* history variables used in meGetStringFromUser */
extern  meUByte   numStrHist ;          /* curent no. of hist. strings    */
extern  meUByte   numBuffHist ;         /* curent no. of hist. buff names */
extern  meUByte   numCommHist ;         /* curent no. of hist. comm names */
extern  meUByte   numFileHist ;         /* curent no. of hist. file names */
extern  meUByte   numSrchHist ;         /* curent no. of hist. search strs*/
extern  meUByte **strHist ;             /* string history list            */
extern  meUByte **buffHist ;            /* etc.                           */
extern  meUByte **commHist ;
extern  meUByte **fileHist ;
extern  meUByte **srchHist ;
extern  meShort   HistNoFilesLoaded ;   /* Count of no files loaded by hist */

#if HILIGHT
extern meUByte     noHilights ;
extern HILNODEPTR *hilights ;
extern meUByte     noIndents ;
extern HILNODEPTR *indents ;
#endif

#if LCLBIND
extern meUByte    useMlBinds;           /* flag to use ml bindings      */
extern meUShort   mlNoBinds;            /* no. of message line bindings */
extern KEYTAB    *mlBinds;              /* pointer to ml local bindings */
#endif

/* uninitialized global external declarations */
extern meUByte    resultStr[MAXBUF] ;   /* $result variable             */
extern meUByte    evalResult[TOKENBUF] ;/* Result string from functions */
extern int        curgoal;              /* Goal for C-P, C-N            */
extern meShort    numWindows;           /* Current number of windows    */
extern WINDOW    *curwp;                /* Current window               */
extern BUFFER    *curbp;                /* Current buffer               */
extern WINDOW    *wheadp;               /* Head of list of windows      */
extern BUFFER    *bheadp;               /* Head of list of buffers      */
extern meABBREV  *aheadp;               /* Head of list of abrev files  */

extern struct KLIST *klhead;            /* Head of klist                */

/* the character lookup tables */
extern meUByte    charMaskTbl1[] ;
extern meUByte    charMaskTbl2[] ;
extern meUByte    charCaseTbl[] ;
extern meUByte    charLatinUserTbl[] ;
extern meUByte    charUserLatinTbl[] ;
extern meUByte    charMaskFlags[] ;
extern meUByte    isWordMask ;

/* the following are global variables but not defined in this file */
extern VVIDEO     vvideo;               /* Virtual video - see display.c */
extern int        mlfirst;              /* initial command, set by respawn() */
extern meUByte    meCopyright[] ;

/* fileio file pointer */
#ifdef _WIN32
extern HANDLE     ffrp;
extern HANDLE     ffwp;
#else
extern FILE      *ffrp;
extern FILE      *ffwp;
#endif

/* the following are the cursor position and ml variables */
extern int        mwRow ;               /* Main Windows current row. */
extern int        mwCol ;               /* Main Windows current column */
extern int        osdCol ;              /* The osd current column */
extern int        osdRow ;              /* The osd current row */

#define MLSTATUS_KEEP    0x01
#define MLSTATUS_RESTORE 0x02
#define MLSTATUS_POSML   0x04
#define MLSTATUS_POSOSD  0x08
#define MLSTATUS_CLEAR   0x10
#define MLSTATUS_NINPUT  0x20
#define MLSTATUS_OSDPOS  0x40
extern meUByte    mlStatus ;
extern int        mlCol ;
extern meUByte   *mlStore ;
extern int        mlStoreCol ;

/**************************************************************************
* Constant declarations for the external definitions above. These are     *
* only processed in main.c                                                *
**************************************************************************/
#ifdef  maindef

/* for MAIN.C */

/* initialized global definitions */
#ifdef _INSENSE_CASE
meDIRLIST curDirList={0,0} ;
#else
meDIRLIST curDirList={1,0} ;
#endif
#if MEOSD
struct osdDIALOG  *osdDialogHd = NULL;  /* Root of the on screen displays */
struct osdDISPLAY *osdDisplayHd = NULL; /* Menu display list */
#endif
meREGISTERS *meRegHead=NULL ;           /* The register Head            */
meREGISTERS *meRegCurr=NULL ;           /* The current register set     */
SELHILIGHT selhilight={1,0} ;           /* Selection hilight            */
meUShort  hilBlockS=20 ;                /* Hilight - HilBlock array siz */
meSTYLE  *styleTable = NULL;            /* Highlighting colour table    */
#if ABBREV
meABBREV *globalAbrev = NULL;           /* Global Abreviation file      */
#endif
mePOS    *mePosition=NULL ;             /* Position stack head          */
meCOLOR   noColors=0 ;                  /* No defined colours           */
int       styleTableSize = 2;           /* Size of the style table      */
HILBLOCK *hilBlock;                     /* Hilighting style change      */
meUByte  *disLineBuff=NULL ;            /* interal display buffer array */
int       disLineSize=512 ;             /* interal display buffer size  */
int       cursorState=0 ;               /* Current state of cursor      */
FRAMELINE *frameStore = NULL;           /* Frame store line pointers    */
meUByte  *progName=NULL ;               /* the program name (argv[0])   */
meUByte   orgModeLineStr[]="%s%r%u me (%e) - %l %b (%f) ";
meUByte  *modeLineStr=orgModeLineStr;   /* current modeline format      */
meInt     autotime = 300 ;              /* auto save time in seconds    */
int       keptVersions = 0 ;            /* No. of kept backup versions  */
meUByte   fillbullet[16]="*)].-";       /* Fill bullet character class  */
meShort   fillbulletlen = 15;           /* Fill lookahead limit         */
meShort   fillcol = 78;                 /* Current fill column          */
meUByte   filleos[16]=".!?";            /* Fill E-O-S character class   */
meShort   filleoslen=1;                 /* Fill E-O-S ' ' insert len    */
meUByte   fillignore[16]=">_@";         /* Fill Ignore character class  */
meUShort  tabsize  = 4;                 /* Virtual Tab size             */
meUShort  tabwidth = 8;                 /* Real TAB size                */
meShort   matchlen = 2000;              /* Fence matching sleep length  */
meUByte  *searchPath=NULL;              /* emf search path              */
meUByte  *homedir=NULL;                 /* Home directory               */
meUByte  *curdir=NULL;                  /* current working directory    */
meUByte  *execstr = NULL;               /* pointer to string to execute */
meUInt    delaytime = 500;              /* mouse-time delay time 500ms  */
meUInt    repeattime = 25;              /* mouse-time repeat time 25ms  */
meUInt    idletime = 1000;              /* idle-time delay time 1sec    */
int       execlevel = 0;                /* execution IF level           */
int       bufHistNo = 1;                /* inc buff hist numbering      */
int       lastCommand = 0 ;             /* The last user executed key   */
int       lastIndex = -1 ;              /* The last user executed comm  */
int       thisCommand = 0 ;             /* The cur. user executed key   */
int       thisIndex = -1 ;              /* The cur. user executed comm  */

#ifdef _WIN32
HANDLE    ffrp;                         /* File read pointer, all func. */
HANDLE    ffwp;                         /* File write pointer, all func.*/
#else
FILE     *ffrp;                         /* File read pointer, all func. */
FILE     *ffwp;                         /* File write pointer, all func.*/
#endif
meUShort  thiskey ;                     /* the current key              */
meUByte   hexdigits[] = "0123456789ABCDEF";
meUInt    cursorBlink = 0;              /* cursor-blink blink time      */
int       blinkState=1;                 /* cursor blink state           */
meCOLOR   cursorColor=meCOLOR_FDEFAULT; /* cursor color                 */
meSCHEME  osdScheme =meSCHEME_NDEFAULT; /* Menu line color scheme       */
meSCHEME  mlScheme  =meSCHEME_NDEFAULT; /* Message line color scheme    */
meSCHEME  mdLnScheme=meSCHEME_RDEFAULT; /* Mode line color scheme       */
meSCHEME  sbarScheme=meSCHEME_RDEFAULT; /* Scroll bar color scheme      */
meSCHEME  globScheme=meSCHEME_NDEFAULT; /* Global color scheme          */
meSCHEME  trncScheme=meSCHEME_NDEFAULT; /* Truncate color scheme        */
int       gsbarmode = (WMUP |           /* Has upper end cap            */
                       WMDOWN |         /* Has lower end cap            */
                       WMBOTTM |        /* Has a mode line character    */
                       WMSCROL |        /* Has a box on scrolling shaft */
                       WMRVBOX |        /* Reverse video on box         */
                       WMSPLIT |        /* Has a splitter               */
                       WMVBAR           /* Window has a vertical bar    */
                      );                /* global scroll bar mode       */
meUByte   boxChars[BCLEN+1] =           /* Set of box characters        */
"|+++++++++-";
meUByte   windowChars[WCLEN+1] =        /* Set of window characters     */
{
    '=',                                /* Mode line current sep */
    '-',                                /* Mode libe inactive sep */
    '#',                                /* Root indicator */
    '*',                                /* Edit indicator */
    '%',                                /* View indicator */
    /* Single scroll bar */
    '=',                                /* Buffer split */
    '^',                                /* Scroll bar up */
    ' ',                                /* Scroll bar up shaft */
    ' ',                                /* Scroll box */
    ' ',                                /* Scroll bar down shaft */
    'v',                                /* Scroll bar down */
    '*',                                /* Scroll bar/mode line point */
    /* Double scroll bar */
    '=','=',                            /* Buffer split */
    '^','^',                            /* Scroll bar up */
    ' ',' ',                            /* Scroll bar up shaft */
    ' ',' ',                            /* Scroll box */
    ' ',' ',                            /* Scroll bar down shaft */
    'v','v',                            /* Scroll bar down */
    '*','*',                            /* Scroll bar/mode line point */
    /* Single horizontal scroll bar */
    '|',                                /* Buffer split */
    '<',                                /* Scroll bar left */
    ' ',                                /* Scroll bar left shaft */
    ' ',                                /* Scroll box */
    ' ',                                /* Scroll bar right shaft */
    '>',                                /* Scroll bar right */
    '*',                                /* Scroll bar/mode line point */
    /* Double horizontal scroll bar */
    '|','|',                            /* Buffer split */
    '<','<',                            /* Scroll bar left */
    ' ',' ',                            /* Scroll bar left shaft */
    ' ',' ',                            /* Scroll box */
    ' ',' ',                            /* Scroll bar right shaft */
    '>','>',                            /* Scroll bar right */
    '*','*',                            /* Scroll bar/mode line point */
    ' ',                                /* OSD title bar char */
    'x',                                /* OSD Title bar kill char */
    '*',                                /* OSD resize char */
    ' ',                                /* OSD button start char e.g. '['   */
    ' ',                                /* OSD button close char e.g. ']'   */
    /* Display characters */
    '>',                                /* tab \t display character */
    '\\',                               /* new-line \n display character */
    '.',                                /* space ' ' display character */
    0
} ;
meUByte   displayTab=' ';               /* tab \t display character */
meUByte   displayNewLine=' ';           /* new-line \n display character */
meUByte   displaySpace=' ';             /* space ' ' display character */
meInt     startTime;                    /* me start time used as offset */
meUByte   thisflag;                     /* Flags, this command          */
meUByte   lastflag;                     /* Flags, last command          */
meUByte   alarmState=0;                 /* Unix auto-save alarm time    */
meUByte   fillmode='B';                 /* Justification mode           */
meUByte   scrollFlag = 1 ;              /* horiz/vert scrolling method  */
meUByte   sgarbf = TRUE;                /* TRUE if screen is garbage    */
meUByte   clexec = FALSE;               /* command line execution flag  */
meUByte   mcStore = FALSE;              /* storing text to macro flag   */
meByte    macbug = 0 ;                  /* macro debuging flag          */
meUByte   cmdstatus = TRUE;             /* last command status          */
meUByte   kbdmode=STOP;                 /* current keyboard macro mode  */
meUByte   lastReplace=0;                /* set to non-zero if last was a replace */
meUByte   modeLineFlags=                /* current modeline flags       */
(WFMODE|WFRESIZE|WFMOVEL) ;
meUInt    meSystemCfg=                  /* ME system config variable    */
#ifdef _DOS
(meSYSTEM_ANSICOLOR|meSYSTEM_XANSICOLOR|meSYSTEM_MSSYSTEM|meSYSTEM_DRIVES|meSYSTEM_DOSFNAMES|meSYSTEM_TABINDANY|meSYSTEM_ALTMENU|meSYSTEM_ALTPRFX1) ;
#endif
#ifdef _WIN32
(meSYSTEM_RGBCOLOR|meSYSTEM_FONTS|meSYSTEM_MSSYSTEM|meSYSTEM_DRIVES|meSYSTEM_DOSFNAMES|meSYSTEM_TABINDANY|meSYSTEM_ALTMENU|meSYSTEM_ALTPRFX1|meSYSTEM_CTCHASPC
#ifdef _IPIPES
|meSYSTEM_IPIPES
#endif
) ;
#endif
#ifdef _UNIX
(meSYSTEM_RGBCOLOR|meSYSTEM_FONTS|meSYSTEM_UNIXSYSTEM|meSYSTEM_IPIPES|meSYSTEM_TABINDANY|meSYSTEM_ALTMENU|meSYSTEM_ALTPRFX1) ;
#endif
#ifdef _WIN32
meUInt    meSYSTEM_MASK=                /* ME system mask - dependant on win32 flavour */
(meSYSTEM_FONTS|meSYSTEM_TABINDANY|meSYSTEM_ALTMENU|meSYSTEM_ALTPRFX1|meSYSTEM_KEEPUNDO|meSYSTEM_FONTFIX|meSYSTEM_CTCHASPC|meSYSTEM_SHOWWHITE|meSYSTEM_HIDEBCKUP|meSYSTEM_TABINDFST|meSYSTEM_NOEMPTYANK
#if !defined (_WIN32s)
|meSYSTEM_CLNTSRVR
#endif
) ;
#endif
meUInt    meMouseCfg=(3|meMOUSE_ENBLE); /* ME mouse config variable    */
meUByte   mouse_pos=0xff;               /* mouse virtual position       */
meShort   mouse_X = 0;                  /* mouse X pos at last event    */
meShort   mouse_Y = 0;                  /* mouse X pos at last event    */
meShort   mouse_dX = 0;                 /* mouse delta X last event pos */
meShort   mouse_dY = 0;                 /* mouse delta X last event pos */

#if MEUNDO
meUInt    undoContFlag=0;               /* continuation of an undo      */
#endif

/* File Modes */
#if (defined _DOS) || (defined _WIN32)
meUShort  meUmask=0x020 ;               /* file creation umask          */
#endif
#ifdef _UNIX
meUShort  meUmask ;                     /* file creation umask          */
mode_t    meXUmask ;                    /* directory creation umask     */
uid_t     meUid ;                       /* current user id              */
gid_t     meGid ;                       /* current group id             */
int       meGidSize ;                   /* number of groups the user belongs to   */
gid_t    *meGidList=NULL ;              /* list of group ids the user belongs to  */
#endif /* _UNIX */

/* Signals */
#ifdef _UNIX
#ifdef _POSIX_SIGNALS
sigset_t  meSigHoldMask ;               /* signals help when spawning and reading */
#endif /* _POSIX_SIGNALS */
#ifdef _BSD_SIGNALS
int       meSigHoldMask = 0;            /* signals held when spawning and reading */
#endif /* _BSD_SIGNALS */
int       meSigLevel = 0;               /* signal level */
#endif /* _UNIX */

/* Environment Variables */
#if ((defined(_UNIX)) && (defined(_NOPUTENV)))
char    **meEnviron = NULL;             /* Our own environment */
#endif /* ((defined(_UNIX)) && (defined(_NOPUTENV))) */

#ifdef _IPIPES
meIPIPE  *ipipes=NULL ;                 /* list of all the current pipes*/
int       noIpipes=0 ;                  /* count of all the cur pipes   */
#endif

LINE     *menuLine = NULL;              /* Menu-poke line */
LINE     *lpStore = NULL;               /* line off which to store macro*/
BUFFER   *lpStoreBp = NULL;             /* help is stored in a buffer   */
meUShort  prefixc[ME_PREFIX_NUM+1]=
{ ME_INVALID_KEY,                       /* unused 0th value             */
  ME_SPECIAL|SKEY_esc,                  /* prefix1 = Escape             */
  'X'-'@',                              /* prefix2 = ^X                 */
  'H'-'@',                              /* prefix3 = ^H                 */
  ME_INVALID_KEY, ME_INVALID_KEY,       /* rest unused                  */
  ME_INVALID_KEY, ME_INVALID_KEY,
  ME_INVALID_KEY, ME_INVALID_KEY,
  ME_INVALID_KEY, ME_INVALID_KEY,
  ME_INVALID_KEY, ME_INVALID_KEY,
  ME_INVALID_KEY, ME_INVALID_KEY,
  ME_INVALID_KEY
} ;
meUShort  reptc    = 'U'-'@';           /* current universal repeat char*/
meUShort  breakc   = 'G'-'@';           /* current abort-command char*/

KLIST    *klhead    = NULL;             /* klist header pointer            */
meUByte   lkbdptr[NKBDM];               /* Holds last keybd macro data     */
int       lkbdlen=0;                    /* Holds length of last macro      */
meUByte  *kbdptr=NULL;                  /* start of current keyboard buf   */
int       kbdlen=0;                     /* len of current macro            */
int       kbdoff=0;                     /* current offset of macro         */
int       kbdrep=0;                     /* number of repetitions           */
meUByte   emptym[]  = "";               /* empty literal                   */
meUByte   errorm[]  = "ERROR";          /* error literal                   */
meUByte   abortm[]  = "ABORT";          /* abort literal                   */
meUByte   truem[]   = "1";              /* true literal            */
meUByte   falsem[]  = "0";              /* false litereal                  */
meVARLIST usrVarList={NULL,0} ;         /* user variables list             */


#if CFENCE
meShort   statementIndent = 4 ;
meShort   continueIndent  = 10 ;
meShort   continueMax     = 0 ;
meShort   commentIndent   = 1 ;
meShort   switchIndent    = 0 ;
meShort   braceIndent     = -4 ;
meShort   caseIndent      = -4 ;
meShort   commentMargin   = -1 ;
meUByte   commentContOrg[]= " * " ;
meUByte  *commentCont     = commentContOrg ;
#endif

/* global buffer names */
meUByte   BvariablesN[] = "*variables*" ;
meUByte   BbindingsN[] = "*bindings*" ;
meUByte   BtranskeyN[] = "*translate-key*" ;
meUByte   BcompleteN[] = "*complete*" ;
meUByte   BcommandsN[] = "*commands*" ;
meUByte   BicommandN[] = "*icommand*" ;
meUByte   BregistryN[] = "*registry*" ;
meUByte   BbuffersN[] = "*buffers*" ;
meUByte   BcommandN[] = "*command*" ;
meUByte   BolhelpN[] = "*on-line help*" ;
meUByte   BserverN[] = "*server*" ;
meUByte   BaboutN[] = "*about*" ;
meUByte   BstdinN[] = "*stdin*" ;
meUByte   BmainN[] = "*scratch*" ;
meUByte   BhelpN[] = "*help*" ;

meUByte  *fileIgnore=NULL ;

#if FLNEXT
meUByte  *flNextFileTemp=NULL ;
meUByte  *flNextLineTemp=NULL ;

meUByte    noNextLine=0 ;
meUByte  **nextName=NULL ;
meUByte   *nextLineCnt=NULL ;
meUByte ***nextLineStr=NULL ;
#endif

meUByte   fileHookCount=0 ;
meUByte **fileHookExt=NULL ;
meUByte **fileHookFunc=NULL ;
meShort  *fileHookArg=NULL ;

#if DORCS
meUByte  *rcsFile=NULL ;
meUByte  *rcsCoStr=NULL ;
meUByte  *rcsCoUStr=NULL ;
meUByte  *rcsCiStr=NULL ;
meUByte  *rcsCiFStr=NULL ;
meUByte  *rcsUeStr=NULL ;
#endif

/* history variables used in meGetStringFromUser */
meUByte   numStrHist = 0 ;              /* curent no. of hist. strings    */
meUByte   numBuffHist = 0 ;             /* curent no. of hist. buff names */
meUByte   numCommHist = 0 ;             /* curent no. of hist. comm names */
meUByte   numFileHist = 0 ;             /* curent no. of hist. file names */
meUByte   numSrchHist = 0 ;             /* curent no. of hist. search strs*/
meUByte **strHist ;                     /* string history list            */
meUByte **buffHist ;                    /* etc.                           */
meUByte **commHist ;
meUByte **fileHist ;
meUByte **srchHist ;
meShort   HistNoFilesLoaded = 0 ;       /* Count of no files loaded by hist */

#if HILIGHT
meUByte     noHilights=0 ;
HILNODEPTR *hilights=NULL ;
meUByte     noIndents=0 ;
HILNODEPTR *indents=NULL ;
#endif

#if LCLBIND
meUByte   useMlBinds=0;                 /* flag to use ml bindings      */
meUShort  mlNoBinds=0;                  /* no. of message line bindings */
KEYTAB   *mlBinds=NULL;                 /* pointer to ml local bindings */
#endif

int       curgoal;                      /* Goal for C-P, C-N            */
meShort   numWindows;                   /* Number of windows            */
WINDOW   *curwp;                        /* Current window               */
BUFFER   *curbp;                        /* Current buffer               */
WINDOW   *wheadp;                       /* Head of list of windows      */
BUFFER   *bheadp;                       /* Head of list of buffers      */
meABBREV *aheadp=NULL;                  /* Head of list of abrev files  */

meUShort  keyTableSize;                 /* The current size of the key table */
meUByte   resultStr [MAXBUF];           /* Result string from commands  */

/* the following are the cursor position and ml variables */
int       mwRow=0 ;                     /* Main Windows current row. */
int       mwCol=0 ;                     /* Main Windows current column */
int       osdCol=0 ;                    /* The osd current column */
int       osdRow=0 ;                    /* The osd current row */
int       mlCol=0 ;                     /* ml current column */
int       mlStoreCol=0 ;                /* ml Store column */
meUByte  *mlStore;                      /* stored message line. */
meUByte   mlStatus=0 ;                  /* ml status
                                         * 0=not using it,
                                         * 1=using it.
                                         * 2=using it & its been broken so
                                         * next time mlerease is used, it will
                                         * restore */
#ifdef _CLIPBRD
meUByte   clipState=0;                  /* clipboard status flag        */
#endif
#endif
