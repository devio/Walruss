rem -- stuff made by ths

@echo off

rem for the XPz...
set LINK=/SUBSYSTEM:CONSOLE,5.01 %LINK%

rem /Zi for the debugz
cl /W2 /MT /Festudiopatch.exe wahlrusse_studio_patch.c
