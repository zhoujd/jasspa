/* -*- c -*-
 *
 * JASSPA MicroEmacs - www.jasspa.com
 * fileio.c - File reading and writing routines.
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
 * Synopsis:    File reading and writing routines.
 * Authors:     Unknown, Steven Phillips
 */

#define	__FILEIOC			/* Define program name */

#include "emain.h"

#if (defined _UNIX) || (defined _DOS) || (defined _WIN32)
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#ifdef _WIN32
#define ffpInvalidVal INVALID_HANDLE_VALUE
#else
#define ffpInvalidVal NULL
#endif

/*
 * char-mask lookup tables
 * 
 * Table 1:
 * 
 * Bits   Function        Use
 * 
 * 0x0f    getType        Used by macro language to determine the arg type
 * 0x10    isDisplayable  Used in display to determine if a character can be directly displayed
 * 0x20    isPokable      Used by screen-poke to determine if a character can be directly poked
 * 0x40    isPrint        Used by print to determine if a character can be directly printed
 * 0x80    isSpace        Is a white char
 * 
 * 
 * Table 2:
 * 
 * Bits   Function        Use
 * 
 * 0x01    isLower        Is Lower case test
 * 0x02    isUpper        Is Upper case test
 * 0x03    isAlpha        Is Alpha test (Lower | Upper)
 * 0x04    isXDigit       Is Hex digit char (0-9 | a-f | A-F)
 * 0x07    isAlphaNum     Is alpha numeric char (Upper|Lower|HexDigit)
 * 0x08    isSpllExt      Is Spell Extended character (e.g. '.', '-' etc)
 * 0x0f    isSpllWord     Is Spell word
 * 0xf0    isWord         Is a word letter - selected by $buffer-mask
 * 
 */

meUByte charMaskTbl1[256] =
{
#if (defined _DOS) || (defined _WIN32)
    0x80, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x2A, 0x2A, 
    0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 
#else
    0x80, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x8A, 0x8A, 0x8A, 0x8A, 0x8A, 0x0A, 0x0A, 
    0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 
#endif
    0xFA, 0x76, 0x79, 0x72, 0x74, 0x73, 0x75, 0x7A, 0x7A, 0x7A, 0x77, 0x7A, 0x7A, 0x78, 0x7C, 0x7A, 
    0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x7B, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 
    0x71, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 
    0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 
    0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 
#if (defined _DOS) || (defined _WIN32)
    0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x3A, 
    0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 
    0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 0x3A, 
    0x7A,
#else
    0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x0A, 
    0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 
    0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
    0x0A,
#endif
          0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 
    0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 
    0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 
    0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 
    0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 
    0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A, 0x7A
};

meUByte charMaskTbl2[256] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x00, 
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x10, 
    0x00, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

meUByte charCaseTbl[256] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x40, 'a',  'b',  'c',  'd',  'e',  'f',  'g',  'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',  
    'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  'x',  'y',  'z',  0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    0x60, 'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',  
    'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z',  0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};   
meUByte charUserLatinTbl[256] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
}; 
meUByte charLatinUserTbl[256] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
    0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
    0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
    0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
    0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
    0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
    0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
    0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
}; 
meUByte charMaskFlags[]="luhs1234dpPwaAMLU" ;
meUByte isWordMask=CHRMSK_DEFWORDMSK;


static int       ffremain ;
       meUByte  *ffbuf=NULL ;
static meUByte  *ffcur ;
static meUByte   ffcrypt=0 ;
static meUByte   ffauto=0 ;
static meUByte   ffautoRet=0 ;
static meUByte   ffnewFile ;
static int       ffbinary=0 ;
static int       ffread ;
static int       ffoffset ;

#define meBINARY_BPL   16   /* bytes per line */
#define meRBIN_BPL    256   /* bytes per line */
#define AUTO_CRLF    0x01
#define AUTO_CTRLZ   0x02

#if MEOPT_SOCKET

#include <stdarg.h>

#ifdef _WIN32
/* winsock2.h must be included before */
#if (_MSC_VER != 900)
#include <winsock2.h>
#else
#include <winsock.h>
#endif
#include <io.h>
typedef void (*meATEXIT)(void) ;
#define meSOCKET SOCKET
#define meBadSocket          INVALID_SOCKET
#define meSocketIsBad(ss)    ((ss) == meBadSocket)

#define meCloseSocket(ss)    (closesocket(ss),ss=meBadSocket)
#else
#include <sys/time.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netinet/in.h>
#define meSOCKET int
#define meBadSocket -1
#define meSocketIsBad(ss)    ((ss) < 0)
#define meCloseSocket(ss)    (close(ss),ss=meBadSocket)
#endif

#define meOpenSocket         socket

#define meSOCKET_TIMEOUT 240000

static meSOCKET ffccsk=meBadSocket ;
static meSOCKET ffsock=meBadSocket ;
static meUByte *ffsockAddr=NULL ;
static meRegNode *ffpasswdReg=NULL ;

/* Define the URL file types */
#define meURLTYPE_FILE 0
#define meURLTYPE_HTTP 1
#define meURLTYPE_FTP  2
#define meURLTYPE_DIR  4
static int fftype=meURLTYPE_FILE ;

static struct sockaddr_in meSockAddr ;
static int ffsize ;
static int ffstartTime ;
#define ffURL_CONSOLE        0x01
#define ffURL_SHOW_CONSOLE   0x02
#define ffURL_SHOW_PROGRESS  0x04
static int ffurlFlags=ffURL_CONSOLE ;
static meBuffer *ffurlBp=NULL ;
static meBuffer *ffurlReadBp ;
static meUByte *ffurlFlagsVName[2]={(meUByte *)"http-flags",(meUByte *)"ftp-flags"} ;
static meUByte *ffurlConsoleBName[2]={(meUByte *)"*http-console*",(meUByte *)"*ftp-console*"} ;

#ifndef INADDR_NONE
/* This may not be defined, particularly suns */
#define INADDR_NONE -1
#endif

#define ftpERROR         0
#define ftpPOS_PRELIMIN  1
#define ftpPOS_COMPLETE  2
#define ftpPOS_INTERMED  3

static void
strBase64Encode3(meUByte *dd, meUByte c1, meUByte c2, meUByte c3)
{
    static meUByte base64Table[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" ;
    dd[0] = base64Table[(c1 >> 2)] ;
    dd[1] = base64Table[((c1 << 4) & 0x30) | ((c2 >> 4) & 0x0f)] ;
    dd[2] = base64Table[((c2 << 2) & 0x3c) | ((c3 >> 6) & 0x03)] ;
    dd[3] = base64Table[(c3 & 0x3f)] ;
}

static meUByte *
strBase64Encode(meUByte *dd, meUByte *ss)
{
    meUByte c1, c2, c3 ;
    
    while((c1=*ss++) != '\0')
    {
        if((c2=*ss++) == '\0')
        {
            c3 = '\0' ;
            strBase64Encode3(dd,c1,c2,c3) ;
            dd += 2 ;
            *dd++ = '=' ;
            *dd++ = '=' ;
            break ;
        }
        else if((c3=*ss++) == '\0')
        {
            strBase64Encode3(dd,c1,c2,c3) ;
            dd += 3 ;
            *dd++ = '=' ;
            break ;
        }
        strBase64Encode3(dd,c1,c2,c3) ;
        dd += 4 ;
    }
    *dd = '\0' ;
    return dd ;
}

/* `hostname' can be of any form described in inet_addr(3);
 * `port' can be a service name, a port number, or NULL to have the
 * OS choose a port (this is probably only useful for servers);
 * `proto' can be "udp" or "tcp"
 */
static int
make_inet_addr(meUByte *hostname, meUByte *port, meUByte *proto)
{
    struct hostent *hp ;
    struct servent *sp ;
    
    memset(&meSockAddr,0,sizeof(meSockAddr)) ;
    meSockAddr.sin_family = AF_INET ;
    
    if ((sp = getservbyname((char *)port,(char *)proto)) != NULL)
    {
        meSockAddr.sin_port = sp->s_port;
    }
    else
    {
        meUShort usport=atoi((char *)port) ;
        if ((meSockAddr.sin_port = htons(usport)) == 0)
            return mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Bad port number %s]", port) ;
    }
    /* First resolve the hostname, then resolve the port */
    if ((meSockAddr.sin_addr.s_addr = inet_addr((char *)hostname)) != INADDR_NONE)
    {
        /* in_addr.s_addr is already set */
    }
    else if(((hp = gethostbyname((char *)hostname)) != NULL) &&
            (hp->h_length <= ((int) sizeof(struct in_addr))) &&
            (hp->h_addrtype == AF_INET))
        memcpy(&(meSockAddr.sin_addr),hp->h_addr,hp->h_length) ;
    else
        return mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Unknown host %s]", hostname) ;
    
    return meTRUE ;
}

