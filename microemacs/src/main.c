/* -*- c -*-
 *
 * JASSPA MicroEmacs - www.jasspa.com
 * main.c - Main entry point.
 *
 * Originally written by Dave G. Conroy for MicroEmacs
 * Copyright (C) 1987 by Daniel M. Lawrence
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
 * Synopsis:    Main entry point.
 * Authors:     Dave G. Conroy, Steve Wilhite, George Jones,
 *              Daniel M. Lawrence,
 *              Jon Green & Steven Phillips (JASSPA)
 */

#define	__MAINC			/* Define program name */

#define	INC_MODE_DEF

#include "evers.h"
#include "emain.h"

#define	maindef		/* Make main defintions - cannot define this at the top
                         * because all the main defs are needed to init edef's vars */

#include "efunc.h"	/* function declarations and name table	*/
#include "eskeys.h"     /* Special key names - Must include before edef.h & ebind.h */
#include "edef.h"	/* The main global variables		*/
#include "ebind.h"	/* default bindings			*/
#include "evar.h"	/* env. vars and user funcs		*/
#include "esearch.h"	/* defines PTBEG PTEND			*/

#if (defined _UNIX) || (defined _DOS) || (defined _WIN32)
#include <sys/types.h>
#include <time.h>
#ifndef _WIN32
#include <sys/time.h>
#endif
#ifdef _UNIX
#include <fcntl.h>
#include <sys/wait.h>
#endif
#endif

static char meHelpPage[]=
"usage     : me [options] [files]\n\n"
"where options can be:-\n"
"  @<file> : Setup using <file>[.emf], default is me.emf\n"
"  -b      : Load next file as a binary file\n"
"  -c      : Continuation mode (last edit, must have history setup)\n"
#if MEOPT_DEBUGM
"  -d      : Debug mode (for macro files)\n"
#endif
"  -h      : For this help page\n"
#ifdef _DOS
"  -i      : Insert the current screen into the *scratch* buffer\n"
#endif
"  -k[key] : Load next file as a crypted file optionally giving the <key>\n"
"  +<n> or\n"
"  -l <n>  : Go to line <n> in the next given file\n"
#if MEOPT_CLIENTSERVER
"  -m <msg>: Post message <msg> to MicroEmacs server\n"
#endif
#ifdef _ME_WINDOW
#ifdef _ME_CONSOLE
#ifdef _WIN32
"  -n      : For no MS window, use console instead\n"
#endif
#ifdef _XTERM
"  -n      : For no X window, use console instead\n"
#endif
#endif /* _ME_CONSOLE */
#endif /* _ME_WINDOW */
#if MEOPT_CLIENTSERVER
"  -o      : One MicroEmacs, use ME server if available\n"
#endif
#if MEOPT_EXTENDED
"  -p      : Pipe stdin into *stdin*, when saved output to stdout\n"
#endif
"  -r      : Read-only, all buffers will be in view mode\n"
"  -s <s>  : Search for string <s> in the next given file\n"
"  -u <n>  : Set user name to <n> (sets $MENAME)\n"
"  -v <v=s>: Set variable <v> to string <s>\n"
#ifdef _UNIX
"  -x      : Don't catch signals\n"
#endif
"  -y      : Load next file as a reduced binary file\n"
"\n"
;

#ifdef _WIN32
#define mePrintHelpMessage(mm) MessageBox(NULL,(char *) mm,ME_FULLNAME " '" meVERSION,MB_OK);
#else
#define mePrintHelpMessage(mm) write(2,mm,meStrlen(mm))
#endif

/*
 * Initialize the data structures used by the display code. The edge vectors
 * used to access the screens are set up. The operating system's terminal I/O
 * channel is set up. All the other things get initialized at compile time.
 * The original window has "WFCHG" set, so that it will get completely
 * redrawn on the first call to "update".
 */

/*
 * Initialize all of the buffers and windows. The buffer name is passed down
 * as an argument, because the main routine may have been told to read in a
 * file by default, and we want the buffer name to be right.
 */
static void
meInit(meUByte *bname)
{
    meBuffer *bp;
    
    if (TTstart() == meFALSE)             /* Started ?? */
        meExit (1);                     /* No quit */
    
    /* add 2 to hilBlockS to allow for a double trunc-scheme change */
    if(((styleTable = malloc(2*meSCHEME_STYLES*sizeof(meStyle))) == NULL) ||
       ((hilBlock = malloc((hilBlockS+2)*sizeof(meSchemeSet))) == NULL) ||
       ((disLineBuff = malloc((disLineSize+32)*sizeof(meUByte))) == NULL))
        meExit(1);

    memcpy(styleTable,defaultScheme,2*meSCHEME_STYLES*sizeof(meStyle));
    /* Set the fore and back colours */
    TTaddColor(meCOLOR_FDEFAULT,255,255,255) ;
    TTaddColor(meCOLOR_BDEFAULT,0,0,0) ;
    
    if((frameCur=meFrameInit(NULL)) == NULL)
        meExit(1);
#if MEOPT_FRAME
    frameList = frameCur ;
#endif
        
    if(((bp = bfind(bname,BFND_CREAT)) == NULL) ||
       (meFrameInitWindow(frameCur,bp) != meTRUE))
        meExit(1);
}

/*
 * insertChar
 *
 * Insert n * 'c' chars into the current buffer
 * Copes with insert and undo modes correctly
 */
int
insertChar(register int c, register int n)
{
    /*---    If we are  in overwrite mode,  not at eol,  and next char is
       not a tab or we are at a tab stop, delete a char forword */

    if(meModeTest(frameCur->bufferCur->mode,MDOVER))
    {
        int index, ii=n ;
        for(index=0,n=0 ; (ii>0) && (frameCur->windowCur->dotOffset < frameCur->windowCur->dotLine->length) ; ii--)
        {
            if((meLineGetChar(frameCur->windowCur->dotLine, frameCur->windowCur->dotOffset) != TAB) ||
               (at_tab_pos(getccol()+index+1) == 0))
            {
                lchange(WFMAIN);
#if MEOPT_UNDO
                meUndoAddRepChar() ;
#endif
                meLineSetChar(frameCur->windowCur->dotLine,frameCur->windowCur->dotOffset++,c) ;
                index = 0 ;
            }
            else
            {
                index++ ;
                n++ ;
            }
        }
        n += ii ;
    }
    /*---    Do the appropriate insertion */
    if(n > 0)
    {
#if MEOPT_UNDO
        if(n == 1)
        {
            meUndoAddInsChar() ;
            if(linsert(1,c) != meTRUE)
                return meFALSE ;
        }
        else
        {
            if(linsert(n, c) != meTRUE)
                return meFALSE ;
            meUndoAddInsChars(n) ;
        }
#else
        if(linsert(n, c) != meTRUE)
            return meFALSE ;
#endif
    }
    return meTRUE ;
}

/*
 * This is the general command execution routine. It handles the fake binding
 * of all the keys to "self-insert". It also clears out the "thisflag" word,
 * and arranges to move it to the "lastflag", so that the next command can
 * look at it. Return the status of command.
 */
