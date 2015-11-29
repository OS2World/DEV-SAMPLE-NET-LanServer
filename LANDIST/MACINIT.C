/*****************************************************************************/
/*                                                                           */
/*   Component   : Sample ANDIS MAC Driver                                   */
/*                                                                           */
/*   Module      : MACINIT.C                                                 */
/*                                                                           */
/*   Description : This module contains the routines that initialize the     */
/*                 MAC.  These routines include the device driver strategy   */
/*                 routine and routines that process PROTOCOL.INI config     */
/*                 data.                                                     */
/*                                                                           */
/*****************************************************************************/

#define INCL_DOS
#include <os2.h>
#include "typedef.h"
#include "ndis.h"
#include "andis.h"
#include "macdef.h"
#include "macproto.h"


extern USHORT EndofData;


/* All global variables should be initialized. */
/* This puts them in the _DATA segment.        */

PFN      pDevHelp     = NULL;   /* Address of DevHelp routine          */
PMODULE  pModuleList  = NULL;   /* Linked list of Modules              */
PHYSADDR FirstModPhys = {0,0};  /* Physical address of first Module    */
USHORT   PMHandle     = 0;      /* Handle to PROTMAN device driver     */
USHORT   LogHandle    = 0;      /* Handle to LAN message device driver */


/*****************************************************************************/
/*                                                                           */
/*   Routine     : Strategy                                                  */
/*                                                                           */
/*   Description : This routine is the C language stragegy routine for the   */
/*                 device driver.  This routine is called from the assembly  */
/*                 language strategy routine, stragegy_asm.                  */
/*                                                                           */
/*                 If an initialization request packet is received then      */
/*                 the Initialize routine is called, else an error is        */
/*                 returned.                                                 */
/*                                                                           */
/*****************************************************************************/

VOID NEAR Strategy( PREQPKT pRequestPacket )
{
   pRequestPacket->Status = STATUS_SUCCESS;

   if ( pRequestPacket->Command == COMMAND_INIT )
      Initialize( (PININITRP)pRequestPacket );
   else
      pRequestPacket->Status = STATUS_ERROR + STATUS_UNKNOWN;

   pRequestPacket->Status |= STATUS_DONE;
}


/*****************************************************************************/
/*                                                                           */
/*   Routine     : EndofCode                                                 */
/*                                                                           */
/*   Description : This routine is the first routine in the INITCODE         */
/*                 segment.  The INITCODE segment will be discarded          */
/*                 when initialization is complete.  The purpose of this     */
/*                 routine is to mark the end of the code segment that is    */
/*                 left when initialization is complete.  All initiali-      */
/*                 zation routines should be in this file following this     */
/*                 routine and should be placed in the INITCODE segment      */
/*                 (using the alloc_text pragma).                            */
/*                                                                           */
/*****************************************************************************/

#pragma alloc_text( INITCODE, EndofCode )

VOID NEAR EndofCode(){}


/*****************************************************************************/
/*                                                                           */
/*   Routine     : Initialize                                                */
/*                                                                           */
/*   Description : This routine is the highest level initialization routine. */
/*                 This routine:                                             */
/*                    - Saves the DevHelp address                            */
/*                    - Opens the LAN message device driver                  */
/*                    - Displays the install message                         */
/*                    - Opens PROTMAN device driver                          */
/*                    - Gets config data from PROTMAN                        */
/*                    - Calls ProcessConfigData                              */
/*                    - Fills in the initializaion request block             */
/*                                                                           */
/*****************************************************************************/

#pragma alloc_text( INITCODE, Initialize )

