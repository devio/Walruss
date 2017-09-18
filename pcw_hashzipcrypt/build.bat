rem -- stuff made by ths

@echo off

rem for the XPz...
set LINK=""

set LINK=/SUBSYSTEM:CONSOLE,5.01 %LINK%

rem /Zi for the debugz
cl /W2 /MT /Fepcw_hashzipcrypto.exe main.c pcw_hashzipcrypto.c helper.c /I..\pcw_updatedecrypt\3rd-party\zlib-1.2.11 /I..\pcw_updatedecrypt\3rd-party\zlib-1.2.11\contrib\minizip ..\pcw_updatedecrypt\3rd-party\zlib-1.2.11\contrib\minizip\unzip.c ..\pcw_updatedecrypt\3rd-party\zlib-1.2.11\contrib\minizip\zip.c ..\pcw_updatedecrypt\3rd-party\zlib-1.2.11\contrib\minizip\miniunz.c ..\pcw_updatedecrypt\3rd-party\zlib-1.2.11\contrib\minizip\minizip.c ..\pcw_updatedecrypt\3rd-party\zlib-1.2.11\inflate.c ..\pcw_updatedecrypt\3rd-party\zlib-1.2.11\deflate.c ..\pcw_updatedecrypt\3rd-party\zlib-1.2.11\crc32.c ..\pcw_updatedecrypt\3rd-party\zlib-1.2.11\adler32.c ..\pcw_updatedecrypt\3rd-party\zlib-1.2.11\inffast.c ..\pcw_updatedecrypt\3rd-party\zlib-1.2.11\contrib/minizip/iowin32.c ..\pcw_updatedecrypt\3rd-party\zlib-1.2.11\contrib/minizip/ioapi.c ..\pcw_updatedecrypt\3rd-party\zlib-1.2.11\zutil.c ..\pcw_updatedecrypt\3rd-party\zlib-1.2.11\trees.c ..\pcw_updatedecrypt\3rd-party\zlib-1.2.11\inftrees.c Shlwapi.lib


