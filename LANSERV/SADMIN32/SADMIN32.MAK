# Makefile for LAN Server API Sample Program: SADMIN32.EXE
# syntax: nmake /F sadmin32.mak

SADMIN32.EXE : SADMIN32.OBJ ERRMSG32.OBJ
   link386 /noi /pm:vio sadmin32.obj+errmsg32.obj,sadmin32.exe,,netapi32,sample32.def;

sadmin32.OBJ : sadmin32.C
   icc /C /GT+ /DPURE_32 sadmin32.c

errmsg32.OBJ : errmsg.C
   icc /C /Foerrmsg32 /GT+ /DPURE_32 errmsg.c