VOID FAR Initialize( PININITRP pInInitRP )
{
   POUTINITRP pOutInitRP = (POUTINITRP)pInInitRP;
   BOOL Initialization = SUCCESS;
   USHORT Action;
   struct RqBlk ReqBlock;

   /* Save the DevHelp address */
   pDevHelp = pInInitRP->pDevHelp;

   /* Open the LAN message device driver */
   if ( DosOpen("\\DEV\\LANMSG$$",&LogHandle,&Action,0L,0,1,0x2042,0L) )
      LogHandle = 0;

   /* Display the install message */
   Display("\r\n");  /* Start with a blank line */
   if ( DisplayMsg( 0, NULL ) == FAILURE )
      Display("Installing ANDISMAC\r\n");

   /* Open PROTMAN */
   if ( !DosOpen("\\DEV\\PROTMAN$",&PMHandle,&Action,0L,0,1,0x2042,0L) )
      {
         /* Get PROTOCOL.INI config data from PROTMAN */
         ReqBlock.Opcode = GetPMInfo;
         if ( !DosDevIOCtl(NULL,&ReqBlock,0x58,0x81,PMHandle) )
            {
               if ( ReqBlock.Status == SUCCESS )
                  {
                     /* Process the PROTOCOL.INI config data and register */
                     /* modules to PROTMAN.                               */
                     if ( ProcessConfigData(ReqBlock.Pointer1) != SUCCESS )
                        Initialization = FAILURE;
                  }
               else  /* GetPMInfo failed */
                  {
                     /* Error getting config data */
                     DisplayMsg( 4, NULL );
                     Initialization = FAILURE;
                  }
            }
         else  /* DosDevIOCtl for GetPMInfo failed */
            {
               /* Error getting config data */
               DisplayMsg( 4, NULL );
               Initialization = FAILURE;
            }

         /* Close PROTMAN */
         DosClose( PMHandle );
      }
   else  /* Could not open PROTMAN */
      {
         DisplayMsg( 3, NULL );
         Initialization = FAILURE;
      }

   if ( Initialization == SUCCESS )
      {
         /* ANDISMAC is installed */
         DisplayMsg( 1, NULL );
         pOutInitRP->CodeEnd = (USHORT)EndofCode;
         pOutInitRP->DataEnd = (USHORT)&EndofData;
      }
   else  /* Initialization failed */
      {
         /* ANDISMAC failed to install */
         DisplayMsg( 2, NULL );
         pOutInitRP->Status  = STATUS_ERROR + STATUS_GENFAIL;
         pOutInitRP->CodeEnd = 0;
         pOutInitRP->DataEnd = 0;
      }

   pOutInitRP->Unit = 0;
   pOutInitRP->pBPB = NULL;

   Display("\r\n");  /* Finish with a blank line */

   /* Close the LAN message device driver if it was opened */
   if ( LogHandle != 0 ) DosClose( LogHandle );
}


/*****************************************************************************/
/*                                                                           */
/*   Routine     : ProcessConfigData                                         */
/*                                                                           */
/*   Description : This routine scans through the PROTOCOL.INI config data   */
/*                 that we got from PROTMAN.  This routine calls             */
/*                 CreateModule for each section that is found which         */
/*                 contains "DriverName = ANDISMC$".  Notice that            */
/*                 "ANDISMC$" is the device name for this device driver      */
/*                 (specified in the device driver header).                  */
/*                                                                           */
/*****************************************************************************/

#pragma alloc_text( INITCODE, ProcessConfigData )

BOOL NEAR ProcessConfigData( struct ModCfg *pConfigData )
{
   struct ModCfg *pModCfg;
   struct KeywordEntry *pKE;

   /* For each section in PROTOCOL.INI that contains "DriverName = ANDISMC$" */
   /*    create a new module                                                 */
   for ( pModCfg=pConfigData; pModCfg!=NULL; pModCfg=pModCfg->NextModCfg )
      for ( pKE=pModCfg->KE; pKE!=NULL; pKE=pKE->NextKeywordEntry )
         if ( StringsEqual(pKE->KeyWord,"DRIVERNAME") )
            if ( StringsEqual(pKE->Params[0].ParVal.ParStr,"ANDISMC$") )
               CreateModule( pModCfg );

   /* If no modules were successfully created then fail initialization */
   if ( pModuleList == NULL )
      return FAILURE;
   else
      return SUCCESS;
}


/*****************************************************************************/
/*                                                                           */
/*   Routine     : CreateModule                                              */
/*                                                                           */
/*   Description : This routine will create a new Module.  A "Module" is     */
/*                 a data segment where all the variables and tables for     */
/*                 an instance of the MAC are kept.                          */
/*                                                                           */
/*                 This routine:                                             */
/*                    - Allocates physical memory for the Module             */
/*                    - Gets a GDT selector and maps it to the Module        */
/*                    - Gets a virtual address for the Module                */
/*                    - Initializes the variables in the Module              */
/*                    - Calls ProcessKeywords to process remaining config    */
/*                    - Calls InitNDISTables and InitANDISTables             */
/*                    - Registers the Module to PROTMAN                      */
/*                    - Adds the Module to the end of the module list        */
/*                                                                           */
/*                 Note: A GDT selector is needed so that the Module and     */
/*                 its tables can be addressed at post-initializaton time.   */
/*                 However, a virtual address is needed at initialization    */
/*                 time to read and write to the Module.                     */
/*                                                                           */
/*****************************************************************************/