static void
ffurlConsoleAddText(meUByte *str, int flags)
{
    if(flags & 0x01)
    {
        ffurlBp->dotLine = meLineGetPrev(ffurlBp->baseLine) ;
        ffurlBp->dotOffset = meLineGetLength(ffurlBp->dotLine) ;
        ffurlBp->dotLineNo = ffurlBp->lineCount - 1 ;
    }
    else
    {
        ffurlBp->dotLine = ffurlBp->baseLine ;
        ffurlBp->dotOffset = 0 ;
        ffurlBp->dotLineNo = ffurlBp->lineCount ;
    }
    meBufferStoreLocation(ffurlBp->dotLine,ffurlBp->dotOffset,ffurlBp->dotLineNo) ;
    if(flags & 0x01)
    {
        meLine *olp, *nlp ;
        meUByte buff[meBUF_SIZE_MAX], cc ;
        int ii, ll ;
        
        olp = ffurlBp->dotLine ;
        ll = ii = ffurlBp->dotOffset ;
        meStrncpy(buff,meLineGetText(olp),ii) ;
        while((cc=*str++) != '\0')
            buff[ii++] = cc ;
        meStrcpy(buff+ii,meLineGetText(olp)+ll) ;
        addLine(olp,buff) ;
        nlp = meLineGetPrev(olp) ;
        nlp->next = olp->next ;
        olp->next->prev = nlp ;
        if(olp->flag & meLINE_ANCHOR)
            meLineResetAnchors(meLINEANCHOR_ALWAYS|meLINEANCHOR_RETAIN,ffurlBp,
                              olp,nlp,0,0) ;
        meFree(olp) ;
    }
    else
        addLineToEob(ffurlBp,str) ;
    if(flags & 0x02)
    {
        ffurlBp->dotLine = meLineGetPrev(ffurlBp->baseLine) ;
        ffurlBp->dotOffset = meLineGetLength(ffurlBp->dotLine) ;
        ffurlBp->dotLineNo = ffurlBp->lineCount - 1 ;
    }
    else
    {
        ffurlBp->dotLine = ffurlBp->baseLine ;
        ffurlBp->dotOffset = 0 ;
        ffurlBp->dotLineNo = ffurlBp->lineCount ;
    }
    meBufferUpdateLocation(ffurlBp,(flags & 0x01) ? 0:1,ffurlBp->dotOffset) ;
    
    if(!(flags & 0x04) && (ffurlFlags & ffURL_SHOW_CONSOLE))
        screenUpdate(meTRUE,2-sgarbf) ;
}

static meSOCKET
ffOpenConnectUrlSocket(meUByte *host, meUByte *port)
{
    meSOCKET ss ;
    int ii ;
    
    if(ffurlBp != NULL)
    {
        meUByte buff[meBUF_SIZE_MAX] ;
        sprintf((char *)buff,"Connecting to: %s",host);
        ffurlConsoleAddText(buff,0) ;
    }
    mlwrite(MWCURSOR,(meUByte *)"[Connecting to %s]",host);
    
    if(!make_inet_addr(host,port,(meUByte *)"tcp"))
        return meBadSocket ;
    
    if((ss=meOpenSocket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Failed to open socket]") ;
        return meBadSocket ;
    }
    if(connect(ss,(struct sockaddr *) &meSockAddr,sizeof(meSockAddr)) != 0)
    {
        meCloseSocket(ss) ;
        mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Failed to connect to %s:%s]",host,port) ;
        return meBadSocket ;
    }
    
    ii = 1 ;
    setsockopt (ss,SOL_SOCKET,SO_KEEPALIVE,(char *) &ii,sizeof(int));
    
    return ss ;
}


static int
ffReadSocketLine(meSOCKET ss, meUByte *buff)
{
    meUByte *dd=buff, cc ;
    
    for(;;)
    {
        if(ffremain == 0)
        {
            ffremain = recv(ss,(char *) ffbuf,meFIOBUFSIZ,0) ;
            if(ffremain <= 0)
                return mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Failed to read line]") ;
            ffbuf[ffremain]='\n' ;
            ffcur = ffbuf ;
        }
        ffremain-- ;
        cc = *ffcur++ ;
        if(cc == '\n')
        {
            *dd = '\0' ;
            if(ffurlBp != NULL)
                ffurlConsoleAddText(buff,0) ;
            return meTRUE ;
        }
        if(cc != '\r')
            *dd++ = cc ;
    }
}

static int
ftpReadReply(void)
{
    meUByte buff[1024] ;
    int ret ;
    
    ffremain = 0 ;
    if(ffReadSocketLine(ffccsk,buff) <= 0)
        return meFALSE ;
    if(meStrlen(buff) < 4)
        return ftpERROR ;
        
    ret = buff[0] - '0' ;
    if(buff[3] == '-')
    {
        /* multi-line reply */
        meUByte c0, c1, c2 ;
        c0 = buff[0] ;
        c1 = buff[1] ;
        c2 = buff[2] ;
        do {
            if(ffReadSocketLine(ffccsk,buff) <= 0)
                return meFALSE ;
        } while((buff[0] != c0) || (buff[1] != c1) ||
                (buff[2] != c2) || (buff[3] != ' ')) ;
    }
    if(ret > ftpPOS_INTERMED)
        ret = ftpERROR ;
    return ret ;
}


static int 
ftpCommand(int flags, char *fmt, ...)
{
    va_list ap;
    int ii ;
    
    va_start(ap,fmt);
    vsprintf((char *)ffbuf,fmt,ap);
    va_end(ap);
    if(ffurlBp != NULL)
    {
        if(!meStrncmp(ffbuf,"PASS",4))
            ffurlConsoleAddText((meUByte *)"PASS XXXX",0) ;
        else
            ffurlConsoleAddText(ffbuf,0) ;
    }
    ii = meStrlen(ffbuf) ;
    ffbuf[ii++] = '\r' ;
    ffbuf[ii++] = '\n' ;
    ffbuf[ii]   = '\0' ;
    if(send(ffccsk,(char *)ffbuf,ii,0) <= 0)
    {
        mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Failed to send command]") ;
        return ftpERROR ;
    }
    if(flags & 0x01)
        return ftpPOS_COMPLETE ;
    return ftpReadReply();
}
    
static void
ffCloseSockets(int logoff)
{
    timerClearExpired(SOCKET_TIMER_ID) ;
    
    if(!meSocketIsBad(ffsock))
        meCloseSocket(ffsock) ;
    
    if(logoff && !meSocketIsBad(ffccsk))
    {
        ftpCommand(1,"QUIT") ;
        meCloseSocket(ffccsk) ;
        if(meNullFree(ffsockAddr))
            ffsockAddr = NULL ;
    }
    ffremain = 0 ;
}


static int
ftpLogin(meUByte *user, meUByte *pass)
{
    int ii ;
    /* get the initial message */
    if(ftpReadReply() != ftpPOS_COMPLETE)
        return meFALSE ;
    
    ii = ftpCommand(0,"USER %s",user) ;
    if(ii == ftpPOS_INTERMED)
        ii = ftpCommand(0,"PASS %s", pass) ;
    if(ii != ftpPOS_COMPLETE)
    {
        if(ffpasswdReg != NULL)
        {
            regDelete(ffpasswdReg) ;
            ffpasswdReg = NULL ;
        }
        return mlwrite(MWABORT,(meUByte *)"[Failed to login]") ;
    }
    return meTRUE ;
}

static int
ftpGetDataChannel(void)
{
    meUByte *aac, *ppc ;
    int aai[4], ppi[2] ;
    meUByte *ss ;
    
    if((ftpCommand(0,"PASV") != ftpPOS_COMPLETE) ||
       ((ss=meStrchr(ffbuf,'(')) == NULL) || 
       (sscanf((char *)ss,"(%d,%d,%d,%d,%d,%d)",aai,aai+1,aai+2,aai+3,ppi,ppi+1) != 6))
        return mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Failed to get PASSIVE address]") ;
    
    aac = (meUByte *)&meSockAddr.sin_addr;
    ppc = (meUByte *)&meSockAddr.sin_port;
    aac[0] = (meUByte) aai[0] ;
    aac[1] = (meUByte) aai[1] ;
    aac[2] = (meUByte) aai[2] ;
    aac[3] = (meUByte) aai[3] ;
    ppc[0] = (meUByte) ppi[0] ;
    ppc[1] = (meUByte) ppi[1] ;

    return meTRUE ;
}

/*
 * Need to start a listen on the data channel before we send the command,
 * otherwise the server's connect may fail.
 */
static int
ftpOpenDataChannel(void)
{
    if(meSocketIsBad(ffsock = meOpenSocket(AF_INET, SOCK_STREAM, 0)))
        return mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Failed to open data channel]") ;
    
    if(connect(ffsock,(struct sockaddr *) &meSockAddr,sizeof(meSockAddr)) < 0)
        return mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Failed to connect data channel]") ;
    
    return meTRUE ;
}


