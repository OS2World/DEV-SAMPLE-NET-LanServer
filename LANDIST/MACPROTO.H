/*****************************************************************************/
/*                                                                           */
/*   Component   : Sample ANDIS MAC Driver                                   */
/*                                                                           */
/*   Module      : MACPROTO.H                                                */
/*                                                                           */
/*   Description : This module contains prototypes for all the routines      */
/*                 in the MAC.                                               */
/*                                                                           */
/*****************************************************************************/


/* Routines in MACINIT.C */
VOID    NEAR Strategy( PREQPKT pRequestPacket );
VOID    NEAR EndofCode( VOID );
VOID    FAR  Initialize( PININITRP pInInitRP );
BOOL    NEAR ProcessConfigData( struct ModCfg *pConfigData );
VOID    NEAR CreateModule( struct ModCfg *pModCfg );
BOOL    NEAR ProcessKeywords( struct KeywordEntry *pKEList, PMODULE pModule );
BOOL    NEAR AddModule( USHORT GDTSel, PPHYSADDR pPhysAddr );
PMODULE NEAR VirtualAddr( PPHYSADDR pPhysAddr );
VOID    NEAR InitNDISTables( PMODULE pModule, USHORT GDTSel );
VOID    NEAR InitANDISTables( PMODULE pModule, USHORT GDTSel );
BOOL    NEAR DisplayMsg( USHORT MsgNumber, PCHAR pIvString );
VOID    NEAR LogMsg( USHORT MsgNumber, USHORT IvCount, PCHAR pIvString );
VOID    NEAR Display( PCHAR pString );
BOOL    NEAR StringsEqual( PUCHAR pFirst, PUCHAR pSecond );
BOOL    NEAR StringCopy( UCHAR Dest[], UCHAR Source[], USHORT DestSize );


/* Routines in MACNDIS.C */
USHORT _loadds PASCAL FAR SystemRequest( ULONG Param1, ULONG Param2,
      USHORT Param3, USHORT Opcode, USHORT MACDS );
USHORT NEAR SRBind( struct CommChar *pProtCCT, struct CommChar **ppMACCCT,
      USHORT MACDS );
USHORT _loadds PASCAL FAR GeneralRequest( USHORT ProtID, USHORT ReqHandle,
      USHORT Param1, PUCHAR Param2, USHORT Opcode, USHORT MACDS );
USHORT NEAR GRInitiateDiagnostics( USHORT ProtID, USHORT ReqHandle,
      USHORT MACDS );
USHORT NEAR GRReadErrorLog( USHORT ProtID, USHORT ReqHandle, USHORT LogLen,
      PUCHAR pLogBuff, USHORT MACDS );
USHORT NEAR GRSetStationAddress( USHORT ProtID, USHORT ReqHandle,
      PUCHAR pAdaptAddr, USHORT MACDS );
USHORT NEAR GROpenAdapter( USHORT ProtID, USHORT ReqHandle, USHORT OpenOptions,
      USHORT MACDS );
USHORT NEAR GRCloseAdapter( USHORT ProtID, USHORT ReqHandle, USHORT MACDS );
USHORT NEAR GRResetMAC( USHORT ProtID, USHORT ReqHandle, USHORT MACDS );
USHORT NEAR GRSetPacketFilter( USHORT ProtID, USHORT ReqHandle,
      USHORT FilterMask, USHORT MACDS );
USHORT NEAR GRAddMulticastAddress( USHORT ProtID, USHORT ReqHandle,
      PUCHAR pMultiAddr, USHORT MACDS );
USHORT NEAR GRDeleteMulticastAddress( USHORT ProtID, USHORT ReqHandle,
      PUCHAR pMultiAddr, USHORT MACDS );
USHORT NEAR GRUpdateStatistics( USHORT ProtID, USHORT ReqHandle, USHORT MACDS );
USHORT NEAR GRClearStatistics( USHORT ProtID, USHORT ReqHandle, USHORT MACDS );
USHORT NEAR GRInterrupt( USHORT ProtID, USHORT MACDS );
USHORT NEAR GRSetFunctionalAddress( USHORT ProtID, USHORT ReqHandle,
      PUCHAR pFunctAddr, USHORT MACDS );
USHORT NEAR GRSetLookAhead( USHORT ProtID, USHORT ReqHandle, USHORT Length,
      USHORT MACDS );
USHORT NEAR RequestConfirm( USHORT ProtID, USHORT ReqHandle, USHORT Status,
      USHORT Request, PMODULE pModule );
USHORT NEAR SIRingStatus( USHORT Status, PUCHAR pIndicate, PMODULE pModule );
USHORT NEAR SIAdapterCheck( USHORT Reason, PUCHAR pIndicate, PMODULE pModule );
USHORT NEAR SIStartReset( PUCHAR pIndicate, PMODULE pModule );
USHORT NEAR SIEndReset( PUCHAR pIndicate, PMODULE pModule );
USHORT NEAR SIInterrupt( PUCHAR pIndicate, PMODULE pModule );
USHORT _loadds PASCAL FAR TransmitChain( USHORT ProtID, USHORT ReqHandle,
      struct TxBufDesc *pTxBufDesc, USHORT MACDS );