int
execute(register int c, register int f, register int n)
{
    register int index;
    meUInt arg ;
    int ii ;

#if MEOPT_CALLBACK
    {
        /* If any of the modifiers have changed since the last key we executed
         * then execute any commands bound to the M-pick or M-drop pseudo keys,
         * e.g. the S-pick key will be generated between the following 2 keys:
         *      right S-right. 
         * These keys only get 'pressed' if a command is bound to them. The
         * commands are executed in a hidden way, so they are not recorded in
         * kbd macros or in the $recent-keys. This could led to a macro failing
         * but alternatives have similar if not worse problems.
         */
        static meUShort lastcc=0 ;
        meUInt arg ;
        meUShort mask, kk, ccx=lastcc ^ c ;
        
        lastcc = c ;
        for(mask=ME_SHIFT ; mask != ME_SPECIAL ; mask<<=1)
        {
            if(ccx & mask)
            {
                if(c & mask)
                    kk = ME_SPECIAL|SKEY_pick|mask ;
                else
                    kk = ME_SPECIAL|SKEY_drop|mask ;
                if((index=decode_key(kk,&arg)) != -1)
                    execFuncHidden(kk,index,arg) ;
            }
        }
    }
#endif
    
    if (kbdmode != mePLAY)
    {
#ifdef _CLIPBRD
        clipState |= CLIP_TRY_GET|CLIP_TRY_SET ;
#endif
#if MEOPT_UNDO
        undoContFlag++ ;
#endif
    }
    lastCommand = thisCommand ;
    lastIndex = thisIndex ;
    thisCommand = c ;

    if(((index = decode_key((meUShort) c,&arg)) >= 0) && (arg != 0))
    {
        f = 1 ;
        n *= (int) (arg + 0x80000000) ;
    }
    thisIndex = index ;
    meRegCurr->f = f ;
    meRegCurr->n = n ;
    /* SWP - 27/2/99 - to enable input commands to not have to jump through
     * so many hoops I've changed to input support so the macros can fail
     * in such a way as to indicate that they have not handled this input,
     * this allows them to pick and choose.
     * The method chosen was to check the command variable status if it aborted, if
     * defined to "0" then its not handled the input
     */
    if((ii=frameCur->bufferCur->inputFunc) >= 0)
    {
        meUByte *ss ;
        if(((cmdstatus = (execFunc(ii,f,n) == meTRUE))) ||
           ((ss=getUsrLclCmdVar((meUByte *)"status",&(cmdTable[ii]->varList))) == errorm) || meAtoi(ss))
            return cmdstatus ;
    }
    if(index >= 0)
        return (cmdstatus = (execFunc(index,f,n) == meTRUE)) ;
    if(selhilight.flags)
    {
        selhilight.bp = NULL;
        selhilight.flags = 0;
    }
    if(c < 0x20 || c > 0xff)    /* If not an insertable char */
    {
        meUByte outseq[40];	/* output buffer for keystroke sequence */
        meGetStringFromKey((meUShort) c,outseq);  /* change to something printable */
        lastflag = 0;                             /* Fake last flags.     */
        cmdstatus = 0 ;
#if MEOPT_MOUSE
        /* don't complain about mouse_move* or mouse_time* as these are
         * special keys that are only added if they are bound and due
         * to their frequence they can be in the input buffer before they're
         * unbound leading to this error */
        c &= ~(ME_SHIFT|ME_CONTROL|ME_ALT) ;
        if(((c >= (ME_SPECIAL|SKEY_mouse_move)) && (c <= (ME_SPECIAL|SKEY_mouse_move_3))) ||
           ((c >= (ME_SPECIAL|SKEY_mouse_time)) && (c <= (ME_SPECIAL|SKEY_mouse_time_3))) )
            return meTRUE ;
#endif
        return mlwrite(MWABORT,(meUByte *)"[Key not bound \"%s\"]", outseq); /* complain */
    }

    if (n <= 0)            /* Fenceposts.          */
    {
        lastflag = 0;
        return (cmdstatus = ((n<0) ? meFALSE : meTRUE));
    }
    thisflag = 0;          /* For the future.      */

    if(bchange() != meTRUE)               /* Check we can change the buffer */
        return (cmdstatus = meFALSE) ;

#if MEOPT_WORDPRO
    /* If a space was  typed, fill column is  defined, the argument is non-
     * negative, wrap mode is enabled, and we are now past fill column, and
     * we are not read-only, perform word wrap. */

    if(meModeTest(frameCur->bufferCur->mode,MDWRAP) &&
       ((c == ' ') || (meStrchr(filleos,c) != NULL)) &&
       (fillcol > 0) && (n >= 0) && (getccol() > fillcol) &&
       !meModeTest(frameCur->bufferCur->mode,MDVIEW))
        wrapWord(meFALSE, 1);
#endif
    
    /* insert the required number of chars */
    if(insertChar(c,n) != meTRUE)
        return (cmdstatus = meFALSE) ;

#if MEOPT_CFENCE
    if(meModeTest(frameCur->bufferCur->mode,MDCMOD))
    {
        if((c == '}') || (c == '#'))
        {
            ii = frameCur->windowCur->dotOffset ;
            frameCur->windowCur->dotOffset = 0 ;
            if(gotoFrstNonWhite() == c)
            {
                frameCur->windowCur->dotOffset = ii ;
                doCindent(&ii) ;
            }
            else
                frameCur->windowCur->dotOffset = ii ;
        }
        else if((commentMargin >= 0) &&
                ((c == '*') || (c == '/')) && (frameCur->windowCur->dotOffset > 2) &&
                (meLineGetChar(frameCur->windowCur->dotLine, frameCur->windowCur->dotOffset-2) == '/') &&
                (meLineGetChar(frameCur->windowCur->dotLine, frameCur->windowCur->dotOffset-3) != '/'))
        {
            ii = frameCur->windowCur->dotOffset - 2 ;
            if((gotoFrstNonWhite() == 0) &&
               (frameCur->windowCur->dotOffset=0,(gotoFrstNonWhite() != 0)) &&
               (frameCur->windowCur->dotOffset < ii))
            {
                frameCur->windowCur->dotOffset = ii ;
                if(ii > commentMargin)
                    ii = (tabsize-1) - ((ii-1)%tabsize) ;
                else
                    ii = commentMargin - ii ;
                linsert(ii,' ') ;
#if MEOPT_UNDO
                meUndoAddInsChars(ii) ;
#endif
            }
            else
                frameCur->windowCur->dotOffset = ii ;
            frameCur->windowCur->dotOffset += 2 ;
        }
    }
    /* check for fence matching */
    if(meModeTest(frameCur->bufferCur->mode,MDFENCE) && ((c == '}') || (c == ')') || (c == ']')))
    {
        frameCur->windowCur->dotOffset-- ;
        /* flag for delay move and only bell in cmode */
        gotoFence(meTRUE,(meModeTest(frameCur->bufferCur->mode,MDCMOD)) ? 3:2) ;
        frameCur->windowCur->dotOffset++ ;
    }
#endif
    lastflag = thisflag;
    return (cmdstatus = meTRUE) ;
}


/*
** Me info
** Creates a *info* buffer and populates it with info on the current state of things.
*/

static void
addModesList(meBuffer *bp, register meUByte *buf, meUByte *name, meMode mode,
             int res)
{
    register int ii, nlen, len, ll ;

    meStrcpy(buf,name) ;
    len = nlen = meStrlen(name) ;
    for (ii = 0; ii < MDNUMMODES; ii++)	/* add in the mode flags */
        if((meModeTest(mode,ii) != 0) == res)
        {
            ll = meStrlen(modeName[ii]);
            if(len+ll >= 78)
            {
                buf[len] = '\0' ;
                addLineToEob(bp,buf) ;
                len = nlen ;
                memset(buf,' ',len) ;
            }
            buf[len++] = ' ' ;
            meStrcpy(buf+len, modeName[ii]);
            len += ll ;
        }
    buf[len] = '\0' ;
    addLineToEob(bp,buf) ;
}

static void
addModesLists(meBuffer *bp, register meUByte *buf, meMode mode)
{
    addLineToEob(bp,(meUByte *)"") ;
    addModesList(bp,buf,(meUByte *)"  Modes on  :",mode,1) ;
    addModesList(bp,buf,(meUByte *)"  Modes off :",mode,0) ;
    addLineToEob(bp,(meUByte *)"") ;
}