int
ffurlGetInfo(int type, meUByte **host, meUByte **port, meUByte **user, meUByte **pass)
{
    meRegNode *root, *reg ;
    meUByte *ss ;
    
    if (((root = regFind (NULL,(meUByte *)"/url")) == NULL) &&
        ((root = regSet (NULL,(meUByte *)"/url", NULL)) == NULL))
        return meFALSE ;
    ss = (meUByte *) ((fftype == meURLTYPE_FTP) ? "ftp":"http") ;
    if (((reg = regFind (root, ss)) == NULL) &&
        ((reg = regSet (root, ss, NULL)) == NULL))
        return meFALSE ;
    if (((root = regFind (reg, *host)) == NULL) &&
        ((root = regSet (reg, *host, NULL)) == NULL))
        return meFALSE ;
    
    reg = regFind (root,(meUByte *)"user") ;
    if (*user != NULL)
    {
        if((*pass == NULL) && (reg != NULL) && !meStrcmp(regGetValue(reg),*user) &&
           ((reg = regFind (root,(meUByte *)"pass")) != NULL))
            *pass = regGetValue(reg) ;
    }
    else if(reg != NULL)
    {
        *user = regGetValue(reg) ;
        if((reg = regFind (root,(meUByte *)"pass")) != NULL)
            *pass = regGetValue(reg) ;
    }
    ffpasswdReg = NULL ;
    if(*user != NULL)
    {
        if(*pass == NULL)
        {
            meUByte buff[mePATBUF_SIZE_MAX] ;
            
            if((meGetString((meUByte *)"Password", MLNOHIST|MLHIDEVAL, 0, buff, mePATBUF_SIZE_MAX-1) <= 0) ||
               ((ffpasswdReg = regSet(root,(meUByte *)"pass",buff)) == NULL))
                return meFALSE ;
            ffpasswdReg->mode |= meREGMODE_INTERNAL ;
            *pass = regGetValue(ffpasswdReg) ;
        }
        else if(((reg = regFind(root,(meUByte *)"pass")) == NULL) || meStrcmp(regGetValue(reg),*pass))
            regSet(root,(meUByte *)"pass",*pass) ;
        if(((reg = regFind(root,(meUByte *)"user")) == NULL) || meStrcmp(regGetValue(reg),*user))
            regSet(root,(meUByte *)"user",*user) ;
    }
    if ((reg = regFind (root,(meUByte *)"host")) != NULL)
        *host = regGetValue(reg) ;
    reg = regFind (root,(meUByte *)"port") ;
    if (*port != NULL)
    {
        if((reg == NULL) || meStrcmp(regGetValue(reg),*port))
            regSet(root,(meUByte *)"port",*port) ;
    }
    else if(reg != NULL)
        *port = regGetValue(reg) ;
    return meTRUE ;
}

void
ffurlCreateName(meUByte *buff, int type, meUByte *host, meUByte *port, meUByte *user, meUByte *pass, meUByte *file)
{
    if(type & meURLTYPE_FTP)
        meStrcpy(buff,"ftp://") ;
    else
        meStrcpy(buff,"http://") ;
    if(user != NULL)
    {
        meStrcat(buff,user) ;
        if(pass != NULL)
        {
            meStrcat(buff,":") ;
            meStrcat(buff,pass) ;
        }
        meStrcat(buff,"@") ;
    }
    meStrcat(buff,host) ;
    if(port != NULL)
    {
        meStrcat(buff,":") ;
        meStrcat(buff,port) ;
    }
    if(file != NULL)
    {
        int ll ;
        meStrcat(buff,file) ;
        ll = meStrlen(buff) ;
        if((type & meURLTYPE_DIR) && (buff[ll-1] != DIR_CHAR))
        {
            buff[ll++] = DIR_CHAR ;
            buff[ll]   = '\0' ;
        }
    }
}

static int ffUrlFileOpen(meUByte *urlName, meUByte *user, meUByte *pass, meUInt rwflag) ;

int
ffFtpFileOpen(meUByte *host, meUByte *port, meUByte *user, meUByte *pass, meUByte *file, meUInt rwflag)
{
    meUByte buff[meBUF_SIZE_MAX] ;
    int dirlist ;
    
    /* are we currently connected to this site?? */
    ffurlCreateName(buff,meURLTYPE_FTP,host,port,user,NULL,NULL) ;
    if((ffsockAddr == NULL) || meStrcmp(buff,ffsockAddr))
    {
        /* if we're already logged on somewhere else kill the connection */
        ffCloseSockets(1) ;
        
        if(port == NULL)
            port = (meUByte *)"21" ;
        if(user == NULL)
        {
            /* user = "anonymous" ;*/
            user = (meUByte *)"ftp" ;
            pass = (meUByte *)"guest" ;
        }
    
        if(meSocketIsBad(ffccsk=ffOpenConnectUrlSocket(host,port)))
            return meFALSE ;
    
        if(ftpLogin(user,pass) <= 0)
        {
            ffCloseSockets(1) ;
            return meFALSE ;
        }
        if(ftpCommand(0,"TYPE I") != ftpPOS_COMPLETE)
        {
            ffCloseSockets(1) ;
            return meFALSE ;
        }
        ffsockAddr = meStrdup(buff) ;
    }
    
    if(ffurlBp != NULL)
    {
        meUByte buff[meBUF_SIZE_MAX] ;
        ffurlConsoleAddText((meUByte *)"",0x04) ;
        if(rwflag & meRWFLAG_WRITE)
            meStrcpy(buff,"Writing: ");
        else if(rwflag & meRWFLAG_READ)
            meStrcpy(buff,"Reading: ");
        else
            meStrcpy(buff,"Deleting: ");
        meStrcat(buff,file);
        ffurlConsoleAddText(buff,0) ;
    }
    mlwrite(MWCURSOR,(meUByte *)"[Connected, %sing %s]",
            (rwflag & meRWFLAG_WRITE) ? "writ":
            (rwflag & meRWFLAG_READ)  ? "read":"delet",file);
    if(rwflag & meRWFLAG_BACKUP)
    {
        /* try to back-up the existing file - no error checking! */
        meUByte filename[meFILEBUF_SIZE_MAX] ;
        
        if(!createBackupName(filename,file,'~',1) &&
           (ftpCommand(0,"RNFR %s",file) == ftpPOS_INTERMED))
            ftpCommand(0,"RNTO %s",filename) ;
    }
    if(rwflag & meRWFLAG_DELETE)
    {
        /* if backing up as well the file is already effectively deleted */
        if(!(rwflag & meRWFLAG_BACKUP))
            ftpCommand(0,"DELE %s",file) ;
    }
    if(rwflag & meRWFLAG_MKDIR)
        ftpCommand(0,"MKD %s",file) ;
    
    if(!(rwflag & (meRWFLAG_WRITE|meRWFLAG_READ)))
        return meTRUE ;
    
    dirlist = meStrlen(file) ;
    dirlist = (file[dirlist-1] == DIR_CHAR) ;
                  
    /* send the read command */
    for(;;)
    {
        if(ftpGetDataChannel() <= 0)
        {
            ffCloseSockets(0) ;
            return meFALSE ;
        }
        
        if(rwflag & meRWFLAG_WRITE)
            ftpCommand(1,"STOR %s",file) ;
        else if(dirlist)
            ftpCommand(1,"LIST %s",file) ;
        else
            ftpCommand(1,"RETR %s",file) ;
    
        /* open up the recv channel */
        if(ftpOpenDataChannel() <= 0)
        {
            ffCloseSockets(0) ;
            return meFALSE ;
        }
        
        /* find out if all's well */
        if(ftpReadReply() == ftpPOS_PRELIMIN)
        {
            if(dirlist && (ffurlReadBp != NULL))
                meModeSet(ffurlReadBp->mode,MDDIR) ;
            return meTRUE ;
        }
        if((rwflag & meRWFLAG_WRITE) || dirlist)
        {
            ffCloseSockets(0) ;
            return mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Failed to %s file]",
                           (rwflag & meRWFLAG_WRITE) ? "write":"read");
        }
        ffCloseSockets(0) ;
        dirlist = 1 ;
    }
}

static int
ffHttpFileOpen(meUByte *host, meUByte *port, meUByte *user, meUByte *pass, meUByte *file, meUInt rwflag)
{
    meUByte buff[meBUF_SIZE_MAX+200], *thost, *tport, *ss, cc ;
    
    if ((ss = getUsrVar((meUByte *)"http-proxy-addr")) != errorm)
    {
        thost = ss ;
        tport = getUsrVar((meUByte *)"http-proxy-port") ;
    }
    else
    {
        thost = host ;
        tport = port ;
    }
    
    if(tport == NULL)
        tport = (meUByte *)"80" ;
    
    if(meSocketIsBad(ffsock=ffOpenConnectUrlSocket(thost,tport)))
        return meFALSE ;
       
    if(ffurlBp != NULL)
    {
        ffurlConsoleAddText((meUByte *)"",0x04) ;
        sprintf((char *)buff,"Reading: %s",file);
        ffurlConsoleAddText(buff,0) ;
    }
    mlwrite(MWCURSOR,(meUByte *)"[Connected, reading %s]", file);

    /* send GET message to http */
    meStrcpy(buff,"GET ") ;
    if(thost == host)
        meStrcpy(buff+4,file) ;
    else
        ffurlCreateName(buff+4,fftype,host,port,NULL,NULL,file) ;
    ss = buff + meStrlen(buff) ;
    meStrcpy(ss," HTTP/1.0\r\nConnection: Keep-Alive\r\nHost: ") ;
    ss += meStrlen(ss) ;
    meStrcpy(ss,host) ;
    ss += meStrlen(ss) ;
    if(user != NULL)
    {
        /* password supplied, encode */
        meUByte tbuff[meBUF_SIZE_MAX] ;
        meStrcpy(tbuff,user) ;
        meStrcat(tbuff,":") ;
        meStrcat(tbuff,pass) ;
        meStrcat(ss,"\r\nAuthorization: Basic ") ;
        ss += 23 ;
        ss = strBase64Encode(ss,user) ;
    }
    meStrcpy(ss,"\r\n\r\n") ;
    if(ffurlBp != NULL)
    {
        meUByte *s2 ;
        ss = s2 = buff ;
        while((cc=*s2++) != '\0')
        {
            if(cc == '\r')
            {
                s2[-1] = '\0' ;
                ffurlConsoleAddText(ss,0) ;
                s2[-1] = '\r' ;
                ss = ++s2 ;
            }
        }
        resetBufferWindows(ffurlBp) ;
        if(ffurlFlags & ffURL_SHOW_CONSOLE)
            screenUpdate(meTRUE,2-sgarbf) ;
    }
    if(send(ffsock,(char *)buff,meStrlen(buff),0) <= 0)
    {
        ffCloseSockets(1) ;
        return meFALSE ;
    }
    /* must now ditch the header, read up to the first blank line */
    while(ffReadSocketLine(ffsock,buff) > 0)
    {
        if(buff[0] == '\0')
        {
            ffread = ffremain ;
            return meTRUE ;
        }
        if(!meStrncmp(buff,"Content-Length:",15))
            ffsize = meAtoi(buff+15) ;
        else if(!meStrncmp(buff,"Location:",9))
        {
            /* The requested file is not here, its at the given location */
            ss = buff+9 ;
            while(((cc=*ss) == ' ') || (cc == '\t'))
                ss++ ;
            if(*ss != '\0')
            {
                /* printf("Got Location: [%s]\n",ss) ;*/
                if(ffurlBp != NULL)
                    ffurlConsoleAddText((meUByte *)"",0x04) ;
                ffCloseSockets(1) ;
                return ffUrlFileOpen(ss,user,pass,rwflag) ;
            }
        }
    }
    ffCloseSockets(1) ;
    return mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Failed to read header of %s]",ss) ;
}