#pragma alloc_text( INITCODE, CreateModule )

VOID NEAR CreateModule( struct ModCfg *pModCfg )
{
   REGS     Regs;
   PHYSADDR PhysAddr;
   PMODULE  pModule;
   USHORT   GDTSel;
   PUSHORT  pGDTSel = &GDTSel;
   struct RqBlk ReqBlock;
   BOOL Result = SUCCESS;

   /* Display which port is being initialized */
   DisplayMsg( 5, pModCfg->ModName );

   /* Get some physical memory for the new module */
   Regs.Word.BX = sizeof(MODULE);
   Regs.Word.AX = 0;
   Regs.Byte.DH = 1;  /* Low Memory */
   Regs.Byte.DL = DEVHELP_ALLOCPHYS;
   Regs.Word.ES = 0L;
   if ( !DevHelper(&Regs) )
      {
         /* Save the address of the physical memory */
         PhysAddr.High = Regs.Word.AX;
         PhysAddr.Low  = Regs.Word.BX;

         /* Get a GDT Selector */
         Regs.Word.ES = SELECTOROF( pGDTSel );
         Regs.Word.DI = OFFSETOF( pGDTSel );
         Regs.Word.CX = 1;  /* Only one selector */
         Regs.Byte.DL = DEVHELP_ALLOCGDTSELECTOR;
         if ( !DevHelper(&Regs) )
            {
               /* Point the GDT selector to the physical memory */
               Regs.Word.AX = PhysAddr.High;
               Regs.Word.BX = PhysAddr.Low;
               Regs.Word.CX = sizeof(MODULE);
               Regs.Word.SI = GDTSel;
               Regs.Byte.DL = DEVHELP_PHYSTOGDTSELECTOR;
               Regs.Word.ES = 0L;
               if ( !DevHelper(&Regs) )
                  {
                     /* Get a virtual address for the physical memory */
                     pModule = VirtualAddr( &PhysAddr );
                     if ( pModule != NULL )
                        {
                           /* Initialize the new module */
                           pModule->pNextModule      = NULL;
                           pModule->NextModPhys.High = 0;
                           pModule->NextModPhys.Low  = 0;
                           StringCopy( pModule->Name, pModCfg->ModName, NAME_LEN );
                           pModule->ProtDS           = 0;
                           pModule->pProtCCT         = NULL;

                           /* Process config data keywords for this module */
                           if ( ProcessKeywords( pModCfg->KE, pModule ) == SUCCESS )
                              {
                                 /* Initialize tables */
                                 InitNDISTables(  pModule, GDTSel );
                                 InitANDISTables( pModule, GDTSel );

                                 /* Register to PROTMAN */
                                 ReqBlock.Opcode   = RegisterModule;
                                 ReqBlock.Pointer1 = pModule->UDT.MudCCp;
                                 ReqBlock.Pointer2 = NULL;
                                 if ( !DosDevIOCtl(NULL,&ReqBlock,0x58,0x81,PMHandle) )
                                    {
                                       if ( ReqBlock.Status == SUCCESS )
                                          {
                                             /* Add the module to the end of the module list */
                                             if ( AddModule( GDTSel, &PhysAddr ) == FAILURE )
                                                {
                                                   /* Internal Error */
                                                   DisplayMsg( 7, NULL );
                                                   Result = FAILURE;
                                                }
                                          }
                                       else  /* Could not register to PROTMAN */
                                          {
                                             DisplayMsg( 6, NULL );
                                             Result = FAILURE;
                                          }
                                    }
                                 else  /* DosDevIOCtl failed */
                                    {
                                       DisplayMsg( 6, NULL );
                                       Result = FAILURE;
                                    }
                              }
                           else  /* Error processing the rest of config data */
                              {
                                 Result = FAILURE;
                              }
                        }
                     else  /* Could not get a virtual address */
                        {
                           Result = FAILURE;
                        }
                  }
               else  /* Could not map the GDT to the physical memory */
                  {
                     /* Internal Error */
                     DisplayMsg( 7, NULL );
                     Result = FAILURE;
                  }

               if ( Result == FAILURE )
                  {
                     /* Free the GDT selector */
                     Regs.Word.AX = GDTSel;
                     Regs.Byte.DL = DEVHELP_FREEGDTSELECTOR;
                     Regs.Word.ES = 0L;
                     DevHelper( &Regs );
                  }
            }
         else  /* Could not get a GDT selector */
            {
               /* Internal Error */
               DisplayMsg( 7, NULL );
               Result = FAILURE;
            }

         if ( Result == FAILURE )
            {
               /* Free the physical memory */
               Regs.Word.AX = PhysAddr.High;
               Regs.Word.BX = PhysAddr.Low;
               Regs.Byte.DL = DEVHELP_FREEPHYS;
               Regs.Word.ES = 0L;
               DevHelper( &Regs );
            }
      }
   else  /* Could not get physical memory */
      {
         /* Internal Error */
         DisplayMsg( 7, NULL );
      }
}


