/* -*- c -*-
 *
 * JASSPA MicroEmacs - www.jasspa.com
 * buffer.c - Buffer operations.
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
 * Synopsis:    Buffer operations.
 * Authors:     Unknown, Jon Green & Steven Phillips
 * Description:
 *     Buffer management.
 *     Some of the functions are internal, and some are actually attached to user
 *     keys. Like everyone else, they set hints for the display system.
 */

#define __BUFFERC                       /* Define filename */

#include "emain.h"
#include "efunc.h"
#include "esearch.h"

int
getBufferName(meUByte *prompt, int opt, int defH, meUByte *buf)
{
    /* Only setup the history to a specific buffer if we're
     * told to and we're not running a macro
     */
    if(defH && (clexec != meTRUE))
    {
	register meBuffer *bp ;
	if(defH == 1)
	    bp = frameCur->bufferCur ;
	else
	{
	    bp = replacebuffer(frameCur->bufferCur) ;
	    defH = 1 ;
	}
	addHistory(MLBUFFER, bp->name) ;
    }
    return meGetString(prompt,opt|MLBUFFERCASE,defH,buf,meBUF_SIZE_MAX) ;
}

#if MEOPT_FILEHOOK
int
addFileHook(int f, int n)
{
    if(n == 0)
    {
        /* arg of 0 frees all hooks off */
        if(fileHookCount)
        {
            n = fileHookCount ;
            while(--n >= 0)
            {
                free(fileHookExt[n]) ;
                free(fileHookFunc[n]) ;
            }
            free(fileHookExt) ;
            free(fileHookFunc) ;
            free(fileHookArg) ;
            fileHookCount=0 ;
            fileHookExt=NULL ;
            fileHookFunc=NULL ;
            fileHookArg=NULL ;
        }
    }
    else
    {
        meUByte buff[meBUF_SIZE_MAX] ;
    
        if(((fileHookExt = meRealloc(fileHookExt,(fileHookCount+1)*sizeof(char *))) == NULL) ||
           ((fileHookFunc = meRealloc(fileHookFunc,(fileHookCount+1)*sizeof(char *))) == NULL) ||
           ((fileHookArg = meRealloc(fileHookArg,(fileHookCount+1)*sizeof(short))) == NULL))
        {
            fileHookCount=0 ;
            return meFALSE ;
        }
        if((meGetString((meUByte *)"File id",0,0,buff,meBUF_SIZE_MAX) != meTRUE) ||
           ((fileHookExt[fileHookCount] = meStrdup(buff)) == NULL) ||
           (meGetString((meUByte *)"Hook func",0,0,buff,meBUF_SIZE_MAX) != meTRUE) ||
           ((fileHookFunc[fileHookCount] = meStrdup(buff)) == NULL))
            return meFALSE ;
        if(f == meFALSE)
            fileHookArg[fileHookCount] = 0 ;
        else
            fileHookArg[fileHookCount] = n ;
        fileHookCount++ ;
    }
    return meTRUE ;
}

/* Check extent.
   Check the extent string for the filename extent. */
				
static int
checkExtent (meUByte *filename, int len, meUByte *sptr, meIFuncSSI cmpFunc)
{
    int  extLen ;
    char cc ;
    
    if(sptr == NULL)
	return 0 ;              /* Exit - fail */
    filename += len ;
    /*---       Run through the list, checking the extents as we go. */
    for(;;)
    {
	
	while(*sptr == ' ')
	    sptr++ ;
	extLen = 0 ;
	while(((cc=sptr[extLen]) != ' ') && ((cc=sptr[extLen]) != '\0'))
	    extLen++ ;
	if((extLen <= len) &&
	   !cmpFunc((char *) sptr,(char *) filename-extLen,extLen))
	    return 1 ;
	if(cc == '\0')
	    return 0 ;
	sptr += extLen+1 ;
    }
}

/*
 * assignHooks.
 * Given a buffer and a hook name, determine the file
 * hooks associated with the file and assign.
 *
 * Note that the hookname (hooknm) is modified, ultimatly
 * returning the 'f' prefixed name.
 */

static meUByte defaultHookName[] = "fhook-default" ;
static meUByte binaryHookName[]  = "fhook-binary" ;
static meUByte rbinHookName[]    = "fhook-rbin" ;
static meUByte triedDefault=0 ;
static meUByte triedBinary=0 ;
static meUByte triedRbin=0 ;

static void
assignHooks (meBuffer *bp, meUByte *hooknm)
{
    int fhook;                          /* File hook indentity */

    /* Get the identity of the hook */
    fhook = decode_fncname(hooknm,1);

    /* Get the file into memory if we have to */
    if((fhook == -1) &&
       ((hooknm != defaultHookName) || !triedDefault) &&
       ((hooknm != binaryHookName) || !triedBinary) &&
       ((hooknm != rbinHookName) || !triedRbin) &&
       (meStrlen(hooknm) > 6))
    {
	meUByte fn[meFILEBUF_SIZE_MAX], buff[meBUF_SIZE_MAX] ;             /* Temporary buffer */

	buff[0] = 'h' ;
	buff[1] = 'k' ;
	meStrcpy(buff+2,hooknm+6) ;
	if(!fileLookup(buff,(meUByte *)".emf",meFL_CHECKDOT|meFL_USESRCHPATH,fn))
	{
	    if(hooknm == defaultHookName)
		triedDefault++ ;
	    else if(hooknm == binaryHookName)
		triedBinary++ ;
	    else if(hooknm == rbinHookName)
		triedRbin++ ;
	    else
		mlwrite(MWABORT|MWWAIT,(meUByte *)"Failed to find file [%s]",buff);
	}
	else
	{
	    dofile(fn,0,1) ;
	    bp->fhook = decode_fncname(hooknm,1) ;
	}
    }
    bp->fhook = decode_fncname(hooknm,1) ;
    *hooknm   = 'b' ;
    bp->bhook = decode_fncname(hooknm,1) ;
    *hooknm   = 'd' ;
    bp->dhook = decode_fncname(hooknm,1) ;
    *hooknm   = 'e' ;
    bp->ehook = decode_fncname(hooknm,1) ;
    *hooknm   = 'f';
    return;
}