static void
ffUrlFileSetupFlags(int flag)
{
    meUByte *ss ;
    
    /* setup the flags and console buffer */
    ffurlFlags = ffURL_CONSOLE ;
    if ((ss = getUsrVar (ffurlFlagsVName[fftype-meURLTYPE_HTTP])) != errorm)
    {
        if(meStrchr(ss,'c') != NULL)
            ffurlFlags |= ffURL_CONSOLE ;
        if(meStrchr(ss,'s') != NULL)
            ffurlFlags |= ffURL_SHOW_CONSOLE ;
        if(meStrchr(ss,'p') != NULL)
            ffurlFlags |= ffURL_SHOW_PROGRESS ;
    }
    if((ffurlFlags & ffURL_CONSOLE) &&
       ((ffurlBp=bfind(ffurlConsoleBName[fftype-meURLTYPE_HTTP],BFND_CREAT)) != NULL))
    {
        meModeClear(ffurlBp->mode,MDUNDO) ;
        if((ffurlFlags & ffURL_SHOW_CONSOLE) && !(flag & 0x01))
            meWindowPopup(ffurlConsoleBName[fftype-meURLTYPE_HTTP],0,NULL) ;
    }
}

static int
ffUrlFileOpen(meUByte *urlName, meUByte *user, meUByte *pass, meUInt rwflag)
{
    meUByte buff[meBUF_SIZE_MAX], sockAddr[meBUF_SIZE_MAX], *host, *port, *ss, *dd, *ee, cc ;
    int ii ;
    
#ifdef _WIN32
    static int init=0 ;
    if(!init)
    {
        WSADATA wsaData;
        WORD    wVersionRequested;
        
        wVersionRequested = MAKEWORD (1, 1);
    
        if(WSAStartup(wVersionRequested, &wsaData))
            return mlwrite(MWABORT|MWPAUSE,"[Failed to initialise sockets]") ;
        atexit((meATEXIT) WSACleanup) ;
        init = 1 ;
    }
#endif
    fftype = (urlName[0] == 'f') ? meURLTYPE_FTP:meURLTYPE_HTTP ;
    if(rwflag & meRWFLAG_READ)
    {
        ffrp = ffpInvalidVal ;
        ffsize = 0x7fffffff ;
    }
    else if(rwflag & meRWFLAG_WRITE)
        ffwp = ffpInvalidVal ;
    
    ffUrlFileSetupFlags(0) ;
    
    /* skip the http: or ftp: */
    ss = (fftype == meURLTYPE_FTP) ? urlName+6:urlName+7 ;
    dd = buff ;
    ee = sockAddr ;
    host = dd ;
    port = NULL ;
    while(((cc=*ss++) != '\0') && (cc != DIR_CHAR))
    {
        *dd++ = cc ;
        *ee++ = cc ;
        if(cc == ':')
        {
            dd[-1] = '\0' ;
            port = dd ;
        }
        else if(cc == '@')
        {
            dd[-1] = '\0' ;
            user = buff ;
            pass = port ;
            host = dd ;
            port = NULL ;
        }
    }
    *dd = '\0' ;
    *ee = '\0' ;
    ss-- ;
    
    if(port != NULL)
        port[-1] = '\0' ;
    
    if(ffurlGetInfo(fftype,&host,&port,&user,&pass) <= 0)
        return meFALSE ;
    
    /* is it a http: or ftp: */
    if(fftype == meURLTYPE_FTP)
        ii = ffFtpFileOpen(host,port,user,pass,ss,rwflag) ;
    else
        ii = ffHttpFileOpen(host,port,user,pass,ss,rwflag) ;
    
    if(ii > 0)
    {
        if((rwflag & meRWFLAG_READ) && (ffurlReadBp != NULL))
        {
            meUByte *ff, buff[meBUF_SIZE_MAX+128] ;
            if(fftype == meURLTYPE_HTTP)
            {
                if(!(rwflag & meRWFLAG_INSERT))
                {
                    ffurlCreateName(buff,fftype,host,port,user,NULL,ss) ;
                    if(fnamecmp(buff,ffurlReadBp->fileName) &&
                       ((ff=meStrdup(buff)) != NULL))
                        /* the original fname is freed in ffReadFile */
                        ffurlReadBp->fileName = ff ;
                }
            }
            else if(meModeTest(ffurlReadBp->mode,MDDIR))
            {
                /* for an ftp dir 2 things must be done:
                 * 1) If the file name does not end in a '/' then add one and correct the buffer name
                 * 2) Add the top Directory Listing line
                 */
                int ll ;
                ll = meStrlen(ffurlReadBp->fileName) ;
                if(ffurlReadBp->fileName[ll-1] != DIR_CHAR)
                {
                    meStrcpy(buff,ffurlReadBp->fileName) ;
                    buff[ll] = DIR_CHAR ;
                    buff[ll+1] = '\0' ;
                    /* the original fname is freed in ffReadFile */
                    ffurlReadBp->fileName = NULL ;
                    resetBufferNames(ffurlReadBp,buff) ;
                }
                meStrcpy(buff,"Directory listing of: ") ;
                ffurlCreateName(buff+22,(fftype|meURLTYPE_DIR),host,port,user,NULL,ss) ;
                addLineToEob(ffurlReadBp,buff) ;
            }
            ffurlReadBp = NULL ;
        }
        if((rwflag & (meRWFLAG_READ|meRWFLAG_WRITE)) && (ffurlBp != NULL))
        {
            struct meTimeval tp ;
            if(ffurlFlags & ffURL_SHOW_PROGRESS)
                ffurlConsoleAddText((meUByte *)"Progress: ",0x02) ;
            gettimeofday(&tp,NULL) ;
            ffstartTime = (tp.tv_sec * 1000) + (tp.tv_usec/1000) ;
        }
    }
    else
        ffurlBp = NULL ;
    return ii ;
}

static int
ffUrlFileClose(meUByte *fname, meUInt rwflag)
{
    if(rwflag & (meRWFLAG_READ|meRWFLAG_WRITE))
    {
        if(fname[0] == 'f')
        {
            if(!meSocketIsBad(ffsock))
                meCloseSocket(ffsock) ;
            /* get the transfer complete */
            if(ftpReadReply() != ftpPOS_COMPLETE)
                mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Problem occurred during download]") ;
        }
        if(ffurlBp != NULL)
        {
            meUByte buff[meBUF_SIZE_MAX] ;
            struct meTimeval tp ;
            int dwldtime ;
            gettimeofday(&tp,NULL) ;
            dwldtime = ((tp.tv_sec * 1000) + (tp.tv_usec/1000)) - ffstartTime ;
            if(dwldtime <= 0)
                dwldtime = 1 ;
            sprintf((char *)buff,"%d bytes %s in %d.%d seconds (%d.%d Kbytes/s)",
                    ffread,(rwflag & meRWFLAG_WRITE) ? "sent":"received",
                    dwldtime/1000,(dwldtime/100) % 10,
                    ffread/dwldtime,((ffread*100)/dwldtime)% 100) ;
            ffurlConsoleAddText(buff,0x04) ;
            ffurlConsoleAddText((meUByte *)"",0) ;
            ffurlBp = NULL ;
        }
    }
    if(ffsockAddr == NULL)
        ffCloseSockets(1) ;
    else
        timerSet(SOCKET_TIMER_ID,-1,meSOCKET_TIMEOUT) ;
    return meTRUE ;
}

#endif

meUByte *
createBackupNameStrcpySub(meUByte *dd, meUByte *ss, int subCount, meUByte **subFrom, meUByte **subTo)
{
    dd += meStrlen(dd) ;
    if(subCount != 0)
    {
        meUByte cc ;
        int ii, ll ;
        while((cc=*ss) != '\0')
        {
            for(ii=0 ; ii<subCount ; ii++)
            {
                ll = meStrlen(subFrom[ii]) ;
                if(!meStrncmp(ss,subFrom[ii],ll))
                {
                    meStrcpy(dd,subTo[ii]) ;
                    dd += meStrlen(dd) ;
                    ss += ll ;
                    break ;
                }
            }
            if((ii == subCount) || (ll == 0))
            {
                *dd++ = cc ;
                ss++ ;
            }
        }
        *dd = '\0' ;
    }
    else
    {
        meStrcpy(dd,ss) ;
        dd += meStrlen(dd) ;
    }
    return dd ;
}

/*
 * creates the standard bckup name with the extra
 * character(s) being backl, where a '~' is used
 * for backups and '#' for auto-saves
 * returns 1 if fails, else 0 and name in filename buffer
 */