meUByte meCopyright[]="Copyright (C) 1988-" meCENTURY meYEAR " " ME_COMPANY_NAME " (" ME_COMPANY_SITE ")" ;
int
meAbout(int f, int n)
{
    meWindow *wp ;
    meBuffer *bp, *tbp ;
    meInt   numchars ;		/* # of chars in file */
    meInt   numlines ;		/* # of lines in file */
    meInt   predchars ;		/* # chars preceding point */
    meInt   predlines ;		/* # lines preceding point */
    meUByte   buf[meBUF_SIZE_MAX] ;
    int     ii ;

    if((wp = wpopup(BaboutN,(BFND_CREAT|BFND_CLEAR|WPOP_USESTR))) == NULL)
        return meFALSE ;
    bp = wp->buffer ;

     /* definitions in evers.h */
    addLineToEob(bp,(meUByte *)ME_FULLNAME " " meVERSION " - Date " meDATE " - " meSYSTEM_NAME) ;
    addLineToEob(bp,(meUByte *)"") ;
    addLineToEob(bp,(meUByte *)"Global Status:") ;
    tbp = bheadp ;
    ii = 0 ;
    while(tbp != NULL)
    {
        ii++ ;
        tbp = tbp->next ;
    }
    sprintf((char *)buf,"  # buffers : %d", ii) ;
    addLineToEob(bp,buf) ;
    addModesLists(bp,buf,globMode) ;
    addLineToEob(bp,(meUByte *)"Current Buffer Status:") ;
    sprintf((char *)buf,"  Buffer    : %s", frameCur->bufferCur->name) ;
    addLineToEob(bp,(meUByte *)buf) ;
    sprintf((char *)buf,"  File name : %s",
            (frameCur->bufferCur->fileName == NULL) ? (meUByte *)"":frameCur->bufferCur->fileName) ;
    addLineToEob(bp,buf) ;
    addLineToEob(bp,(meUByte *)"") ;

    getBufferInfo(&numlines,&predlines,&numchars,&predchars) ;
    sprintf((char *)buf,"  Lines     : Total %6ld, Current %6ld",numlines,predlines) ;
    addLineToEob(bp,buf) ;
    sprintf((char *)buf,"  Characters: Total %6ld, Current %6ld",numchars,predchars) ;
    addLineToEob(bp,buf) ;

    addModesLists(bp,buf,frameCur->bufferCur->mode) ;
    addLineToEob(bp,meCopyright) ;
    bp->dotLine = meLineGetNext(bp->baseLine);
    bp->dotOffset = 0 ;
    bp->dotLineNo = 0 ;
    meModeClear(bp->mode,MDEDIT);     /* don't flag this as a change */
    meModeSet(bp->mode,MDVIEW);       /* put this buffer view mode */
    resetBufferWindows(bp) ;
    return meTRUE ;
}

/*
 * Quit command. If an argument, always quit. Otherwise confirm if a buffer
 * has been changed and not written out. Normally bound to "C-X C-C".
 */
int
exitEmacs(int f, int n)
{
    int s=meTRUE ;

    /* Argument forces it.  */
    if(f == meFALSE)
    {
        char buff[128] ;
        
        if(anyChangedBuffer())
            strcpy(buff,"Modified buffer") ;
        else
            buff[0] = '\0' ;
#if MEOPT_SPELL
        if(anyChangedDictionary())
        {
            strcat(buff,(buff[0] == '\0') ? "Modified":",") ;
            strcat(buff," dictionary") ;
        }
#endif
#if MEOPT_REGISTRY
        if(anyChangedRegistry())
        {
            strcat(buff,(buff[0] == '\0') ? "Modified":",") ;
            strcat(buff," registry") ;
        }
#endif
#if MEOPT_IPIPES
        if(anyActiveIpipe())
        {
            strcat(buff,(buff[0] == '\0') ? "A":" and a") ;
            strcat(buff,"ctive process") ;
        }
#endif
        if(buff[0] != '\0')
        {
            /* somethings changed - check the user is happy */
            strcat(buff," exists, leave anyway") ;
            s = mlyesno((meUByte *)buff) ;
        }
    }

    if(s == meTRUE)
    {
        meBuffer *bp, *nbp ;

        /* User says it's OK.   */
#if MEOPT_CLIENTSERVER
        /* the client-server is a psuedo ipipe but it needs
         * to be stopped properly, therefore stop it first as
         * the following ipipeRemove only does half a job and
         * TTkillClientServer will be very confused if called after
         */
        TTkillClientServer() ;
#endif
        /* go round removing all the active ipipes,
         * execute any dhooks set on active buffers
         * and remove all auto-write files
         */
#if MEOPT_IPIPES
        while(ipipes != NULL)
            ipipeRemove(ipipes) ;
#endif
        bp = bheadp ;
        while(bp != NULL)
        {
            nbp = bp->next ;
            autowriteremove(bp) ;
#if MEOPT_FILEHOOK
            if(!meModeTest(bp->mode,MDNACT) && (bp->dhook >= 0))
                execBufferFunc(bp,bp->dhook,0,1) ;     /* Execute the delete hook */
#endif
            bp = nbp ;
        }
#if MEOPT_REGISTRY
        saveHistory(meTRUE,0) ;
#endif
#if MEOPT_CALLBACK
        {
            /* call the shut-down command if its defined */
            int func = decode_fncname((meUByte *)"shut-down",1) ;
            if(func >= 0)
                execFunc(func,meFALSE,1) ;
        }
#endif
#if MEOPT_SOCKET
        ffFileOp(NULL,NULL,meRWFLAG_FTPCLOSE|meRWFLAG_NOCONSOLE) ;
#endif
        TTend();

#ifdef _ME_CONSOLE
#ifdef _TCAP
        if(!(alarmState & meALARM_PIPED)
#ifdef _ME_WINDOW
           && (meSystemCfg & meSYSTEM_CONSOLE)
#endif /* _ME_WINDOW */
           )
            TCAPputc('\n');
#endif /* _TCAP */
#endif /* _ME_CONSOLE */

#ifdef _ME_FREE_ALL_MEMORY
        {
            /* free of everything we can */
            extern void freeToken(meHilight * root) ;
            extern void meFrameFree(meFrame *frame) ;
            extern void printFreeMemory(void) ;
            extern void osdFreeMemory(void) ;
            extern void regFreeMemory(void) ;
            extern void srchFreeMemory(void) ;
            extern void TTfreeTranslateKey(void) ;
            extern meUByte *ffbuf ;
            extern meUByte *defHistFile ;
            extern meUInt *colTable ;
            meMacro      *mac ;
            meVariable   *nuv, *cuv ;
            meKill        *thiskl ;
            meKillNode         *next, *kill ;
            meAbbrev     *abrev ;
            int           ii, jj ;

            while(bheadp->next != NULL)
            {
                bheadp->next->intFlag |= BIFBLOW ;
                zotbuf(bheadp->next,1) ;
            }
            /* ensure the buffer we're left with is a simple *scratch* */
            bheadp->intFlag |= BIFBLOW ;
            zotbuf(bheadp,1) ;
            bclear(bheadp) ;
            meFree(bheadp->baseLine);             /* Release header line. */
            meNullFree(bheadp->fileName) ;
            meNullFree(bheadp->name) ;
            meNullFree(bheadp->modeLineStr) ;
            meFree(bheadp);                             /* Release buffer block */
            meNullFree(mlBinds) ;

            meFrameLoopBegin() ;
            meFrameFree(loopFrame) ;
            meFrameLoopEnd() ;
            
            meFree(hilBlock) ;
            meNullFree(defHistFile) ;
#ifdef _XTERM
            meNullFree(colTable) ;
#endif
            meNullFree(searchPath) ;
            meNullFree(homedir) ;
            meNullFree(curdir) ;

            for(ii=0 ; ii<numStrHist ; ii++)
                free(strHist[ii]) ;
            for(ii=0 ; ii<numBuffHist ; ii++)
                free(buffHist[ii]) ;
            for(ii=0 ; ii<numCommHist ; ii++)
                free(commHist[ii]) ;
            for(ii=0 ; ii<numFileHist ; ii++)
                free(fileHist[ii]) ;
            for(ii=0 ; ii<numSrchHist ; ii++)
                free(srchHist[ii]) ;
            free(strHist) ;

            while(klhead != NULL)
            {
                thiskl = klhead ;
                klhead = klhead->next ;
                kill = thiskl->kill ;
                while(kill != NULL)
                {
                    next = kill->next;
                    free(kill);
                    kill = next;
                }
                free(thiskl) ;
            }
            for(ii=0 ; ii<CK_MAX ; ii++)
            {
                cuv = cmdTable[ii]->varList.head ;
                while(cuv != NULL)
                {
                    nuv = cuv->next ;
                    meNullFree(cuv->value) ;
                    free(cuv) ;
                    cuv = nuv ;
                }
            }
            for( ; ii<cmdTableSize ; ii++)
            {
                mac = getMacro(ii) ;
                meFree(mac->name) ;
                meNullFree(mac->fname) ;
                freeLineLoop(mac->hlp,1) ;
                cuv = mac->varList.head ;
                while(cuv != NULL)
                {
                    nuv = cuv->next ;
                    meNullFree(cuv->value) ;
                    free(cuv) ;
                    cuv = nuv ;
                }
                free(mac) ;
            }
            if(cmdTable != __cmdTable)
                free(cmdTable) ;
            cuv = usrVarList.head ;
            while(cuv != NULL)
            {
                nuv = cuv->next ;
                meNullFree(cuv->value) ;
                free(cuv) ;
                cuv = nuv ;
            }
            meFree(meRegHead) ;
            meFree(ffbuf) ;
            meFree(progName) ;
            if(modeLineStr != orgModeLineStr)
                meNullFree(modeLineStr) ;
            meNullFree(flNextFileTemp) ;
            meNullFree(flNextLineTemp) ;
            if(commentCont != commentContOrg)
                meFree(commentCont) ;

            meNullFree(rcsFile) ;
            meNullFree(rcsCiStr) ;
            meNullFree(rcsCiFStr) ;
            meNullFree(rcsCoStr) ;
            meNullFree(rcsCoUStr) ;
            meNullFree(rcsUeStr) ;

            {
                extern meNamesList buffNames ;
                extern meDirList   fileNames ;
                extern meNamesList commNames ;
                extern meNamesList modeNames ;
                extern meNamesList varbNames ;

                meNullFree(curDirList.path) ;
                freeFileList(curDirList.size,curDirList.list) ;
                meNullFree(fileNames.mask) ;
                meNullFree(fileNames.path) ;
                freeFileList(fileNames.size,fileNames.list) ;
                meNullFree(modeNames.mask) ;
                meNullFree(commNames.list) ;
                meNullFree(commNames.mask) ;
                meNullFree(buffNames.list) ;
                meNullFree(buffNames.mask) ;
                if(varbNames.list != NULL)
                    freeFileList(varbNames.size,varbNames.list) ;
                meNullFree(varbNames.mask) ;
            }

            if(noNextLine > 0)
            {
                for(ii=0 ; ii<noNextLine ; ii++)
                {
                    meFree(nextName[ii]) ;
                    for(jj=0 ; jj<nextLineCnt[ii] ; jj++)
                        meFree(nextLineStr[ii][jj]) ;
                    meNullFree(nextLineStr[ii]) ;
                }
                meFree(nextLineCnt) ;
                meFree(nextLineStr) ;
                meFree(nextName) ;
            }
            addFileHook(meTRUE,0) ;

            deleteDict(1,6) ;
            addSpellRule(1,0) ;
            printFreeMemory() ;
            osdFreeMemory() ;
            regFreeMemory() ;
            srchFreeMemory() ;
            TTfreeTranslateKey() ;
#if MEOPT_HILIGHT
            if(noHilights > 0)
            {
                meUByte hilno ;

                for(hilno=0 ; hilno < noHilights ; hilno++)
                {
                    if(hilights[hilno] != NULL)
                    {
                        hilights[hilno]->close = NULL ;
                        hilights[hilno]->rtoken = NULL ;
                        freeToken(hilights[hilno]) ;
                    }
                }
                meFree(hilights) ;
            }
#endif
            meNullFree (fileIgnore) ;

            while(aheadp != NULL)
            {
                abrev = aheadp ;
                aheadp = abrev->next ;
                freeLineLoop(&(abrev->hlp),0) ;
                meFree(abrev) ;
            }
            meFree(styleTable) ;
        }
#endif
        meExit(0);
    }
    mlerase(MWCLEXEC);

    return s ;
}