/*
 * findBufferContext - buffer context hook identification.
 *
 * Search for buffer context hook information this informs MicroEmacs
 * at the last knockings what buffer hook it should be using. This
 * over-rides the file extension buffer hook which is used as a
 * fall-back.
 *
 * The first line is searched for magic strings embedded in the text
 * performing a case insensitive search for a magic string as defined
 * by add-magic-hook. This is higher priority than the file extension, but
 * lower priority than than the explit mode string.
 *
 * Search for information on the first non-blank line of the form
 * "-*- name -*-" or "-*- mode: name -*-" in much the same way as
 * GNU Emacs. There are so many files in Linux with these attributes
 * we may as well use them.
 */
static void
setBufferContext(meBuffer *bp)
{
    register meBuffer *tbp ;
    meUInt flag;
    meInt vertScroll ;
    int ii ;
    
#if MEOPT_COLOR
    /* First setup the global scheme - this can be missed by buffers loaded with -c */
    bp->scheme = globScheme;
#endif
    /* must do this to do the hook properly */
    if((tbp = frameCur->bufferCur) != bp)
    {
	storeWindBSet(tbp,frameCur->windowCur) ;
        flag = frameCur->windowCur->flag ;
        vertScroll = frameCur->windowCur->vertScroll ;
        tbp->windowCount-- ;
	frameCur->bufferCur = frameCur->windowCur->buffer = bp ;
        bp->windowCount++ ;
    }
    /* must do this to do the hook properly */
    restoreWindBSet(frameCur->windowCur,bp) ;
    isWordMask = bp->isWordMask ;
    
    /* Search the buffer for alternative hook information */
    if ((bp->intFlag & BIFFILE) && !meModeTest(bp->mode,MDBINRY) &&
        !meModeTest(bp->mode,MDRBIN) && !meModeTest(bp->mode,MDDIR) &&
        ((ii = fileHookCount) > 0))
    {
	meUByte *pp, cc ;
	meLine *lp ;
	int nn ;
	
	for(lp=frameCur->windowCur->dotLine ; lp!=bp->baseLine ; lp = meLineGetNext(lp))
	{
	    for (pp = lp->text; (cc=*pp++) != '\0' ; )
		if (!(isSpace (cc)))
		    break ;
	    if(cc)
	    {
		frameCur->windowCur->dotLine = lp ;
		while(--ii >= 0)
		{
		    if((nn=fileHookArg[ii]) != 0)
		    {
			int flags = ISCANNER_MAGIC|ISCANNER_QUIET ;
			if(nn < 0)
			    nn = -nn ;
			else
			    flags |= ISCANNER_EXACT ;
			if(iscanner(fileHookExt[ii],nn,flags,NULL) == meTRUE)
			{
			    /* Must set the region flag to zero or it will
			     * be displayed
			     */
			    selhilight.flags = 0 ;
			    assignHooks(bp,fileHookFunc[ii]) ;
			    break ;
			}
		    }
		}
		restoreWindBSet(frameCur->windowCur,bp) ;
		break ;
	    }
	}
    }
    if(bp->fhook < 0)
    {
	meUByte *hooknm ;
	
	/* Do file hooks */
	if(meModeTest(bp->mode,MDBINRY))
	    hooknm = binaryHookName ;
	else if(meModeTest(bp->mode,MDRBIN))
	    hooknm = rbinHookName ;
	else
	{
	    meUByte *sp ;
	    int    ll ;
	    
	    sp = bp->name ;
	    /* Find the length of the string to pass into check_extension.
	     * Check if its name has a <?> and/or a backup ~, if so reduce
	     * the length so '<?>' & '~'s not inc.
	     */
	    ll = meStrlen(sp) ;
	    if(bp->intFlag & BIFNAME)
	    {
		while(sp[--ll] != '<')
		    ;
	    }
	    if(sp[ll-1] == '~')
		ll-- ;
            if(ll)
            {
                meUByte cc = sp[ll-1] ;
                if(cc == '~')
                {
                    ll-- ;
                    if((ll > 2) && (sp[ll-1] == '~') && (sp[ll-2] == '.'))
                        ll -= 2 ;
                }
                else if(isDigit(cc))
                {
                    int ii=ll-2 ;
                    while(ii > 0)
                    {
                        cc = sp[ii--] ;
                        if(!isDigit(cc))
                        {
                            if((cc == '~') && (sp[ii] == '.'))
                                ll = ii ;
                            break ;
                        }
                    }
                }
            }
	    ii = fileHookCount ;
	    while(--ii >= 0)
		if((fileHookArg[ii] == 0) &&
		   checkExtent(sp,ll,fileHookExt[ii],
#ifdef _INSENSE_CASE
			       strnicmp
#else
			       strncmp
#endif
			       ))
	    {
		hooknm = fileHookFunc[ii] ;
		break ;
	    }
	    if(ii < 0)
		hooknm = defaultHookName ;
	}
	assignHooks(bp,hooknm) ;
    }
    if(bp->fhook >= 0)
        execBufferFunc(bp,bp->fhook,meEBF_ARG_GIVEN,(bp->intFlag & BIFFILE)) ;
    storeWindBSet(bp,frameCur->windowCur) ;
    if(tbp != frameCur->bufferCur)
    {
        frameCur->bufferCur->windowCount-- ;
	frameCur->bufferCur = frameCur->windowCur->buffer = tbp ;
        tbp->windowCount++ ;
        frameCur->windowCur->vertScroll = vertScroll ;
        frameCur->windowCur->flag |= flag ;
	restoreWindBSet(frameCur->windowCur,tbp) ;
	isWordMask = tbp->isWordMask ;
    }    
    resetBufferWindows(bp) ;
}
#endif

