# Walruss PC-Wahl 10 Studio Patcher

**PC-Wahl 10 Studio Patcher** is a program that can be used to swap votes for party 1 and party 2 during final and preliminary result submission.

## Build

To build the program via Microsoft Developer Command Prompt, go to the source directory and execute the build.bat batch file. 

## Usage

The command line syntax is the following:

```
studiopatch.exe studio.exe
```

Afterwards, the resulting XML will show, that instead of the original output

```xml
...
<Stimme1 UnSt="0" GuSt="800">
<Partei Id="0002">400</Partei>
<Partei Id="0001">300</Partei>
...
```

the following output is generated for submission:

```xml
...
<Stimme1 UnSt="0" GuSt="800">
<Partei Id="0001">400</Partei>
<Partei Id="0002">300</Partei>
...
```

## PC-Wahl 10 studio patch and shellcode

The following patches to the studio.exe executable are applied:

```c
[...]
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
[...]
```

## Authors

PC-Wahl 10 Studio Patcher (c) 2017 
by Thorsten (THS) Schroeder and Martin Tschirsich

