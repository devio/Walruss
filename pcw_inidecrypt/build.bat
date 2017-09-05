rem -- stuff made by ths

@echo off

rem for the XPz...
set LINK=/SUBSYSTEM:CONSOLE,5.01 %LINK%

rem /Zi for the debugz
cl /W2 /MT /Fepcw_inidecrypt.exe /I3rd-party\iniparser\src pcw_inidecrypt.c pcw_lodlodl.c helper.c 3rd-party\iniparser\src\dictionary.c 3rd-party\iniparser\src\iniparser.c shlwapi.lib