/*
 * Better quit command, query saves buffers and then queries the exit
 * if modified buffers still exist
 */
int
saveExitEmacs(int f, int n)
{
    if((saveSomeBuffers(f,(n & 0x01)) == meTRUE)
#if MEOPT_SPELL
       && (saveDict(f,2|(n & 0x01)) != meFALSE)
#endif
#if MEOPT_REGISTRY
       && (saveRegistry(f,2|(n & 0x01)) != meFALSE)
#endif
       )
        return exitEmacs(f, n) ;            /* conditionally quit   */
    return meFALSE ;
}

/*
 * Fancy quit command, as implemented by Norm. If the any buffer has
 * changed do a write on that buffer and exit emacs, otherwise simply exit.
 */
int
quickExit(int f, int n)
{
    return saveExitEmacs(1,0) ;
}

/*
 * Abort.
 * Beep the beeper. Kill off any keyboard macro, etc., that is in progress.
 * Sometimes called as a routine, to do general aborting of stuff.
 */
int
ctrlg(int f, int n)
{
    /* stop recording - execution will stop at this point so there's no
     * point continuing, this provides an easy get out if the user forgets
     * the macro end record key */
    if(kbdmode == meRECORD)
    {
        kbdmode = meSTOP ;
        addModeToWindows(WFMODE) ;  /* update ALL mode lines */
    }
    if(n)
        mlwrite(MWABORT,(meUByte *)"[Aborted]");
    return meABORT ;
}

/*
** function to report a commands not available (i.e. compiled out!) */
int
notAvailable(int f, int n)
{
    return mlwrite(MWABORT,(meUByte *)"[Command not available]");
}

/*
** function to report a no mark set error */
int
noMarkSet(void)
{
    return mlwrite(MWABORT|MWCLEXEC,(meUByte *)"No mark set in this window");
}

/* tell the user that this command is illegal while we are in
   VIEW (read-only) mode                */
int
rdonly(void)
{
    return mlwrite(MWABORT|MWCLEXEC,(meUByte *)"[Command illegal in VIEW mode]");
}

/* void function, does nothing but return false if an argument of zero past,
 * else return true. Used to get rid of unwanted unbounded-key bells
 */
int
voidFunc(int f, int n)
{
    return ((n) ? meTRUE:meFALSE) ;
}

int
prefixFunc(int f, int n) /* dummy prefix function */
{
    return meTRUE ;
}
int
uniArgument(int f, int n)         /* dummy function for binding to universal-argument */
{
    return meTRUE ;
}

#ifdef _UNIX
/* sigchild; Handle a SIGCHILD action. Note for the older types of signal it
 * is not necessary to reset the signal handler because they "reappear" as
 * soon as the signal handler is reset - hence we do it at the bottom */
static void
sigchild(SIGNAL_PROTOTYPE)
{
    meWAIT_STATUS status ;              /* Status return from waitpid() */

#if MEOPT_IPIPES
    if(noIpipes != 0)
    {
        meIPipe *ipipe ;

        ipipe = ipipes ;
        while(ipipe != NULL)
        {
            if((ipipe->pid > 0) &&
               (meWaitpid(ipipe->pid,&status,WNOHANG) == ipipe->pid))
            {
                if(WIFEXITED(status))
                    ipipe->pid = -4 ;
                else if(WIFSIGNALED(status))
                    ipipe->pid = -3 ;
#ifdef WCOREDUMP
                else if(WCOREDUMP(status))
                    ipipe->pid = -2 ;
#endif
#if 0
                else
                {
                    if(WIFSTOPPED(status))
                        mlwrite(0,"Process %s has STOPPED",ipipe->buf->name) ;
#ifdef _IRIX
                    else if(WIFCONTINUED(status))
                        mlwrite(0,"Process %s has CONTINUED",ipipe->buf->name) ;
#endif
                    else
                        mlwrite(0,"Process %s sent an UNKNOWN SIGNAL",ipipe->buf->name) ;
                }
#endif
            }
            ipipe = ipipe->next ;
        }
    }
#endif
    /* clear up any zoombie children */
    meWaitpid(-1,&status,WNOHANG) ;
    
    /* Reload the signal handler. Note that the child is a special case where
     * the signal is reset at the end of the handler as opposed to the head of
     * the handler. For the POSIX or BSD signals this is not required. */
#if !((defined _POSIX_SIGNALS) || (defined _BSD_SIGNALS))
    signal (SIGCHLD, sigchild);
#endif
}
#endif

