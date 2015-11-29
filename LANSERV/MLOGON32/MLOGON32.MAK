# Makefile for LAN Server API Sample Program: MLOGON32.EXE
# syntax: nmake /F mlogon32.mak

MLOGON32.EXE : MLOGON32.OBJ ERRMSG32.OBJ
   link386 /noi /pm:vio mlogon32.obj+errmsg32.obj,mlogon32.exe,,netapi32 upm,sample32.def;

mlogon32.OBJ : mlogon32.C
   icc /C /GT+ /DPURE_32 mlogon32.c

errmsg32.OBJ : errmsg.C
   icc /C /Foerrmsg32 /GT+ /DPURE_32 errmsg.c

