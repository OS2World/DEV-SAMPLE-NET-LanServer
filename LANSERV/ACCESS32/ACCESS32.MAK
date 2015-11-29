# Makefile for LAN Server API Sample Program: ACCESS32.EXE
# syntax: nmake /F access32.mak

ACCESS32.EXE : ACCESS32.OBJ ERRMSG32.OBJ
   link386 /noi /pm:vio access32.obj+errmsg32.obj,access32.exe,,netapi32,sample32.def;

access32.OBJ : access32.C
   icc /C /GT+ /DPURE_32 access32.c

errmsg32.OBJ : errmsg.C
   icc /C /Foerrmsg32 /GT+ /DPURE_32 errmsg.c

