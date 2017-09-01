rem -- stuff made by ths

@echo off

rem /Zi for the debugz

cl /W2 /MT /Fepcw_updatedecrypt.exe pcw_deupd.c helper.c /I3rd-party\zlib-1.2.11 /I3rd-party\zlib-1.2.11\contrib\minizip 3rd-party\zlib-1.2.11\contrib\minizip\unzip.c 3rd-party\zlib-1.2.11\contrib\minizip\zip.c 3rd-party\zlib-1.2.11\contrib\minizip\miniunz.c 3rd-party\zlib-1.2.11\contrib\minizip\minizip.c 3rd-party\zlib-1.2.11\inflate.c 3rd-party\zlib-1.2.11\deflate.c 3rd-party\zlib-1.2.11\crc32.c 3rd-party\zlib-1.2.11\adler32.c 3rd-party\zlib-1.2.11\inffast.c         3rd-party\zlib-1.2.11\contrib/minizip/iowin32.c 3rd-party\zlib-1.2.11\contrib/minizip/ioapi.c 3rd-party\zlib-1.2.11\zutil.c 3rd-party\zlib-1.2.11\trees.c 3rd-party\zlib-1.2.11\inftrees.c
