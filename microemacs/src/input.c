/* -*- c -*-
 *
 * JASSPA MicroEmacs - www.jasspa.com
 * input.c - Various input routines.
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
 * Created:     5/9/86
 * Synopsis:    Various input routines.
 * Authors:     Daniel Lawrence, Jon Green & Steven Phillips
 */

#define	__INPUTC			/* Define the name of the file */

#include "emain.h"
#include "efunc.h"
#include "evar.h"
#include "eskeys.h"

/*
 * Ask a yes or no question in the message line. Return either meTRUE, meFALSE, or
 * meABORT. The meABORT status is returned if the user bumps out of the question
 * with a ^G. Used any time a confirmation is required.
 */
int
mlCharReply(meUByte *prompt, int mask, meUByte *validList, meUByte *helpStr)
{
    int   inpType=0, cc ;
    meUByte buff[meTOKENBUF_SIZE_MAX] ;
    meUByte *pp=prompt ;
    
    for(;;)
    {
        if((clexec == meFALSE) || inpType)
        {
            if((kbdmode != mePLAY) || (kbdoff >= kbdlen))
            {
                /* We are going to get a key of the user!! */
                if(mask & mlCR_QUIT_ON_USER)
                    return -2 ;
                if(mask & mlCR_UPDATE_ON_USER)
                    update(meTRUE) ;
                
                if(frameCur->mlStatus & MLSTATUS_POSOSD)
                {
                    frameCur->mlStatus = MLSTATUS_POSOSD ;
                    mlResetCursor() ;
                }
                else
                {
                    /* switch off the status cause we are replacing it */
                    frameCur->mlStatus = 0 ;
                    mlwrite(((mask & mlCR_CURSOR_IN_MAIN) ? 0:MWCURSOR)|MWSPEC,pp) ;
                    pp = prompt ;
                    /* switch on the status so we save it */
                    frameCur->mlStatus = (mask & mlCR_CURSOR_IN_MAIN) ? MLSTATUS_KEEP:(MLSTATUS_KEEP|MLSTATUS_POSML) ;
                    inpType = 2 ;
                }
            }
            cc = meGetKeyFromUser(meFALSE,0,meGETKEY_SILENT|meGETKEY_SINGLE) ;
            frameCur->mlStatus &= ~(MLSTATUS_KEEP|MLSTATUS_RESTORE|MLSTATUS_POSML) ;
            if((cc == breakc) && !(mask & mlCR_QUOTE_CHAR))
                return -1 ;
        }
        else
        {
            meUByte *ss = execstr ;
            execstr = token(execstr,buff) ;
            if((buff[0] == '@') && (buff[1] == 'm') && (buff[2] == 'x'))
            {
                cc = buff[3] ;
                execstr = token(execstr, buff);
                if(cc == 'a')
                    execstr = ss ;
                meStrcpy(resultStr,prompt) ;
                if(lineExec (0, 1, buff) != meTRUE)
                    return -1 ;
                cc = resultStr[0] ;
            }
            else if((buff[0] == '@') && (buff[1] == 'm') && (buff[2] == 'n'))
            {
                /* if @mna (get all input from user) then rewind the execstr */
                if(buff[3] == 'a')
                    execstr = ss ;
                inpType = 1 ;
                continue ;
            }
            else
            {
                /* evaluate it */
                meUByte *res = getval(buff) ;
                if(res == abortm)
                    return -1 ;
                cc = *res ;
            }
        }
        
        if((mask & mlCR_QUOTE_CHAR) &&
           ((cc = quoteKeyToChar((meUShort) cc)) < 0))
        {
            if(validList == NULL)
                return -1 ;
        }
        else
        {
            if((mask & mlCR_LOWER_CASE) && ((cc & 0xff00) == 0))
                cc = toLower(cc) ;
            
            if((cc == '?') && (helpStr != NULL))
                pp = helpStr ;
            else if((cc != '\0') &&
                    (!(mask & mlCR_ALPHANUM_CHAR) || isAlphaNum(cc)) &&
                    ((validList == NULL) || (((cc & 0xff00) == 0) && (meStrchr(validList,cc) != NULL))))
            {
                if (inpType == 2)
                {
                    meGetStringFromKey((meUShort) cc,buff) ;
                    mlwrite(MWCURSOR,(meUByte *)"%s%s",prompt,buff) ;
                }
                return cc ;
            }
        }
        inpType = 1 ;
    }
}

int
mlyesno(meUByte *prompt)
{
    meUByte buf[meBUF_SIZE_MAX] ;    /* prompt to user */
    int ret ;
    
    /* build and prompt the user */
    meStrcpy(buf,prompt) ;
    meStrcat(buf," [y/n]? ") ;

    ret = mlCharReply(buf,mlCR_LOWER_CASE,(meUByte *)"yn",NULL) ;
    
    if(ret == -1)
        return ctrlg(meFALSE,1) ;
    else if(ret == 'n')
        return meFALSE ;
    return meTRUE ;
}

/*
 * These are very simple character inserting, character deleting, move
 * over or erase spaces forwards or backwards routines. They are used
 * by the meGetString routine to edit the command line.
 */

static int
cins(meUByte cc, meUByte *buf, int *pos, int *len, int max)
{
    /*
     * Insert a character "c" into the buffer pointed to by "buf" at
     * position pos (which we update), if the length len (which we
     * also update) is less than the maximum length "max".
     *
     * Return:  meTRUE    if the character will fit
     *          meFALSE   otherwise
     */
    int ll ;
    ll = meStrlen(buf);
    if(ll + 1 >= max)
        return meFALSE ;
    
    if(*pos == ll)
    {
        /* The character goes on the end of the buffer. */
        buf[ll++] = cc;
        buf[ll] = '\0';
        *len = ll;
        *pos = ll;
    }
    else
    {
        /* The character goes in the middle of the buffer. */
        meUByte *ss, c1, c2 ;
        
        ss = buf + *pos ;
        c2 = cc ;
        
        do {
            c1 = c2 ;
            c2 = *ss ;
            *ss++ = c1 ;
        } while(c1 != '\0') ;
        
        /* update the pos & length */
        *len = ll+1;
        (*pos)++ ;
    }
    return meTRUE ;
}

static void
cdel(meUByte *buf, int pos, int *len)
{
    /*
     * Delete the character at position 'pos' in the buffer, update len.
     */
    meStrcpy(&buf[pos], &buf[pos + 1]);
    buf[--*len] = '\0';
}

#define ERASE (-1)      /* erase whatever we move over	*/
#define LEAVE 1         /* leave whatever we move over	*/

/* buf		pointer to buffer 		*/
/* pos		position in above buffer	*/
/* len		length of buffer (ie no. of chars in it) */
/* state	either ERASE or LEAVE		*/
static void
fspace(meUByte *buf, int *pos, int *len, int state)
{
    /*
     * Move over or erase spaces in the buffer (depending on the value
     * of "state") pointed to by "buf", after the current cursor position,
     * given by "pos", updating the length (ie the number of characters
     * in the buffer).
     */
    register meUByte c;
    
    for(;;)
    {
        if(*pos >= *len)
            return;
        
        if((c = buf[*pos]) != ' ' && c!= '\t' && c != DIR_CHAR)
            return;
        
        if(state == ERASE)
            cdel(buf, *pos, len);
        else
            ++*pos;
    }
}

