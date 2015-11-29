/*****************************************************************************/
/*                                                                           */
/*   Component   : Sample ANDIS MAC Driver                                   */
/*                                                                           */
/*   Module      : MACDEF.H                                                  */
/*                                                                           */
/*   Description : This module contains structure and symbol definitions.    */
/*                                                                           */
/*****************************************************************************/


typedef struct                          /* Physical Address structure */
   {
      USHORT    High;
      USHORT    Low;
   } PHYSADDR, *PPHYSADDR;


typedef struct module *PMODULE;

typedef struct module                   /* Module structure */
   {
      PMODULE   pNextModule;
      PHYSADDR  NextModPhys;
      UCHAR     Name[NAME_LEN];
      USHORT    ProtDS;
      struct CommChar   *pProtCCT;
      struct CommChar    CCT;
      struct MACSpecChar SSCT;
      struct MACSpecStat SSST;
      struct MACUprDisp  UDT;
      struct ProtLwrDisp ProtLDT;
      XVPPAT    PPAT;
      ENTRYTBL  PCMEntryTbl;
   } MODULE;


typedef struct                          /* Request Packet */
   {
      UCHAR     Length;
      UCHAR     Unit;
      UCHAR     Command;
      USHORT    Status;
      ULONG     Reserved;
      ULONG     QueueLink;
   } REQPKT, *PREQPKT;


typedef struct                          /* Input Init Request Packet */
   {
      UCHAR     Length;
      UCHAR     Unit;
      UCHAR     Command;
      USHORT    Status;
      ULONG     Reserved;
      ULONG     QueueLink;
      UCHAR     Data1;
      PFN       pDevHelp;
      PCHAR     pInitArg;
      UCHAR     FirstUnit;
   } ININITRP, *PININITRP;


typedef struct                          /* Output Init Request Packet */
   {
      UCHAR     Length;
      UCHAR     Unit;
      UCHAR     Command;
      USHORT    Status;
      ULONG     Reserved;
      ULONG     QueueLink;
      UCHAR     UnitCount;
      USHORT    CodeEnd;
      USHORT    DataEnd;
      PVOID     pBPB;
      UCHAR     Data2;
   } OUTINITRP, *POUTINITRP;


typedef union                           /* Register structure for DevHelps */
   {
      struct
         {
            USHORT  AX;
            USHORT  BX;
            USHORT  CX;
            USHORT  DX;
            USHORT  SI;
            USHORT  DI;
            USHORT  CFlag;
            USHORT  ES;
         } Word;
      struct
         {
            UCHAR   AL;
            UCHAR   AH;
            UCHAR   BL;
            UCHAR   BH;
            UCHAR   CL;
            UCHAR   CH;
            UCHAR   DL;
            UCHAR   DH;
         } Byte;
   } REGS, *PREGS;


typedef struct                          /* LAN message structure */
   {
      PCHAR   pCompID;
      PCHAR  *pIvArray;
      USHORT  IvCount;
      USHORT  MsgNumber;
      PCHAR   pMsgFile;
      USHORT  MsgType;
      USHORT  Display;
      PCHAR   pLogPath;
   } LANMSG;


/* General definitions */

#define FAILURE       TRUE
#ifndef SUCCESS
#define SUCCESS       FALSE
#endif


/* Request Packet Commands */

#define COMMAND_INIT    0x00
#define COMMAND_OPEN    0x0D
#define COMMAND_CLOSE   0x0E
#define COMMAND_IOCTL   0x10


/* Request Packet Status */

#define STATUS_DONE     0x0100
#define STATUS_ERROR    0x8000
#define STATUS_GENFAIL  0x000C
#define STATUS_UNKNOWN  0x0003
#define STATUS_SUCCESS  0x0000


/* DevHelp Op Codes */

#define DEVHELP_PHYSTOVIRT         0x15
#define DEVHELP_ALLOCPHYS          0x18
#define DEVHELP_FREEPHYS           0x19
#define DEVHELP_ALLOCGDTSELECTOR   0x2D
#define DEVHELP_PHYSTOGDTSELECTOR  0x2E
#define DEVHELP_FREEGDTSELECTOR    0x53