#if (defined _UNIX) || (defined _WIN32)
int
meDie(void)
{
    register meBuffer *bp;    /* scanning pointer to buffers */

#ifdef _ME_WINDOW
#ifdef _XTERM
    int      usedX = 0 ;
#endif /* _XTERM */
#endif /* _ME_WINDOW */
    
    /*
     * To get here we have received a signal and am about to die!
     * Warn the user.
     */
    /* we know about the signal and we're dealing with it */
    alarmState = 0 ;

#ifdef _ME_WINDOW
#ifdef _XTERM
    /* As the X window will always go, always output to the console
     * or terminal, so if using X, do some grotty stuff.
     * This is really important if the X-term window is destroyed,
     * see waitForEvent in unixterm.c. This is because the windows
     * already gone, so we can't use it!
     */
#ifdef _ME_CONSOLE
    if(!(meSystemCfg & meSYSTEM_CONSOLE))
#endif /* _ME_CONSOLE */
    {
        usedX = 1 ;
        meSystemCfg |= meSYSTEM_CONSOLE ;
    }
#endif /* _XTERM */
#endif /* _ME_WINDOW */
    
    /* Make a noisy BELL */
    meModeClear(globMode,MDQUIET) ;
    mlwrite(MWCURSOR|MWABORT,(meUByte *)"*** Emergency quit ***");
#ifdef _ME_CONSOLE
#ifdef _TCAP
    TCAPputc('\n');
#endif /* _TCAP */
#endif /* _ME_CONSOLE */

    bp = bheadp;
    while (bp != NULL)
    {
        if(bufferNeedSaving(bp))
        {
            autowriteout(bp) ;
#ifdef _ME_CONSOLE
#ifdef _TCAP
            TCAPputc('\n');
#endif /* _TCAP */
#endif /* _ME_CONSOLE */
        }
        bp = bp->next;            /* on to the next buffer */
    }
#if MEOPT_REGISTRY
    saveHistory(1,0) ;
    saveRegistry(1,2) ;
#endif
#if MEOPT_IPIPES
    /* try to kill any child processes */
    while(ipipes != NULL)
        ipipeRemove(ipipes) ;
#endif

#ifdef _ME_CONSOLE
#ifdef _TCAP
    TCAPputc('\n');
#endif /* _TCAP */
#endif /* _ME_CONSOLE */

#ifdef _ME_WINDOW
#ifdef _XTERM
    /* Don't bother with the TTend if this was an xterm, to dangerous */
    if(!usedX)
#endif /* _XTERM */
#endif /* _ME_WINDOW */
        TTend();

    meExit(1) ;
    return 0;
}
#endif

void
autoSaveHandler(void)
{
    struct meTimeval tp ;
    register meBuffer *bp ;
    register meInt tim, next=0x7fffffff ;

    gettimeofday(&tp,NULL) ;
    tim = ((tp.tv_sec-startTime)*1000) + (tp.tv_usec/1000) ;
    bp  = bheadp;
    while (bp != NULL)
    {
        if(bp->autotime >= 0)
        {
            if(bp->autotime <= tim)
                autowriteout(bp) ;
            else if(bp->autotime < next)
                next = bp->autotime ;
        }
        bp = bp->next ;
    }
    if(next != 0x7fffffff)
        timerSet(AUTOS_TIMER_ID,next,next-tim) ;
    else
        timerClearExpired(AUTOS_TIMER_ID) ;
}


#ifdef _UNIX
static void
sigeat(SIGNAL_PROTOTYPE)
{
    /* Reload the signal handler. For the POSIX or BSD signals this is not
     * required. */
#if !((defined _POSIX_SIGNALS) || (defined _BSD_SIGNALS))
    signal (sig, sigeat);
#endif
    /*
     * Trap nasty signals under UNIX. Write files and quit.
     * Can't just do that cos god knows what me is doing (broken links etc).
     * So can only flag request here and hope its seen.
     */
    alarmState |= meALARM_DIE ;
}
#endif    /* _UNIX */

int commandDepth=0 ;
void
doOneKey(void)
{
    register int    c;
    register int    f;
    register int    n;
    register int    mflag;
    int     basec;              /* c stripped of meta character   */

    update(meFALSE);                          /* Fix up the screen    */

    /*
     * If we are not playing or recording a macro. This is the ONLY place
     * where we may get an idle key back from the input stream. Set the kdb
     * mode to IDLE to indicate that we will accept an idle key.
     *
     * kdbmode is probably not the best variable to use. However it is global
     * and it is not currntly being used - hence it is available for abuse !!
     */
    if (kbdmode == meSTOP)
        kbdmode = meIDLE;             /* In an idle state  */
    
    c = meGetKeyFromUser(meFALSE, 1, meGETKEY_COMMAND);     /* Get a key */

    if (frameCur->mlStatus & MLSTATUS_CLEAR)
        mlerase(MWCLEXEC) ;

    f = meFALSE;
    n = 1;

    /* do ME_PREFIX1-# processing if needed */
    basec = c & ~ME_PREFIX1 ;        /* strip meta char off if there */
    if((c & ME_PREFIX1) && ((basec >= '0' && basec <= '9') || basec == '-'))
    {
        f = meTRUE;
        if(basec == '-')
        {
            mflag = -1 ;
            n = 0;
        }
        else
        {
            mflag = 1 ;
            n = basec - '0' ;
        }
        while(((c=meGetKeyFromUser(meTRUE,(n * mflag),meGETKEY_COMMAND)) >= '0') && (c <= '9'))
            n = n * 10 + (c - '0');
        n *= mflag;    /* figure in the sign */
    }

    /* do ^U repeat argument processing */
    if(c == reptc)
    {                           /* ^U, start argument   */
        f = meTRUE;               /* In case not set */
        mflag = 1;              /* current minus flag */
        for(;;c = meGetKeyFromUser(f,n,meGETKEY_COMMAND))
        {
            switch(c)
            {
            case 21 :                   /* \U (^U) */
                n *= 4;
                continue;    /* Loop for more */
            case '-':
                /* If '-', and argument is +ve, make arg -ve */
                mflag = -1;
                if(n < 0)
                    break;
                n *= -1;
                continue;    /* Loop for more */
            }
            /* Not ^U or first '-' .. drop out */
            break;
        }
        /* handle "^U n" to give "n" (GNU Emacs does this) */
        if ((c >= '0' && c <= '9'))
        {
            for(n=c-'0' ;;)
            {
                /*
                 * get the next key, if a digit, update the
                 * count note that we do not handle "-" here
                 */
                c = meGetKeyFromUser(meTRUE,(mflag*n),meGETKEY_COMMAND);
                if(c >= '0' && c <= '9')
                    n = n * 10 + (c - '0');
                else
                    break;
            }
            n *= mflag;    /* figure in the sign */
        }
    }

    if(f == meTRUE)        /* Zap count from cmd line ? */
        mlerase(MWCLEXEC);
    commandDepth++ ;
    execute(c, f, n) ;   /* Do it. */
    commandDepth-- ;
}