/* buf		pointer to buffer 		*/
/* pos		position in above buffer	*/
/* len		length of buffer (ie no. of chars in it) */
/* state	either ERASE or LEAVE		*/
static void
bspace(meUByte *buf, int *pos, int *len, int state)
{
    /*
     * Identical to fspace() except that we go backwards instead of
     * forwards.
     */
    register meUByte c;
    
    for(;;)
    {
        if(*pos == 0)
            return;
        
        if((c = buf[*pos - 1]) != ' ' && c != '\t' && c != DIR_CHAR)
            return;
        
        if(state == ERASE)
            cdel(buf, *pos - 1, len);
        
        --*pos;
    }
}

/* buf		pointer to buffer 		*/
/* pos		position in above buffer	*/
/* len		length of buffer (ie no. of chars in it) */
/* state	either ERASE or LEAVE		*/
static void
fword(meUByte *buf, int *pos, int *len, int state)
{
    /*
     * Move forward over a "word", erasing it if "state" is ERASE.
     * The defintion of "word" is very rudimentary. It is considered
     * to be delimited by a space or by the dir char "/" character.
     */
    register meUByte c;
    
    for(;;)
    {
        if(*pos >= *len)
            return ;
        
        if((c = buf[*pos]) == ' ' || c == '\t' || c == DIR_CHAR)
            return;
        
        if(state == ERASE)
            cdel(buf, *pos, len);
        else
            ++*pos;
    }
}

/* buf		pointer to buffer 		*/
/* pos		position in above buffer	*/
/* len		length of buffer (ie no. of chars in it) */
/* state	either ERASE or LEAVE		*/
static void
bword(meUByte *buf, int *pos, int *len, int state)
{
    /*
     * As for fword() but go backwards to the start, instead of forwards.
     */
    register meUByte c;
    
    for(;;)
    {
        if(*pos == 0)
            return;
        
        if((c = buf[*pos - 1]) == ' ' || c == '\t' || c == DIR_CHAR)
            return;
        
        if(state == ERASE)
            cdel(buf, *pos - 1, len);
        
        --*pos;
    }
}

/*
 * mldisp() - expand control characters in "prompt" and "buf" (ie \t -> <TAB>,
 * \n -> <NL> etc etc). The third argument, "cpos" causes mldisp() to return
 * the column position of the "cpos" character in the "buf" string, ie if
 * mldisp is called as:
 *
 *        mldisp("hello", "\n\nthere", 3);
 *
 * Then mldisp will return:
 *
 *        5 + 4 + 4 + 1 = 10
 *        ^   ^   ^   ^
 *        |   |   |   |
 *    lenght of   |   |   length of "t"
 *       prompt   |   |   ^
 *      "hello"   lenght  |
 *            of <NL> |
 *            ^        |
 *            |-------|
 *            |
 *            3 characters into "buf"
 *
 * If "cpos" is < 0 then the the length of the expanded prompt + buf
 * combination, or term.t_ncol is returned (the routine displays only
 * term.t_ncol characters of the expanded buffer.
 * Added continue string which is printed after buf.
 */

#define    RESTSIZ        200    /* length of buf expand buffer      */

/* prompt         prompt to be displayed      */
/* buf            buffer to expand controls from */
/* cont           continuing str to be displayed */
/* cpos           offset into "buf"          */
/* expChr         0=dont expand unprintables, use ?, 1=expand ?'s */
void
mlDisp(meUByte *prompt, meUByte *buf, meUByte *cont, int cpos)
{
    int    len ;               /* offset into current buffer   */
    char   expbuf[RESTSIZ+1];    /* expanded buf                 */
    int    start, col, maxCol ;
    int    promsiz ;
    
    col = -1 ;
    len = expandexp(-1,buf,RESTSIZ,0,(meUByte *)expbuf,cpos,&col,0) ;
    if(col < 0)
        col = len ;
    if(cont != NULL)
    {
        meStrncpy(expbuf+len,cont,RESTSIZ-len);
        expbuf[RESTSIZ] = '\0' ;
        len += strlen(expbuf+len) ;
    }
    /* switch off the status cause we are replacing it */
    frameCur->mlStatus = 0 ;
    maxCol = frameCur->width ;
    promsiz = meStrlen(prompt) ;
    col += promsiz ;
    len += promsiz ;
    /*
     * Having expanded it, display it.
     *
     * "ret" which is the actual column number of the "cpos" character
     * is used as an indication of how to display the line. If it is
     * too long for the terminal to display then:
     *    if "ret" is between 0 and max-column-number
     *    then
     *        display with $ in right hand column
     *    else
     *        display with $ after prompt
     *
     * Be careful about putting characters in the lower left hand corner
     * of the screen as this may cause scrolling on some terminals.
     */
    if(col > maxCol - 1)
    {
        int div = maxCol >> 1 ;
        
        start = (((col - (div >> 1)) / div) * div) ;
        len -= start ;
        col -= start-1 ;
        maxCol-- ;
    }
    else
        start = 0 ;
    if(len > maxCol)
    {
        expbuf[start+maxCol-promsiz-1] = '$';
        expbuf[start+maxCol-promsiz] = '\0';
    }
    frameCur->mlColumn = col ;
    if(start >= promsiz)
        mlwrite(MWUSEMLCOL|MWCURSOR,(meUByte *)"%s%s",(start) ? "$":"",expbuf+start-promsiz) ;
    else
        mlwrite(MWUSEMLCOL|MWCURSOR,(meUByte *)"%s%s%s",(start) ? "$":"",prompt+start,expbuf) ;
    
    /* switch on the status so we save it */
    frameCur->mlStatus = MLSTATUS_KEEP|MLSTATUS_POSML ;
}



/*    tgetc:    Get a key from the terminal driver, resolve any keyboard
 *              macro action
 */
