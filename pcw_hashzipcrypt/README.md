# Walruss PC-Wahl 10.0 studio.hash zip - Password Extractor

**PC-Wahl 10.0 studio.hash zip - Password Extractor** is a program that can be used to recover secret studiohash.zip-passwords from PC-Wahl 10 studio.exe program.

## Build

To build the program via Microsoft Developer Command Prompt, go to the source directory and execute the build.bat batch file. 

## Usage

```
>pcw_hashzipcrypto.exe
[!] usage: pcw_hashzipcrypto.exe [-c|-x] <version> <exepath> <zippath> <directory>
[!] Examples:
[!]    -x 170905 <path\studio.exe> <path\studiohash.zip> studiohash-outputdir - extract studiohash.zip
[!]    -c 170905 <path\studio.exe> <path\studiohash.zip> studiohash-inputdir  - create studiohash.zip
[!]
[!] Supported versions:
[!]    170905: studio.exe shipped with update from September 5th, 2017.
```

The latest version of September 13th has not been implemented yet.

## PC-Wahl 10.0 studio.hash zip - Password Extractor

The masqueraded AES-password for the studiohash.zip file is located at a certain offset in studio.exe. This tool can be used to extract and unmasq the secret password for the ZIP-file. The following algorithm has been used to masquerade the password:

```
char * hashzip_pw_demasq(char *buf, unsigned int len)
{
   int i = 0;
   
   for(i=0; i<len; i++) {
      buf[i] = 0xFF - buf[i];
   }
   buf[i] = 0;

   return buf;
}
```

The algorithm to process/calculate the content of the ZIP file will be released later.

## Authors

PC-Wahl 10.0 studio.hash zip - Password Extractor (c) 2017 
Thorsten (THS) Schroeder <ths [at] ccc [dot] de>