int
swbuffer(meWindow *wp, meBuffer *bp)        /* make buffer BP current */
{
    register meWindow *twp ;
    register meBuffer *tbp ;
    long             lineno=0 ;
    
    tbp = wp->buffer ;
#if MEOPT_FILEHOOK
    if((wp == frameCur->windowCur) && (frameCur->bufferCur->ehook >= 0))
        execBufferFunc(frameCur->bufferCur,frameCur->bufferCur->ehook,0,1) ;
#endif
    tbp->histNo = ++bufHistNo ;
    if(--tbp->windowCount == 0)
    {
        /* Get the line number before it beomes corrupted */
        if ((meModeTest(bp->mode,MDNACT)) && (bp->intFlag & BIFFILE))
        {
            lineno = bp->dotLineNo ;
            storeWindBSet(tbp,wp) ;
            bp->dotLineNo = lineno ;
        }
        else
            storeWindBSet(tbp,wp) ;
    }
    if(meModeTest(bp->mode,MDNACT))
    {   
	/* buffer not active yet */
	if(bp->intFlag & BIFFILE)
	{
	    /* The buffer is linked to a file, load it in */
	    lineno = bp->dotLineNo ;
	    readin(bp,bp->fileName);
	}
	meModeClear(bp->mode,MDNACT) ;
#if MEOPT_FILEHOOK
	/* Now set the buffer context */
	setBufferContext(bp) ;
#endif
    }
    
    /* Switch. reset any scroll */
    wp->buffer = bp ;
    wp->horzScrollRest = 0 ;
    wp->horzScroll = 0 ;
    wp->flag |= WFMODE|WFMAIN|WFSBOX ;

    if(wp == frameCur->windowCur)
    {
        frameCur->bufferCur = bp;
#if MEOPT_EXTENDED
        if(isWordMask != frameCur->bufferCur->isWordMask)
        {
            isWordMask = frameCur->bufferCur->isWordMask ;
#if MEOPT_MAGIC
            mereRegexClassChanged() ;
#endif
        }
#endif
    }
    if(bp->windowCount++ == 0)
    {
	/* First use.           */
	restoreWindBSet(wp,bp) ;
        if(meModeTest(frameCur->bufferCur->mode,MDPIPE))
        {
            /* if its a piped command we can't rely on the bp markp being correct,
             * reset */
	    wp->markLine = NULL;                 /* Invalidate "mark"    */
	    wp->markOffset = 0;
	    wp->markLineNo = 0;
        }            
	if(lineno > 0)
	    gotoLine(meTRUE,lineno) ;
    }
    else
    {
        meFrameLoopBegin() ;
	twp = loopFrame->windowList;                            /* Look for old.        */
	while (twp != NULL)
	{
	    if (twp!=wp && twp->buffer==bp) 
	    {
		restoreWindWSet(wp,twp) ;
		break;
	    }
	    twp = twp->next;
	}
        meFrameLoopBreak(twp!=NULL) ;
        meFrameLoopEnd() ;
    }
#if MEOPT_IPIPES
    if(meModeTest(bp->mode,MDPIPE) &&
       ((wp == frameCur->windowCur) || (bp->windowCount == 1)))
	ipipeSetSize(wp,bp) ;
#endif        
#if MEOPT_FILEHOOK
    if((wp == frameCur->windowCur) && (frameCur->bufferCur->bhook >= 0))
        execBufferFunc(frameCur->bufferCur,frameCur->bufferCur->bhook,0,1) ;
#endif        
    
    if(bp->intFlag & BIFLOAD)
    {
        bp->intFlag &= ~BIFLOAD ;
	if(bufferOutOfDate(bp))
	{
            lineno = bp->dotLineNo ;
            update(meTRUE) ;
            if((mlyesno((meUByte *)"File changed on disk, reload") == meTRUE) &&
               (bclear(bp) == meTRUE))
            {
                /* achieve cheekily by calling itself as the bclear make it inactive,
                 * This ensures the re-evaluation of the hooks etc.
                 */
                /* SWP 7/7/98 - If the file did not exist to start with, i.e. new file,
                 * and now it does then we need to set the BIFFILE flag */
                bp->intFlag |= BIFFILE ;
                bp->dotLineNo = lineno+1 ;
		return swbuffer(wp,bp) ;
	    }
	}
	else
	    mlwrite(MWCLEXEC,(meUByte *)"[Old buffer]");
    }
    return meTRUE ;
}

/*
 * Attach a buffer to a window. The
 * values of dot and mark come from the buffer
 * if the use count is 0. Otherwise, they come
 * from some other window.
 */
int
findBuffer(int f, int n)
{
    register meBuffer *bp;
    register int    s;
    meUByte           bufn[meBUF_SIZE_MAX];

    if((s = getBufferName((meUByte *)"Find buffer",0,2,bufn)) != meTRUE)
	return s ;
    if(n > 0)
	f = BFND_CREAT ;
    else
	f = 0 ;
    if((bp=bfind(bufn,f)) == NULL)
	return meFALSE ;
    if(n < 0)
	return HideBuffer(bp) ;
    return swbuffer(frameCur->windowCur, bp) ;
}

#if MEOPT_EXTENDED
/*
 * Find a buffer into another window. This is an alternative to mapping
 * ^X2 (ie C-x 2) onto splitWindVert().
 *
 *                                      Tony Bamford 31/8/86 tony@root.UUCP
 */
int     
nextWndFindBuf(int f, int n)
{
    register meBuffer *bp;
    register int    s;
    meUByte           bufn[meBUF_SIZE_MAX];
    
    if((s = getBufferName((meUByte *)"Use buffer", 0, 2, bufn)) != meTRUE)
	return(s);
    if(n)
	n = BFND_CREAT ;
    if((wpopup(NULL,WPOP_MKCURR) == NULL) ||
       ((bp=bfind(bufn,n)) == NULL))
	return meFALSE ;
    return swbuffer(frameCur->windowCur, bp) ;
}
#endif

int     
nextBuffer(int f, int n)   /* switch to the next buffer in the buffer list */
{
    register meBuffer *bp, *pbp;
    
    bp = frameCur->bufferCur ;
    if(n < 0)
    {
        /* cycle backward through the buffers to find an eligable one */
        while(n < 0)
        {
            if(bp == bheadp)
                bp = NULL;
            pbp = bheadp ;
            while(pbp->next != bp)
                pbp = pbp->next;
            bp = pbp ;
            if(bp == frameCur->bufferCur)
                break ;
            if(!meModeTest(bp->mode,MDHIDE))
                n++ ;
        }
    }
    else
    {
        /* cycle forwards through the buffers to find an eligable one */
        while(n > 0)
        {
            bp = bp->next;
            if(bp == NULL)
                bp = bheadp;
            if(bp == frameCur->bufferCur)
                break ;
            if(!meModeTest(bp->mode,MDHIDE))
                n-- ;
        }
    }
    if(bp == frameCur->bufferCur)
        return ctrlg(meFALSE,1);
    return(swbuffer(frameCur->windowCur,bp));
}


