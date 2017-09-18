//
// PC-Wahl 10.0 studio.hash zip - Password Extractor (c) 2017 
// by Thorsten (THS) Schroeder <ths [at] ccc [dot] de>
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
#include <stdint.h>

#include "pcw_hashzipcrypto.h"
#include "helper.h"

#include "unzip.h"
#include "zip.h"

static int pcwahl_hash_zipextract(char *zipfile, char *key, char *directory);

// PC-Wahl Version 05. September 2017, encrypted ZIP password LEN 
#define PCW170905_CT_OFFSET   0x58907c

int main(int ac, char** av)
{
   char *pwbuf = NULL;
   char *studioexe_path = NULL;
   char *studiohashzip_path = NULL;
   char *studiohashdir_path = NULL;
   char *studiohash_path = "C:\\Users\\ths\\Dev\\github\\Walruss\\pcw_hashzipcrypt\\studiohash_170905\\studio.hash";
   char studiohash_data[1024] = {0};
   
   unsigned int extract = 0;
   unsigned int version = 0;
   unsigned int file_offset = 0;
   unsigned int br = 0;
   
   uint8_t ciphertext_len = 0;
   
   FILE *fd = NULL;
   
   if(ac < 5) {
      printf("[!] usage: %s [-c|-x] <version> <exepath> <zippath> <directory>\n", av[0]);
      printf("[!] Examples:\n");
      printf("[!]    -x 170905 <path\\studio.exe> <path\\studiohash.zip> studiohash-outputdir - extract studiohash.zip\n");
      printf("[!]    -c 170905 <path\\studio.exe> <path\\studiohash.zip> studiohash-inputdir  - create studiohash.zip\n");
      printf("[!]\n[!] Supported versions:\n");
      printf("[!]    170905: studio.exe shipped with update from September 5th, 2017.\n");
      
      return -1;
   }

   if(StrCmp(av[1], "-x") == 0) { 
      extract = 1;
   } else {
      extract = 0;
   }
   
   version = atoi(av[2]);
   studioexe_path     = av[3];
   studiohashzip_path = av[4];
   studiohashdir_path = av[5];
   
   printf("[d] extract: %d\n", extract);
   printf("[d] version: %d\n", version);
   printf("[d] exe: %s\n", studioexe_path);
   printf("[d] zip: %s\n", studiohashzip_path);
   printf("[d] dir: %s\n", studiohashdir_path);
   
   switch(version) {
      case 170905:
         file_offset = PCW170905_CT_OFFSET;
         break;
      default:
         printf("[!] unknown version: %d - please contact me at ths [at] ccc [dot] de and let me know.\n", version);
         file_offset = 0;
         break;
   }
   
   if(file_offset == 0)
      return -1;
   
   fd = fopen(studioexe_path, "r");
   
   if(fd == NULL) {
      printf("[e] fopen(%s, \"rb\") failed.\n", studioexe_path);
      return -1;
   }
   
   fseek(fd, file_offset, SEEK_SET);
   br = fread(&ciphertext_len, 1, 1, fd);
   
   pwbuf = (char *) malloc(ciphertext_len + 1);
   
   if(pwbuf == NULL) {
      printf("[e] malloc(%d) failed.\n", ciphertext_len + 1);
      return -1;
   }
   
   memset(pwbuf, 0, ciphertext_len + 1);
   
   br = fread(pwbuf, ciphertext_len, 1, fd);
   
   hashzip_pw_demasq(pwbuf, ciphertext_len);
   
   printf("[+] studio.hash zip password from studio.exe: %s\n", pwbuf);
   
   pcwahl_hash_zipextract(studiohashzip_path, pwbuf, studiohashdir_path);
   
   fclose(fd);

  fd = fopen(studiohash_path, "r");
   
   if(fd == NULL) {
      printf("[e] fopen(%s, \"rb\") failed.\n", studioexe_path);
      return -1;
   }
   
   fread(&studiohash_data, 1024, 1, fd);
   fclose(fd);
   
   printf("[d] vorher:\n");
   print_hex_ascii_line(studiohash_data, 128, 0);
   
   // :-)
   //hashzip_hash_decrypt(studiohash_data, 100);
   //printf("[d] nachher:\n");
   //print_hex_ascii_line(studiohash_data, 128, 0);
   
   return 0;
}

static int pcwahl_hash_zipextract(char *zipfile, char *key, char *directory)
{
   zlib_filefunc64_def ffunc;
   unzFile uf=NULL;
   char current_dir[_MAX_PATH];

   GetCurrentDirectory(_MAX_PATH, current_dir);
   
   fill_win32_filefunc64A(&ffunc);
   uf = unzOpen2_64(zipfile, &ffunc);
   
   if(uf == NULL) {
      printf("[e] pcwahl_upd_zipextract: unzOpen failed.\n");
      return -1;   
   }
   
   printf("[d] opened zipfile: %s\n", zipfile);

   CreateDirectory(directory, NULL);
   SetCurrentDirectory(directory);
   
   do_extract(uf, 0, 1, key);

   unzClose(uf);
   
   SetCurrentDirectory(current_dir);
   return 0;
}

