/*****************************************************************************/
/*                                                                           */
/*   Component   : Sample ANDIS MAC Driver                                   */
/*                                                                           */
/*   Module      : MACNDIS.C                                                 */
/*                                                                           */
/*   Description : This module contains routines which implement the NDIS    */
/*                 interface.  These routines talk to a protocol stack.      */
/*                                                                           */
/*****************************************************************************/

#define INCL_DOS
#include <os2.h>
#include "typedef.h"
#include "ndis.h"
#include "andis.h"
#include "macdef.h"
#include "macproto.h"


/************************************************************************/
/*                       System Requests                                */
/************************************************************************/


USHORT _loadds PASCAL FAR SystemRequest( ULONG Param1, ULONG Param2,
      USHORT Param3, USHORT Opcode, USHORT MACDS )
{
   switch( Opcode )
      {
         case InitiateBind:
            return NOT_SUPPORTED;
         case Bind:
            return SRBind( (struct CommChar *)Param1,
                  (struct CommChar **)Param2, MACDS );
         default:
            return INVALID_FUNCTION;
      }
}


USHORT NEAR SRBind( struct CommChar *pProtCCT, struct CommChar **ppMACCCT,
      USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   if ( pModule->pProtCCT == NULL )
      {
         /* Exchange Common Characteristics Tables */
         pModule->pProtCCT = pProtCCT;
         *ppMACCCT = &(pModule->CCT);

         /* Save the protocol's DS */
         pModule->ProtDS = pProtCCT->CcDataSeg;

         /* Save the protocol's lower dispatch entry points */
         pModule->ProtLDT.PldReqConfirm  = pProtCCT->CcLDp->PldReqConfirm;
         pModule->ProtLDT.PldXmitConfirm = pProtCCT->CcLDp->PldXmitConfirm;
         pModule->ProtLDT.PldRcvLkAhead  = pProtCCT->CcLDp->PldRcvLkAhead;
         pModule->ProtLDT.PldIndComplete = pProtCCT->CcLDp->PldIndComplete;
         pModule->ProtLDT.PldRcvChain    = pProtCCT->CcLDp->PldRcvChain;
         pModule->ProtLDT.PldStatInd     = pProtCCT->CcLDp->PldStatInd;

         return SUCCESS;
      }
   else return GENERAL_FAILURE;
}


/************************************************************************/
/*                       General Requests                               */
/************************************************************************/


USHORT _loadds PASCAL FAR GeneralRequest( USHORT ProtID, USHORT ReqHandle,
      USHORT Param1, PUCHAR Param2, USHORT Opcode, USHORT MACDS )
{
   switch( Opcode )
      {
         case InitiateDiagnostics:
            return GRInitiateDiagnostics( ProtID, ReqHandle, MACDS );
         case ReadErrorLog:
            return GRReadErrorLog( ProtID, ReqHandle, Param1, Param2, MACDS );
         case SetStationAddress:
            return GRSetStationAddress( ProtID, ReqHandle, Param2, MACDS );
         case OpenAdapter:
            return GROpenAdapter( ProtID, ReqHandle, Param1, MACDS );
         case CloseAdapter:
            return GRCloseAdapter( ProtID, ReqHandle, MACDS );
         case ResetMAC:
            return GRResetMAC( ProtID, ReqHandle, MACDS );
         case SetPacketFilter:
            return GRSetPacketFilter( ProtID, ReqHandle, Param1, MACDS );
         case AddMulticastAddress:
            return GRAddMulticastAddress( ProtID, ReqHandle, Param2, MACDS );
         case DeleteMulticastAddress:
            return GRDeleteMulticastAddress( ProtID, ReqHandle, Param2, MACDS );
         case UpdateStatistics:
            return GRUpdateStatistics( ProtID, ReqHandle, MACDS );
         case ClearStatistics:
            return GRClearStatistics( ProtID, ReqHandle, MACDS );
         case Interrupt:
            return GRInterrupt( ProtID, MACDS );
         case SetFunctionalAddress:
            return GRSetFunctionalAddress( ProtID, ReqHandle, Param2, MACDS );
         case SetLookAhead:
            return GRSetLookAhead( ProtID, ReqHandle, Param1, MACDS );
         default:
            return INVALID_FUNCTION;
      }
}


