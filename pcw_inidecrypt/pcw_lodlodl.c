//
// PC-Wahl 10.0 INI-Passwort Decrypter (c) 2017 
// lodlod-decrapter by Thorsten (THS) Schroeder <ths [at] ccc [dot] de>
//

/*
   BSD 3-Clause License

   Copyright (c) 2017, Chaos Computer Club
   Author: Thorsten (THS) Schroeder <ths [at] ccc [dot] de>
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

#include <windows.h>
#include <Strsafe.h>
#include <Shlwapi.h>
#include <stdio.h>

#include "pcw_lodlodl.h"
#include "helper.h"

char * lodlodl_decrapt(char *ciphertext_string)
{
   char *ciphertext_bytes = NULL;
   char *plaintext_string = NULL;
   
   char b = 0;
   size_t slen = 0;
   size_t clen = 0;
   int i = 0;
   int j = 0;
   
   char key = 0;
   
   if(ciphertext_string == NULL)
      return NULL;
      
   slen = strlen(ciphertext_string);
   clen = slen/2;
    
   ciphertext_bytes = (char *)malloc(clen);
   plaintext_string = (char *)malloc((clen) + 1);
   
   if(ciphertext_bytes == NULL || plaintext_string == NULL)
      return NULL;
   
   memset(plaintext_string, 0, (clen) + 1);
      
   for(i=0,j=0; i<slen; i+=2) {
      ciphertext_bytes[j]  = decode_nibble(ciphertext_string[i]) << 4;
      ciphertext_bytes[j] |= decode_nibble(ciphertext_string[i+1]) & 0xF;
      j++;
   }
   
   key = 0xe9;
   
   for(i=0; i<clen; i++) {
      plaintext_string[i] = ciphertext_bytes[i] ^ key;
      key -= 2;
   }
   
   if(ciphertext_bytes) {
      free(ciphertext_bytes);
      ciphertext_bytes = NULL;
   }
      
   return plaintext_string;
}