int
createBackupName(meUByte *filename, meUByte *fn, meUByte backl, int flag)
{
    meUByte *s, *t ;
#ifndef _DOS
    static int     backupPathFlag=0 ;
    static meUByte  *backupPath=NULL ;
    static int     backupSubCount=0 ;
    static meUByte **backupSubFrom=NULL ;
    static meUByte **backupSubTo=NULL ;
    if(!backupPathFlag)
    {
	if(((s=meGetenv("MEBACKUPPATH")) != NULL) && (meStrlen(s) > 0) &&
	   ((backupPath=meStrdup(s)) != NULL))
        {
            fileNameConvertDirChar(backupPath) ;
            if((backupPath[0] == DIR_CHAR)
#ifdef _DRV_CHAR
               || (isAlpha(backupPath[0]) && (backupPath[1] == _DRV_CHAR))
#endif
               )
                backupPathFlag = 1 ;
            else
                backupPathFlag = 2 ;
        }
	else
	    backupPathFlag = -1 ;
	if(((s=meGetenv("MEBACKUPSUB")) != NULL) && (meStrlen(s) > 0))
        {
            meUByte cc, dd ;
            int ii ;
            
            while((dd=*s++) != '\0')
            {
                if(dd == 's')
                {
                    ii=backupSubCount++ ;
                    if(((dd = *s++) == '\0') ||
                       ((backupSubFrom=meRealloc(backupSubFrom,backupSubCount*sizeof(meUByte *))) == NULL) ||
                       ((backupSubTo  =meRealloc(backupSubTo  ,backupSubCount*sizeof(meUByte *))) == NULL))
                    {
                        backupSubCount = 0 ;
                        break ;
                    }
                    backupSubFrom[ii] = s ;
                    while(((cc=*s) != '\0') && (cc != dd))
                        s++ ;
                    if(cc != '\0')
                    {
                        *s++ = '\0' ;
                        backupSubTo[ii] = s ;
                        while(((cc=*s) != '\0') && (cc != dd))
                            s++ ;
                        *s++ = '\0' ;
                    }
                    if(cc == '\0')
                    {
                        backupSubCount = 0 ;
                        break ;
                    }
                }
            }
        }
    }
#endif
    if(fn == NULL)
        return 1 ;
#if MEOPT_SOCKET
    if((backl == '#') && isUrlLink(fn))
    {
        meUByte tmp[meFILEBUF_SIZE_MAX] ;
        s = getFileBaseName(fn) ;
        meStrcpy(tmp,s) ;
        s = tmp ;
        while((s=meStrchr(s,DIR_CHAR)) != NULL)  /* got a '/', -> '.' */
            *s++ = '.' ;
        mkTempName(filename,tmp,NULL) ;
    }
    else
#endif
#ifndef _DOS
    if((backupPathFlag > 0) && !isUrlLink(fn))
    {
        if(backupPathFlag == 1)
        {
            meStrcpy(filename,backupPath) ;
            t = filename + meStrlen(backupPath) ;
            s = fn ;
            if(s[0] == DIR_CHAR)
                s++ ;
            createBackupNameStrcpySub(t,s,backupSubCount,backupSubFrom,backupSubTo) ;
        }
        else
        {
            int ll ;
            s = getFileBaseName(fn) ;
            ll = ((size_t)s - (size_t)fn) ;
            meStrncpy(filename,fn,ll) ;
            t = filename+ll ;
            t = createBackupNameStrcpySub(t,backupPath,backupSubCount,
                                          backupSubFrom,backupSubTo) ;
            createBackupNameStrcpySub(t,s,backupSubCount,backupSubFrom,backupSubTo) ;
        }
#ifdef _DRV_CHAR
        /* ensure the path has no ':' in it - breaks every thing, change to a / */
        while((t=meStrchr(t,_DRV_CHAR)) != NULL)
            *t++ = DIR_CHAR ;
#endif
        if(flag & meBACKUP_CREATE_PATH)
        {
            int ii=0 ;
            while(((t=meStrrchr(filename,DIR_CHAR)) != NULL) && (t != filename))
            {
                *t = '\0' ;
                if(!meTestExist(filename))
                {
                    if(meTestDir(filename))
                        return 1 ;
                    *t = DIR_CHAR ;
                    break ;
                }
                ii++ ;
            }
            while(--ii >= 0)
            {
                if(ffWriteFileOpen(filename,meRWFLAG_MKDIR,NULL) <= 0)
                    return 1 ;
                filename[meStrlen(filename)] = DIR_CHAR ;
            }
        }
    }
    else
#endif
        meStrcpy(filename,fn) ;
    
#ifdef _UNIX
    if((backl == '~') && ((meSystemCfg & (meSYSTEM_DOSFNAMES|meSYSTEM_HIDEBCKUP)) == meSYSTEM_HIDEBCKUP))
    {
        if((t=meStrrchr(filename,DIR_CHAR)) == NULL)
            t = filename ;
        else
            t++ ;
        s = filename + meStrlen(filename) + 1 ;
        do
            s[0] = s[-1] ;
        while(--s != t) ;
        s[0] = '.' ;
    }
#endif            
    t = filename + meStrlen(filename) ;
#ifndef _DOS
    if(meSystemCfg & meSYSTEM_DOSFNAMES)
#endif
    {
        s = t ;
        while(--s != filename)
            if((*s == '.') || (*s == DIR_CHAR))
                break ;
        if(*s != '.')
        {
            *t = '.' ;
            t[1] = backl ;
            t[2] = backl ;
            t[3] = backl ;
            t[4] = '\0' ;
        }
#ifndef _DOS
        /* In the Win32 environment. If the extension is longer
         * than 3 characters then replace the last character with
         * the backup character. Otherwise behave like a standard
         * DOS name.
         */
        else if ((t - s) > 3)
            t[-1] = backl;
#endif    
        else
        {
            t = s ;
            if(t[2] == '\0')
                t[2] = backl ;
            t[3] = backl ;
            t[4] = '\0' ;
        }
        if(!meStrcmp(filename,fn))
            return 1 ;
    }
#ifndef _DOS
    else
    {
        *t++ = backl ;
        *t   = 0 ;
    }
#endif
    return 0 ;
}

static int
ffgetBuf(void)
{
#if MEOPT_SOCKET
    if(ffrp == ffpInvalidVal)
    {
        if((ffremain = ffsize-ffread) > 0)
        {
            if(ffremain > meFIOBUFSIZ)
                ffremain = meFIOBUFSIZ ;
            ffremain = recv(ffsock,(char *) ffbuf,ffremain,0) ;
        }
        if(ffremain <= 0)
        {
            ffremain = -1 ;
            return meTRUE ;
        }
        if((ffurlBp != NULL) &&
           (ffurlFlags & ffURL_SHOW_PROGRESS))
            ffurlConsoleAddText((meUByte *)"#",(meLineGetLength(ffurlBp->dotLine) >= frameCur->width - 2) ? 0x02:0x03) ;
    }
    else
#endif
#ifdef _WIN32
    {
        if(ReadFile(ffrp,ffbuf,meFIOBUFSIZ,&ffremain,NULL) == 0)
            return mlwrite(MWABORT,"File read error %d",GetLastError());
        
        if(ffremain <= 0)
        {
            ffremain = -1 ;
            return meTRUE ;
        }
    }
#else
    {
        ffremain = fread(ffbuf,1,meFIOBUFSIZ,ffrp) ;
        if(ffremain <= 0)
        {
            if(ferror(ffrp))
                return mlwrite(MWABORT,(meUByte *)"File read error %d",errno);
            ffremain = -1 ;
            return meTRUE ;
        }
    }
#endif
#if MEOPT_CRYPT
    if(ffcrypt)
        meCrypt(ffbuf, ffremain);
#endif
    ffbuf[ffremain]='\n' ;
    ffread += ffremain ;
    return meTRUE ;
}

/*
 * Read a line from a file and create a new line to hold it.
 * If mode & MDBINARY then loading it in binary mode an a line
 * is upto 16 bytes long.
 * 
 * Returns meABORT if a serious system problem arose (read or malloc
 * failure).
 * Returns Error is nothing was read cos at the end of the file
 * Else success so returns meTRUE.
 */