static meUShort
tgetc(void)
{
    meUShort cc ;    /* fetched character */

    /* if we are playing a keyboard macro back, */
    if (kbdmode == mePLAY)
    {
kbd_rep:
        /* if there is some left... */
        if(kbdoff < kbdlen)
        {
            cc = (meUShort) kbdptr[kbdoff++] ;
            if(cc == 0xff)
            {
                cc = kbdptr[kbdoff++] ;
                if(cc == 0x02)
                {
                    meUByte dd ;    /* fetched character */
                    cc = ((meUShort) kbdptr[kbdoff++]) << 8 ;
                    if(((dd = kbdptr[kbdoff++]) != 0xff) ||
                       ((dd = kbdptr[kbdoff++]) != 0x01))
                        cc |= dd ;
                }
                else if(cc == 0x01)
                    cc = 0 ;
            }
            return cc ;
        }
        /* at the end of last repitition? */
        if (--kbdrep > 0)
        {
            /* reset the macro to the begining for the next rep */
            kbdoff = 0 ;
            goto kbd_rep ;
        }
        kbdmode = meSTOP;
#if MEOPT_UNDO
        undoContFlag++ ;
#endif
        /* force a screen update after all is done */
        update(meFALSE);
    }

    if(kbdmode == meRECORD)
    {
#if MEOPT_MOUSE
        /* get and save a key */
        /* ignore mouse keys while recording a macro - get another */
        do {
            /* fetch a character from the terminal driver */
            cc = TTgetc();
        } while(((cc & (ME_SPECIAL|0x00ff)) >= (ME_SPECIAL|SKEY_mouse_drop_1))  &&
                ((cc & (ME_SPECIAL|0x00ff)) <= (ME_SPECIAL|SKEY_mouse_time_3))) ;
#else
        cc = TTgetc();
#endif
        /* Each 'key' could take 5 chars to store - if we haven't got room
         * stop so we don't overrun the buffer */
        if(kbdlen > meKBDMACRO_SIZE_MAX - 5)
        {
            kbdmode = meSTOP;
            TTbell();
        }
        else
        {
            meUByte dd ;
            /* must store 0xaabb as ff,2,aa,bb
             * also must store 0x00ff as ff,ff & 0x0000 as 0xff01
             * Also 0x??00 stored as ff,2,??,ff,01
             */
            if(cc > 0xff)
            {
                kbdptr[kbdlen++] = 0xff ;
                kbdptr[kbdlen++] = 2 ;
                kbdptr[kbdlen++] = cc >> 8 ;
            }
            dd = (cc & 0xff) ;
            if(dd == 0xff)
            {
                kbdptr[kbdlen++] = 0xff ;
                kbdptr[kbdlen++] = 0xff ;
            }
            else if(dd == 0x0)
            {
                kbdptr[kbdlen++] = 0xff ;
                kbdptr[kbdlen++] = 0x01 ;
            }
            else
                kbdptr[kbdlen++] = dd ;
        }
    }
    else
        cc = TTgetc();

    /* and finally give the char back */
    return cc ;
}


static int
getprefixchar(int f, int n, int ctlc, int flag)
{
    meUByte buf[20] ;
    int c ;

    if(!(flag & meGETKEY_SILENT))
    {
        buf[meGetStringFromChar((meUShort) ctlc,buf)] = '\0' ;
        if(f==meTRUE)
            mlwrite(MWCURSOR,(meUByte *)"Arg %d: %s", n, buf);
        else
            mlwrite(MWCURSOR,(meUByte *)"%s", buf);
    }
    c = tgetc();
    if(!(c & (ME_SHIFT|ME_CONTROL|ME_ALT|ME_SPECIAL)))
        c = toLower(c) ;
    return c ;
}

/*    GETCMD:    Get a command from the keyboard. Process all applicable
        prefix keys
 */
meUShort
meGetKeyFromUser(int f, int n, int flag)
{
    meUShort cc ;        /* fetched keystroke */
    int ii ;
    
    if(kbdmode == mePLAY)
    {
        if(TTbreakTest(0))
        {
            ctrlg(meFALSE,1) ;
#if MEOPT_UNDO
            undoContFlag++ ;
#endif
            /* force a screen update */
            update(0);
        }
        else
            flag |= meGETKEY_SILENT ;
    }
    if(f && !(flag & meGETKEY_SILENT))
        mlwrite(MWCURSOR,(meUByte *)"Arg %d:",n);

    /* get initial character */
    cc = tgetc();
    
    /* If we are in idle mode then disable now */
    if (kbdmode == meIDLE)            /* In the idle state */
        kbdmode = meSTOP;                 /* Restore old state */

    if(!(flag & meGETKEY_SINGLE))
    {
        /* process prefixes */
        ii = ME_PREFIX_NUM+1 ;
        while(--ii > 0)
            if(cc == prefixc[ii])
            {
                cc = ((ii << ME_PREFIX_BIT) | getprefixchar(f,n,cc,flag)) ;
                break ;
            }
        if(flag & meGETKEY_COMMAND)
            thiskey = cc ;
    }
    /* return the key */
    return cc ;
}

static meIFuncSSI curCmpIFunc ;
static meIFuncSS  curCmpFunc ;

static int
getFirstLastPos(int noStr,meUByte **strs, meUByte *str, int option,
                int *fstPos, int *lstPos)
{
    register int nn ;
    
    if((nn = meStrlen(str)) == 0)
    {
        *fstPos = 0 ;
        *lstPos = noStr - 1 ;
        if(noStr <= 0)
            return 0 ;
    }
    else
    {
        register int ii, rr ;
        
        for(ii=0 ; ii<noStr ; ii++)
            if(!(rr = curCmpIFunc((char *)str,(char *)strs[ii],nn)))
                break ;
            else if(rr < 0)
                return 0 ;
        
        if(ii == noStr)
            return 0 ;
        
        *fstPos = ii ;
        for(++ii ; ii<noStr ; ii++)
            if(curCmpIFunc((char *)str,(char *)strs[ii],nn))
                break ;
        *lstPos = ii - 1 ;
    }
#if MEOPT_EXTENDED
    if((option & MLFILE) && (fileIgnore != NULL))
    {
        /* try to eat away at the number of possible completions
         * until there is only one. If successful the return as
         * just one. Can remove fileIngore type files which are 
         * usually . .. and backup files (*~) etc.
         */
        register int ff, ll, ii=1, len, fil ;
        register meUByte *ss, *fi, cc ;
        ff = *fstPos ;
        ll = *lstPos ;
        
        while((ff <= ll) && (ii >= 0))
        {
            if(ii)
                ss = strs[ll] ;
            else
                ss = strs[ff] ;
            len = meStrlen(ss) ;
            fi = fileIgnore ;
            for(;;)
            {
                fil=1 ;
                while(((cc=fi[fil]) != ' ') && (cc != '\0'))
                    fil++ ;
                if((fil <= len) &&
                   !curCmpIFunc((char *)ss+len-fil,(char *)fi,fil))
                {
                    if(ii)
                        ll-- ;
                    else
                        ff++ ;
                    break ;
                }
                fi += fil ;
                if(*fi++ == '\0')
                {
                    ii-- ;
                    break ;
                }
            }
        }
        if(ff == ll)
            *fstPos = *lstPos = ff ;
    }
#endif
    return 1 ;
}

int
createBuffList(meUByte ***listPtr, int noHidden)
{
    register meBuffer *bp = bheadp ;    /* index buffer pointer    */
    register int     i, n ;
    register meUByte **list ;

    n = 0 ;
    while(bp != NULL)
    {
        if(!noHidden || !meModeTest(bp->mode,MDHIDE))
            n++;
        bp = bp->next ;
    }
    if((list = (meUByte **) meMalloc(sizeof(meUByte *) * n)) == NULL)
        return 0 ;
    bp = bheadp ;
    for(i=0 ; i<n ; )
    {
        if(!noHidden || !meModeTest(bp->mode,MDHIDE))
            list[i++] = bp->name ;
        bp = bp->next ;
    }
    *listPtr = list ;
    return i ;
}
    
