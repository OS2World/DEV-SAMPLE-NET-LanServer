# Makefile for LAN Server API Sample Program: FILEEN32.EXE
# syntax: nmake /F fileen32.mak

FILEEN32.EXE : FILEEN32.OBJ ERRMSG32.OBJ
   link386 /noi /pm:vio fileen32.obj+errmsg32.obj,fileen32.exe,,netapi32,sample32.def;

fileen32.OBJ : fileen32.C
   icc /C /GT+ /DPURE_32 fileen32.c

errmsg32.OBJ : errmsg.C
   icc /C /Foerrmsg32 /GT+ /DPURE_32 errmsg.c

