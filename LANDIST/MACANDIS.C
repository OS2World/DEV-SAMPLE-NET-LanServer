/*****************************************************************************/
/*                                                                           */
/*   Component   : Sample ANDIS MAC Driver                                   */
/*                                                                           */
/*   Module      : MACANDIS.C                                                */
/*                                                                           */
/*   Description : This module contains routines which implement the ANDIS   */
/*                 extentions to NDIS.  These routines talk to a PCM to      */
/*                 establish and take down connections.                      */
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
/*                       Port Management                                */
/************************************************************************/


USHORT _loadds PASCAL FAR PortManagement( USHORT PortID, USHORT ReqHandle,
      USHORT Param1, ULONG Param2, USHORT FunctionCode, USHORT MACDS )
{
   switch( FunctionCode )
      {
         case ANDIS_FC_REG_PCM:
            return RegisterPCM( ReqHandle, Param1, (PENTRYTBL)Param2, MACDS );
         case ANDIS_FC_REL_PCM:
            return ReleasePCM( ReqHandle, Param1, MACDS );
         case ANDIS_FC_ACT_MODE:
            return ActivateMode( ReqHandle, Param1, MACDS );
         case ANDIS_FC_RET_PPI_STAT:
            return RetrievePPIStatus( ReqHandle, (PULONG)Param2, MACDS );
         case ANDIS_FC_GET_PPAT_PARMS:
            return GetPPATParms( ReqHandle, Param1, (PXVPPAT)Param2, MACDS );
         case ANDIS_FC_SET_PPAT_PARMS:
            return SetPPATParms( ReqHandle, (PXVPPAT)Param2, MACDS );
         case ANDIS_FC_RESET_PORT:
            return ResetPort( ReqHandle, Param1, MACDS );
         case ANDIS_FC_ACT_PHYS_CONN:
            return ActivatePhysConn( ReqHandle, Param1, Param2, MACDS );
         case ANDIS_FC_DEACT_PHYS_CONN:
            return DeactivatePhysConn( ReqHandle, Param2, MACDS );
         case ANDIS_FC_REG_NULL_DLC:
            return RegisterNullDLC( ReqHandle,Param1,(PENTRYTBL)Param2,MACDS );
         case ANDIS_FC_REL_NULL_DLC:
            return ReleaseNullDLC( ReqHandle, Param1, MACDS );
         case ANDIS_FC_ACT_DTR:
            return ActivateDTR( ReqHandle, MACDS );
         case ANDIS_FC_ACT_RTS:
            return ActivateRTS( ReqHandle, MACDS );
         default:
            return ANDIS_RC_INVALID_FUNC;
      }
}


USHORT NEAR RegisterPCM( USHORT ReqHandle, USHORT PortID, PENTRYTBL pEntryTbl,
      USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   if ( pModule->PCMEntryTbl.PortMgmtInd == NULL )
      {
         pModule->PCMEntryTbl.PortMgmtInd    = pEntryTbl->PortMgmtInd;
         pModule->PCMEntryTbl.RecvImmData    = pEntryTbl->RecvImmData;
         pModule->PCMEntryTbl.XmitImmDataCnf = pEntryTbl->XmitImmDataCnf;
         pModule->PCMEntryTbl.pcm_ds         = pEntryTbl->pcm_ds;
      }
   else return ANDIS_RC_ALREADY_REG;

   return ANDIS_RC_SUCCESS;
}


USHORT NEAR ReleasePCM( USHORT ReqHandle, USHORT PortID, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   pModule->PCMEntryTbl.PortMgmtInd    = NULL;
   pModule->PCMEntryTbl.RecvImmData    = NULL;
   pModule->PCMEntryTbl.XmitImmDataCnf = NULL;
   pModule->PCMEntryTbl.pcm_ds         = 0;

   return ANDIS_RC_SUCCESS;
}


USHORT NEAR ActivateMode( USHORT ReqHandle, USHORT MACMode, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return ANDIS_RC_SUCCESS;
}


USHORT NEAR RetrievePPIStatus( USHORT ReqHandle, PULONG pPPIStatus,
      USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return ANDIS_RC_SUCCESS;
}


USHORT NEAR GetPPATParms( USHORT ReqHandle, USHORT PPATSize, PXVPPAT pPPAT,
      USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return ANDIS_RC_SUCCESS;
}


USHORT NEAR SetPPATParms( USHORT ReqHandle, PXVPPAT pPPAT, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return ANDIS_RC_SUCCESS;
}


USHORT NEAR ResetPort( USHORT ReqHandle, USHORT ResetType, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return ANDIS_RC_SUCCESS;
}


USHORT NEAR ActivatePhysConn( USHORT ReqHandle, USHORT Mode, ULONG Timeout,
      USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return ANDIS_RC_SUCCESS;
}


USHORT NEAR DeactivatePhysConn( USHORT ReqHandle, ULONG Timeout, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return ANDIS_RC_SUCCESS;
}


USHORT NEAR RegisterNullDLC( USHORT ReqHandle, USHORT PortID,
      PENTRYTBL pEntryTbl, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return ANDIS_RC_SUCCESS;
}