/*
** Find a replacement buffer.
** given a buffer oldbuf (assumed to be displayed), then find the best
** buffer to replace it by. There are 3 main catagories
** 1) undisplayed normal buffer.
** 2) displayed normal buffer.
** 3) default buffer main. (created).
** buffers higher in the history have preference.
** returns a pointer to the replacement (guaranteed not to be the oldbuf).
*/
meBuffer *
replacebuffer(meBuffer *oldbuf)
{
    meBuffer *bp, *best=NULL, *next=NULL ;
    int     histNo=-1, nextNo=-1 ;
    
    for(bp=bheadp ; bp != NULL;bp = bp->next)
    {
	if((bp != oldbuf) && !meModeTest(bp->mode,MDHIDE))
	{
	    if((bp->windowCount == 0) && (bp->histNo > histNo))
	    {
		histNo = bp->histNo ;
		best = bp ;
	    }
	    else if(nextNo <= bp->histNo)
	    {
		nextNo = bp->histNo ;
		next = bp ;
	    }
	}
    }
    /*
    ** if didnt find a buffer, create a new *scratch* 
    */
    if(best == NULL)
	best = next ;
    if(best == NULL)
	best = bfind(BmainN,BFND_CREAT) ;
	
    return best ;
}


int
HideBuffer(meBuffer *bp)
{
    register meBuffer *bp1 ;
    meWindow          *wp ;
    
    while(bp->windowCount > 0)
    {
#if MEOPT_FRAME
        meFrame *fc=frameCur ;
        for(frameCur=frameList ; frameCur!=NULL ; frameCur=frameCur->next)
        {
#endif
            for(wp = frameCur->windowList; wp != (meWindow*) NULL; wp = wp->next)
            {
                if(wp->buffer == bp)
                {
                    /* find a replacement buffer */
                    /* if its the same then can't replace so return */
                    if((bp1 = replacebuffer(bp)) == bp)
                        /* this is true if only *scratch* is left */
                        return meFALSE ;
		    
                    if(swbuffer(wp, bp1) != meTRUE)
                        return meFALSE ;
                }
            }
#if MEOPT_FRAME
        }
        frameCur = fc ;
#endif
    }
    bp->histNo = 0 ;
    
    return meTRUE ;
}


void
resetBufferWindows(meBuffer *bp)
{
    register meWindow *wp ;
    
    meFrameLoopBegin() ;
    wp = loopFrame->windowList ;
    while(wp != NULL)
    {
	if(wp->buffer == bp)
	{
            restoreWindBSet(wp,bp) ;
	    wp->flag |= WFMOVEL|WFREDRAW|WFSBOX ;
	}
	wp = wp->next ;
    }   
    meFrameLoopEnd() ;
}
/*
 * This routine blows away all of the text
 * in a buffer. If the buffer is marked as changed
 * then we ask if it is ok to blow it away; this is
 * to save the user the grief of losing text. The
 * window chain is nearly always wrong if this gets
 * called; the caller must arrange for the updates
 * that are required. Return meTRUE if everything
 * looks good.
 */
/* Note for Dynamic file names
 * bclear does not touch the file name.
 */
int
bclear(register meBuffer *bp)
{
    meMode bmode ;
    register meAMark *mk, *nmk ;
    register meLine    *lp ;
    register int      s;
    
#if MEOPT_IPIPES
    if(meModeTest(bp->mode,MDPIPE))
    {
	meIPipe *ipipe ;
	
	if(!(bp->intFlag & BIFBLOW) &&
	   ((s=mlyesno((meUByte *)"Kill active process")) != meTRUE))
	    return s ;
	
        /* while waiting for a Yes from the user, the process may
         * have finished, so be careful */
        ipipe = ipipes ;
	while(ipipe != NULL)
        {
            if(ipipe->bp == bp)
            {
                ipipeRemove(ipipe) ;
                break ;
            }
	    ipipe = ipipe->next ;
        }
    }
#endif        
    if(bufferNeedSaving(bp) && !(bp->intFlag & BIFBLOW))
    {   /* Something changed    */
	meUByte prompt[meBUF_SIZE_MAX+20] ;

	if(bp->fileName != NULL)
	    meStrcpy(prompt,bp->fileName) ;
	else
	    meStrcpy(prompt,bp->name) ;
        meStrcat(prompt,": Discard changes") ;
	s = mlyesno(prompt) ;
	if(s != meTRUE)
	    return(s);
        autowriteremove(bp) ;
    }
    
#if MEOPT_FILEHOOK
    /* There is a problem when we kill a buffer, if it is the current
     * buffer (frameCur->windowCur) then the end hook needs to be executed before
     * the local variables are blown away. It would seem sensible that
     * it is done here. Execute the end hook and then kill off the end
     * hook to ensure that it is not executed again. On a re-read of
     * a file the file hooks are re-assigned so this should not cause
     * a problem - I hope !!.
     * 
     * Note that this problem has been introduced by local variables
     * it is not a problem if global variables are used.
     * 
     * zotbuf() calls bclear; hence any operation to delete a buffer
     * will cause the end hook to be executed.
     */
    if((frameCur->windowCur->buffer == bp) && (bp->ehook >= 0))
    {
	execBufferFunc(bp,bp->ehook,0,1) ;      /* Execute the end hook */
	bp->ehook = -1;                         /* Disable the end hook */
    }
    if(!meModeTest(bp->mode,MDNACT) && (bp->dhook >= 0))
	execBufferFunc(bp,bp->dhook,0,1) ;      /* Execute the delete hook */
#endif    
    /* Continue the destruction of the buffer space. */
    /* The following modes to preserve are: MDBINARY, MDRBIN, MDCRYPT, MDDEL */
    meModeCopy(bmode,bp->mode) ;
    meModeCopy(bp->mode,globMode) ;
    meModeSet(bp->mode,MDNACT) ;
    if(meModeTest(bmode,MDBINRY))
        meModeSet(bp->mode,MDBINRY) ;
    else if(meModeTest(bmode,MDRBIN))
        meModeSet(bp->mode,MDRBIN) ;
    if(meModeTest(bmode,MDCRYPT))
        meModeSet(bp->mode,MDCRYPT) ;
    if(meModeTest(bmode,MDDEL))
        meModeSet(bp->mode,MDDEL) ;
    bp->intFlag &= ~(BIFLOAD|BIFBLOW|BIFLOCK) ;
    lp = bp->baseLine ;
    freeLineLoop(lp,0) ;
    lp->flag  = 0 ;
    bp->dotLine  = lp ;
    bp->vertScroll = 0 ;
    bp->dotOffset  = 0;
    bp->dotLineNo = 0;
    bp->markLine = NULL;                     /* Invalidate "mark"    */
    bp->markOffset = 0;
    bp->markLineNo = 0;
    bp->lineCount = 0;
#if MEOPT_FILEHOOK
    bp->fhook = bp->dhook = bp->bhook = bp->ehook = -1 ;
#endif
#if MEOPT_UNDO
    meUndoRemove(bp) ;
#endif
    /* Clean out the local buffer variables */
    if (bp->varList.head != NULL)
    {
	meVariable *bv, *tv;
	bv = bp->varList.head ;
	do
	{
	    tv = bv;
	    bv = bv->next;
	    meNullFree(tv->value) ;
	    free(tv) ;
	} while (bv != NULL);
	bp->varList.head = NULL ;
	bp->varList.count = 0 ;
    }
    mk = bp->amarkList;
    while(mk != NULL)
    {
	nmk = mk->next ;
	meFree(mk) ;
	mk = nmk ;
    }
    bp->amarkList = NULL ;
#if MEOPT_LOCALBIND
    if(bp->bindList != NULL)
	meFree(bp->bindList) ;
    bp->bindList = NULL ;
    bp->bindCount = 0 ;
#endif
#if MEOPT_NARROW
    if(bp->narrow != NULL)
    {
        meNarrow *nw, *nnw ;
        nnw = bp->narrow ;
        do
        {
            nw = nnw ;
            nnw = nw->next ;
            /* Create the loop for freeing */
            nw->elp->next = nw->slp ;
            freeLineLoop(nw->slp,1) ;
            meFree(nw) ;
        } while(nnw != NULL) ;
        bp->narrow = NULL ;
    }
#endif
    resetBufferWindows(bp) ;
    return meTRUE ;
}