void
mesetup(int argc, char *argv[])
{
    extern meUByte *ffbuf ;
    meBuffer *bp, *mainbp ;
    int     carg,rarg;          /* current arg to scan            */
    int     noFiles=0 ;
    meUByte  *file=NULL ;
#ifdef _UNIX
    int     sigcatch=1 ;        /* Dont catch signals             */
#endif
#ifdef _DOS
    int     dumpScreen=0 ;
#endif
#if MEOPT_CLIENTSERVER
    meUByte  *clientMessage=NULL ;
    int     userClientServer=0 ;
#endif
    startTime = time(NULL) ;

#ifdef _UNIX
    /* Get the usr id and group id for mode-line and file permissions */
    meXUmask = umask(0);
    umask(meXUmask);
    meXUmask = (meXUmask & (S_IROTH|S_IWOTH|S_IXOTH|S_IRGRP|S_IWGRP|S_IXGRP|S_IRUSR|S_IWUSR|S_IXUSR)) ^ 
              (S_IROTH|S_IWOTH|S_IXOTH|S_IRGRP|S_IWGRP|S_IXGRP|S_IRUSR|S_IWUSR|S_IXUSR) ; /* 00666 */
    meUmask = meXUmask & (S_IROTH|S_IWOTH|S_IRGRP|S_IWGRP|S_IRUSR|S_IWUSR) ;
    meUid = getuid();
    meGid = getgid();
    /* get a list of groups the user belongs to */
    meGidSize = getgroups(0,NULL) ;
    if((meGidSize > 1) &&
       ((meGidList = malloc(meGidSize*sizeof(gid_t))) != NULL))
        meGidSize = getgroups(meGidSize,meGidList) ;
    else
        meGidSize = 0 ;

    /* Set the required alarms first so we always have them */
    /* setup alarm process for timers */
#ifdef _POSIX_SIGNALS
    {
        struct sigaction sa ;

        sigemptyset(&meSigHoldMask) ;
        sigaddset(&meSigHoldMask,SIGCHLD);
        sigaddset(&meSigHoldMask,SIGALRM);
        meSigRelease() ;

        sigemptyset(&sa.sa_mask) ;
        sa.sa_flags=0;
        sa.sa_handler=sigAlarm ;
        sigaction(SIGALRM,&sa,NULL) ;
        /* sigset(SIGALRM,sigAlarm);*/
        /* setup child process exit capture for ipipe */
        sa.sa_handler=sigchild ;
        sigaction(SIGCHLD,&sa,NULL) ;
        /* sigset(SIGCHLD,sigchild);*/
    }
#else /* _POSIX_SIGNALS */
#ifdef _BSD_SIGNALS
    /* Set up the hold mask */
    meSigHoldMask = sigmask (SIGCHLD) | sigmask (SIGALRM);
#endif /* _BSD_SIGNALS */
    signal (SIGCHLD, sigchild);
    signal (SIGALRM, sigAlarm);
#endif /* _POSIX_SIGNALS */
#endif /* _UNIX */
    count_key_table() ;

    if(((meRegHead = meMalloc(sizeof(meRegister))) == NULL) ||
       ((ffbuf = meMalloc(meFIOBUFSIZ+1)) == NULL))
        exit(1) ;
    /* Make the head registers point back to themselves so that
     * accessing #p? gets #g? and not a core-dump
     */
    meRegHead->prev = meRegHead ;
    meRegHead->commandName = NULL ;
    meRegHead->execstr = NULL ;
    meRegHead->varList = NULL ;
    meRegHead->force = 0 ;
    meRegCurr = meRegHead ;
    /* Initialise the head as this is dumped in list-variables */
    for(carg = 0 ; carg<meREGISTER_MAX ; carg++)
        meRegHead->reg[carg][0] = '\0' ;

    /* initialize the editor and process the command line arguments */
    initHistory() ;           /* allocate history space */
    set_dirs();               /* setup directories */

    {
        /* setup the $progname make it an absolute path. */
        if(executableLookup((meUByte *)argv[0],evalResult))
            progName = meStrdup(evalResult) ;
        else
#ifdef _ME_FREE_ALL_MEMORY
            /* stops problems on exit */
            progName = meStrdup(argv[0]) ;
#else
            progName = (meUByte *)argv[0] ;
#endif
    }
    /* scan through the command line and get all global options */
    carg = rarg = 1 ;
    for( ; carg < argc; ++carg)
    {
        /* if its a switch, process it */
        if (argv[carg][0] == '-')
        {
            switch (argv[carg][1])
            {
            case 'b':    /* -b bin flag */
            case 'k':    /* -k crypt flag */
            case 'y':    /* -y rbin flag */
                /* don't want these options yet as they apply to the
                 * next file on the command line
                 */
                argv[rarg++] = argv[carg] ;
                break ;

            case 'c':
                HistNoFilesLoaded = -1 ;
                break ;

#if MEOPT_DEBUGM
            case 'd':    /* -d debug */
                macbug = meTRUE;
                break;
#endif

            case 'h':
                mePrintHelpMessage(meHelpPage) ;
                meExit(0) ;
#ifdef _DOS
            case 'i':
                dumpScreen = 1 ;
                break ;
#endif
            case 'l':    /* -l for initial goto line */
                /* don't want these options yet as they apply to the
                 * next file on the command line
                 */
                argv[rarg++] = argv[carg] ;
                if (argv[carg][2] == 0)
                {
                    if (carg == argc-1)
                    {
missing_arg:
                        sprintf((char *)evalResult,"%s Error: Argument expected with option %s\nOption -h gives further help\n",
                                argv[0],argv[carg]);
                        mePrintHelpMessage(evalResult) ;
                        meExit(1);
                    }
                    argv[rarg++] = argv[++carg] ;
                }
                break ;

#if MEOPT_CLIENTSERVER
            case 'm':    /* -m send a client server message */
                userClientServer = 1 ;
                if(argv[carg][2] == '\0')
                {
                    if (carg == argc-1)
                        goto missing_arg ;
                    clientMessage = (meUByte *)argv[++carg] ;
                }
                else
                    clientMessage = (meUByte *)argv[carg]+2 ;
                break;
#endif
#ifdef _ME_WINDOW
#ifdef _ME_CONSOLE
            case 'n':
                meSystemCfg |= meSYSTEM_CONSOLE ;
                break ;
#endif /* _ME_CONSOLE */
#endif /* _ME_WINDOW */
#if MEOPT_CLIENTSERVER
            case 'o':
                userClientServer=1 ;
                break ;
#endif
#if MEOPT_EXTENDED
            case 'p':
                alarmState |= meALARM_PIPED ;
                break ;
#endif

            case 'r':
                meModeSet(globMode,MDVIEW) ;
                break ;

            case 's':    /* -s for initial search string */
                argv[rarg++] = argv[carg] ;
                if(argv[carg][2] == '\0')
                {
                    if (carg == argc-1)
                        goto missing_arg ;
                    argv[rarg++] = argv[++carg] ;
                }
                break;

            case 'u':    /* -u for setting the user name */
                {
                    char *un ;
                    if (argv[carg][2] == 0)
                    {
                        if (carg == argc-1)
                            goto missing_arg ;
                        un = argv[++carg] ;
                    }
                    else
                        un = argv[carg] + 2 ;
                    meStrcpy(evalResult,"MENAME=") ;
                    meStrcpy(evalResult+7,un) ;
                    mePutenv(meStrdup(evalResult)) ;
                    break;
                }

            case 'v':
                {
                    char *ss, *tt, cc ;
                    if (argv[carg][2] == 0)
                    {
                        if (carg == argc-1)
                            goto missing_arg ;
                        ss = argv[++carg] ;
                    }
                    else
                        ss = argv[carg] + 2 ;
                    if((tt = strchr(ss,'=')) == NULL)
                    {
                        sprintf((char *)evalResult,"%s Error: Mal-formed -v option\n",argv[0]) ;
                        mePrintHelpMessage(evalResult) ;
                        meExit(1) ;
                    }
                    
                    if(((cc=getMacroTypeS(ss)) != TKREG) && (cc != TKVAR) &&
                       (cc != TKCVR) && (cc != TKENV))
                    {
                        *tt = '\0' ;
                        sprintf((char *)evalResult,"%s Error: Cannot set variable [%s] from the command-line\n",argv[0],ss) ;
                        mePrintHelpMessage(evalResult) ;
                        meExit(1) ;
                    }
                    if((tt = strchr(ss,'=')) != NULL)
                    {
                        *tt++ = '\0' ;
                        if(setVar((meUByte *)ss,(meUByte *)tt,meRegCurr) != meTRUE)  /* set a variable */
                        {
                            sprintf((char *)evalResult,"%s Error: Unable to set variable [%s]\n",argv[0],ss) ;
                            mePrintHelpMessage(evalResult) ;
                            meExit(1) ;
                        }
                        execstr = NULL ;
                        clexec = meFALSE ;
                    }
                    break ;
                }
#ifdef _UNIX
            case 'x':
                sigcatch = 0 ;
                break;
#endif

            default:
                {
                    sprintf((char *)evalResult,"%s Error: Unknown option %s\nOption -h gives further help\n",argv[0],argv[carg]) ;
                    mePrintHelpMessage(evalResult) ;
                    meExit(1) ;
                }
            }
        }
        else if(argv[carg][0] == '@')
            file = (meUByte *) argv[carg] + 1 ;
        else if((argv[carg][0] == '+') && (argv[carg][1] == '\0'))
            goto missing_arg ;
        else
            argv[rarg++] = argv[carg] ;
    }
#if MEOPT_CLIENTSERVER
    if(userClientServer && TTconnectClientServer())
    {
        int     binflag=0 ;         /* load next file as a binary file*/
        int     gline = 0 ;         /* what line? (-g option)         */

        for(carg=1 ; carg < rarg ; carg++)
        {
            /* if its a switch, process it */
            if(argv[carg][0] == '-')
            {
                switch(argv[carg][1])
                {
                case 'l':    /* -l for initial goto line */
                    if (argv[carg][2] == 0)
                        gline = meAtoi(argv[++carg]);
                    else
                        gline = meAtoi((argv[carg])+2);
                    break;
                case 'b':    /* -b bin flag */
                    binflag |= BFND_BINARY ;
                    break;
                case 'k':    /* -k crypt flag */
                    binflag |= BFND_CRYPT ;
                    break;
                case 's':
                    /* -s not supported across client-server */
                    if (argv[carg][2] == 0)
                        carg++ ;
                    break ;
                case 'y':    /* -y rbin flag */
                    binflag |= BFND_RBIN ;
                    break;
                }
            }
            else if (argv[carg][0] == '+')
                gline = meAtoi((argv[carg])+1);
            else
            {
                /* set up a buffer for this file */
                noFiles += findFileList((meUByte *)argv[carg],(BFND_CREAT|BFND_MKNAM|binflag),gline) ;
                gline = 0 ;
                binflag = 0 ;
            }
        }
        if(clientMessage != NULL)
            TTsendClientServer(clientMessage) ;
        bp = bheadp ;
        while(bp != NULL)
        {
            if(bp->fileName != NULL)
            {
                meUByte buff[meBUF_SIZE_MAX+32] ;
                int nn=1 ;
                if(meModeTest(bp->mode,MDBINRY))
                    nn |= BFND_BINARY ;
                if(meModeTest(bp->mode,MDCRYPT))
                    nn |= BFND_CRYPT ;
                if(meModeTest(bp->mode,MDRBIN))
                    nn |= BFND_RBIN ;
                sprintf((char *)buff,"C:ME:%d find-file \"%s\"\n",nn,bp->fileName) ;
                TTsendClientServer(buff) ;
                if(bp->dotLineNo != 0)
                {
                    sprintf((char *)buff,"C:ME:goto-line %d\n",(int) bp->dotLineNo) ;
                    TTsendClientServer(buff) ;
                }
            }
            bp = bp->next ;
        }
        /* send a 'make-current command to server */
        TTsendClientServer("C:ME:2 popup-window\n") ;
        meExit(0) ;
    }
    else if(userClientServer && clientMessage)
    {
        sprintf((char *)evalResult,"%s Error: Unable to connect to server\n",argv[0]) ;
        mePrintHelpMessage(evalResult) ;
        meExit(1) ;
    }
#endif

    meInit(BmainN);           /* Buffers, windows.    */

#ifdef _DOS
    if(dumpScreen)
    {
        extern void TTdump(meBuffer *) ;
        TTdump(frameCur->bufferCur) ;
        gotobob(meFALSE,1) ;
        carg++ ;
    }
#endif
#if MEOPT_EXTENDED
    if(alarmState & meALARM_PIPED)
    {
#ifdef _WIN32
        ffrp = GetStdHandle(STD_INPUT_HANDLE) ;
#else
        ffrp = stdin ;
#endif
        bp = bfind(BstdinN,BFND_CREAT);
        readin(bp,NULL) ;
        
#ifdef _ME_WINDOW
#ifdef _ME_CONSOLE
        if((meSystemCfg & meSYSTEM_CONSOLE) == 0)
#endif /* _ME_CONSOLE */
            /* when not in a console mode allow ME to run as normal */
            alarmState &= ~meALARM_PIPED ;
#endif /* _ME_WINDOW */
    }
#endif

    mlerase(0);                /* Delete command line */
    /* run me.emf unless an @... arg was given in which case run that */
    startup(file) ;

    /* initalize *scratch* colors and modes to global defaults */
    if((mainbp=bfind(BmainN,0)) != NULL)
    {
        meModeCopy(mainbp->mode,globMode) ;
#if MEOPT_COLOR
        mainbp->scheme = globScheme;
#endif
        /* make *scratch*'s history number very low so any other
         * buffer is preferable */
        mainbp->histNo = -1 ;
    }
#if MEOPT_COLOR
#if MEOPT_CLIENTSERVER
    /* also initialize the client server color scheme */
    if((ipipes != NULL) && (ipipes->pid == 0))
        ipipes->bp->scheme = globScheme;
#endif
#endif
    
    {
        meUByte  *searchStr=NULL, *cryptStr=NULL ;
        int     binflag=0 ;         /* load next file as a binary file*/
        int     gline = 0 ;         /* what line? (-g option)         */
        int     obufHistNo ;
        
        obufHistNo = bufHistNo ;

        /* scan through the command line and get the files to edit */
        for(carg=1 ; carg < rarg ; carg++)
        {
            /* if its a switch, process it */
            if(argv[carg][0] == '-')
            {
                switch(argv[carg][1])
                {
                case 'l':    /* -l for initial goto line */
                    if (argv[carg][2] == 0)
                        gline = meAtoi(argv[++carg]);
                    else
                        gline = meAtoi((argv[carg])+2);
                    break;
                case 'b':    /* -b bin flag */
                    binflag |= BFND_BINARY ;
                    break;
                case 'k':    /* -k crypt flag */
                    binflag |= BFND_CRYPT ;
                    if (argv[carg][2] != 0)
                        cryptStr = (meUByte *) argv[carg] + 2 ;
                    break;
                case 's':
                    /* -s not supported across client-server */
                    if (argv[carg][2] == 0)
                        searchStr = (meUByte *) argv[++carg] ;
                    else
                        searchStr = (meUByte *) argv[carg]+2 ;
                    break ;
                case 'y':    /* -y rbin flag */
                    binflag |= BFND_RBIN ;
                    break;
                }
            }
            else if (argv[carg][0] == '+')
                gline = meAtoi((argv[carg])+1);
            else
            {
                /* set up a buffer for this file - force the history so the first file
                 * on the command-line has the highest bufHistNo so is shown first */
                bufHistNo = obufHistNo + rarg - carg ;
                noFiles += findFileList((meUByte *)argv[carg],(BFND_CREAT|BFND_MKNAM|binflag),gline) ;
                if((cryptStr != NULL) || (searchStr != NULL))
                {
                    /* Deal with -k<key> and -s <search> */
                    bp = bheadp ;
                    while(bp != NULL)
                    {
                        if(bp->histNo == bufHistNo)
                        {
#if MEOPT_CRYPT
                            if(cryptStr != NULL)
                                setBufferCryptKey(bp,cryptStr) ;
#endif
                            if(searchStr != NULL)
                            {
                                meBuffer *cbp ;
                                int histNo, flags ;
                                cbp = frameCur->bufferCur ;
                                histNo = cbp->histNo ;
                                swbuffer(frameCur->windowCur,bp) ;
                                flags = ISCANNER_QUIET ;
                                if(meModeTest(bp->mode,MDMAGIC))
                                    flags |= ISCANNER_MAGIC ;
                                if(meModeTest(bp->mode,MDEXACT))
                                    flags |= ISCANNER_EXACT ;
                                /* what can we do if we fail to find it? */
                                iscanner(searchStr,0,flags,NULL) ;
                                swbuffer(frameCur->windowCur,cbp) ;
                                cbp->histNo = histNo ;
                                bp->histNo = bufHistNo ;
                            }
                        }
                        bp = bp->next ;
                    }
                    cryptStr = NULL ;
                    searchStr = NULL ;
                }
                gline = 0 ;
                binflag = 0 ;
            }
        }
        bufHistNo = obufHistNo + rarg ;
    }

    /* load-up the com-line or -c first and second files */
    if(!noFiles)
        noFiles = HistNoFilesLoaded ;
    else
        HistNoFilesLoaded = 0 ;
    
    if(noFiles > 0)
    {
        if((frameCur->bufferCur == mainbp) && ((bp = replacebuffer(NULL)) != mainbp) &&
           (bp->fileName != NULL))
        {
            if(HistNoFilesLoaded && isUrlLink(bp->fileName))
            {
                meUByte prompt[meFILEBUF_SIZE_MAX+16] ;
                meStrcpy(prompt,"Reload file ") ;
                meStrcat(prompt,bp->fileName) ;
                if(mlyesno(prompt) != meTRUE)
                {
                    bp = NULL ;
                    noFiles = 0 ;
                }
            }
	}
        else
            bp = NULL ;
        if(bp != NULL)
        {
            swbuffer(frameCur->windowCur,bp) ;
	    mainbp->histNo = -1 ;
        }
        if((noFiles > 1) && ((bp = replacebuffer(NULL)) != mainbp) &&
           (bp->fileName != NULL))
        {
            if(!HistNoFilesLoaded && !isUrlLink(bp->fileName))
            {
                splitWindVert(meTRUE,2) ;
                swbuffer(frameCur->windowCur,replacebuffer(NULL)) ;
                prevwind(meFALSE,1) ;
            }
        }
    }
#ifdef _UNIX
    /*
     * Catch nasty signals when running under UNIX unless the -x
     * option appeared on the command line.
     *
     * These can't be trapped as caused by ME failure and will cause loop
     *
     * signal(SIGILL, sigeat);
     * signal(SIGTRAP, sigeat);
     * signal(SIGBUS, sigeat);
     * signal(SIGSEGV, sigeat);
     * signal(SIGSYS, sigeat);
     * signal(SIGIOT, sigeat);
     * signal(SIGEMT, sigeat);
     */
    if(sigcatch)
    {
#ifdef _POSIX_SIGNALS
        struct sigaction sa ;

        sigemptyset(&sa.sa_mask) ;
        sa.sa_flags=0;
        sa.sa_handler=sigeat ;
        sigaction(SIGHUP,&sa,NULL) ;
        sigaction(SIGINT,&sa,NULL) ;
        sigaction(SIGQUIT,&sa,NULL) ;
        sigaction(SIGTERM,&sa,NULL) ;
        sigaction(SIGUSR1,&sa,NULL) ;
#else /* _POSIX_SIGNALS */
        signal(SIGHUP,sigeat) ;
        signal(SIGINT,sigeat) ;
        signal(SIGQUIT,sigeat) ;
        signal(SIGTERM,sigeat) ;
        signal(SIGUSR1,sigeat) ;
#endif /* _POSIX_SIGNALS */
    }
#endif /* _UNIX */

    /* setup to process commands */
    lastflag = 0;                       /* Fake last flags.     */

    /* Set the screen for a redraw at this point cos its after the
     * startup which can screw things up
     */
    sgarbf = meTRUE;			 /* Erase-page clears */

#if MEOPT_CALLBACK
    carg = decode_fncname((meUByte *)"start-up",1) ;
    if(carg >= 0)
        execFunc(carg,meFALSE,1) ;
#endif
}