int
createVarList(meUByte ***listPtr)
{
    meVariable *vptr;     	/* User variable pointer */
    int     ii, nn ;
    meUByte  **list ;

    nn = NEVARS + usrVarList.count + frameCur->bufferCur->varList.count ;
    
    if((list = (meUByte **) meMalloc(sizeof(meUByte *) * nn)) == NULL)
        return 0 ;
    *listPtr = list ;
    
    for(ii=0 ; ii<NEVARS; ii++)
    {
        if((list[ii] = meMalloc(meStrlen(envars[ii])+2)) == NULL)
            return 0 ;
        list[ii][0] = '$' ;
        meStrcpy(list[ii]+1,envars[ii]) ;
    }
    for(vptr=usrVarList.head ; vptr != NULL ; vptr = vptr->next,ii++)
    {
        if((list[ii] = meMalloc(meStrlen(vptr->name)+2)) == NULL)
            return 0 ;
        list[ii][0] = '%' ;
        meStrcpy(list[ii]+1,vptr->name) ;
    }
    for(vptr=frameCur->bufferCur->varList.head ; vptr != NULL ; vptr = vptr->next,ii++)
    {
        if((list[ii] = meMalloc(meStrlen(vptr->name)+2)) == NULL)
            return 0 ;
        list[ii][0] = ':' ;
        meStrcpy(list[ii]+1,vptr->name) ;
    }
    return ii ;
}
    
int
createCommList(meUByte ***listPtr, int noHidden)
{
    meCommand *cmd ;
    register int ii ;
    register meUByte **list ;
    
    if((list = meMalloc(sizeof(meUByte *) * (cmdTableSize))) == NULL)
        return 0 ;
    ii = 0 ;
    cmd = cmdHead ;
    while(cmd != NULL)
    {
        if(!noHidden || (cmd->id < CK_MAX) ||
           !(((meMacro *) cmd)->hlp->flag & meMACRO_HIDE))
            list[ii++] = cmd->name ;
        cmd = cmd->anext ;
    }
    *listPtr = list ;
    return ii ;
}


#if MEOPT_LOCALBIND
meUByte oldUseMlBinds ;
#endif
meUByte **mlgsStrList ;
int    mlgsStrListSize ;
static meWindow  *mlgsOldCwp=NULL ;
static meUByte *mlgsStoreBuf=NULL ;
static meInt    mlgsSingWind=0 ;
#if MEOPT_EXTENDED
static int      mlgsCursorState=0 ;
#endif

static void
mlfreeList(int option, int noStrs, meUByte **strList)
{
    frameCur->mlStatus = MLSTATUS_CLEAR ;
#if MEOPT_LOCALBIND
    useMlBinds = oldUseMlBinds ;
#endif
    if(mlgsStoreBuf != NULL)
    {
        meFree(mlgsStoreBuf) ;
        mlgsStoreBuf = NULL ;
    }
    if(strList != NULL)
    {
        if(option & MLVARBL)
            freeFileList(noStrs,strList) ;
        else if(option & (MLBUFFER|MLCOMMAND))
            meFree(strList) ;
    }
    if(mlgsOldCwp != NULL)
    {
        meBuffer *bp=frameCur->bufferCur ;
        if(mlgsSingWind)
        {
            delwind(meFALSE,meFALSE) ;
            frameCur->windowCur->vertScroll = mlgsSingWind-1 ;
        }
        else
        {
            meBuffer *rbp=NULL, *tbp=bheadp ;
            while(tbp != NULL)
            {
                if((tbp != bp) && ((rbp == NULL) || (rbp->histNo < tbp->histNo)))
                    rbp = tbp ;
                tbp = tbp->next ;
            }
            swbuffer(frameCur->windowCur,rbp) ;
            makeCurWind(mlgsOldCwp) ;
        }
        zotbuf(bp,1) ;
        mlgsOldCwp = NULL ;
    }
#if MEOPT_EXTENDED
    if(mlgsCursorState < 0)
        showCursor(meTRUE,mlgsCursorState) ;
#endif
}

#if MEOPT_MOUSE
static int
mlHandleMouse(meUByte *inpBuf, int inpBufSz, int compOff)
{
    int row, col ;
    
    if((mlgsOldCwp != NULL) &&
       ((row=mouse_Y-frameCur->windowCur->frameRow) >= 0) && (row < frameCur->windowCur->depth-1) &&
       ((col=mouse_X-frameCur->windowCur->frameColumn) >= 0) && (col < frameCur->windowCur->width))
    {
        if (col >= frameCur->windowCur->textWidth)
        {
            /* only do scroll bar if on pick and bars are enabled */
            if((inpBuf == NULL) && (frameCur->windowCur->vertScrollBarMode & WMSCROL))
            {
                int ii ;
                for (ii = 0; ii <= (WCVSBML-WCVSBSPLIT); ii++)
                    if (mouse_Y < (meShort) frameCur->windowCur->vertScrollBarPos[ii])
                        break;
                if(ii == (WCVSBUP-WCVSBSPLIT))
                    scrollUp(1,1) ;
                else if(ii == (WCVSBUSHAFT-WCVSBSPLIT))
                    scrollUp(0,1) ;
                else if(ii == (WCVSBDOWN-WCVSBSPLIT))
                    scrollDown(1,1) ;
                else if(ii == (WCVSBDSHAFT-WCVSBSPLIT))
                    scrollDown(0,1) ;
                update(meTRUE) ;
            }
        }
        else
        {
            meLine *lp ;
            int ii, jj, lineNo ;
            
            row += frameCur->windowCur->vertScroll ;
            lineNo = row ;
            lp = frameCur->bufferCur->baseLine->next ;
            while(--row >= 0)
                lp = meLineGetNext(lp) ;
            if((lp->flag & meLINE_NOEOL) && (col >= frameCur->windowCur->markOffset))
                ii = frameCur->windowCur->markOffset ;
            else
                ii = 0 ;
            if((ii == 0) && (lp->flag & meLINE_NOEOL))
            {
                jj = 1 ;
                while((meLineGetChar(lp,jj) != ' ') || (meLineGetChar(lp,jj+1) != ' '))
                    jj++ ;
            }
            else
                jj = meLineGetLength(lp) ;
            if(jj > col)
            {
                frameCur->windowCur->dotLine = lp ;
                frameCur->windowCur->dotOffset = ii ;
                frameCur->windowCur->dotLineNo = lineNo ;
                setShowRegion(frameCur->bufferCur,lineNo,ii,lineNo,jj) ;
                frameCur->windowCur->flag |= WFMOVEL|WFSELHIL ;
                if(inpBuf != NULL)
                {
                    if((jj -= ii) >= (inpBufSz-compOff))
                        jj = inpBufSz-compOff-1 ;
                    /* if we already have this, assume the user has double clicked
                     * to 'select', so return 2 to exit */
                    if((inpBuf[compOff+jj] == '\0') && 
                       !meStrncmp(inpBuf+compOff,meLineGetText(lp)+ii,jj))
                        return 2 ;
                    meStrncpy(inpBuf+compOff,meLineGetText(lp)+ii,jj) ;
                    inpBuf[compOff+jj] = '\0' ;
                }
                update(meTRUE) ;
                return 1 ;
            }
        }
    }
    return 0 ;
}
#endif