USHORT NEAR ReleaseNullDLC( USHORT ReqHandle, USHORT PortID, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return ANDIS_RC_SUCCESS;
}


USHORT NEAR ActivateDTR( USHORT ReqHandle, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return ANDIS_RC_SUCCESS;
}


USHORT NEAR ActivateRTS( USHORT ReqHandle, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   return ANDIS_RC_SUCCESS;
}


/************************************************************************/
/*                   Port Management Indications                        */
/************************************************************************/


USHORT NEAR ConnectionActive( USHORT ReqHandle, PMODULE pModule )
{
   return pModule->PCMEntryTbl.PortMgmtInd( pModule->PPAT.Header.Port_ID,
         ReqHandle, pModule->PPAT.PPI_Status, pModule->PPAT.Xmit_Speed,
         ANDIS_FC_CONN_ACTIVE, pModule->PCMEntryTbl.pcm_ds );
}


USHORT NEAR ConnectionInactive( USHORT ReqHandle, PMODULE pModule )
{
   return pModule->PCMEntryTbl.PortMgmtInd( pModule->PPAT.Header.Port_ID,
         ReqHandle, pModule->PPAT.PPI_Status, 0L, ANDIS_FC_CONN_INACTIVE,
         pModule->PCMEntryTbl.pcm_ds );
}


USHORT NEAR PPISignal( USHORT ReqHandle, PMODULE pModule )
{
   return pModule->PCMEntryTbl.PortMgmtInd( pModule->PPAT.Header.Port_ID,
         ReqHandle, pModule->PPAT.PPI_Status, 0L, ANDIS_FC_PPI_SIGNAL,
         pModule->PCMEntryTbl.pcm_ds );
}


USHORT NEAR PPIException( USHORT ReqHandle, USHORT Exception, PMODULE pModule )
{
   return pModule->PCMEntryTbl.PortMgmtInd( pModule->PPAT.Header.Port_ID,
         ReqHandle, Exception, 0L, ANDIS_FC_PPI_EXCEPTION,
         pModule->PCMEntryTbl.pcm_ds );
}


USHORT NEAR PortMgmtReqComplete( USHORT ReqHandle, USHORT Status,
      PMODULE pModule )
{
   return pModule->PCMEntryTbl.PortMgmtInd( pModule->PPAT.Header.Port_ID,
         ReqHandle, pModule->PPAT.PPI_Status, (ULONG)Status,
         ANDIS_FC_PM_REQ_COMPLETE, pModule->PCMEntryTbl.pcm_ds );
}


/************************************************************************/
/*                     Transmit Immediate Data                          */
/************************************************************************/


USHORT _loadds PASCAL FAR TransmitImmData( USHORT PortID, USHORT ReqHandle,
      USHORT DataLength, ULONG pImmData, USHORT FunctionCode, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   if ( FunctionCode != ANDIS_FC_TX_IMM_DATA ) return ANDIS_RC_INVALID_FUNC;

   return ANDIS_RC_SUCCESS;
}


/************************************************************************/
/*                 Transmit Immediate Data Confirm                      */
/************************************************************************/


USHORT NEAR TransmitImmDataConfirm( USHORT ReqHandle, USHORT Status,
      PMODULE pModule )
{
   return pModule->PCMEntryTbl.XmitImmDataCnf( pModule->PPAT.Header.Port_ID,
         ReqHandle, pModule->PPAT.PPI_Status, (ULONG)Status,
         ANDIS_FC_TX_IMM_DATA_CNF, pModule->PCMEntryTbl.pcm_ds );
}


/************************************************************************/
/*                     Receive Immediate Data                           */
/************************************************************************/


USHORT NEAR ReceiveImmData( USHORT ReqHandle, USHORT DataLength,
      PUCHAR pImmData, PMODULE pModule )
{
   return pModule->PCMEntryTbl.RecvImmData( pModule->PPAT.Header.Port_ID,
         ReqHandle, DataLength, (ULONG)pImmData, ANDIS_FC_RX_IMM_DATA,
         pModule->PCMEntryTbl.pcm_ds );
}


/************************************************************************/
/*                 Receive Immediate Data Confirm                       */
/************************************************************************/


USHORT _loadds PASCAL FAR ReceiveImmDataConfirm( USHORT PortID,
      USHORT ReqHandle, USHORT Param1, ULONG Param2, USHORT FunctionCode,
      USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   if ( FunctionCode != ANDIS_FC_RX_IMM_DATA_CNF )
      return ANDIS_RC_INVALID_FUNC;

   return ANDIS_RC_SUCCESS;
}


/************************************************************************/
/*                         PPAT Management                              */
/************************************************************************/


USHORT _loadds PASCAL FAR PPATManagement( USHORT PortID, USHORT ReqHandle,
      USHORT Param1, ULONG pPPAT, USHORT FunctionCode, USHORT MACDS )
{
   PMODULE pModule = MAKEP(MACDS,0);

   if ( FunctionCode != ANDIS_FC_UPDATE_PPAT ) return ANDIS_RC_INVALID_FUNC;

   return ANDIS_RC_SUCCESS;
}


