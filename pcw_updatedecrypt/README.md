# Walruss PC-Wahl 10 Update Decryptor

**PC-Wahl 10 Update Decryptor** is a program that can be used to extract and build update packages for PC-Wahl 10 installations that are deployed on computers, used in election offices.

## Build

To build the program via Microsoft Developer Command Prompt, go to the source directory and execute the build.bat batch file. 

## Usage

The command line syntax is the following:

```
pcw_updatedecrypt.exe -x <input-file> <output-directory>
pcw_updatedecrypt.exe -c <input-directory> <output-file>
```

To **extract** a downloaded update package, unzip the downloaded file (e. g. pcw10dat1.zip) and pass the resulting encrypted package pcw10dat1.010 to pcw_updatedecrypt.exe using the `-x` flag: 

`pcw_updatedecrypt.exe -x pcw10dat1.010 pcw10dat1-extracted`

To **create** an update package, pass the path to the directory-structure to pcw_updatedecrypt.exe using the `-c` flag: 

`pcw_updatedecrypt.exe -c pcw10dat1-malicious pcw10dat1.010`

The resuting file must then be zipped again, using any zip compatible program.

## PC-Wahl 10 update file encraption

The encrypted file (extrension .010) has a 16 byte file-header. The first 4 bytes is a file magic field. The subsequent data is encrypted 0x2800 byte block-wise, using the following algorithm:

```c
char secret_key[] = {
   0x20, 0x6f, 0x17, 0x48, 0x40, 0x79, 0x0c, 0x0c, 
   0x98, 0x64, 0x03, 0x4d, 0x4e, 0xd3, 0x91, 0x25, 
   0x85, 0x0c,
   0x20, 0x6f, 0x17, 0x48, 0x40, 0x79, 0x0c, 0x0c, 
   0x98, 0x64, 0x03, 0x4d, 0x4e, 0xd3, 0x91, 0x25, 
   0x85, 0x0c };
   
[...]
   unsigned int i = 1;
   int counter = 0;
   char key;
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
[...]
```

## Authors

PC-Wahl 10.0 Update-Package Decrypter (c) 2017 
by Thorsten (THS) Schroeder <ths [at] ccc [dot] de>