/*****************************************************************************/
/*                                                                           */
/*   Routine     : ProcessKeywords                                           */
/*                                                                           */
/*   Description : This is a place holder routine.  This routine will        */
/*                 process the config data for a section in PROTOCOL.INI     */
/*                 and initialize varibles in the newly created Module.      */
/*                 If there is an error processing the config data then      */
/*                 then routine returns FAILURE else it returns SUCCESS.     */
/*                                                                           */
/*****************************************************************************/

#pragma alloc_text( INITCODE, ProcessKeywords )

BOOL NEAR ProcessKeywords( struct KeywordEntry *pKEList, PMODULE pModule )
{
   struct KeywordEntry *pKE;

   for ( pKE=pKEList; pKE!=NULL; pKE=pKE->NextKeywordEntry )
      {
         /* Process keywords in this section of the PROTOCOL.INI file */
         /* and set variables in the new module data structure.       */
      }

   return SUCCESS;
}


/*****************************************************************************/
/*                                                                           */
/*   Routine     : AddModule                                                 */
/*                                                                           */
/*   Description : This routine adds the newly created Module to the end     */
/*                 of the linked list of Modules.                            */
/*                                                                           */
/*                 Strictly speaking, a linked list of Modules is not        */
/*                 needed because the GDT selector for a module is passed    */
/*                 to the MAC (in MACDS) on each NDIS and ANDIS call to      */
/*                 the MAC.  I just like to have all my allocated data       */
/*                 segments tethered together so I can get to them if I      */
/*                 need to.                                                  */
/*                                                                           */
/*****************************************************************************/

#pragma alloc_text( INITCODE, AddModule )

BOOL NEAR AddModule( USHORT GDTSel, PPHYSADDR pPhysAddr )
{
   PMODULE pModule;

   if ( pModuleList == NULL )   /* If the module list is empty */
      {
         /* Add the new module to the head of the list */
         pModuleList = MAKEP( GDTSel, 0 );
         FirstModPhys.High = pPhysAddr->High;
         FirstModPhys.Low  = pPhysAddr->Low;
      }
   else  /* The module list is not empty */
      {
         /* Get a virtual address for the first module */
         pModule = VirtualAddr( &FirstModPhys );
         if ( pModule == NULL ) return FAILURE;

         /* Find the last module in the list */
         while ( pModule->pNextModule != NULL )
            {
               /* Get a virtual address for the next module */
               pModule = VirtualAddr( &(pModule->NextModPhys) );
               if ( pModule == NULL ) return FAILURE;
            }

         /* Add the new module to the end of the list */
         pModule->pNextModule = MAKEP( GDTSel, 0 );
         pModule->NextModPhys.High = pPhysAddr->High;
         pModule->NextModPhys.Low  = pPhysAddr->Low;
      }

   return SUCCESS;
}


/*****************************************************************************/
/*                                                                           */
/*   Routine     : VirtualAddr                                               */
/*                                                                           */
/*   Description : This routine calls DevHelp to create a virtual address    */
/*                 for the specified physical memory.                        */
/*                                                                           */
/*****************************************************************************/

#pragma alloc_text( INITCODE, VirtualAddr )