void
linkBuffer(meBuffer *bp)
{    
    register meBuffer *sb, *lb;   /* buffer to insert after */
    register meUByte  *bname=bp->name ;
    
    /* find the place in the list to insert this buffer */
    lb = NULL ;
    sb = bheadp;
    while(sb != NULL)
    {
#ifdef _INSENSE_CASE
	if(meStridif(sb->name,bname) >= 0)
#else
	if(meStrcmp(sb->name,bname) >= 0)
#endif
	    break ;
	lb = sb ;
	sb = sb->next;
    }
    /* and insert it */
    bp->next = sb ;
    if(lb == NULL)
	/* insert at the begining */
	bheadp = bp;
    else
	lb->next = bp ;
}

void
unlinkBuffer(meBuffer *bp)
{
    register meBuffer *bp1 ;
    register meBuffer *bp2 ;
    
    bp1 = NULL;                             /* Find the header.     */
    bp2 = bheadp;
    while (bp2 != bp)
    {
	bp1 = bp2;
	bp2 = bp2->next;
    }
    bp2 = bp2->next;                      /* Next one in chain.   */
    if (bp1 == NULL)                        /* Unlink it.           */
	bheadp = bp2;
    else
	bp1->next = bp2;
}

/* Note for Dynamic file names
 * zotbuf frees the file name so it must be dynamic and unique to buffer 
 *        or NULL.
 */
int      
zotbuf(register meBuffer *bp, int silent) /* kill the buffer pointed to by bp */
{
    register int     s ;
    
    /* Blow text away. last chance for user to abort so do first */
    if ((s=bclear(bp)) != meTRUE)
	return (s);
    /* if this is the scratch and the only buffer then don't delete */
    
    /*
    ** If the buffer we want to kill is displayed on the screen, make
    ** it the current window and then remove it from the screen. Once
    ** this has been done, we can then go about zapping it.
    */
    /* This must really be the frameCur->windowCur, ie frameCur->bufferCur must be bp otherwise
    ** swbuffer fucks up big time.
    */
    if(HideBuffer(bp) != meTRUE)
	/* only scratch left */
	return meTRUE ;
    
    meFree(bp->baseLine);             /* Release header line. */
    unlinkBuffer(bp) ;
    meNullFree(bp->fileName) ;
#if MEOPT_CRYPT
    meNullFree(bp->cryptKey) ;
#endif
#if MEOPT_EXTENDED
    meNullFree(bp->modeLineStr) ;
#endif
    if(!silent)
	/* say it went ok       */
	mlwrite(0,(meUByte *)"[buffer %s killed]", bp->name);
    meNullFree(bp->name) ;
    meFree(bp);                             /* Release buffer block */
    return (meTRUE);
}

/*
 * Dispose of a buffer, by name.
 * Ask for the name. Look it up (don't get too
 * upset if it isn't there at all!). Get quite upset
 * if the buffer is being displayed. Clear the buffer (ask
 * if the buffer has been changed). Then free the header
 * line and the buffer header. Bound to "C-X K".
 */
int
delBuffer(int f, int n)
{
    register meBuffer *bp;
    register int s;
    meUByte bufn[meBUF_SIZE_MAX];

    if((s = getBufferName((meUByte *)"Delete buffer", MLNOSTORE,1,bufn)) != meTRUE)
	return(s);
    
    if((bp=bfind(bufn, 0)) == NULL) 
	/* Easy if unknown.     */
	return mlwrite(MWABORT|MWCLEXEC,(meUByte *)"[%s: no such buffer]", bufn);
    if(bp->intFlag & BIFNODEL)
	return mlwrite(MWABORT|MWCLEXEC,(meUByte *)"[%s cannot be deleted]", bufn);
        
    if(!(n & 0x01))
	bp->intFlag |= BIFBLOW ;

    return zotbuf(bp,clexec) ;
}


int
delSomeBuffers(int f, int n)
{
    meBuffer *bp, *nbp ;
    meUByte   prompt[meBUF_SIZE_MAX] ;
    int     s ;
    
    bp = bheadp ;
    while(bp != NULL)
    {
	nbp = bp->next ;
	if(!meModeTest(bp->mode,MDHIDE))
	{
	    if(!(n & 0x01) ||
	       ((sprintf((char *)prompt,"Delete buffer [%s] ",bp->name)),
		((s=mlyesno(prompt)) == meTRUE)))
	    {
                if(!(n & 0x01))
                    bp->intFlag |= BIFBLOW ;
		if(zotbuf(bp,clexec) != meTRUE)
		    return meABORT ;
	    }
	    else if(s == meABORT)
		return meABORT ;
	}
	bp = nbp ;
    }
    return meTRUE ;
}