USHORT NEAR GRInitiateDiagnostics( USHORT ProtID, USHORT ReqHandle,
      USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR GRReadErrorLog( USHORT ProtID, USHORT ReqHandle, USHORT LogLen,
      PUCHAR pLogBuff, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR GRSetStationAddress( USHORT ProtID, USHORT ReqHandle,
      PUCHAR pAdaptAddr, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR GROpenAdapter( USHORT ProtID, USHORT ReqHandle, USHORT OpenOptions,
      USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR GRCloseAdapter( USHORT ProtID, USHORT ReqHandle, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR GRResetMAC( USHORT ProtID, USHORT ReqHandle, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR GRSetPacketFilter( USHORT ProtID, USHORT ReqHandle,
      USHORT FilterMask, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR GRAddMulticastAddress( USHORT ProtID, USHORT ReqHandle,
      PUCHAR pMultiAddr, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR GRDeleteMulticastAddress( USHORT ProtID, USHORT ReqHandle,
      PUCHAR pMultiAddr, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR GRUpdateStatistics( USHORT ProtID, USHORT ReqHandle, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR GRClearStatistics( USHORT ProtID, USHORT ReqHandle, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR GRInterrupt( USHORT ProtID, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR GRSetFunctionalAddress( USHORT ProtID, USHORT ReqHandle,
      PUCHAR pFunctAddr, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR GRSetLookAhead( USHORT ProtID, USHORT ReqHandle, USHORT Length,
      USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR RequestConfirm( USHORT ProtID, USHORT ReqHandle, USHORT Status,
      USHORT Request, PMODULE pModule )
{
   return (USHORT)pModule->ProtLDT.PldReqConfirm( ProtID,
         pModule->CCT.CcModuleID, ReqHandle, Status, Request, pModule->ProtDS );
}


/************************************************************************/
/*                       Status Indications                             */
/************************************************************************/


USHORT NEAR SIRingStatus( USHORT Status, PUCHAR pIndicate, PMODULE pModule )
{
   return (USHORT)pModule->ProtLDT.PldStatInd( pModule->CCT.CcModuleID, Status,
         pIndicate, RingStatus, pModule->ProtDS );
}


USHORT NEAR SIAdapterCheck( USHORT Reason, PUCHAR pIndicate, PMODULE pModule )
{
   return (USHORT)pModule->ProtLDT.PldStatInd( pModule->CCT.CcModuleID, Reason,
         pIndicate, AdapterCheck, pModule->ProtDS );
}


USHORT NEAR SIStartReset( PUCHAR pIndicate, PMODULE pModule )
{
   return (USHORT)pModule->ProtLDT.PldStatInd( pModule->CCT.CcModuleID, 0,
         pIndicate, StartReset, pModule->ProtDS );
}


USHORT NEAR SIEndReset( PUCHAR pIndicate, PMODULE pModule )
{
   return (USHORT)pModule->ProtLDT.PldStatInd( pModule->CCT.CcModuleID, 0,
         pIndicate, EndReset, pModule->ProtDS );
}


USHORT NEAR SIInterrupt( PUCHAR pIndicate, PMODULE pModule )
{
   return (USHORT)pModule->ProtLDT.PldStatInd( pModule->CCT.CcModuleID, 0,
         pIndicate, InterruptStatus, pModule->ProtDS );
}


/************************************************************************/
/*                        Direct Primitives                             */
/************************************************************************/


USHORT _loadds PASCAL FAR TransmitChain( USHORT ProtID, USHORT ReqHandle,
      struct TxBufDesc *pTxBufDesc, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR TransmitConfirm( USHORT ProtID, USHORT ReqHandle, USHORT Status,
      PMODULE pModule )
{
   return (USHORT)pModule->ProtLDT.PldXmitConfirm( ProtID,
         pModule->CCT.CcModuleID, ReqHandle, Status, pModule->ProtDS );
}


USHORT NEAR ReceiveLookahead( USHORT FrameSize, USHORT BytesAvail,
      PUCHAR pBuffer, PUCHAR pIndicate, PMODULE pModule )
{
   return (USHORT)pModule->ProtLDT.PldRcvLkAhead( pModule->CCT.CcModuleID,
         FrameSize, BytesAvail, pBuffer, pIndicate, pModule->ProtDS );
}


USHORT _loadds PASCAL FAR TransferData( PUSHORT pBytesCopied,
      USHORT FrameOffset, struct TDBufDesc *pTDBufDesc, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT NEAR IndicationComplete( PMODULE pModule )
{
   return (USHORT)pModule->ProtLDT.PldIndComplete( pModule->CCT.CcModuleID,
         pModule->ProtDS );
}


USHORT NEAR ReceiveChain( USHORT FrameSize, USHORT ReqHandle,
      struct RxBufDesc *pRxBufDesc, PUCHAR pIndicate, PMODULE pModule )
{
   return (USHORT)pModule->ProtLDT.PldRcvChain( pModule->CCT.CcModuleID,
         FrameSize, ReqHandle, (PUCHAR)pRxBufDesc, pIndicate, pModule->ProtDS );
}


USHORT _loadds PASCAL FAR ReceiveRelease( USHORT ReqHandle, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT _loadds PASCAL FAR IndicationsOff( USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


USHORT _loadds PASCAL FAR IndicationsOn( USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return SUCCESS;
}


