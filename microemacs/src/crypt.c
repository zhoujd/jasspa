/*****************************************************************************
*
*	Title:		%M%
*
*	Synopsis:	Encryption Routines
*
******************************************************************************
*
*	Filename:		%P%
*
*	Author:			Dana Hoggatt and Daniel Lawrence
*
*	Creation Date:		14/05/86 12:37		<000907.1403>
*
*	Modification date:	%G% : %U%
*
*	Current rev:		%I%
*
*	Special Comments:	
*  
*       The CRYPT encoding for MicroEMACS has been modified in V3.12 this 
*       encoding has been applied to the 3.8 source files for compatibility.
*       
*       Jon Green 24/03/97.
* 
*       SWP 8/97 - Changed the CRYPT algorithm, found the current one
*       was far from secure and the non-encryption of white spaces leads
*       to easy decoding.
* 
*	Contents Description:	
*
*****************************************************************************
* 
* (C)opyright 1987 by Daniel M. Lawrence
* MicroEMACS 3.8 can be copied and distributed freely for any
* non-commercial purposes. MicroEMACS 3.8 can only be incorporated
* into commercial software with the permission of the current author.
* 
* Modifications to the original file by Jasspa. 
* 
* Copyright (C) 1988 - 1999, JASSPA 
* The MicroEmacs Jasspa distribution can be copied and distributed freely for
* any non-commercial purposes. The MicroEmacs Jasspa Distribution can only be
* incorporated into commercial software with the expressed permission of
* JASSPA.
* 
****************************************************************************/

/*---	Include defintions */

#define	__CRYPTC		/* Define file */

/*---	Include files */

#include "emain.h"

#if	CRYPT

/*---	Local macro definitions */

#define USE_OLD_CRYPT 0                 /* 0 = me3.8; 1 = me3.12 */

/* reset encryption key of given buffer */
int
setBufferCryptKey(BUFFER *bp, uint8 *key)
{
    uint8 keybuf[NPAT]; 	/* new encryption string */
	
    if(key == NULL)
    {
	/* get the string to use as an encrytion string */
        if(mlreply((uint8 *)"Encryption String", MLNOHIST, 0,keybuf, NPAT-1) != TRUE)
            return FALSE ;
        key = keybuf ;
        mlerase(MWCLEXEC);		/* clear it off the bottom line */
    }
    meNullFree(bp->b_key) ;
    bp->b_key = NULL ;
    if((key[0] == 0) ||
       ((bp->b_key = meStrdup(key)) == NULL))
        meModeClear(bp->b_mode,MDCRYPT) ;
    else
    {
        /* and encrypt it */
        meModeSet(bp->b_mode,MDCRYPT) ;
        meCrypt(NULL, 0);
        meCrypt(bp->b_key, meStrlen(key));
    }
    addModeToBufferWindows(bp,WFMODE) ;
    return TRUE ;
}

int
setCryptKey(int f, int n)	/* reset encryption key of current buffer */
{
    return setBufferCryptKey(curbp,NULL) ;
}

#if USE_OLD_CRYPT            
static int 
mod95(register int val)
{
    /* The mathematical MOD does not match the computer MOD  */
    
    /* Yes, what I do here may look strange, but it gets the
       job done, and portably at that.  */

    while (val >= 9500)
        val -= 9500;
    while (val >= 950)
        val -= 950;
    while (val >= 95)
        val -= 95;
    while (val < 0)
        val += 95;
    return (val);
}
#endif

