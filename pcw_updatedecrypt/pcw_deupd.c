//
// PC-Wahl 10.0 Update-Archive Decrypter/Creator (c) 2017 
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

#include "helper.h"
#include "unzip.h"
#include "zip.h"

#define DEBUG 1
#define WRITEBUFFERSIZE (16384)
#define MAXFILENAME (256)

#define FOPEN_FUNC(filename, mode) fopen64(filename, mode)

typedef struct _pcwupdate_fileheader {
   uint32_t magic;
   uint32_t unk1;
   uint32_t unk2;
   uint32_t unk3;
} pcw_update_header;

char secret_key[] = {
   0x20, 0x6f, 0x17, 0x48, 0x40, 0x79, 0x0c, 0x0c, 
   0x98, 0x64, 0x03, 0x4d, 0x4e, 0xd3, 0x91, 0x25, 
   0x85, 0x0c,
   0x20, 0x6f, 0x17, 0x48, 0x40, 0x79, 0x0c, 0x0c, 
   0x98, 0x64, 0x03, 0x4d, 0x4e, 0xd3, 0x91, 0x25, 
   0x85, 0x0c };
   
int pcwahl_upd_crypt_data(char *result, size_t resultsize, char *input)
{
   unsigned int i = 1;
   int counter = 0;
   char key = 0;
   
   if(result == NULL || input == NULL) {
      printf("[!] result == NULL || ciphertext == NULL\n");
      return -1;
   }
      
   ZeroMemory(result, sizeof(result));
   
   do {
      
      counter++;
      counter &= 0x0F;
      counter += 0x0D;
      key     = *(secret_key + counter - 3) & 0xFF;
      counter = i;
      counter = counter & 0xFF;
      key     = key ^ counter;
      
      result[i-1] = input[i-1] ^ key;
      i++;
        
   } while(i <= 0x2800);
   
   return 0;
}


int pcwahl_upd_zipextract(char *zipfile, char *directory)
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
   
   do_extract(uf, 0, 1, NULL);

   unzClose(uf);
   
   SetCurrentDirectory(current_dir);
   return 0;
}

int pcwahl_zip_file_add(zipFile zf, char *file)
{
   int err=0;
   int size_buf=0;
   void* buf=NULL;

   FILE * fin;
   int size_read;
   const char* filenameinzip = file;
   const char *savefilenameinzip;
   zip_fileinfo zi;
   unsigned long crcFile=0;
   int zip64 = 0;

   zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour = 0;
   zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
   zi.dosDate = 0;
   zi.internal_fa = 0;
   zi.external_fa = 0;

   size_buf = WRITEBUFFERSIZE;
   buf = (void*)malloc(size_buf);
   
   if (buf==NULL) {
      printf("[e] pcwahl_zip_file_add: pcwahl_zip_file_add: Error allocating memory\n");
      return ZIP_INTERNALERROR;
   }

   filetime(filenameinzip, &zi.tmz_date, &zi.dosDate);
   getFileCrc(filenameinzip, buf, size_buf, &crcFile);
   zip64 = isLargeFile(filenameinzip);

   /* The path name saved, should not include a leading slash. */
   /*if it did, windows/xp and dynazip couldn't read the zip file. */
   savefilenameinzip = filenameinzip;
   
   while( savefilenameinzip[0] == '\\' || savefilenameinzip[0] == '/' ) {
      savefilenameinzip++;
   }

   err = zipOpenNewFileInZip3_64(zf, savefilenameinzip, &zi,
                  NULL, 0, NULL, 0, NULL,
                  Z_DEFLATED, 9,
                  0,
                  /* -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY, */
                  -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                  NULL, crcFile, zip64);

   if (err != ZIP_OK) {
     printf("[e] pcwahl_zip_file_add: error in opening %s in zipfile\n", filenameinzip);
   } else {
      fin = FOPEN_FUNC(filenameinzip, "rb");
      if (fin == NULL) {
         err = ZIP_ERRNO;
         printf("[e] pcwahl_zip_file_add: error in opening %s for reading\n", filenameinzip);
      }
   }

   if (err == ZIP_OK)
      do {
         err = ZIP_OK;
         size_read = (int)fread(buf, 1, size_buf, fin);
         
         if (size_read < size_buf)
             if (feof(fin) == 0) {
             printf("[e] pcwahl_zip_file_add: error in reading %s\n",filenameinzip);
             err = ZIP_ERRNO;
         }

         if (size_read > 0)
         {
             err = zipWriteInFileInZip(zf, buf, size_read);
             if (err<0)
             {
                 printf("[e] pcwahl_zip_file_add: error in writing %s in the zipfile\n", filenameinzip);
             }

         }
     } while ((err == ZIP_OK) && (size_read>0));

   if (fin)
     fclose(fin);

   if (err<0) {
     err = ZIP_ERRNO;
   } else {
     err = zipCloseFileInZip(zf);
     if (err != ZIP_OK)
         printf("[e] pcwahl_zip_file_add: error in closing %s in the zipfile\n", filenameinzip);
   }
   
   return 0;
}

