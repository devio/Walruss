# Walruss PC-Wahl 10 Password Decryptor

**PC-Wahl 10 Password Decryptor** is a program that can be used to recover secret passwords from PC-Wahl 10 configuration (INI) files.

## Build

To build the program via Microsoft Developer Command Prompt, go to the source directory and execute the build.bat batch file. 

## Usage

To dump and highlight decrypted passwords from ini-files, execute pcw_inidecrypt.exe with the path to PC-Wahl 10 directory as an argument. Example: `pcw_inidecrypt.exe "c:\wahlruss\PC-Wahl 10"`

pcw_inidecrypt.exe will recursivly go through the directories to find all ini-files. If an ini-file contains an "encrypted" value, it will be decrypted using *PC-Wahl 10 ini-encraption* and highlighted.

## PC-Wahl 10 ini-encraption

The ciphertext is encoded using a decimal octet string representation of binary data. E. g. a binary sequence in hex {0xff, 0x05, 0x7F} is encoded as ASCII string "255005127". The encryption algorithm does not even use a secret key:

```c
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
```

## Authors

PC-Wahl 10.0 INI-Passwort Decrypter (c) 2017 
by xonox and Thorsten (THS) Schroeder <ths [at] ccc [dot] de>