/*************************************************************************
 *
 *	crypt - in place encryption/decryption of a buffer
 *
 *	(C) Copyright 1986, Dana L. Hoggatt
 *	1216, Beck Lane, Lafayette, IN
 *
 *	When consulting directly with the author of this routine, 
 *	please refer to this routine as the "DLH-POLY-86-B CIPHER".  
 *
 *	This routine was written for Dan Lawrence, for use in V3.8 of
 *	MICRO-emacs, a public domain text/program editor.  
 *
 *	I kept the following goals in mind when preparing this function:
 *
 *	    1.	All printable characters were to be encrypted back
 *		into the printable range, control characters and
 *		high-bit characters were to remain unaffected.	this
 *		way, encrypted would still be just as cheap to 
 *		transmit down a 7-bit data path as they were before.
 *
 *	    2.	The encryption had to be portable.  The encrypted 
 *		file from one computer should be able to be decrypted 
 *		on another computer.
 *
 *	    3.	The encryption had to be inexpensive, both in terms
 *		of speed and space.
 *
 *	    4.	The system needed to be secure against all but the
 *		most determined of attackers.
 *
 *	For encryption of a block of data, one calls crypt passing 
 *	a pointer to the data block and its length. The data block is 
 *	encrypted in place, that is, the encrypted output overwrites 
 *	the input.  Decryption is totally isomorphic, and is performed 
 *	in the same manner by the same routine.  
 *
 *	Before using this routine for encrypting data, you are expected 
 *	to specify an encryption key.  This key is an arbitrary string,
 *	to be supplied by the user.  To set the key takes two calls to 
 *	crypt().  First, you call 
 *
 *		crypt(NULL, vector)
 *
 *	This resets all internal control information.  Typically (and 
 *	specifically in the case on MICRO-emacs) you would use a "vector" 
 *	of 0.  Other values can be used to customize your editor to be 
 *	"incompatable" with the normally distributed version.  For 
 *	this purpose, the best results will be obtained by avoiding
 *	multiples of 95.
 *
 *	Then, you "encrypt" your password by calling 
 *
 *		crypt(pass, strlen(pass))
 *
 *	where "pass" is your password string.  Crypt() will destroy 
 *	the original copy of the password (it becomes encrypted), 
 *	which is good.	You do not want someone on a multiuser system 
 *	to peruse your memory space and bump into your password.  
 *	Still, it is a better idea to erase the password buffer to 
 *	defeat memory perusal by a more technical snooper.  
 *
 *	For the interest of cryptologists, at the heart of this 
 *	function is a Beaufort Cipher.	The cipher alphabet is the 
 *	range of printable characters (' ' to '~'), all "control" 
 *	and "high-bit" characters are left unaltered.
 *
 *	The key is a variant autokey, derived from a wieghted sum 
 *	of all the previous clear text and cipher text.  A counter 
 *	is used as salt to obiterate any simple cyclic behavior 
 *	from the clear text, and key feedback is used to assure 
 *	that the entire message is based on the original key, 
 *	preventing attacks on the last part of the message as if 
 *	it were a pure autokey system.
 *
 *	Overall security of encrypted data depends upon three 
 *	factors:  the fundamental cryptographic system must be 
 *	difficult to compromise; exhaustive searching of the key 
 *	space must be computationally expensive; keys and plaintext 
 *	must remain out of sight.  This system satisfies this set
 *	of conditions to within the degree desired for MicroEMACS.
 *
 *	Though direct methods of attack (against systems such as 
 *	this) do exist, they are not well known and will consume 
 *	considerable amounts of computing time.  An exhaustive
 *	search requires over a billion investigations, on average.
 *
 *	The choice, entry, storage, manipulation, alteration, 
 *	protection and security of the keys themselves are the 
 *	responsiblity of the user.  
 *
 *************************************************************************/