static int	
ffgetline(meLine **line)
{
    register meUByte *text ;
    register meUByte *endp ;
    register int    len=0, newl, ii ;
    register meLine  *lp ;
    
    do {
        if(ffremain <= 0)
        {
            if(ffgetBuf() <= 0)
                return meABORT ;
            if(ffremain < 0)
                break ;
            ffcur = ffbuf ;
        }
        if(ffbinary)
        {
            if(len == 0)
            {
                ii = (ffbinary == meBINARY_BPL) ? (meBINARY_BPL*4)+14 : 2*meRBIN_BPL ;
                if((lp = meLineMalloc(ii,0)) == NULL)
                    return meABORT ;
            }
            if(ffremain < (ffbinary-len))
            {
                memcpy(lp->text+len,ffcur,ffremain) ;
                len += ffremain ;
                ffremain = -1 ;
            }
            else
            {
                ii = ffbinary-len ;
                memcpy(lp->text+len,ffcur,ii) ;
                ffremain -= ii ;
                ffcur += ii ;
                len = ffbinary ;
            }
        }
        else
        {
            register meUByte cc ;
            endp = ffcur ;
            newl = 0 ;
            while(((cc=*ffcur++) != '\n') && (cc != '\r'))
                if(cc != '\0')
                    newl++ ;
            /* If we ended in '\r' check for a '\n' */
            if(cc == '\r')
            {
                if(*ffcur == '\n')
                {
#ifdef _CTRLM
                    if(ffauto)
                        ffautoRet |= AUTO_CRLF ;
                    ffcur[-1] = '\n' ;
#else
                    if(ffauto)
                    {
                        ffautoRet |= AUTO_CRLF ;
                        ffcur[-1] = '\n' ;
                    }
                    else
                        newl++;
#endif
                    ffcur++ ;
                }
                else
                    ffcur[-1] = '\n' ;
            }
            
            ffremain -= ((size_t) ffcur) - ((size_t) endp) ;
            
            if(len == 0)
            {
                len = newl ;
                if((lp = (meLine *) meLineMalloc(len,0)) == NULL)
                    return meABORT ;
                text = lp->text ;
            }
            else
            {
                size_t ss ;
                ii = len ;
                len += newl ;
                ss = meLineMallocSize(len) ;
                if((lp = (meLine *) meRealloc(lp,ss)) == NULL)
                    return meABORT ;
                text = lp->text + ii ;
                lp->unused = ss - len - meLINE_SIZE ;
                lp->length = len ;
            }
            while((cc = *endp++) != '\n')
            {
                if(cc != '\0')
                    *text++ = cc ;
            }
            *text = '\0' ;
            if(len >= (0xfffe - meFIOBUFSIZ))
            {
                /* TOO Big! break out */
                ffremain = -2 ;
                break ;
            }
        }
    } while(ffremain < 0) ;
    
    if(ffbinary)
    {
        if(len == 0)
            return meFALSE ;
        
        if(ffbinary == meBINARY_BPL)
        {
            meInt cpos ;
            meUByte cc ;
            
            text = lp->text ;
            newl = meBINARY_BPL ;
            while(--newl >= 0)
            {
                if(newl < len)
                {
                    cc = text[newl] ;
                    text[(newl*3)+10] = hexdigits[cc >> 4] ;
                    text[(newl*3)+11] = hexdigits[cc & 0x0f] ;
                    text[(newl*3)+12] = ' ' ;
                    if(!isDisplayable(cc))
                        cc = '.' ;
                }
                else
                {
                    text[(newl*3)+10] = ' ' ;
                    text[(newl*3)+11] = ' ' ;
                    text[(newl*3)+12] = ' ' ;
                    cc = ' ' ;
                }
                text[newl+(meBINARY_BPL*3)+14] = cc ;
            }
            cpos = (ffoffset + ffread - ffremain - 1) & ~((meInt) (meBINARY_BPL-1)) ;
            newl=8 ;
            while(--newl >= 0)
            {
                text[newl] = hexdigits[cpos&0x0f] ;
                cpos >>= 4 ;
            }
            text[8] = ':' ;
            text[9] = ' ' ;
            
            text[(meBINARY_BPL*3)+10] = ' ' ;
            text[(meBINARY_BPL*3)+11] = '|' ;
            text[(meBINARY_BPL*3)+12] = ' ' ;
            text[(meBINARY_BPL*3)+13] = ' ' ;
            text[(meBINARY_BPL*4)+14] = '\0' ;
        }
        else
        {
            meUByte cc ;
            
            text = lp->text ;
            lp->length = len*2 ;
            text[len*2] = '\0' ;
            while(--len >= 0)
            {
                cc = text[len] ;
                text[(len*2)]   = hexdigits[cc >> 4] ;
                text[(len*2)+1] = hexdigits[cc & 0x0f] ;
            }
        }
        *line = lp ;
        return meTRUE ;
    }
    if(ffremain < 0)  /* had to break */
    {
        if(ffremain == -2)
            /* had to break because line was too long */
            ;
        else if(len == 0)
            return meFALSE ;
        else if(ffauto && (len == 1) && (lp->text[0] == 26))
        {
            ffautoRet |= AUTO_CTRLZ ;
            meFree(lp) ;
            return meFALSE ;
        }
        /* Don't flag as the end, otherwise we won't get a proper linep */
        /* mlwrite(MWCURSOR|MWPAUSE,"Warning - Incomplete last line");*/
            
        lp->flag = meLINE_NOEOL ;				/* line has no \n */
    }
    *line = lp ;

    return meTRUE ;
}

static int
ffReadFileOpen(meUByte *fname, meUInt flags, meBuffer *bp)
{
    if(bp != NULL)
    {
        if(meModeTest(bp->mode,MDBINRY))
            ffbinary = meBINARY_BPL ;
        else if(meModeTest(bp->mode,MDRBIN))
            ffbinary = meRBIN_BPL ;
        else
            ffbinary = 0 ;
        ffcrypt  = (meModeTest(bp->mode,MDCRYPT) != 0) ;
        ffauto   = (meModeTest(bp->mode,MDAUTO) != 0) ;
    }
    else
    {
        ffbinary = 0 ;
        ffcrypt = ((flags & meRWFLAG_CRYPT) != 0) ;
        ffauto = 1 ;
    }        
    ffautoRet= 0 ;
    ffremain = 0 ;
    ffread = 0 ;
    
    if(fname != NULL)
    {
        if(isUrlLink(fname))
        {
#if MEOPT_SOCKET
#ifdef _UNIX
            /* must stop any signals or the connection will fail */
            meSigHold() ;
#endif
            ffurlReadBp = bp ;
            if(ffUrlFileOpen(fname,NULL,NULL,flags) <= 0)
            {
#ifdef _UNIX
                meSigRelease() ;
#endif
                return meABORT ;
            }
            /* cannot return here, must do ffrsetup */
#else
            return mlwrite(MWABORT|MWPAUSE,(meUByte *) "[No url support in this version]") ;
#endif
        }        
        else
        {
#ifdef _WIN32
            if((ffrp=CreateFile(fname,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL,NULL)) == INVALID_HANDLE_VALUE)
#else
            if ((ffrp=fopen((char *)fname, "rb")) == NULL)
#endif
            {
                if(!(flags & meRWFLAG_SILENT))
                    /* File not found.      */
                    mlwrite(MWCLEXEC,(meUByte *)"[%s: No such file]", fname);
                return meABORT ;
            }
        }
    }
#ifdef _UNIX
#if MEOPT_SOCKET
    if(ffrp != ffpInvalidVal)
#endif
        meSigHold() ;
#endif

    return meTRUE ;
}

/* close the read file handle, Ignore any close error */
static void
ffReadFileClose(meUByte *fname, meUInt flags)
{
    /* Windows pipeCommand comes in as a pipe because the source file
     * has already been opened, but from now one treat it as a normal
     * file.
     * The output file has been set up as a temporary file with the
     * 'delete when closed' flag.
     */
#if MEOPT_SOCKET
    if(ffrp == ffpInvalidVal)
        ffUrlFileClose(fname,meRWFLAG_READ) ;
    else
#endif
    {
#ifdef _WIN32
        if((ffrp != GetStdHandle(STD_INPUT_HANDLE)) &&
           (CloseHandle(ffrp) == 0) && !(flags & meRWFLAG_SILENT))
            mlwrite(MWABORT|MWPAUSE,(meUByte *)"Error closing file");
#else
#ifdef _UNIX
        if(fname == NULL)
        {
            if(ffrp != stdin)
                pclose(ffrp);
        }
        else if((fclose(ffrp) != 0) && !(flags & meRWFLAG_SILENT))
            mlwrite(MWABORT|MWPAUSE,(meUByte *)"Error closing file");
#else
        if((ffrp != stdin) && (fclose(ffrp) != 0) && !(flags & meRWFLAG_SILENT))
            mlwrite(MWABORT|MWPAUSE,(meUByte *)"Error closing file");
#endif
#endif
    }
#ifdef _UNIX
    meSigRelease() ;
#endif
}

int
ffReadFile(meUByte *fname, meUInt flags, meBuffer *bp, meLine *hlp, 
           meInt offset, meInt length)
{
    meLine *lp0, *lp1, *lp2 ;
    int   ss, nline ;
#if MEOPT_SOCKET
    int ff ;
    ff = ((bp != NULL) && (fname != NULL) && (bp->fileName == fname)) ;
#endif
    flags |= meRWFLAG_READ ;
    if(ffReadFileOpen(fname,flags,bp) <= 0)
        return meABORT ;
    
    ffoffset = 0 ;
    if(length > 0)
    {
        /* partial reading only supported by the insert-file command on regular files */
        if(offset < 0)
        {
#ifdef _WIN32
            GetFileSize(ffrp,&offset) ;
            offset -= length ;
#else
            fseek(ffrp,0,SEEK_END) ;
            offset = ftell(ffrp) - length ;
#endif
            if(offset < 0)
                offset = 0 ;
        }
        ffoffset = offset ;
        if(ffbinary)
        {
            offset &= ffbinary-1 ;
            ffoffset -= offset ;
            length += offset ;
        }
#ifdef _WIN32
        SetFilePointer(ffrp,ffoffset,NULL,FILE_BEGIN) ;
#else
        fseek(ffrp,ffoffset,SEEK_SET) ;
#endif
    }
    
    nline = 0;
    lp2 = hlp ;	        /* line after  insert */
    lp0 = lp2->prev ;	/* line before insert */
    while((ss=ffgetline(&lp1)) == meTRUE)
    {
        /* re-link new line between lp0 and lp1 */
        lp0->next = lp1;
        lp1->prev = lp0;
        lp0 = lp1 ;
        nline++ ;
        if((length > 0) && ((ffread-ffremain) >= length))
            break ;
        if(TTbreakTest(1))
        {
            ss = ctrlg(meFALSE,1) ;
            break ;
        }
    }
    /* complete the link */
    lp2->prev = lp0;
    lp0->next = lp2;
    
    ffReadFileClose(fname,flags) ;
    
#if MEOPT_SOCKET
    /* loading an ftp or http file can lead to a file name change, free
     * off the old one now that we don't need it any more */
    if(ff && (bp->fileName != fname))
        free(fname) ;
#endif
    if(bp != NULL)
    {
        bp->lineCount += nline ;
        if(!(flags & meRWFLAG_INSERT) && ffauto && nline)
        {
            if(ffautoRet & AUTO_CRLF)
                meModeSet(bp->mode,MDCRLF) ;
            else
                meModeClear(bp->mode,MDCRLF) ;
            if(ffautoRet & AUTO_CTRLZ)
                meModeSet(bp->mode,MDCTRLZ) ;
            else
                meModeClear(bp->mode,MDCTRLZ) ;
        }
    }
    if(length > 0)
        sprintf(resultStr,"|%d|%d|",ffoffset,ffoffset+ffread-ffremain) ;
    return ss ;
}