#ifndef NDEBUG

/* _meAssert
 * The receiving end of the macro meAssert(<boolean>). This keeps microEMACS
 * up allowing the debugger to be attached. This is essential under Windows
 * since Microsofts assert() is the pits and invariably just exits the
 * program without invoking the debugger.
 */

void
_meAssert (char *file, int line)
{
    meUByte buf[meBUF_SIZE_MAX];                  /* String buffer */
    meUByte cc;                           /* Character input */

    /* Put out the string */
    sprintf ((char *) buf,
             "!! ASSERT FAILED %s:%d !! - <S>ave. <Q>uit. <C>ontinue",
             file, line);
    mlwrite (MWABORT, buf);

    TTinflush ();                       /* Flush the input buffer */
    for (;;)
    {
        cc = (meUByte) TTgetc() ;         /* Get character from keyboard */
        /* Try and perform an emergency save for the user - no guarantees
         * here I am afraid - we may be totally screwed at this point in
         * time. */
        if (cc == 'S')
        {
            register meBuffer *bp;        /* scanning pointer to buffers */

            bp = bheadp;
            while (bp != NULL)
            {
                if(bufferNeedSaving(bp))
                    autowriteout(bp) ;
                bp = bp->next;        /* on to the next buffer */
            }
            saveHistory(meTRUE,0) ;
        }
        else if (cc == 'Q')
            meExit (1);                 /* Die the death */
        else if (cc == 'C')
            break;                      /* Let the sucker go !! */
    }
}
#endif