meUByte *compSole     = (meUByte *)" [Sole completion]" ;
meUByte *compNoMch    = (meUByte *)" [No match]" ;
meUByte *compNoExp    = (meUByte *)" [No expansion]" ;
meUByte *compFailComp = (meUByte *)" [Failed to create]" ;

#if MEOPT_OSD
#define mlgsDisp(prom,buf,contstr,ipos) \
((frameCur->mlStatus & MLSTATUS_POSOSD) ? osdDisp(buf,contstr,ipos):mlDisp(prom,buf,contstr,ipos))
#else
#define mlgsDisp(prom,buf,contstr,ipos) \
(mlDisp(prom,buf,contstr,ipos))
#endif
/* prompt - prompt associated with what we're typing         */
/* option - bit field which modifies our behaviour slightly  */
/* defnum - the default no. in history (0 = no default)      */
/* buf    - where it all goes at the end of the day          */
/* nbuf   - amount of space in buffer                        */
int
meGetStringFromUser(meUByte *prompt, int option, int defnum, meUByte *buf, int nbuf)
{
    register int cc ;
    int     ii ;
    int     ipos ;                      /* input position in buffer */
    int     ilen ;                      /* number of chars in buffer */
    int     cont_flag ;                 /* Continue flag */
    meUByte **history ;
    meUByte   onHist, numHist, *numPtr ;
    meUByte  *defaultStr ;
    meUByte   prom[meBUF_SIZE_MAX] ;
    meUByte   ch, **strList ;
    meUByte  *contstr=NULL ;
    int     gotPos=1, fstPos, lstPos, curPos, noStrs ;
    int     changed=1, compOff=0 ;
    
    if((mlgsStoreBuf = meMalloc(nbuf)) == NULL)
       return meABORT ;

    mlerase(0);            /* Blank line to force update */

    if(option & MLINSENSCASE)
    {
        curCmpFunc  = stridif ;
        curCmpIFunc = strnicmp ;
    }
    else
    {
        curCmpFunc  = strcmp ;
        curCmpIFunc = strncmp ;
    }
    numHist = setupHistory(option, &numPtr, &history) ;
    if(option & MLBUFFER)
        noStrs = createBuffList(&strList,1) ;
    else if(option & MLVARBL)
        noStrs = createVarList(&strList) ;
    else if(option & MLCOMMAND)
        noStrs = createCommList(&strList,1) ;
    else if(option & MLUSER)
    {
        strList = mlgsStrList ;
        noStrs = mlgsStrListSize ;
    }
    else
    {
        strList = NULL ;
        noStrs = 0 ;
    }
    if(strList != NULL)
        sortStrings(noStrs,strList,0,curCmpFunc) ;

    if(option & MLNOHIST)
        onHist = ~0 ;
    else
        onHist = numHist+1 ;
    meStrcpy(prom,prompt) ;
    ii = meStrlen(prom) ;
    if((defnum > 0) && (defnum <= numHist))
    {
        defaultStr = history[defnum-1] ;
        meStrncpy(prom+ii," (default [",11) ;
        ii = expandexp(-1,defaultStr,meBUF_SIZE_MAX-5,ii+11,prom,-1,NULL,0) ;
        meStrcpy(prom+ii,"]): ") ;
    }
    else
    {
        defaultStr = NULL ;
        meStrcpy(prom+ii,": ") ;
    }

    if(option & MLNORESET)
    {
        ilen = meStrlen(buf) ;
#if MEOPT_OSD
        if(frameCur->mlStatus & MLSTATUS_OSDPOS)
        {
            meUByte *s1, *s2 ;
            s1 = buf ;
            while((--osdRow >= 0) && ((s2 = meStrchr(s1,meNLCHAR)) != NULL))
                s1 = s2+1 ;
            if(osdRow >= 0)
                ipos = ilen ;
            else
            {
                ipos = (int) s1 - (int) buf + osdCol ;
                if(ipos > ilen)
                    ipos = ilen ;
                if(((s2 = meStrchr(s1,meNLCHAR)) != NULL) &&
                    (((int) s2 - (int) buf) < ipos))
                    ipos = (int) s2 - (int) buf ;
            }
        }
        else
#endif
            ipos = ilen ;
    }
    else
    {
        ipos = ilen = 0 ;
        buf[0] = '\0' ;
    }
#if MEOPT_LOCALBIND
    oldUseMlBinds = useMlBinds ;
    useMlBinds = 1 ;
#endif
#if MEOPT_EXTENDED
    if((mlgsCursorState=cursorState) < 0)
        showCursor(meFALSE,1) ;
#endif
    for (cont_flag = 1; cont_flag != 0;)
    {
        meUInt arg ;
        int idx ;
        int ff ;
        
        if(option & MLHIDEVAL)
        {
            meUByte hbuf[meBUF_SIZE_MAX] ;
            ff = meStrlen(buf) ;
            meAssert(ff < meBUF_SIZE_MAX) ;
            memset(hbuf,'*',ff) ;
            hbuf[ff] = '\0' ;
            mlgsDisp(prom,hbuf,contstr,ipos) ;
        }
        else
            mlgsDisp(prom,buf,contstr,ipos) ;
        contstr = NULL ;

        /*
        ** mlfirst  may be set by meShell. If both are unset, get a 
        ** character from the user. 
        */

        if(mlfirst != -1)
        {
            cc = mlfirst ;
            mlfirst = -1;
        }
        else
            cc = meGetKeyFromUser(meFALSE,0,meGETKEY_SILENT) ;
        
        idx = decode_key((meUShort) cc,&arg) ;
        if(arg)
        {
            ff = 1 ;
            ii = (int) (arg + 0x80000000) ;
        }
        else
        {
            ff = 0 ;
            ii = 1 ;
        }
        switch(idx)
        {
        case CK_GOBOL:          /* ^A : Move to start of buffer */
            if(frameCur->mlStatus & MLSTATUS_NINPUT)
            {
                while(ipos && (buf[ipos-1] != meNLCHAR))
                    ipos-- ;
            }
            else
                ipos = 0;
            break;
            
        case CK_BAKCHR:    /* ^B : Backwards char */
            while(ipos && ii--)
                ipos--;
            break;
            
        case CK_DELFOR:    /* ^D : Delete the character under the cursor */
            while((ipos < ilen) && ii--)
            {
                cdel(buf, ipos, &ilen);
                /*                if(ipos > ilen)*/
                /*                    ipos = ilen;*/
                changed=1 ;
            }
            break;
            
        case CK_GOEOL:    /* ^E : Move to end of buffer */
            if(frameCur->mlStatus & MLSTATUS_NINPUT)
            {
                while((ipos < ilen) && (buf[ipos] != meNLCHAR))
                    ipos++ ;
            }
            else
                ipos = ilen ;
            break;
            
        case CK_FORCHR:    /* ^F : Move forward one character */
            while((ipos < ilen) && ii--)
                ipos++;
            break;
            
        case CK_ABTCMD: /* ^G : Abort input and return */
            mlfreeList(option,noStrs,strList) ;
            return ctrlg(meFALSE,1);
            
        case CK_DELBAK:    /* ^H : backwards delete. */
            while(ipos && ii--)
            {
                cdel(buf, ipos-1, &ilen);
                ipos--;
                changed=1 ;
            }
            break;
            
        case CK_DOTAB:    /* ^I : Tab character */
            if(frameCur->mlStatus & MLSTATUS_NINPUT)
            {
                cont_flag = 0;
                break;
            }
            cc = '\t' ;    /* ^I for search strings */
            if(!(option & (MLCOMMAND | MLFILE | MLBUFFER | MLVARBL | MLUSER)))
                goto input_addexpand ;
input_expand:
            if(option & MLFILE)
            {
                meUByte fname[meFILEBUF_SIZE_MAX], *base ;
                
                pathNameCorrect(buf,PATHNAME_PARTIAL,fname,&base) ;
                meStrcpy(buf,fname) ;
                ipos = ilen = meStrlen(buf) ;
                /* if current buff is xxx/yyy/ then pathNameCorrect will
                 * return the path as xxx/ and base as yyy/ but for completion
                 * we want to list yyy/ so move the base for the case */
                if(buf[ilen-1] != DIR_CHAR)
                    *base = '\0' ;
                compOff = meStrlen(fname) ;
                getDirectoryList(fname,&curDirList) ;
                noStrs = curDirList.size ;
                strList = curDirList.list ;
            }
            if(strList == NULL)
            {
                contstr = compNoExp ;
                TTbell();
                break ;
            }
            if(changed)
            {
                changed = 0 ;
                if((gotPos = getFirstLastPos
                    (noStrs,strList,buf+compOff,option,&fstPos,&lstPos)) == 0)
                {
                    contstr = compNoMch ;
                    TTbell();
                    break ;
                }
                curPos = fstPos-1 ;
                if(fstPos == lstPos)
                {
                    meStrcpy(buf+compOff,(strList[lstPos])) ;
                    ilen += meStrlen(buf+ilen) ;
                    if((option & MLFILE) && (buf[ilen-1] == DIR_CHAR))
                        /* if we're entering a file name and we've just
                         * completed to the a directory name then the next
                         * 'TAB' completion should be for inside the new directory,
                         * so effectively we have changed the input point */
                        changed = 1 ;
                }
                else
                {
                    for(ii=fstPos ; ii<=lstPos ; ii++)
                        if(strList[ii][ilen-compOff] == cc)
                            goto input_addexpand ;
                    for(;;)
                    {
                        ch = strList[lstPos][ilen-compOff] ;
                        for(ii=fstPos ; ii<lstPos ; ii++)
                            if((strList[ii][ilen-compOff] != ch) &&
                               (((option & MLINSENSCASE) == 0) ||
                                ((meUByte) toLower(strList[ii][ilen-compOff]) != (meUByte) toLower(ch))))
                                break ;
                        if(ii != lstPos)
                            break ;
                        if(ch == '\0')
                            break ;
                        buf[ilen++] = ch ;
                    }
                    buf[ilen] = '\0' ;
                }
                ipos = ilen ;
            }
            else if(!gotPos)
                contstr = compNoMch ;
            else if(fstPos == lstPos)
                contstr = compSole ;
            else
            {
                meUByte line[150] ;
                int len, lwidth ;
                
                for(ii=fstPos ; ii<=lstPos ; ii++)
                    if(strList[ii][ilen-compOff] == cc)
                        goto input_addexpand ;
                if(mlgsOldCwp == NULL)
                {
                    if(frameCur->windowList->next == NULL)
                        mlgsSingWind = frameCur->windowCur->vertScroll+1 ;
                    else
                        mlgsSingWind = 0 ;
                    mlgsOldCwp = frameCur->windowCur ;
                }
                
                if(wpopup(BcompleteN,BFND_CREAT|BFND_CLEAR|WPOP_MKCURR) == NULL)
                {
                    contstr = compFailComp ;
                    break ;
                }
                /* remove any completion list selection hilighting */
                if(selhilight.bp == frameCur->bufferCur)
                    selhilight.flags &= ~SELHIL_ACTIVE ;
                
                /* Compute the widths available from the window width */
                lwidth = frameCur->windowCur->textWidth >> 1;
                if (lwidth > 75)
                    lwidth = 75 ;
                frameCur->windowCur->markOffset = lwidth ;
                for(ii=fstPos ; ii<=lstPos ; ii++)
                {
                    meUByte flag ;
                    
                    if(((len= meStrlen(strList[ii])) < lwidth) && (ii<lstPos) &&
                       (((int) meStrlen(strList[ii+1])) < lwidth-1))
                    {
                        meStrcpy(line,strList[ii++]) ;
                        memset(line+len,' ',lwidth-len) ;
                        meStrcpy(line+lwidth,strList[ii]) ;
                        flag = meLINE_NOEOL ;
                    }
                    else
                    {
                        meStrncpy(line,strList[ii],148) ;
                        line[149] = '\0' ;
                        flag = 0 ;
                    }
                    addLineToEob(frameCur->bufferCur,line) ;
                    frameCur->bufferCur->baseLine->prev->flag |= flag ;
                }
                gotobob(meFALSE,meFALSE) ;
                update(meTRUE) ;
            }
            break ;
            
        case CK_MOVUWND:
            if(mlgsOldCwp != NULL)
            {
                scrollUp(ff,ii) ;
                update(meTRUE) ;
            }
            break ;
            
        case CK_MOVDWND:
            if(mlgsOldCwp != NULL)
            {
                scrollDown(ff,ii) ;
                update(meTRUE) ;
            }
            break ;
            
        case CK_BAKLIN: /* M-P - previous match in complete list */
            if(frameCur->mlStatus & MLSTATUS_NINPUT)
            {
                ii = ipos ;
                while((--ii >= 0) && (buf[ii] != meNLCHAR))
                    ;
                if(ii >= 0)
                {
                    int jj ;
                    jj = ipos - ii ;
                    ipos = ii ;
                    /* work out the new line offset */
                    while((--ii >= 0) && (buf[ii] != meNLCHAR))
                        ;
                    if((ii+jj) < ipos)
                        ipos = ii+jj ;
                }
            }
            else
            {
                if(ff || !(option & (MLCOMMAND | MLFILE | MLBUFFER | MLVARBL | MLUSER)))
                    goto mlgs_prevhist ;
                if((strList != NULL) && !changed && gotPos)
                {
                    if(fstPos == lstPos)
                        contstr = compSole ;
                    else
                    {
                        if(--curPos < fstPos)
                            curPos = lstPos ;
                        meStrncpy(buf+compOff,strList[curPos],nbuf-compOff) ;
                        buf[nbuf-1] = '\0' ;
                        ipos = ilen = meStrlen(buf) ;
                    }
                }
            }
            break ;
            
        case CK_FORLIN: /* ^N - next match in complete list */
            if(frameCur->mlStatus & MLSTATUS_NINPUT)
            {
                ii = ipos ;
                while((ii < ilen) && (buf[ii] != meNLCHAR))
                    ii++ ;
                if(ii < ilen)
                {
                    int jj ;
                    jj = ipos ;
                    while(jj-- && (buf[jj] != meNLCHAR))
                        ;
                    jj = ipos-jj-1 ;
                    ipos = ii+1 ;
                    while((--jj >= 0) && (ipos < ilen) && (buf[ipos] != meNLCHAR))
                        ipos++ ;
                }
            }
            else
            {
                if(ff || !(option & (MLCOMMAND | MLFILE | MLBUFFER | MLVARBL | MLUSER)))
                    goto mlgs_nexthist ;
                if((strList != NULL) && !changed && gotPos)
                {
                    if(fstPos == lstPos)
                        contstr = compSole ;
                    else
                    {
                        if(++curPos > lstPos)
                            curPos = fstPos ;
                        meStrncpy(buf+compOff,strList[curPos],nbuf-compOff) ;
                        buf[nbuf-1] = '\0' ;
                        ipos = ilen = meStrlen(buf) ;
                    }
                }
            }
            break ;
            
        case CK_KILEOL:    /* ^K : Kill to end of line */
            if(ipos < ilen)
            {
                if(frameCur->mlStatus & MLSTATUS_NINPUT)
                {
                    ii = ilen ;
                    while((ipos < ilen) && (buf[ipos] != meNLCHAR))
                        cdel(buf, ipos, &ilen);
                    if((ii==ilen) ||
                       (!meModeTest(globMode,MDLINE) &&         /* if line kill mode    */
                        (ipos < ilen) &&                        /* some chars left      */
                        (!ipos || (buf[ipos-1] == meNLCHAR))))  /* whole line           */
                        cdel(buf, ipos, &ilen);
                }
                else
                {
                    buf[ipos] = '\0';
                    ilen = ipos;
                }
                changed=1 ;
            }
            break;
            
        case CK_RECENT:  /* ^L : Redraw the screen */
            sgarbf = meTRUE;
            update(meTRUE) ;
            mlerase(0);
            break;
            
        case CK_NEWLIN:  /* ^J : New line. Finish processing */
            if(frameCur->mlStatus & MLSTATUS_NINPUT)
            {
                cc = meNLCHAR ;
                goto input_addexpand ;
            }
            cont_flag = 0;
            break;
            
#if MEOPT_WORDPRO
        case CK_GOEOP:    /* M-N : Got to next in history list */
#endif
            /* Note the history list is reversed, ie 0 most recent,
            ** (numHist-1) the oldest. However if numHist then its
            ** the current one (wierd but easiest to implement
            */
mlgs_nexthist:
            if(!(option & MLNOHIST) && (numHist > 0))
            {
                if(onHist > numHist)
                    /* if on current then save */
                    meStrcpy(mlgsStoreBuf,buf) ;
                if(onHist == 0)
                {
                    onHist = numHist+1 ;
                    meStrcpy(buf,mlgsStoreBuf) ;
                }
                else if(onHist > numHist)
                {
                    meStrcpy(prom+meStrlen(prompt),": ") ;
                    defaultStr = NULL ;
                    onHist = numHist ;
                    buf[0] = '\0' ;
                }
                else
                {
                    meStrncpy(buf,history[--onHist],nbuf) ;
                    buf[nbuf-1] = '\0' ;
                }                    
                ipos = ilen = meStrlen(buf) ;
                changed=1 ;
            }
            break;
            
#if MEOPT_WORDPRO
        case CK_GOBOP:    /* M-P : Got to previous in history list */
#endif
mlgs_prevhist:
            if(!(option & MLNOHIST) && (numHist > 0))
            {
                if(onHist > numHist)
                {
                    /* if on current then save */
                    meStrcpy(mlgsStoreBuf,buf) ;
                    onHist = 0 ;
                }
                else
                    ++onHist ;
                if(onHist > numHist)
                    meStrcpy(buf,mlgsStoreBuf) ;
                else if(onHist == numHist)
                {
                    meStrcpy(prom+meStrlen(prompt),": ") ;
                    defaultStr = NULL ;
                    buf[0] = '\0' ;
                }
                else
                {
                    meStrncpy(buf,history[onHist],nbuf) ;
                    buf[nbuf-1] = '\0' ;
                }
                ipos = ilen = meStrlen(buf) ;
                changed=1 ;
            }
            break;
            
        case CK_TRNCHR:
            /* ^T : Transpose the character before the cursor
             *      with the one under the cursor.
             */
            if(ipos && ipos < ilen)
            {
                cc = buf[ipos - 1];
                buf[ipos - 1] = buf[ipos];
                buf[ipos] = cc;
                changed=1 ;
            }
            break;
            
        case CK_KILREG:    /* ^W : Kill the whole line */
            ipos = 0 ;
            ilen = 0 ;
            buf[ipos] = '\0';
            changed=1 ;
            break;
        case CK_QUOTE:    /* ^Q - quote the next character */
            if((cc = quoteKeyToChar(tgetc())) < 0)
            {
                TTbell() ;
                break ;
            }
            while(ii--)
                if(cins((meUByte) cc, buf, &ipos, &ilen, nbuf) == meFALSE)
                    TTbell();
            changed=1 ;
            break;
            
        case CK_OPNLIN:    /* ^O : Insert current line into buffer */
            {
                register meUByte *p = frameCur->windowCur->dotLine->text;
                register int count = frameCur->windowCur->dotLine->length;
                
                while(*p && count--)
                    cins(*p++, buf, &ipos, &ilen, nbuf);
                changed=1 ;
                break ;
            }
        case CK_YANK:    /* ^Y : insert yank buffer */
            {
                register meUByte *pp, cy ;
                meKillNode *killp;
                
#ifdef _CLIPBRD
                if((clipState & CLIP_TRY_GET) || (kbdmode != mePLAY))
                    TTgetClipboard() ;
#endif
                if(klhead == (meKill*) NULL)
                {
                    TTbell() ;
                    break ;
                }
                killp = klhead->kill;
                
                while(killp != NULL)
                {
                    pp = killp->data ;
                    while((cy=*pp++))
                        if(cins(cy, buf, &ipos, &ilen, nbuf) == meFALSE)
                        {
                            TTbell() ;
                            break ;
                        }
                    killp = killp->next;
                }
                changed=1 ;
                break;
            }
            
        case CK_INSFLNM:    /* insert file name */
            {
                register meUByte ch, *p = frameCur->bufferCur->fileName;
                
                if(p != NULL)
                    while(ii--)
                        while((ch=*p++) && cins(ch, buf, &ipos, &ilen, nbuf))
                            ;
                break ;
            }
            
            /*
             * Handle a limited set of Meta sequences. Note that
             * there is no conflict when the end-of-line char is
             * escape, since we will never reach here if it is.
             */
        case CK_BAKWRD:    /* M-B : Move to start of previous word */
            while(ipos && ii--)
            {
                bspace(buf, &ipos, &ilen, LEAVE); /* move over space after word */
                bword(buf, &ipos, &ilen, LEAVE);  /* move over word */
            }
            break;
            
        case CK_CAPWRD:    /* M-C : Capitalise next word and move past it. */
            while(ilen && (ipos != ilen) && ii--)
            {
                fspace(buf, &ipos, &ilen, LEAVE); /* Move over spaces before word */
                /* capitalise if lower-case letter */
                buf[ipos] = toUpper(buf[ipos]) ;
                for(ipos++ ; (ipos<ilen) && isWord(buf[ipos]) ; ipos++)
                    buf[ipos] = toLower(buf[ipos]) ;
                changed=1 ;
            }
            break;
            
        case CK_HIWRD:    /* M-U : Capitalise the whole of the next
                           *      word and move past it.
                           */
            while(ii--)
            {
                /* move over spaces before word */
                fspace(buf, &ipos, &ilen, LEAVE);
                
                /* capitalise if lower-case letter */
                for(; (ipos<ilen) && isWord(buf[ipos]) ; ipos++)
                    buf[ipos] = toUpper(buf[ipos]) ;
                changed=1 ;
            }
            break;
            
        case CK_LOWWRD:   /* M-L : Converts to lower case the whole of the next
                           *      word and move past it.
                           */
            while(ii--)
            {
                /* move over spaces before word */
                fspace(buf, &ipos, &ilen, LEAVE);
                
                /* to lower-case letter */
                for(; (ipos<ilen) && isWord(buf[ipos]) ; ipos++)
                    buf[ipos] = toLower(buf[ipos]) ;
                changed=1 ;
            }
            break;
            
        case CK_DELFWRD: /* Delete word forwards. */
            while(ilen && ii--)
            {
                fspace(buf, &ipos, &ilen, ERASE);    /* eat spaces before word */
                fword(buf, &ipos, &ilen, ERASE);    /* eat word */
                changed=1 ;
            }
            break;
            
        case CK_FORWRD:    /* M-F : Move forward to start of next word. */
            while((ipos != ilen) && ii--)
            {
                /* move over spaces before word */
                fspace(buf, &ipos, &ilen, LEAVE);
                fword(buf, &ipos, &ilen, LEAVE);    /* move over word */
            }
            break;
            
        case CK_SETMRK:
            /*
             * Do nothing - one day this will set the
             * mark at the current cursor position.
             */
            TTbell();
            break;
            
        case CK_DELWBAK:
            /* Delete word backwards. If spaces before
             * the input position then delete those too.
             */
            while(ipos && ii--)
            {
                bspace(buf, &ipos, &ilen, ERASE);    /* eat spaces after word */
                bword(buf, &ipos, &ilen, ERASE);    /* eat word */
                changed=1 ;
            }
            break;
            
        case CK_REYANK:    /* M-Y or M-^Y : Yank the current buffername. */
            {
                register meUByte *p = frameCur->bufferCur->name;
                
                while(*p && cins(*p++, buf, &ipos, &ilen, nbuf))
                    ;
                changed=1 ;
                break;
            }
            
        case CK_FISRCH:
        case CK_FORSRCH:
        case CK_BISRCH:
        case CK_BAKSRCH:
            if(option & MLISEARCH)
            {
                mlfirst = cc ;
                mlfreeList(option,noStrs,strList) ;
                return meTRUE ;
            }
            TTbell() ;
            break ;
#if MEOPT_MOUSE            
        case CK_CTOMOUSE:
            /* a binding to set-cursor-to-mouse is used to handle mouse
             * events, needs to handle osd pick and normal completion lists */
            /* is it a pick event */
            if((cc == (ME_SPECIAL|SKEY_mouse_pick_1)) ||
               (cc == (ME_SPECIAL|SKEY_mouse_pick_2)) ||
               (cc == (ME_SPECIAL|SKEY_mouse_pick_3)) )
            {
#if MEOPT_OSD
                if(frameCur->mlStatus & MLSTATUS_POSOSD)
                {
                    if(osdMouseContextChange(1))
                        cont_flag = 0;
                }
                else
#endif
                    mlHandleMouse(NULL,0,0) ;
            }
            /* a drop event */
            else if(!(frameCur->mlStatus & MLSTATUS_POSOSD) && ((cc=mlHandleMouse(buf,nbuf,compOff)) != 0))
            {
                ipos = ilen = meStrlen(buf) ;
                if(cc == 2)
                    cont_flag = 0;
                else
                    changed = 1 ;
            }
            break ;
#endif            
        case CK_VOIDFUNC:
            break ;
            
        case -1:
            if(cc & 0xff00)    /* if control, meta or prefix then scrap */
            {
#if MEOPT_MOUSE
                /* ignore mouse move events - these are sometimes generated
                 * when executing a command from osd */
                if((cc & 0x00ff) != SKEY_mouse_move)
#endif                
                    TTbell();
                break ;
            }
            
            if (cc == ' ')    /* space */
            {
                if(option & (MLCOMMAND | MLFILE | MLBUFFER | MLVARBL | MLUSER))
                    goto input_expand ;
                if(option & MLNOSPACE)
                {
                    TTbell() ;
                    break ;
                }
            }
input_addexpand:
            /* Else ordinary letter to be stored */
            
            if(option&MLUPPER && cc >= 'a' && cc <= 'z')
                cc -= 'a' - 'A';
            else if(option&MLLOWER && cc >= 'A' && cc <= 'Z')
                cc += 'a' - 'A';
            /*
             * And insert it ....
             */
            if(cins((meUByte) cc, buf, &ipos, &ilen, nbuf) == meFALSE)
                TTbell();
            changed=1 ;
            break;
            
        default:
            TTbell();
        }
    }

    /*---    Terminate the input. */
    if(ilen == 0)
    {
        if(defaultStr != NULL)
        {
            meStrncpy(buf,defaultStr,nbuf) ;
            buf[nbuf-1] = '\0' ;
        }
    }
    
    /* Store the history if it is not disabled. */
    if((option & (MLNOHIST|MLNOSTORE)) == 0)
    {
        meUByte *ss=mlgsStoreBuf ;
        
        /* If the number of history buffers is at it's maximum 
         * then swap out the last history buffer (mlgsStoreBuf is
         * deleted soon), otherwise the slot will be NULL and
         * nothing will get freed
         */
        mlgsStoreBuf = history[meHISTORY_SIZE-1] ;
        meStrcpy(ss,buf) ;
        if(numHist < meHISTORY_SIZE)
        {
            numHist++ ;
            (*numPtr)++ ;
        }
        for(ii=numHist-1 ; ii>0 ; ii--)
            history[ii] = history[ii-1] ;
        history[0] = ss ;
    }
    mlfreeList(option,noStrs,strList) ;

    return meTRUE;
}