PMODULE NEAR VirtualAddr( PPHYSADDR pPhysAddr )
{
   REGS Regs;

   /* Get a virtual address for the physical memory */
   Regs.Word.AX = pPhysAddr->High;
   Regs.Word.BX = pPhysAddr->Low;
   Regs.Word.CX = sizeof(MODULE);
   Regs.Byte.DH = 1;  /* Put virtual address in ES:DI */
   Regs.Byte.DL = DEVHELP_PHYSTOVIRT;
   Regs.Word.ES = 0L;
   if ( DevHelper(&Regs) )
      {
         /* Internal Error */
         DisplayMsg( 7, NULL );
         return NULL;
      }

   return MAKEP( Regs.Word.ES, Regs.Word.DI );
}


/*****************************************************************************/
/*                                                                           */
/*   Routine     : InitNDISTables                                            */
/*                                                                           */
/*   Description : This routine initializes the tables used by the NDIS      */
/*                 interface.  The ANDIS extentions are enabled by setting   */
/*                 bit 17 of the Service Flags field in the Service          */
/*                 Specific Characteristics table and the first reserved     */
/*                 pointer of the Common Characteristics table is set to     */
/*                 point to the ANDIS PPAT table.                            */
/*                                                                           */
/*                 Note: This is a very quick and simple initialization.     */
/*                 A MAC developer should carefully consider the correct     */
/*                 values for each parameter.                                */
/*                                                                           */
/*****************************************************************************/

#pragma alloc_text( INITCODE, InitNDISTables )