USHORT NEAR TransmitConfirm( USHORT ProtID, USHORT ReqHandle, USHORT Status,
      PMODULE pModule );
USHORT NEAR ReceiveLookahead( USHORT FrameSize, USHORT BytesAvail,
      PUCHAR pBuffer, PUCHAR pIndicate, PMODULE pModule );
USHORT _loadds PASCAL FAR TransferData( PUSHORT pBytesCopied,
      USHORT FrameOffset, struct TDBufDesc *pTDBufDesc, USHORT MACDS );
USHORT NEAR IndicationComplete( PMODULE pModule );
USHORT NEAR ReceiveChain( USHORT FrameSize, USHORT ReqHandle,
      struct RxBufDesc *pRxBufDesc, PUCHAR pIndicate, PMODULE pModule );
USHORT _loadds PASCAL FAR ReceiveRelease( USHORT ReqHandle, USHORT MACDS );
USHORT _loadds PASCAL FAR IndicationsOff( USHORT MACDS );
USHORT _loadds PASCAL FAR IndicationsOn( USHORT MACDS );


/* Routines in MACANDIS.C */
USHORT _loadds PASCAL FAR PortManagement( USHORT PortID, USHORT ReqHandle,
      USHORT Param1, ULONG Param2, USHORT FunctionCode, USHORT MACDS );
USHORT NEAR RegisterPCM( USHORT ReqHandle, USHORT PortID,
      PENTRYTBL pEntryTbl, USHORT MACDS );
USHORT NEAR ReleasePCM( USHORT ReqHandle, USHORT PortID, USHORT MACDS );
USHORT NEAR ActivateMode( USHORT ReqHandle, USHORT MACMode, USHORT MACDS );
USHORT NEAR RetrievePPIStatus( USHORT ReqHandle, PULONG pPPIStatus,
      USHORT MACDS );
USHORT NEAR GetPPATParms( USHORT ReqHandle, USHORT PPATSize, PXVPPAT pPPAT,
      USHORT MACDS );
USHORT NEAR SetPPATParms( USHORT ReqHandle, PXVPPAT pPPAT, USHORT MACDS );
USHORT NEAR ResetPort( USHORT ReqHandle, USHORT ResetType, USHORT MACDS );
USHORT NEAR ActivatePhysConn( USHORT ReqHandle, USHORT Mode, ULONG Timeout,
      USHORT MACDS );
USHORT NEAR DeactivatePhysConn( USHORT ReqHandle, ULONG Timeout, USHORT MACDS );
USHORT NEAR RegisterNullDLC( USHORT ReqHandle, USHORT PortID,
      PENTRYTBL pEntryTbl, USHORT MACDS );
USHORT NEAR ReleaseNullDLC( USHORT ReqHandle, USHORT PortID, USHORT MACDS );
USHORT NEAR ActivateDTR( USHORT ReqHandle, USHORT MACDS );
USHORT NEAR ActivateRTS( USHORT ReqHandle, USHORT MACDS );
USHORT NEAR ConnectionActive( USHORT ReqHandle, PMODULE pModule );
USHORT NEAR ConnectionInactive( USHORT ReqHandle, PMODULE pModule );
USHORT NEAR PPISignal( USHORT ReqHandle, PMODULE pModule );
USHORT NEAR PPIException( USHORT ReqHandle, USHORT Exception, PMODULE pModule );
USHORT NEAR PortMgmtReqComplete( USHORT ReqHandle, USHORT Status,
      PMODULE pModule );
USHORT _loadds PASCAL FAR TransmitImmData( USHORT PortID, USHORT ReqHandle,
      USHORT DataLength, ULONG pImmData, USHORT FunctionCode, USHORT MACDS );
USHORT NEAR TransmitImmDataConfirm( USHORT ReqHandle, USHORT Status,
      PMODULE pModule );
USHORT NEAR ReceiveImmData( USHORT ReqHandle, USHORT DataLength,
      PUCHAR pImmData, PMODULE pModule );
USHORT _loadds PASCAL FAR ReceiveImmDataConfirm( USHORT PortID,
      USHORT ReqHandle, USHORT Param1, ULONG Param2, USHORT FunctionCode,
      USHORT MACDS );
USHORT _loadds PASCAL FAR PPATManagement( USHORT PortID, USHORT ReqHandle,
      USHORT Param1, ULONG pPPAT, USHORT FunctionCode, USHORT MACDS );


/* Routines in MACASM.ASM */
USHORT  FAR  DevHelper( PREGS pRegs );