static int
ffputBuf(void)
{
    int written ;
#if MEOPT_CRYPT
    if(ffcrypt)
        meCrypt(ffbuf,ffremain) ;
#endif
#if MEOPT_SOCKET
    if(ffwp == ffpInvalidVal)
    {
        if(((written = (int) send(ffsock,(char *) ffbuf,ffremain,0)) == ffremain) &&
           (ffurlBp != NULL) && (ffurlFlags & ffURL_SHOW_PROGRESS))
            ffurlConsoleAddText((meUByte *)"#",(meLineGetLength(ffurlBp->dotLine) >= frameCur->width - 2) ? 0x02:0x03) ;
    }
    else
#endif
#ifdef _WIN32
    {
        if(WriteFile(ffwp,ffbuf,ffremain,&written,NULL) == 0)
            written = -1 ;
    }
#else
        written = (int) fwrite(ffbuf,1,ffremain,ffwp) ;
#endif
    if(written != ffremain)
        return mlwrite(MWABORT,(meUByte *)"Write I/O error");
    ffremain = 0 ;
    return 0 ;
}

#define ffputc(c)                                                            \
(((ffbuf[ffremain++]=c),(ffremain == meFIOBUFSIZ))? ffputBuf():0)
    
int
ffWriteFileOpen(meUByte *fname, meUInt flags, meBuffer *bp)
{
    if(bp != NULL)
    {
#if MEOPT_CRYPT
        if(resetkey(bp) <= 0)
            return meFALSE ;
#endif
    }
    if(fname == NULL)
    {
#ifdef _WIN32
        ffwp = GetStdHandle(STD_OUTPUT_HANDLE) ;
#else
        ffwp = stdout ;
#endif
        ffnewFile = 0 ;
    }
    else if(isFtpLink(fname))
    {
#if MEOPT_SOCKET
#ifdef _UNIX
        /* must stop any signals or the connection will fail */
        meSigHold() ;
#endif
        if(ffUrlFileOpen(fname,NULL,NULL,(flags & (meRWFLAG_WRITE|meRWFLAG_BACKUP|meRWFLAG_DELETE|meRWFLAG_MKDIR))) <= 0)
        {
#ifdef _UNIX
            meSigRelease() ;
#endif
            return meABORT ;
        }
        if(!(flags & meRWFLAG_WRITE))
        {
            int rr ;
            rr = ffUrlFileClose(fname,flags & (meRWFLAG_WRITE|meRWFLAG_BACKUP|meRWFLAG_DELETE|meRWFLAG_MKDIR)) ;
#ifdef _UNIX
            meSigRelease() ;
#endif
            return rr ;
        }
        ffnewFile = 0 ;
#else
        return mlwrite(MWABORT|MWPAUSE,(meUByte *) "[No url support in this version]") ;
#endif
    }
    else
    {
        if(!(ffnewFile=meTestExist(fname)))
        {
            if(flags & meRWFLAG_BACKUP)
            {
                meUByte filename[meFILEBUF_SIZE_MAX] ;
                int ii ;
                
                ii = createBackupName(filename,fname,'~',1) ;
                if(!ii)
                {
#ifdef _WIN32
                    /* Win95 first version has nasty bug in MoveFile, doing
                     * 
                     * MoveFile("a.bbb","a.bbb~")
                     * CreateFile("a.bbb",..)
                     * 
                     * Resulted in a.bbb backing up to a.bbb~ and then for some unknown bloody
                     * reason opening a.bbb opens a.bbb~ instead of creating a new file.
                     * 
                     * So alternatively do:
                     *
                     * MoveFile("a.bbb","a.bbb~.~_~")
                     * MoveFile("a.bbb~.~_~","a.bbb~")
                     * CreateFile("a.bbb",..)
                     * 
                     * Which sadly works (we love you bill!)
                     * 
                     * This is done for all flavours of windows as these drives can be networked
                     */
                    extern int platformId;
                    meUByte filenameOldB[meFILEBUF_SIZE_MAX], *filenameOld ;
                    
                    if(!(meSystemCfg & meSYSTEM_DOSFNAMES))
                    {
                        strcpy(filenameOldB,fname) ;
                        strcat(filenameOldB,".~a~") ;
                        filenameOld = filenameOldB ;
                        if(!meTestExist(filenameOld) && meUnlink(filenameOld))
                            mlwrite(MWABORT|MWPAUSE,"[Unable to remove backup file %s]", filenameOld) ;
                        else if(meRename(fname,filenameOld) && (ffFileOp(fname,filenameOld,meRWFLAG_DELETE) <= 0))
                            mlwrite(MWABORT|MWPAUSE,"[Unable to backup file to %s]",filenameOld) ;
                    }
                    else
                        filenameOld = fname ;
#else
#define filenameOld fname
#endif
#ifndef _DOS
                    if(!(meSystemCfg & meSYSTEM_DOSFNAMES) && (keptVersions > 0))
                    {
                        int ll ;
                        
                        ll = meStrlen(filename)-1 ;
                        filename[ll++] = '.' ;
                        filename[ll++] = '~' ;
                        for(ii=0 ; ii<keptVersions ; ii++)
                        {
                            sprintf((char *)filename+ll,"%d~",ii) ;
                            if(meTestExist(filename))
                                break ;
                        }
                        if(ii == keptVersions)
                        {
                            if(meUnlink(filename))
                                mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Unable to remove backup file %s]", filename) ;
                            ii-- ;
                        }
                        if(ii)
                        {
                            meUByte filename2[meFILEBUF_SIZE_MAX] ;
                            meStrcpy(filename2,filename) ;
                            while(ii > 0)
                            {
                                sprintf((char *)filename2+ll,"%d~",ii--) ;
                                sprintf((char *)filename+ll,"%d~",ii) ;
                                if(meRename(filename,filename2) && (ffFileOp(filename,filename2,meRWFLAG_DELETE) <= 0))
                                {
                                    mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Unable to backup file to %s (%d - %s)]", 
                                            filename2,errno,sys_errlist[errno]) ;
                                    if(meUnlink(filename))
                                    {
                                        mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Unable to remove backup file %s]", filename) ;
                                        break ;
                                    }
                                }
                            }
                        }
                    }
#endif
                    if(!meTestExist(filename) && meUnlink(filename))
                        mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Unable to remove backup file %s]", filename) ;
                    else if(meRename(filenameOld,filename) && (ffFileOp(filenameOld,filename,meRWFLAG_DELETE) <= 0))
                        mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Unable to backup file to %s (%d - %s)]", 
                                filename,errno,sys_errlist[errno]) ;
                    else if(flags & 0xffff)
                        meChmod(filename,(flags & 0xffff)) ;
                }
                else
                    mlwrite(MWABORT|MWPAUSE,(meUByte *)"[Unable to backup file %s]",fname) ;
            }
            else if(flags & meRWFLAG_DELETE)
            {
                /* if backing up as well the file is already effectively deleted */
                if(meUnlink(fname))
                {
                    int ii = GetLastError() ;
                    printf("Error %d\n",ii) ;
                    return meABORT ;
                }
            }
        }
        if(!(flags & meRWFLAG_WRITE))
        {
            if(flags & meRWFLAG_MKDIR)
            {
#ifdef _WIN32
                if(CreateDirectory(fname,NULL) == 0)
                    return meABORT ;
#else
#ifdef _DOS
                if(mkdir((char *)fname,0) != 0)
                    return meABORT ;
#else
                if(mkdir((char *)fname,meXUmask) != 0)
                    return meABORT ;
#endif
#endif
            }
            return meTRUE ;
        }
#ifdef _WIN32
        {
            DWORD create ;
            if(flags & meRWFLAG_OPENTRUNC)
                create = TRUNCATE_EXISTING ;
            else if(flags & meRWFLAG_OPENEND)
                create = OPEN_ALWAYS ;
            else
                create = CREATE_ALWAYS ;
            /* Windows must open the file with the correct permissions to support the
             * compress attribute
             */
            if((ffwp=CreateFile(fname,GENERIC_WRITE,FILE_SHARE_READ,NULL,create,
                               ((bp == NULL) ? meUmask:bp->stats.stmode),
                               NULL)) == INVALID_HANDLE_VALUE)
                return mlwrite(MWABORT,(meUByte *)"[Cannot open file \"%s\" for writing - %d]",fname,GetLastError());
            if(flags & meRWFLAG_OPENEND)
                SetFilePointer(ffwp,0,NULL,FILE_END) ;
        }