#if MEOPT_EXTENDED
int
commandWait(int f, int n)
{
    meVarList *varList=NULL ;
    meUByte clexecSv ;
    int execlevelSv ;
    meUByte *ss ;
    
    if(f)
    {
        f = clexec ;
        clexec = meFALSE ;
        if(n < 0)
            TTsleep(0-n,1) ;
        else
            TTsleep(n,0) ;
        clexec = f ;
        return meTRUE ;
    }
    if((meRegCurr->commandName != NULL) &&
       ((f=decode_fncname(meRegCurr->commandName,1)) >= 0))
        varList = &(cmdTable[f]->varList) ;
    
    clexecSv = clexec;
    execlevelSv = execlevel ;
    clexec = meFALSE ;
    execlevel = 0 ;
    do
    {
        doOneKey() ;
        if(TTbreakFlag)
        {
            TTinflush() ;
            if((selhilight.flags & (SELHIL_ACTIVE|SELHIL_KEEP)) == SELHIL_ACTIVE)
                selhilight.flags &= ~SELHIL_ACTIVE ;
            TTbreakFlag = 0 ;
        }
    } while((varList != NULL) && 
            ((ss=getUsrLclCmdVar((meUByte *)"wait",varList)) != errorm) && meAtoi(ss)) ;
    clexec = clexecSv ;
    execlevel = execlevelSv ;
    return meTRUE ;
}
#endif

#ifndef _WIN32
int
main(int argc, char *argv[])
{
    mesetup(argc,argv) ;
    while(1)
    {
        doOneKey() ;
        if(TTbreakFlag)
        {
            TTinflush() ;
            if((selhilight.flags & (SELHIL_ACTIVE|SELHIL_KEEP)) == SELHIL_ACTIVE)
                selhilight.flags &= ~SELHIL_ACTIVE ;
            TTbreakFlag = 0 ;
        }
    }
    return 0 ;
}
#endif