int
changeBufName(int f, int n)     /*      Rename the current buffer       */
{
    meUByte bufn[meBUF_SIZE_MAX], *nn;     /* buffer to hold buffer name */
    register int s ;
    
    /* prompt for and get the new buffer name */
    if((s = getBufferName((meUByte *)"New buffer name", 0, 0, bufn)) != meTRUE)
	return s ;

    unlinkBuffer(frameCur->bufferCur) ;
    if(n & 1)
    {
	/* check for duplicates */
	if(bfind(bufn,0) != NULL)
	{
	    /* if the names the same */
	    linkBuffer(frameCur->bufferCur) ;
	    return mlwrite(MWABORT|MWCLEXEC,(meUByte *)"[Already exists!]") ;
	}
    }
    else
	/* if bit one is clear then make it a valid name */
	frameCur->bufferCur->intFlag = (frameCur->bufferCur->intFlag & ~BIFNAME) | makename(bufn,bufn) ;
    if((nn = meMalloc(meStrlen(bufn)+1)) == NULL)
    {
	linkBuffer(frameCur->bufferCur) ;
	return meABORT ;
    }
    meStrcpy(nn,bufn);   /* copy buffer name to structure */
    meFree(frameCur->bufferCur->name) ;
    frameCur->bufferCur->name = nn ;
    addModeToWindows(WFMODE) ;
    linkBuffer(frameCur->bufferCur) ;
    
    return meTRUE ;
}


/*
 * The argument "text" points to
 * a string. Append this line to the
 * given buffer. Handcraft the EOL
 * on the end. Return meTRUE if it worked and
 * meFALSE if you ran out of room.
 * if pos is meTRUE then insert to current line
 * else to the end of the buffer
 */
int 
addLine(register meLine *ilp, meUByte *text)
{
    register meLine  *lp ;
    register int    ntext ;
    register meUByte *ss ;
    
    ntext = meStrlen(text);
    if ((lp=lalloc(ntext)) == NULL)
	return 0 ;
    
    ss = lp->text ;
    while((*ss++ = *text++))
	;
    
    ilp->prev->next = lp;
    lp->prev = ilp->prev;
    ilp->prev = lp;
    lp->next = ilp ;
    return 1 ;
}

/*
 * This routine rebuilds the
 * text in the given buffer
 * that holds the buffer list. It is called
 * by the list buffers command. Return meTRUE
 * if everything works. Return meFALSE if there
 * is an error (if there is no memory).
 */
static void
makelist(meBuffer *blistp, int verb)
{
    register meBuffer *bp ;
    register meUByte   cc, *cp1, *cp2 ;
    register int     ii, ff ;
    meUByte line[meBUF_SIZE_MAX] ;
    
    addLineToEob(blistp,(meUByte *)"AC    Size Buffer          File") ;
    addLineToEob(blistp,(meUByte *)"--    ---- ------          ----") ;
    bp = bheadp;                            /* For all buffers      */
    
    /* output the list of buffers */
    while (bp != NULL)
    {
	if(!verb && meModeTest(bp->mode,MDHIDE))
	{
	    bp = bp->next;
	    continue;
	}
	cp1 = line ;                      /* Start at left edge   */
      
	/* output status of ACTIVE flag (has the file been read in? */
	if(meModeTest(bp->mode,MDDEL))         /* "D" to be deleted    */
	    *cp1++ = 'D';
	else if(meModeTest(bp->mode,MDNACT))   /* "@" if activated     */
	    *cp1++ = ' ';
	else
	    *cp1++ = '@';
	
	/* output status of changed flag */
	if(meModeTest(bp->mode,MDSAVE))       /* "S" if to be saved   */
	    *cp1++ = 'S';
	else if(meModeTest(bp->mode,MDEDIT))  /* "*" if changed       */
	    *cp1++ = '*';
	else if(meModeTest(bp->mode,MDVIEW))  /* "%" if in view mode  */
	    *cp1++ = '%';
	else
	    *cp1++ = ' ';
	if((bp == blistp) || meModeTest(bp->mode,MDNACT))
	    cp2 = (meUByte *) "-" ;
	else
	    cp2 = meItoa(bp->lineCount+1);
	ii = 8 - meStrlen(cp2) ;
	while(ii-- > 0)
	    *cp1++ = ' ' ;
	while((cc = *cp2++) != '\0')
	    *cp1++ = cc ;
	cp2 = bp->name ;
	ii = 16 ;
	if((ff=(meStrchr(cp2,' ') != NULL)))
	    *cp1 = '"';
	else
	    *cp1 = ' ';
	while((*++cp1 = *cp2++) != 0)
	    ii-- ;
	if(ff)
	    *cp1++ = '"';
	else
	    *cp1++ = ' ';
	ii-- ;
	
	for( ; ii>0 ; ii--)
	    *cp1++ = ' ';
	
	if(verb)
	{
#if MEOPT_UNDO
	    meUndoNode *nn ;
#endif
	    meLine   *ll ;
	    int     jj ;
	    
	    ii = 0 ;
	    jj = 0 ;
	    ll = bp->baseLine ;
	    do
	    {
		ii += meLineGetLength(ll)+1 ;
		jj += ll->size ;
		ll = meLineGetNext(ll) ;
	    } while(ll != bp->baseLine) ;
	    *cp1++ = ' ';
	    cp2 = meItoa(ii) ;
	    while((cc = *cp2++) != '\0')
		*cp1++ = cc ;
	    ff = line + 45 - cp1 ;
	    while(ff-- > 0)
		*cp1++ = ' ' ;
	    jj += (sizeof(meLine)*(bp->lineCount+1)) + sizeof(meBuffer) ;
	    cp2 = meItoa(jj) ;
	    while((cc = *cp2++) != '\0')
		*cp1++ = cc ;
#if MEOPT_UNDO
	    ff = line + 53 - cp1 ;
	    while(ff-- > 0)
		*cp1++ = ' ' ;
	    ii = 0 ;
	    nn = bp->undoHead ;
	    while(nn != NULL)
	    {
		ii += sizeof(meUndoNode) ;
		if(nn->type & meUNDO_MDEL)
		    ii += meStrlen(nn->str) ;
		if(nn->type & meUNDO_REPL)
		    ii += sizeof(meUndoCoord)*nn->doto ;
		nn = nn->next ;
	    }
	    cp2 = meItoa(ii) ;
	    while((cc = *cp2++) != '\0')
		*cp1++ = cc ;
#endif
	}
	else if((bp->name[0] != '*') &&
	   ((cp2 = bp->fileName) != NULL))
	{
	    if((ii=meStrlen(cp2))+(cp1-line) > frameCur->width)
	    {
		*cp1++ = '$' ;
		cp2 += ii - (frameCur->width-(1+cp1-line)) ;
	    }
	    while((cc = *cp2++) != '\0')
		*cp1++ = cc ;
	}
	*cp1 = '\0' ;
	addLineToEob(blistp,line) ;
	bp = bp->next;
    }
    meModeSet(blistp->mode,MDVIEW) ;        /* put this buffer view mode */
}