VOID NEAR InitNDISTables( PMODULE pModule, USHORT GDTSel )
{
   struct CommChar    *pCCT  = &(pModule->CCT);
   struct MACSpecChar *pSSCT = &(pModule->SSCT);
   struct MACSpecStat *pSSST = &(pModule->SSST);
   struct MACUprDisp  *pUDT  = &(pModule->UDT);
   PXVPPAT             pPPAT = &(pModule->PPAT);

   /* Initialize the Common Characteristics Table */
   pModule->CCT.CcSize     = sizeof(struct CommChar);
   pModule->CCT.CcLvlMjr   = MAJOR_NDIS_VERSION;
   pModule->CCT.CcLvlMnr   = MINOR_NDIS_VERSION;
   pModule->CCT.CcSpLvl    = 0;
   pModule->CCT.CcMjrVer   = 1;
   pModule->CCT.CcMnrVer   = 0;
   pModule->CCT.CcBindFnc  = 1L;
   StringCopy( pModule->CCT.CcName, pModule->Name, NAME_LEN );
   pModule->CCT.CcUPLevel  = MACLvl;
   pModule->CCT.CcUIType   = MACTyp;
   pModule->CCT.CcLPLevel  = 0;
   pModule->CCT.CcLIType   = 0;
   pModule->CCT.CcModuleID = 0;
   pModule->CCT.CcDataSeg  = GDTSel;
   pModule->CCT.CcSysReq   = (PUCHAR)SystemRequest;
   pModule->CCT.CcSCp      = MAKEP(GDTSel,OFFSETOF(pSSCT)-OFFSETOF(pModule));
   pModule->CCT.CcSSp      = MAKEP(GDTSel,OFFSETOF(pSSST)-OFFSETOF(pModule));
   pModule->CCT.CcUDp      = MAKEP(GDTSel,OFFSETOF(pUDT) -OFFSETOF(pModule));
   pModule->CCT.CcLDp      = NULL;
   /* Put the address of the PPAT in the first reserved pointer */
   pModule->CCT.CcRsv1     = (ULONG)MAKEP(GDTSel,OFFSETOF(pPPAT)-OFFSETOF(pModule));
   pModule->CCT.CcRsv2     = 0L;

   /* Initialize the Service Specific Characteristics Table */
   pModule->SSCT.MscSize = sizeof(struct MACSpecChar);
   StringCopy( pModule->SSCT.MscType, "HDLC", NAME_LEN );
   pModule->SSCT.MscStnAdrSz       = ADDR_SIZE;
   pModule->SSCT.MscPermStnAdr[0]  = 0;
   pModule->SSCT.MscCurrStnAdr[0]  = 0;
   pModule->SSCT.MscCurrFncAdr     = 0L;
   pModule->SSCT.MscMCp            = NULL;
   pModule->SSCT.MscLinkSpd        = 9600L;
   /* Set bit 17 (SV_ANDIS) to indicate that this is an ANDIS MAC */
   pModule->SSCT.MscService        = SV_ANDIS;
   pModule->SSCT.MscMaxFrame       = 0;
   pModule->SSCT.MscTBufCap        = 0;
   pModule->SSCT.MscTBlkSz         = 0;
   pModule->SSCT.MscRBufCap        = 0;
   pModule->SSCT.MscRBlkSz         = 0;
   pModule->SSCT.MscVenCode[0]     = 0xFF;
   pModule->SSCT.MscVenCode[1]     = 0xFF;
   pModule->SSCT.MscVenCode[2]     = 0xFF;
   pModule->SSCT.MscVenAdapter     = 0;
   pModule->SSCT.MscVenAdaptDesc   = NULL;
   pModule->SSCT.MscInterrupt      = 3;
   pModule->SSCT.MscTxQDepth       = 1;
   pModule->SSCT.MscMaxNumDataBlks = 1;

   /* Initialize the Service Specific Status Table */
   pModule->SSST.MssSize    = sizeof(struct MACSpecStat);
   pModule->SSST.MssDiagDT  = 0xFFFFFFFFL;
   pModule->SSST.MssStatus  = 0L;
   pModule->SSST.MssFilter  = 0;
   pModule->SSST.MssM8Sp    = NULL;
   pModule->SSST.MssClearDT = 0xFFFFFFFFL;
   pModule->SSST.MssFR      = 0xFFFFFFFFL;
   pModule->SSST.MssRFCRC   = 0xFFFFFFFFL;
   pModule->SSST.MssFRByt   = 0xFFFFFFFFL;
   pModule->SSST.MssRFLack  = 0xFFFFFFFFL;
   pModule->SSST.MssFRMC    = 0xFFFFFFFFL;
   pModule->SSST.MssFRBC    = 0xFFFFFFFFL;
   pModule->SSST.MssRFErr   = 0xFFFFFFFFL;
   pModule->SSST.MssRFMax   = 0xFFFFFFFFL;
   pModule->SSST.MssRFMin   = 0xFFFFFFFFL;
   pModule->SSST.MssFRMCByt = 0xFFFFFFFFL;
   pModule->SSST.MssFRBCByt = 0xFFFFFFFFL;
   pModule->SSST.MssRFHW    = 0xFFFFFFFFL;
   pModule->SSST.MssFS      = 0xFFFFFFFFL;
   pModule->SSST.MssFSByt   = 0xFFFFFFFFL;
   pModule->SSST.MssFSMC    = 0xFFFFFFFFL;
   pModule->SSST.MssFSBC    = 0xFFFFFFFFL;
   pModule->SSST.MssFSBCByt = 0xFFFFFFFFL;
   pModule->SSST.MssFSMCByt = 0xFFFFFFFFL;
   pModule->SSST.MssSFTime  = 0xFFFFFFFFL;
   pModule->SSST.MssSFHW    = 0xFFFFFFFFL;

   /* Initialize the MAC Upper Dispatch Table */
   pModule->UDT.MudCCp        = MAKEP(GDTSel,OFFSETOF(pCCT)-OFFSETOF(pModule));
   pModule->UDT.MudGReq       = (PUCHAR)GeneralRequest;
   pModule->UDT.MudXmitChain  = (PUCHAR)TransmitChain;
   pModule->UDT.MudXferData   = (PUCHAR)TransferData;
   pModule->UDT.MudRcvRelease = (PUCHAR)ReceiveRelease;
   pModule->UDT.MudIndOn      = (PUCHAR)IndicationsOn;
   pModule->UDT.MudIndOff     = (PUCHAR)IndicationsOff;

   /* Initialize the Protocol Lower Dispatch Table */
   pModule->ProtLDT.PldCCp         = NULL;
   pModule->ProtLDT.PldIFF         = 0L;
   pModule->ProtLDT.PldReqConfirm  = NULL;
   pModule->ProtLDT.PldXmitConfirm = NULL;
   pModule->ProtLDT.PldRcvLkAhead  = NULL;
   pModule->ProtLDT.PldIndComplete = NULL;
   pModule->ProtLDT.PldRcvChain    = NULL;
   pModule->ProtLDT.PldStatInd     = NULL;
}


/*****************************************************************************/
/*                                                                           */
/*   Routine     : InitANDISTables                                           */
/*                                                                           */
/*   Description : This routine initalizes the tables used by the ANDIS      */
/*                 interface.  This is mainly the PPAT table.                */
/*                                                                           */
/*                 Note: This is a very quick and simple initialization.     */
/*                 A MAC developer should carefully consider the correct     */
/*                 values for each parameter.                                */
/*                                                                           */
/*****************************************************************************/

