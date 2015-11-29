# Makefile for LAN Server API Sample Program: ALIAS32.EXE
# syntax: nmake /F alias32.mak

ALIAS32.EXE : ALIAS32.OBJ ERRMSG32.OBJ
   link386 /noi /pm:vio alias32.obj+errmsg32.obj,alias32.exe,,netapi32,sample32.def;

alias32.OBJ : alias32.C
   icc /C /GT+ /DPURE_32 alias32.c

errmsg32.OBJ : errmsg.C
   icc /C /Foerrmsg32 /GT+ /DPURE_32 errmsg.c