/*---   List all  of the  active buffers.   First update  the special buffer
	that holds the list.  Next make sure at least 1 window is displaying
	the buffer  list, splitting  the screen  if this  is what  it takes.
	Lastly, repaint  all of  the windows  that are  displaying the list.
	Bound to "C-X C-B".   */
	
int
listBuffers (int f, int n)
{
    register meWindow    *wp ;
    register meBuffer    *bp ;
    
    if((wp=wpopup(BbuffersN,(BFND_CREAT|BFND_CLEAR|WPOP_USESTR))) == NULL)
	return mlwrite(MWABORT,(meUByte *)"[Failed to create list]") ;
    bp = wp->buffer ;
    makelist(bp,f) ;
    
    bp->dotLine = meLineGetNext(bp->baseLine);
    bp->dotOffset = 0 ;
    bp->dotLineNo = 0 ;
    resetBufferWindows(bp) ;
    return meTRUE ;
}

/*
** Returns :-
** meTRUE  if the given buffer needs saving
** meFALSE if it doesnt
 */
int
bufferNeedSaving(meBuffer *bp)
{
    /* If changed (edited) and valid buffer name (not system) */
    if(meModeTest(bp->mode,MDEDIT) && (bp->name[0] != '*'))
	return meTRUE ;
    return meFALSE ;
}

/*
 * Look through the list of
 * buffers. Return meTRUE if there
 * are any changed buffers. Buffers
 * that hold magic internal stuff are
 * not considered; who cares if the
 * list of buffer names is hacked.
 * Return meFALSE if no buffers
 * have been changed.
 */
int
anyChangedBuffer(void)
{
    register meBuffer *bp;
    
    bp = bheadp;
    while (bp != NULL)
    {
	if(bufferNeedSaving(bp))
	    return (meTRUE);
	bp = bp->next;
    }
    return (meFALSE);
}

meBuffer *
createBuffer(register meUByte *bname)
{
    register meBuffer *bp;
    register meLine   *lp;

    if((bp=(meBuffer *)meMalloc(sizeof(meBuffer))) == NULL)
	return NULL ;
    if((lp=lalloc(0)) == NULL)
    {
	meFree(bp);
	return NULL ;
    }
    /* set everything to 0 */
    memset(bp,0,sizeof(meBuffer)) ;
    if((bp->name = meMalloc(meStrlen(bname)+1)) == NULL)
	return NULL ;
    meStrcpy(bp->name, bname);
    linkBuffer(bp) ;
    
    /* and set up the non-zero buffer fields */
    meModeCopy(bp->mode,globMode) ;
    meModeSet(bp->mode,MDNACT) ;
    meFiletimeInit(bp->stats.stmtime) ;
    bp->dotLine = bp->baseLine = lp;
    bp->autotime = -1 ;
    bp->stats.stmode  = meUmask ;
    bp->stats.stsize  = 0 ;
#ifdef _UNIX
    bp->stats.stuid = meUid ;
    bp->stats.stgid = meGid ;
    bp->stats.stdev = -1 ;
    bp->stats.stino = -1 ;
#endif
#if MEOPT_IPIPES
    bp->ipipeFunc = -1 ;
#endif
    bp->inputFunc = -1 ;
#if MEOPT_EXTENDED
    bp->isWordMask = CHRMSK_DEFWORDMSK ;
#endif
#if MEOPT_FILEHOOK
    bp->fhook = -1 ;
    bp->bhook = -1 ;
    bp->dhook = -1 ;
    bp->ehook = -1 ;
#endif
#if MEOPT_COLOR
    /* set the colors of the new window */
    bp->scheme = globScheme;
#endif
    lp->next = lp;
    lp->prev = lp;
    
    return bp ;
}


/*
 * Find a buffer, by name. Return a pointer
 * to the meBuffer structure associated with it.
 * If the buffer is not found
 * and the "cflag" is meTRUE, create it. The "bflag" is
 * the settings for the flags in in buffer.
 */
meBuffer *
bfind(register meUByte *bname, int cflag)
{
    meBuffer *bp;
    meMode  sglobMode ;
    meUByte intFlag=0 ;
    meUByte *bnm ;
    meUByte bn[meBUF_SIZE_MAX] ;
    int ii ;
    
    if(cflag & BFND_MKNAM)
    {
	intFlag = makename(bn,bname) ;
	bnm = bn ;
    }
    else
    {
	bp = bheadp;
	while(bp != NULL)
	{
#ifdef _INSENSE_CASE
	    if((ii=meStricmp(bp->name,bname)) == 0)
#else
	    if((ii=meStrcmp(bp->name,bname)) == 0)
#endif
	    {
		if(cflag & BFND_CLEAR)
		{
		    bp->intFlag |= BIFBLOW ;             /* Don't complain!      */
                    bclear(bp) ;
                    /* The BFND_CLEAR functionality is only used for system buffers
                     * such as *help* *command* etc. i.e. not user loaded buffer. 
                     * These buffers do not and must not be linked to a filename.
                     * This can happen if the user renames "./foo" to *compile* say
                     * and then runs another compile, it its still linked to foo then
                     * it gets loaded */
                    bp->intFlag &= ~BIFFILE ;
                    if(bp->fileName != NULL)
                    {
                        meFree(bp->fileName) ;
                        bp->fileName = NULL ;
                    }
		    /* set the modes correctly */
		    if(cflag & BFND_BINARY)
			meModeSet(bp->mode,MDBINRY) ;
		    else if(cflag & BFND_RBIN)
			meModeSet(bp->mode,MDRBIN) ;
		    if(cflag & BFND_CRYPT)
			meModeSet(bp->mode,MDCRYPT) ;
		}
		return bp ;
	    }
	    if(ii>0)
		break ;
	    bp = bp->next;
	}
	if(!cflag)
	    return NULL ;
	bnm = bname ;
    }
    meModeCopy(sglobMode,globMode) ;
    if(cflag & BFND_BINARY)
	meModeSet(globMode,MDBINRY) ;
    else if(cflag & BFND_RBIN)
	meModeSet(globMode,MDRBIN) ;
    if(cflag & BFND_CRYPT)
	meModeSet(globMode,MDCRYPT) ;
    bp = createBuffer(bnm) ;
    bp->intFlag |= intFlag ;
    meModeCopy(globMode,sglobMode) ;
    return bp ;
}



