//
// PC-Wahl 10.0 studio patch (c) 2017 
// by Martin Tschirsich & Thorsten (THS) Schroeder
//

/*
   BSD 3-Clause License

   Copyright (c) 2017, Chaos Computer Club
   Author: Thorsten (THS) Schroeder <ths [at] ccc [dot] de>
   Author: Martin Tschirsich
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

   * Neither the name of the copyright holder nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   * NON-MILITARY-USAGE CLAUSE
     Redistribution and use in source and binary form for military use and
     military research is not permitted. Infringement of these clauses may
     result in publishing the source code of the utilizing applications and
     libraries to the public. As this software is developed, tested and
     reviewed by *international* volunteers, this clause shall not be refused
     due to the matter of *national* security concerns.
    
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
   SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
   OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

// target file offset: 00588fbb
// shellcode len: 2

char disable_selfcheck[] = {
/* 00989BBB */      "\xEB\x13"               /* jmp     short studio.00989BD0 */
};

// target file offset: 002af0d4
// shellcode len: 5

char call_swapfunc[] = {
/* 006AFCD4 */      "\xE8\x99\x6B\xDE\xFF"   /* call    studio.00496872 */
};

// target file offset: 00095c72
// shellcode len: 30

char swapfunc[] = {
/* 00496872 */      "\x83\xF8\x01"           /* cmp     eax, 1 */
/* 00496875 */      "\x75\x07"               /* jnz     short studio.0049687E */
/* 00496877 */      "\xB8\x02\x00\x00\x00"   /* mov     eax, 2 */
/* 0049687C */      "\xEB\x0A"               /* jmp     short studio.00496888 */
/* 0049687E */      "\x83\xF8\x02"           /* cmp     eax, 2 */
/* 00496881 */      "\x75\x05"               /* jnz     short studio.00496888 */
/* 00496883 */      "\xB8\x01\x00\x00\x00"   /* mov     eax, 1 */
/* 00496888 */      "\xE8\x2F\x1E\x02\x00"   /* call    studio.004B86BC */
/* 0049688D */      "\xC3"                   /* retn */
/* 0049688E */      "\x90"                   /* nop  */
/* 0049688F */      "\x90"                   /* nop  */
};

#include <stdio.h>

int main(int ac, char** av)
{
   char *studio = NULL;
   
   FILE *fd = NULL;
   
   if(ac < 2) {
      printf("[!] usage: %s studio.exe\n", av[0]);
      return -1;
   }
   
   studio = av[1];
   
   fd = fopen(studio, "rb+");
   if(fd == NULL) {
      printf("[e] fopen(%s, \"rb+\") failed.\n", studio);
      return -1;
   }
   
   fseek(fd, 0x00095c72, SEEK_SET);
   fwrite(swapfunc, 30, 1, fd);
   
   fseek(fd, 0x002af0d4, SEEK_SET);
   fwrite(call_swapfunc, 5, 1, fd);
   
   fseek(fd, 0x00588fbb, SEEK_SET);
   fwrite(disable_selfcheck, 2, 1, fd);
   
   fflush(fd);
   
   fclose(fd);
   
   return 0;
}