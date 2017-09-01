/*
 * Copyright (c) 2017
 *    Thorsten Schroeder <ths at ccc dot de>
 *
 * All rights reserved.
 *
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Thorsten Schroeder <ths at modzero dot ch> wrote this file. As long as you 
 * retain this notice you can do whatever you want with this stuff. If we meet 
 * some day, and you think this stuff is worth it, you can buy me a beer in 
 * return. Thorsten Schroeder.
 *
 * NON-MILITARY-USAGE CLAUSE
 * Redistribution and use in source and binary form for military use and 
 * military research is not permitted. Infringement of these clauses may
 * result in publishing the source code of the utilizing applications and 
 * libraries to the public. As this software is developed, tested and
 * reviewed by *international* volunteers, this clause shall not be refused 
 * due to the matter of *national* security concerns.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE DDK PROJECT BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: helper.c
 * Description: Implementation of helper functions.
 */

#include <string.h>
#include <ctype.h>

#include <stdio.h>

#include "helper.h"

// bin to hexascii nibble
static const unsigned char _tab[] = "0123456789ABCDEF";

void print_hex_byte(char b)
{
    putchar( _tab[ (b >> 4) & 0xF ] );
    putchar( _tab[ (b >> 0) & 0xF ] );
}

void print_hex_word(int16_t w)
{
    putchar( _tab[ (w >> 12) & 0xF ] );
    putchar( _tab[ (w >>  8) & 0xF ] );
    putchar( _tab[ (w >>  4) & 0xF ] );
    putchar( _tab[ (w >>  0) & 0xF ] );
}


void print_hex_dword(int32_t d)
{
    putchar( _tab[ (d >> 28) & 0xF ] );
    putchar( _tab[ (d >> 24) & 0xF ] );
    putchar( _tab[ (d >> 20) & 0xF ] );
    putchar( _tab[ (d >> 16) & 0xF ] );
    putchar( _tab[ (d >> 12) & 0xF ] );
    putchar( _tab[ (d >>  8) & 0xF ] );
    putchar( _tab[ (d >>  4) & 0xF ] );
    putchar( _tab[ (d >>  0) & 0xF ] );
}

void print_hex_buf(char *s, uint32_t l)
{

    while(l--) {
        putchar( _tab[ (*s >> 4) & 0xF ] );
        putchar( _tab[ (*s >> 0) & 0xF ] );
        s++;
    }
}


/*
 * print data in rows of 16 bytes: offset   hex   ascii
 *
 * 00000   47 45 54 20 2f 20 48 54  54 50 2f 31 2e 31 0d 0a   GET / HTTP/1.1..
 */
void print_hex_ascii_line(const unsigned char *payload, int slen, int offset)
{

   int i;
   int gap, len;
   const unsigned char *ch, *ch2;

   len = 16;
   ch2 = ch = payload;

  do {

      /* offset */
      //printf("%05x   ", offset);
      print_hex_word(offset & 0xffff);
      putchar(' ');

      /* hex */
      if(slen < len)
         len=slen;

      for(i = 0; i < len; i++) {

         //printf("%02x ", *ch);

         print_hex_byte(*ch);
         putchar(' ');

         ch++;
         /* print extra space after 8th byte for visual aid */
         if (i == 7)
            putchar(' ');
      }
      /* print space to handle line less than 8 bytes */
      if (len < 8)
         putchar(' ');

      /* fill hex gap with spaces if not full line */
      if (len < 16) {
         gap = 16 - len;
         for (i = 0; i < gap; i++) {
            printf("   ");
         }
      }
      printf("   ");

      /* ascii (if printable) */
      for(i = 0; i < len; i++) {
         if (isprint(*ch2)){
            printf("%c", *ch2);
         } else
            putchar(' ');
         ch2++;
      }

      puts("");
      slen -= 16;
      offset += len;
   } while( slen > 0 ) ;


   return;
}