#else
        {
            char *ss ;
            if(flags & meRWFLAG_OPENEND)
                ss = "ab" ;
            else
                ss = "wb" ;
            if((ffwp=fopen((char *)fname,ss)) == NULL)
                return mlwrite(MWABORT,(meUByte *)"Cannot open file [%s] for writing",fname);
        }
#endif
        mlwrite(MWCURSOR|MWCLEXEC,(meUByte *)"[%sriting %s]",
                (flags & meRWFLAG_AUTOSAVE) ? "Auto-w":"W",fname);	/* tell us were writing */
    }
    ffremain = 0 ;
    if(bp != NULL)
    {
        if(meModeTest(bp->mode,MDBINRY))
            ffbinary = meBINARY_BPL ;
        else if(meModeTest(bp->mode,MDRBIN))
            ffbinary = meRBIN_BPL ;
        else
            ffbinary = 0 ;
        ffcrypt  = (meModeTest(bp->mode,MDCRYPT) != 0) ;
        if(!ffbinary &&
#ifdef _CTRLZ
           (!meModeTest(bp->mode,MDAUTO) || meModeTest(bp->mode,MDCTRLZ))
#else
           (meModeTest(bp->mode,MDAUTO) && meModeTest(bp->mode,MDCTRLZ))
#endif
           )
            ffauto = 1 ;
        else
            ffauto = 0 ;
    
        if(!ffbinary &&
#ifdef _CTRLM
           (!meModeTest(bp->mode,MDAUTO) || meModeTest(bp->mode,MDCRLF))
#else
           (meModeTest(bp->mode,MDAUTO) && meModeTest(bp->mode,MDCRLF))
#endif
           )
            ffautoRet = 1 ;
        else
            ffautoRet = 0 ;
#if MEOPT_NARROW
        if((bp->narrow != NULL) && !(flags & meRWFLAG_IGNRNRRW))
            meBufferExpandNarrowAll(bp) ;
#endif
#if MEOPT_TIMSTMP
        if(!(flags & meRWFLAG_AUTOSAVE))
            set_timestamp(bp);			/* Perform time stamping */
#endif
    }
    else
    {
        ffbinary = 0 ;
        ffcrypt  = ((flags & meRWFLAG_CRYPT) != 0) ;
        ffauto   = 0 ;
        ffautoRet= 0 ;
    }
#ifdef _UNIX
#if MEOPT_SOCKET
    if(ffwp != ffpInvalidVal)
#endif
        meSigHold() ;
#endif

#if (defined _DOS) || (defined _WIN32)
    /* the directory time stamps on dos are not updated so the best
     * we can do is flag that the dirLists are out of date when WE
     * create a new file. Obviously if something else creates a file
     * then that file many be missed in hte completion lists etc
     */
    if(ffnewFile == 1)
    {
#if MEOPT_EXTENDED
        extern meDirList fileNames ;
        fileNames.timeStamp = 1 ;
#endif
        curDirList.timeStamp = 1 ;
    }
#endif
    return meTRUE ;
}


int
ffWriteFileWrite(register int len, register meUByte *buff, int eolFlag)
{
    meUByte cc, dd ;
    
    if(ffbinary)
    {
        if(ffbinary == meBINARY_BPL)
        {
            while(--len >= 0)
                if(*buff++ == ':')
                    break ;
            for(;;)
            {
                while((--len >= 0) && ((cc = *buff++) == ' '))
                    ;
                if((len < 0) || (cc == '|'))
                    break ;
                if(!isXDigit(cc) || (--len < 0) ||
                   ((dd = *buff++),!isXDigit(dd)))
                    return mlwrite(MWABORT|MWPAUSE,(meUByte *)"Binary format error");
                cc = (hexToNum(cc)<<4) | hexToNum(dd) ;
                if(ffputc(cc))
                    return meABORT ;
            }
        }
        else
        {
            while(--len >= 0)
            {
                cc = *buff++ ;
                if(!isXDigit(cc) || (--len < 0) ||
                   ((dd = *buff++),!isXDigit(dd)))
                    return mlwrite(MWABORT|MWPAUSE,(meUByte *)"[rbin format error]");
                cc = (hexToNum(cc)<<4) | hexToNum(dd) ;
                if(ffputc(cc))
                    return meABORT ;
            }
        }            
    }
    else
    {
        while(--len >= 0)
        {
            cc=*buff++ ;
            if(ffputc(cc))
                return meABORT ;
        }
        if(eolFlag)
        {
            if(ffautoRet && ffputc('\r'))
                return meABORT ;
            if(ffputc('\n'))
                return meABORT ;
        }
    }
    return meTRUE ;
}

int
ffWriteFileClose(meUByte *fname, meUInt flags, meBuffer *bp)
{
    int ret ;
    
    /* add a ^Z at the end of the file if needed */
    if((ffauto && ffputc(26)) || ffputBuf())
        ret = meABORT ;
#if MEOPT_SOCKET
    else if(ffwp == ffpInvalidVal)
        ret = ffUrlFileClose(fname,flags) ;
#endif
#ifdef _WIN32
    else if((ffwp != GetStdHandle(STD_OUTPUT_HANDLE)) && (CloseHandle(ffwp) == 0))
        ret = mlwrite(MWABORT|MWPAUSE,(meUByte *)"Error closing file");
#else
    else if((ffwp != stdout) && (fclose(ffwp) != 0))
        ret = mlwrite(MWABORT|MWPAUSE,(meUByte *)"Error closing file");
#endif
    else
        ret = meTRUE ;
#ifdef _UNIX
    meSigRelease() ;
#endif

#if MEOPT_NARROW
    if((bp != NULL) && (bp->narrow != NULL) && !(flags & meRWFLAG_IGNRNRRW))
        meBufferCollapseNarrowAll(bp) ;
#endif
    return ret ;
}


int
ffWriteFile(meUByte *fname, meUInt flags, meBuffer *bp)
{
    register int   ss ;
    
    flags |= meRWFLAG_WRITE ;
    if((ss=ffWriteFileOpen(fname,flags,bp)) == meTRUE)
    {
        register meLine *lp ;
        long noLines ;
        
        /* store the number of lines now cos the close may narrow the buffer
         * and the number of lines will be wrong */
        noLines = bp->lineCount+1 ;
        lp = meLineGetNext(bp->baseLine);            /* First line.          */
        while(lp != bp->baseLine)
        {
#if MEOPT_EXTENDED
            if(lp->flag & meLINE_MARKUP)
                noLines-- ;
            else
#endif
            {
                if((ss=ffWriteFileWrite(meLineGetLength(lp),meLineGetText(lp),
                                        !(lp->flag & meLINE_NOEOL))) <= 0)
                    break ;
            }
            lp = meLineGetNext(lp) ;
        }
        ffWriteFileClose(fname,flags,bp) ;
        if(ss > 0)
        {
            if(ffnewFile)
                mlwrite(MWCLEXEC,(meUByte *)"[New file %s, Wrote %d lines]",fname,noLines);
            else
                mlwrite(MWCLEXEC,(meUByte *)"[Wrote %d lines]",noLines);
            return meTRUE ;
        }
    }
    return meFALSE ;
}

int
ffFileOp(meUByte *sfname, meUByte *dfname, meUInt dFlags)
{
    int rr=meTRUE ;
    if((dfname != NULL) && (dFlags & meRWFLAG_DELETE))
    {
        /* simply move the file if the source is to be deleted and they are
         * regular files, the WriteOpen will handle backups etc */
        if(ffWriteFileOpen(dfname,meRWFLAG_DELETE|(dFlags & meRWFLAG_BACKUP),NULL) <= 0)
            return meABORT ;
        if(!meRename(sfname,dfname))
        {
            dFlags &= ~meRWFLAG_DELETE ;
            dfname = NULL ;
        }
    }
    if(dfname != NULL)
    {
        if(ffReadFileOpen(sfname,meRWFLAG_READ,NULL) <= 0)
            return meABORT ;
        if(ffWriteFileOpen(dfname,meRWFLAG_WRITE|(dFlags & meRWFLAG_BACKUP),NULL) <= 0)
        {
            ffReadFileClose(sfname,meRWFLAG_READ) ;
            return meABORT ;
        }
        for(;;)
        {
            if((rr=ffgetBuf()) <= 0)
                break ;
            if(ffremain < 0)
                break ;
            if(ffputBuf() != 0)
            {
                rr = meABORT ;
                break ;
        }
        }
        ffReadFileClose(sfname,meRWFLAG_READ) ;
        ffremain = 0 ;
        ffWriteFileClose(dfname,meRWFLAG_WRITE,NULL) ;
    }
    if((rr > 0) && (dFlags & (meRWFLAG_DELETE|meRWFLAG_MKDIR)))
    {
        rr = ffWriteFileOpen(sfname,dFlags & (meRWFLAG_DELETE|meRWFLAG_MKDIR|meRWFLAG_BACKUP),NULL) ;
    }
#if MEOPT_SOCKET
    if((rr > 0) && (dFlags & meRWFLAG_FTPCLOSE) && !meSocketIsBad(ffccsk))
    {
#ifdef _UNIX
        meSigRelease() ;
#endif
        if(!(dFlags & meRWFLAG_NOCONSOLE))
            ffUrlFileSetupFlags((dFlags & meRWFLAG_SILENT) ? 1:0) ;
        ffCloseSockets(1) ;
#ifdef _UNIX
        meSigRelease() ;
#endif
    }
#endif
    return rr ;
}