#pragma alloc_text( INITCODE, InitANDISTables )

VOID NEAR InitANDISTables( PMODULE pModule, USHORT GDTSel )
{
   /* Initialize the PPAT header */
   pModule->PPAT.Header.PPAT_Length            = sizeof(XVPPAT);
   pModule->PPAT.Header.PPAT_Ver_Major         = 1;
   pModule->PPAT.Header.PPAT_Ver_Minor         = 0;
   pModule->PPAT.Header.Port_Interface_Type    = 1;
   pModule->PPAT.Header.Port_Interface_Version = 1;
   pModule->PPAT.Header.Conn_Type              = 2;
   pModule->PPAT.Header.Phys_Conn_State        = 0;
   pModule->PPAT.Header.ANDIS_MAC_Mode         = 0;
   pModule->PPAT.Header.ANDIS_MAC_DS           = GDTSel;
   pModule->PPAT.Header.MAC_CCT_ptr            = pModule->UDT.MudCCp;
   StringCopy( pModule->PPAT.Header.Port_Name, pModule->Name, NAME_LEN );
   StringCopy( pModule->PPAT.Header.Phys_Interface, "RS232D", NAME_LEN );
   pModule->PPAT.Header.MAC_PPAT_Mgmt          = PPATManagement;
   pModule->PPAT.Header.PPAT_Mgmt              = NULL;/* Filled in by PROTMAN */
   pModule->PPAT.Header.Port_Mgmt              = PortManagement;
   pModule->PPAT.Header.Xmit_Imm               = TransmitImmData;
   pModule->PPAT.Header.Rcv_Imm_Cnf            = ReceiveImmDataConfirm;
   pModule->PPAT.Header.Port_Log_ptr           = NULL;
   pModule->PPAT.Header.Max_Speed              = 0L;
   pModule->PPAT.Header.Max_Xmit_Buff          = 0L;
   pModule->PPAT.Header.Port_ID                = 0;
   pModule->PPAT.Header.Slot_Number            = 0;
   pModule->PPAT.Header.Port_Number            = 0;
   pModule->PPAT.Header.Channel_Number         = 0;
   pModule->PPAT.Header.ANDIS_Align            = 0;
   pModule->PPAT.Header.ANDIS_Reserve[0]       = 0;
   pModule->PPAT.Header.ANDIS_Reserve[1]       = 0;
   pModule->PPAT.Header.ANDIS_Reserve[2]       = 0;
   pModule->PPAT.Header.ANDIS_Reserve[3]       = 0;
   pModule->PPAT.Header.ANDIS_Reserve[4]       = 0;
   pModule->PPAT.Header.ANDIS_Reserve[5]       = 0;

   /* Initialize the rest of the PPAT */
   pModule->PPAT.PPI_Status      = 0;
   pModule->PPAT.Port_Attribs    = 1L;
   pModule->PPAT.Clock_Type      = 1;
   pModule->PPAT.XVRESERVE_1     = 0;
   pModule->PPAT.Xmit_Speed      = 9600L;
   pModule->PPAT.Rcv_Speed       = 9600L;
   pModule->PPAT.Comm_Type       = 2;
   pModule->PPAT.Port_Mode       = 1;
   pModule->PPAT.Transparency    = 2;
   pModule->PPAT.Hard_Hand       = 0x0C;
   pModule->PPAT.XOFF_Char       = 0x13;
   pModule->PPAT.XON_Char        = 0x11;
   pModule->PPAT.Port_Trans_Mode = 8;
   pModule->PPAT.Signal_Mask     = 0;
   pModule->PPAT.Hw_Err_Mask     = 0;

   /* Initialize PCM Entry Table */
   pModule->PCMEntryTbl.PortMgmtInd    = NULL;
   pModule->PCMEntryTbl.RecvImmData    = NULL;
   pModule->PCMEntryTbl.XmitImmDataCnf = NULL;
   pModule->PCMEntryTbl.pcm_ds         = 0;
}


