# Makefile for LAN Server API Sample Program: USER32.EXE
# syntax: nmake /F user32.mak

USER32.EXE : USER32.OBJ ERRMSG32.OBJ
   link386 /noi /pm:vio user32.obj+errmsg32.obj,user32.exe,,netapi32,sample32.def;

user32.OBJ : user32.C
   icc /C /GT+ /DPURE_32 user32.c

errmsg32.OBJ : errmsg.C
   icc /C /Foerrmsg32 /GT+ /DPURE_32 errmsg.c

