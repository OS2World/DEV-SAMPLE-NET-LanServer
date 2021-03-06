# Makefile for LAN Server API Sample Program: MESSAG32.EXE
# syntax: nmake /F messag32.mak

MESSAG32.EXE : MESSAG32.OBJ ERRMSG32.OBJ
   link386 /noi /pm:vio messag32.obj+errmsg32.obj,messag32.exe,,netapi32,sample32.def;

messag32.OBJ : messag32.C
   icc /C /GT+ /DPURE_32 messag32.c

errmsg32.OBJ : errmsg.C
   icc /C /Foerrmsg32 /GT+ /DPURE_32 errmsg.c