int
adjustMode(meBuffer *bp, int nn)  /* change the editor mode status */
{
    register meUByte *mode ;
    int   func ;
    meUByte prompt[50];            /* string to prompt user with */
    meUByte cbuf[mePATBUF_SIZE_MAX];            /* buffer to recieve mode name into */
    
    if(nn >= 128)
    {
	nn -= 128 ;
	func = 0 ;
    }
    else if(nn < 0)
    {
	nn = 0 - nn ;
	func = -1 ;
    }
    else if(nn == 0)
	func = 0 ;
    else
	func = 1 ;
    
    if (bp == NULL)
	mode = globMode ;
    else
	mode = bp->mode ;
    
    if(nn < 2)
    {
	/* build the proper prompt string */
	meStrcpy(prompt,"Buffer mode to ");
	if(bp == NULL)
	    meStrncpy(prompt,"Global",6);
	
	if(func == 0)
	    meStrcpy(prompt+15, "toggle");
	else if(func > 0)
	    meStrcpy(prompt+15, "add");
	else
            meStrcpy(prompt+15, "delete");
	
	/* Setup the completion */
	mlgsStrList = modeName ;
	mlgsStrListSize = MDNUMMODES ;
	
	/* prompt the user and get an answer */
	if(meGetString(prompt, MLLOWER|MLUSER, 0, cbuf, mePATBUF_SIZE_MAX) == meABORT)
	    return(meFALSE);
    
	/* test it against the modes we know */
	for (nn=0; nn < MDNUMMODES ; nn++) 
	    if(meStrcmp(cbuf,modeName[nn]) == 0) 
		break ;
    }
    else
	nn -= 2 ;
    
    if(nn >= MDNUMMODES)
	return mlwrite(MWABORT,(meUByte *)"[No such mode]");
    
    switch(nn)
    {
    case MDEDIT:
	if(bp == NULL)
	    goto invalid_global ;
		
	if(meModeTest(bp->mode,MDEDIT))
	{
	    if(func <= 0)
	    {
		autowriteremove(bp) ;
#if MEOPT_UNDO
		meUndoRemove(bp) ;
#endif
		meModeClear(bp->mode,MDEDIT) ;
		addModeToWindows(WFMODE) ;
	    }
	}
	else if(func >= 0)
	{
	    if(bp != frameCur->bufferCur)
	    {
		register meBuffer *tbp = frameCur->bufferCur ;
		
		storeWindBSet(tbp,frameCur->windowCur) ;
		swbuffer(frameCur->windowCur,bp) ;
		
		nn = bchange() ;
		
		swbuffer(frameCur->windowCur,tbp) ;
		restoreWindBSet(frameCur->windowCur,tbp) ;
	    }
	    else
		nn = bchange() ;
            
            if(nn != meTRUE)
                return nn ;
	}
        if((func == 0) && !clexec)
            mlwrite(0,(meUByte *)"[%s edit mode is now %s]",
                    (bp == NULL) ? "Global":"Buffer",
                    meModeTest(mode,MDEDIT) ? "on":"off");
	return meTRUE ;
	
    case MDQUIET:
	/* quiet mode - only set globaly */
	if(bp != NULL)
	    goto invalid_global ;
	break ;
	
    case MDLOCK:
	if((bp == NULL) || !meModeTest(bp->mode,MDPIPE)) 
	    goto invalid_global ;
	break ;
	    
    case MDHIDE:
    case MDDEL:
    case MDSAVE:
	if(bp == NULL)
	    goto invalid_global ;
	if((nn == MDSAVE) && !bufferNeedSaving(bp))
	    return mlwrite(0,(meUByte *)"[No changes made]") ;
	break ;
	
    case MDDIR:
    case MDNACT:
    case MDNRRW:
    case MDPIPE:
invalid_global:
	return mlwrite(MWABORT,(meUByte *)"[Cannot change this mode]");
    }
    /* make the mode change */
    if(func == 0)
    {
        meModeToggle(mode,nn) ;
        if(!clexec)
            mlwrite(0,(meUByte *)"[%s %s mode is now %s]",
                    (bp == NULL) ? "Global":"Buffer",modeName[nn],
                    meModeTest(mode,nn) ? "on":"off");
    }
    else if(func > 0)
	meModeSet(mode,nn) ;
    else
	meModeClear(mode,nn) ;
    
    if(meModeTest(mode,nn))
    {
	/*
	 * make sure that, if INDENT is set, CMODE isnt
	 * and vice versa and same for BNIARY and RBIN
	 */
	if(nn == MDINDEN)
	    meModeClear(mode,MDCMOD) ;
	else if(nn == MDCMOD)
	    meModeClear(mode,MDINDEN) ;
	else if(nn == MDBINRY)
	    meModeClear(mode,MDRBIN) ;
	else if(nn == MDRBIN)
	    meModeClear(mode,MDBINRY) ;
    }
    else if(bp != NULL)
    {
#if MEOPT_UNDO
	if(nn == MDUNDO)
	    meUndoRemove(bp) ;
	else
#endif
            if(nn == MDATSV)
	    bp->autotime = -1 ;
    }
    /* display new mode line */
    if(bp != NULL)
        /* and update all buffer window mode lines */
        addModeToBufferWindows(bp,WFMODE) ;
    
    return meTRUE ;
}
   
int     
bufferMode(int f, int n)        /* prompt and set an editor mode */
{
    return adjustMode(frameCur->bufferCur,(f) ? n:0) ;
}

int     
globalMode(int f, int n)        /* prompt and set a global editor mode */
{
    return adjustMode(NULL,(f) ? n:0);
}

int     
namedBufferMode(int f, int n)   /* prompt and set an editor mode */
{
    register meBuffer *bp;
    register int s;
    meUByte bufn[meBUF_SIZE_MAX];
    
    if((s = getBufferName((meUByte *)"Buffer name", MLNOSTORE,1,bufn)) != meTRUE)
	return s ;
    
    if((bp=bfind(bufn, 0)) == NULL) 
	/* Easy if unknown.     */
	return mlwrite(MWABORT|MWCLEXEC,(meUByte *)"[No such buffer %s]", bufn);
    return adjustMode(bp,(f) ? n:0) ;
}