/*****************************************************************************/
/*                                                                           */
/*   Routine     : DisplayMsg                                                */
/*                                                                           */
/*   Description : This routine displays a message, with an optional         */
/*                 insertion string, on the screen.  The message is also     */
/*                 put in the LAN message log file.                          */
/*                                                                           */
/*                 If there is an error then FAILURE is returned.  This      */
/*                 allows the Initialze routine to put the name of this      */
/*                 MAC on the screen even if the message file is not found.  */
/*                                                                           */
/*****************************************************************************/

#pragma alloc_text( INITCODE, DisplayMsg )

BOOL NEAR DisplayMsg( USHORT MsgNumber, PCHAR pIvString )
{
   CHAR   MsgBuffer[256];
   USHORT MsgLength,IvCount;
   BOOL   Result = SUCCESS;

   /* If there is an insertion string then set IvCount to 1 */
   if ( pIvString == NULL )
      IvCount = 0;
   else
      IvCount = 1;

   /* Get the message from the message file */
   if ( DosGetMessage( &pIvString, IvCount, MsgBuffer, 256, MsgNumber,
         "ADS.MSG", &MsgLength ) )
      {
         Result = FAILURE;
      }

   /* Display the message on the screen */
   if ( DosPutMessage( 0, MsgLength, MsgBuffer ) )
      {
         Result = FAILURE;
      }

   /* Put the message in the LANTRAN.LOG file */
   LogMsg( MsgNumber, IvCount, pIvString );

   return Result;
}


/*****************************************************************************/
/*                                                                           */
/*   Routine     : LogMsg                                                    */
/*                                                                           */
/*   Description : This routine sends a message to the LAN message file      */
/*                 (LANTRAN.LOG).  If the LAN message device driver was      */
/*                 not successfully opened then this routine does nothing.   */
/*                                                                           */
/*****************************************************************************/

#pragma alloc_text( INITCODE, LogMsg )

VOID NEAR LogMsg( USHORT MsgNumber, USHORT IvCount, PCHAR pIvString )
{
   LANMSG LANMsg;
   CHAR   MsgOut[8];

   if ( LogHandle != 0 )
      {
         LANMsg.pCompID   = "ANDISMC$";
         LANMsg.pIvArray  = &pIvString;
         LANMsg.IvCount   = IvCount;
         LANMsg.MsgNumber = MsgNumber;
         LANMsg.pMsgFile  = "ADS.MSG";
         LANMsg.MsgType   = 0x4900;
         LANMsg.Display   = FALSE;
         LANMsg.pLogPath  = NULL;

         DosDevIOCtl( &MsgOut, &LANMsg, 1, 0x90, LogHandle );
      }
}


/*****************************************************************************/
/*                                                                           */
/*   Routine     : Display                                                   */
/*                                                                           */
/*   Description : This routine displays a string on the screen.             */
/*                                                                           */
/*****************************************************************************/

#pragma alloc_text( INITCODE, Display )

VOID NEAR Display( PCHAR pString )
{
   PCHAR  pEnd = pString;
   USHORT BytesWritten;

   /* Move pEnd to the terminating null byte */
   while ( *pEnd != 0 ) pEnd++;

   /* Write the string to the screen */
   DosWrite( 0, pString, pEnd-pString, &BytesWritten );
}


/*****************************************************************************/
/*                                                                           */
/*   Routine     : StringsEqual                                              */
/*                                                                           */
/*   Description : This routine returns TRUE is two strings are exactly      */
/*                 equal, else it returns FALSE.  The compare is case        */
/*                 sensitive and the strings must be the same length.        */
/*                                                                           */
/*****************************************************************************/

#pragma alloc_text( INITCODE, StringsEqual )

BOOL NEAR StringsEqual( PUCHAR pFirst, PUCHAR pSecond )
{
   for ( ; *pFirst == *pSecond; pFirst++, pSecond++ )
      if ( *pFirst == 0 ) return TRUE;

   return FALSE;
}


/*****************************************************************************/
/*                                                                           */
/*   Routine     : StringCopy                                                */
/*                                                                           */
/*   Description : This routine copies a string.                             */
/*                                                                           */
/*****************************************************************************/

#pragma alloc_text( INITCODE, StringCopy )

BOOL NEAR StringCopy( UCHAR Dest[], UCHAR Source[], USHORT DestSize )
{
   register USHORT x;

   for ( x=0; x<DestSize; x++ )
      if ( (Dest[x] = Source[x]) == 0 ) return FALSE;  /* Good return */

   return TRUE;   /* Error: Overflowed the destination buffer */
}