/* bptr - buffer of characters to be encrypted */
/* len	- number of characters in the buffer */
int
meCrypt(register uint8 *bptr, register uint32 len)
{
    register int cc;	/* current character being considered */
    
    static int32 key = 0;	/* 29 bit encipherment key */
    static int salt = 0;	/* salt to spice up key with */
    
    if (!bptr)
    {	/* is there anything here to encrypt? */
        key = len;	/* set the new key */
        salt = len;	/* set the new salt */
        return(TRUE);
    }
    while (len--)
    { 	/* for every character in the buffer */
        
        cc = *bptr;	/* get a character out of the buffer */
#if 0
        /* only encipher printable characters */
        if ((cc >= ' ') && (cc <= '~'))
        {
            
#if USE_OLD_CRYPT            
            /**  If the upper bit (bit 29) is set, feed it back into the key.  This 
               assures us that the starting key affects the entire message.  **/
            
            key &= 0x1FFFFFFFL;	/* strip off overflow */
            if (key & 0x10000000L)
                key ^= 0x0040A001L;	/* feedback */
            
            /**  Down-bias the character, perform a Beaufort encipherment, and 
               up-bias the character again.  We want key to be positive 
               so that the left shift here will be more portable and the 
               mod95() faster	 **/
            
            cc = mod95((int)(key % 95) - (cc - ' ')) + ' ';
            
            /**  the salt will spice up the key a little bit, helping to obscure 
               any patterns in the clear text, particularly when all the 
               characters (or long sequences of them) are the same.  We do 
               not want the salt to go negative, or it will affect the key 
               too radically.	It is always a good idea to chop off cyclics 
               to prime values.  **/
            
            if (++salt >= 20857)
            	/* prime modulus */
                salt = 0;
            
            /**  our autokey (a special case of the running key) is being 
               generated by a wieghted checksum of clear text, cipher 
               text, and salt.   **/

            key = key + key + cc + *bptr + salt;
#else
            /*	We feed the upper few bits of the key back into itself. This
             * assures us that the starting key affects the entire message.
             * We go through some effort here to prevent the key from
             * exceeding 29 bits.  This is so the aritmetic caluclation in a
             * later statement, which impliments our autokey, won't overflow,
             * making the key go negative. Machine behavior in these cases
             * does not tend to be portable. */

            key = (key & 0x1FFFFFFFl) ^ ((key >> 29) & 0x3l);

            /* Down-bias the character, perform a Beaufort encipherment, and 
             * up-bias the character again.  We want key to be positive 
             * so that the left shift here will be more portable and the 
             * mod95() faster */

            cc = mod95((int)(key % 95) - (cc - ' ')) + ' ';

            /* The salt will spice up the key a little bit, helping to obscure 
             * any patterns in the clear text, particularly when all the 
             * characters (or long sequences of them) are the same.  We do 
             * not want the salt to go negative, or it will affect the key 
             * too radically.  It is always a good idea to chop off cyclics 
             * to prime values. */

            if (++salt >= 20857) {	/* prime modulus */
                salt = 0;
            }

            /* Our autokey (a special case of the running key) is being 
             * generated by a weighted checksum of clear text, cipher 
             * text, and salt.
             *
             * I had to allow the older broken key calculation to let us
             * decrypt old files.  I hope to take this out eventually. */

            key = key + key + (cc ^ *bptr) + salt;
#endif
        }
#else
        /*	We feed the upper few bits of the key back into itself. This
         * assures us that the starting key affects the entire message.
         * We go through some effort here to prevent the key from
         * exceeding 29 bits.  This is so the aritmetic caluclation in a
         * later statement, which impliments our autokey, won't overflow,
         * making the key go negative. Machine behavior in these cases
         * does not tend to be portable. */
        
        key = (key & 0x1FFFFFFFl) ^ ((key >> 29) & 0x3l);

        /* Down-bias the character, perform a Beaufort encipherment, and 
         * up-bias the character again.  We want key to be positive 
         * so that the left shift here will be more portable and the 
         * mod255() faster */

        cc = (((key & 0xff) - cc) & 0xff) ;

        /* The salt will spice up the key a little bit, helping to obscure 
         * any patterns in the clear text, particularly when all the 
         * characters (or long sequences of them) are the same.  We do 
         * not want the salt to go negative, or it will affect the key 
         * too radically.  It is always a good idea to chop off cyclics 
         * to prime values. */
        
        if (++salt >= 20857) {	/* prime modulus */
            salt = 0;
        }

        /* Our autokey (a special case of the running key) is being 
         * generated by a weighted checksum of clear text, cipher 
         * text, and salt.
         *
         * I had to allow the older broken key calculation to let us
         * decrypt old files.  I hope to take this out eventually. */

        key = key + key + (cc ^ *bptr) + salt;
#endif
        *bptr++ = cc;	/* put character back into buffer */
    }
    return(TRUE);
}
#endif