unsigned int count;

int pcwahl_zip_directory(zipFile zf, char *base_dir)
{
   HANDLE hfind = INVALID_HANDLE_VALUE;
   WIN32_FIND_DATA ffd;
   char ff_dir[_MAX_PATH];
   
   ZeroMemory( &ff_dir, sizeof(ff_dir) );
   
   // PathAppendA(ff_dir, "*");
   // StringCchCopyA(ff_dir, _MAX_PATH, base_dir);
   StringCchPrintfA(ff_dir, _MAX_PATH, "%s\\*", base_dir);
   
   hfind = FindFirstFile(ff_dir, &ffd);
   
   if (INVALID_HANDLE_VALUE == hfind) {
      if(DEBUG) printf("[e] pcwahl_zip_directory: [%d] find files in %s failed: %x\n", count, ff_dir, GetLastError());
      return -1;
   }

   do {
      char tmppath[_MAX_PATH];
            
      if( ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY 
            && strcmp(ffd.cFileName, ".") != 0 
            && strcmp(ffd.cFileName, "..") != 0 ) {
               
         StringCchPrintf(
            tmppath,
            _MAX_PATH,
            "%s\\%s",
            base_dir, ffd.cFileName);
            
         pcwahl_zip_directory(zf, tmppath);
         
      } else if ( strcmp(ffd.cFileName, ".") != 0 
               && strcmp(ffd.cFileName, "..") != 0) {

         if( (*base_dir == '.') && ( (*(base_dir+1) == '\\') || (*(base_dir+1) == '/') ) )
            StringCchPrintf(tmppath, _MAX_PATH, "%s\\%s", base_dir+2, ffd.cFileName);
         else if(strcmp(base_dir, ".") == 0)
            StringCchPrintf(tmppath, _MAX_PATH, "%s", ffd.cFileName);
         else
            StringCchPrintf(tmppath, _MAX_PATH, "%s\\%s", base_dir, ffd.cFileName);
            
         pcwahl_zip_file_add(zf, tmppath);
         count++;
      }
      
   } while (FindNextFileA(hfind, &ffd) != 0);
   
   return count;
}


int pcwahl_upd_zipcreate(char *directory, char *zipfile)
{
   zipFile zf = NULL;
   unsigned int count = 0;
   char current_dir[_MAX_PATH];
   
   zlib_filefunc64_def ffunc;
   
   GetCurrentDirectory(_MAX_PATH, current_dir);
   SetCurrentDirectory(directory);
      
   fill_win32_filefunc64A(&ffunc);
   zf = zipOpen2_64(zipfile, 0, NULL, &ffunc);

   if (zf == NULL) {
      printf("[e] pcwahl_upd_zipcreate: zipOpen2_64 failed.\n");
      return -1;   
   } 
   
   printf("[*] creating %s\n", zipfile);
   
   count = pcwahl_zip_directory(zf, ".");
   zipClose(zf,NULL);
   
   SetCurrentDirectory(current_dir);
   printf("[+] added %d files to %s\n", count, zipfile);
   return 0;
}

int pcw_extract_package(char *source_package, char *target_directory)
{
   BOOL result = FALSE;
   int resultlen = 0;
   int resultwritten = 0;
   int totalwritten = 0;
   int i = 0;
   pcw_update_header fileheader;
   HANDLE hFile, hFileOut; 
   char plaintext[0x2800];
   char block[0x2800];

   char temp_dir[_MAX_PATH];
   char temp_file[_MAX_PATH];
   GetTempPathA(_MAX_PATH, temp_dir);
   GetTempFileNameA(temp_dir, "pcwx", 0, temp_file);
   
   ZeroMemory(&plaintext, sizeof(plaintext));
   ZeroMemory(&fileheader, sizeof(fileheader));
   ZeroMemory(&block, sizeof(block));
   
   hFile = CreateFile(source_package, 
         GENERIC_READ, 
         FILE_SHARE_READ,
         NULL,
         OPEN_EXISTING,
         FILE_ATTRIBUTE_NORMAL,
         NULL);

   if (hFile == INVALID_HANDLE_VALUE) 
   { 
      printf("[e] pcw_extract_package: CreateFile: unable to open file \"%s\" for read. getlasterror: %x\n", source_package, GetLastError());
      return -1; 
   }
   
   hFileOut = CreateFile(temp_file, 
         GENERIC_WRITE, 
         0,
         NULL,
         CREATE_ALWAYS,
         FILE_ATTRIBUTE_NORMAL,
         NULL);

   if (hFileOut == INVALID_HANDLE_VALUE) 
   { 
      printf("[e] pcw_extract_package: CreateFile: unable to open file \"%s\" for write. getlasterror: %x\n", temp_file, GetLastError());
      return -1; 
   }
   
   result = ReadFile(hFile, &fileheader, sizeof(fileheader), &resultlen, NULL);
   
   if(result == FALSE) {
      printf("[e] pcw_extract_package: ReadFile: read %d byte - getlasterror: %x\n", resultlen, GetLastError());
      return -1;
   }
   
   printf("[*] file header:\n");
   printf("[+]    magic: %08x\n", fileheader.magic);
   printf("[+]    unk 1: %08x\n", fileheader.unk1);
   printf("[+]    unk 2: %08x\n", fileheader.unk2);
   printf("[+]    unk 3: %08x\n", fileheader.unk3);
   
   if(fileheader.magic != 0x00cf1974) {
      
      printf("[e] pcw_extract_package: file header magic does not match.\n");
      CloseHandle(hFile);
      CloseHandle(hFileOut);
      return -1;
   }
      
   do {
      result = ReadFile(hFile, block, sizeof(block), &resultlen, NULL);
      
      if(result == FALSE) {
         printf("[e] pcw_extract_package: ReadFile: read %d byte - getlasterror: %x\n", resultlen, GetLastError());
         break;
      }
      
      pcwahl_upd_crypt_data(plaintext, sizeof(plaintext), block);
      
      WriteFile(
            hFileOut,
            plaintext,
            resultlen,
            &resultwritten,
            NULL);  
            
      totalwritten += resultwritten;
      
   } while(resultlen > 0); 
   
   CloseHandle(hFile);
   CloseHandle(hFileOut);

   printf("[*] %d bytes written to %s\n", totalwritten, temp_file);
   pcwahl_upd_zipextract(temp_file, target_directory);
   
   DeleteFile(temp_file);
   
   return 0;
}


int pcw_build_package(char *source_directory, char *target_package)
{
   BOOL result = FALSE;
   int resultlen = 0;
   int resultwritten = 0;
   int totalwritten = 0;
   int i = 0;
   HANDLE hFile, hFileOut; 
   char plaintext[0x2800];
   char block[0x2800];
   
   pcw_update_header fileheader;
   char temp_dir[_MAX_PATH];
   char temp_file[_MAX_PATH];
   GetTempPathA(_MAX_PATH, temp_dir);
   GetTempFileNameA(temp_dir, "pcwx", 0, temp_file);
   
   ZeroMemory(&plaintext, sizeof(plaintext));
   ZeroMemory(&fileheader, sizeof(fileheader));
   ZeroMemory(&block, sizeof(block));
   
   pcwahl_upd_zipcreate(source_directory, temp_file);
   
   // open temp_file and output file
   
   hFile = CreateFile(temp_file, 
         GENERIC_READ, 
         FILE_SHARE_READ,
         NULL,
         OPEN_EXISTING,
         FILE_ATTRIBUTE_NORMAL,
         NULL);

   if (hFile == INVALID_HANDLE_VALUE) 
   { 
      printf("[e] pcw_build_package: CreateFile: unable to open file \"%s\" for read.\n", temp_file);
      return -1; 
   }
   
   hFileOut = CreateFile(target_package, 
         GENERIC_WRITE, 
         0,
         NULL,
         CREATE_ALWAYS,
         FILE_ATTRIBUTE_NORMAL,
         NULL);

   if (hFileOut == INVALID_HANDLE_VALUE) 
   { 
      printf("[e] pcw_build_package: CreateFile: unable to open file \"%s\" for write.\n", target_package);
      return -1; 
   }
   
   // write file header to output file
   
   fileheader.magic = 0x00cf1974;
   fileheader.unk1  = 0x3e;
   fileheader.unk1  = 0x3e;
   fileheader.unk3  = 0;

   WriteFile(hFileOut, &fileheader, 16, &resultwritten, NULL);  

   // encrypt temp_file and store output
   
   do {
      result = ReadFile(hFile, block, sizeof(block), &resultlen, NULL);
      
      if(result == FALSE) {
         printf("ReadFile: read %d byte - getlasterror: %x\n", resultlen, GetLastError());
         break;
      }
      
      pcwahl_upd_crypt_data(plaintext, sizeof(plaintext), block);
      WriteFile(hFileOut, plaintext, resultlen, &resultwritten, NULL);  
      totalwritten += resultwritten;
      
   } while(resultlen > 0); 
   
   // close file handles
   CloseHandle(hFile);
   CloseHandle(hFileOut);

   printf("[*] %d bytes written to %s\n[*] Press any key to quit.\n", totalwritten, target_package);
   
   DeleteFile(temp_file);
   
   return 0;
}


int main(int ac, char** av)
{
   if(ac<4) {
      printf("[!] usage: %s [-x|-c] <input> <output>\n", av[0]);
      printf("[*] Press enter to quit.");
      getchar();
      return 1;  
   }
   
   if(StrCmp(av[1], "-x") == 0) { 
      pcw_extract_package(av[2], av[3]);
   } else {
      pcw_build_package(av[2], av[3]);
   }
   
   printf("[*] Press enter to quit.");
   getchar();
   return 0;
}

