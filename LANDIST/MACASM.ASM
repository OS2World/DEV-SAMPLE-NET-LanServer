;*****************************************************************************
;*                                                                           *
;*   Component   : Sample ANDIS MAC Driver                                   *
;*                                                                           *
;*   Module      : MACASM.ASM                                                *
;*                                                                           *
;*   Description : This module contains assembly language routines and       *
;*                 segment definitions needed by the MAC.                    *
;*                                                                           *
;*****************************************************************************

extrn           _Strategy: near
extrn           _pDevHelp: dword

;*****************************************************************************
;*                             Data Segments                                 *
;*****************************************************************************

HEADER          segment dword public 'DATA'     ;Device driver header

public          MACHeader

MACHeader       DD      0FFFFFFFFH
                DW      8080H           ; Char device, No Open/Close, Level 1
                DW      strategy_asm    ; ASM strategy entry point
                DW      0               ; No IDC entry point
                DB      'ANDISMC$'
                DW      00H
                DW      00H
                DW      00H
                DW      00H
                DD      00H

HEADER          ends

_DATA           segment dword public 'DATA'      ;Global initialized data
_DATA           ends

CONST           segment dword public 'CONST'     ;Global constants
CONST           ends

_BSS            segment dword public 'BSS'       ;Global uninitialized data
_BSS            ends

END_DATA        segment dword public 'END_DATA'  ;Marks the end of the data

public          _EndofData
_EndofData      dw      0FFFFH

END_DATA        ends


;*****************************************************************************
;*                             Code Segments                                 *
;*****************************************************************************

_TEXT           segment dword public 'CODE'     ;Not initialization code


;*****************************************************************************
;*                                                                           *
;*   Routine     : strategy_asm                                              *
;*                                                                           *
;*   Description : This routine is the strategy entry point for the device   *
;*                 driver.  This routine is simply the assembly language     *
;*                 interface to the C language stragegy routine.  The        *
;*                 address of the request block is pushed onto the stack     *
;*                 and the C language stragegy routine is called.            *
;*                                                                           *
;*****************************************************************************

public          strategy_asm

strategy_asm    proc    far

                push    es              ; Push addr of Req Pkt on stack
                push    bx

                call    _Strategy       ; Call C strategy routine

                pop     bx
                pop     es
                ret

strategy_asm    endp


;*****************************************************************************
;*                                                                           *
;*   Routine     : _DevHelper                                                *
;*                                                                           *
;*   Description : This is an assembly language routine that can be called   *
;*                 from C to issue DevHelp calls to the operating system.    *
;*                 The calling C routine passes in a register structure      *
;*                 (defined below).  This routine copies the values from     *
;*                 the structure into the actual registers and calls         *
;*                 DevHelp.  The result of the DevHelp (specified with the   *
;*                 carry flag) is returned to the C routine in the AX        *
;*                 register (1=Error,0=Success).                             *
;*                                                                           *
;*****************************************************************************

Registers struc
                axo     dw      ?       ;ax register
                bxo     dw      ?       ;bx register
                cxo     dw      ?       ;cx register
                dxo     dw      ?       ;dx register
                sio     dw      ?       ;si register
                dio     dw      ?       ;di register
                cflag   dw      ?       ;flags register
                eso     dw      ?       ;es register-must be NULL or real value
Registers ends

devhelpstack struc
                oldbp   dw      ?       ;saved bp
                retadr  dd      ?       ;long return address
                regadd  dd      ?       ;long pointer to reg structure
devhelpstack ends

public          _DevHelper

_DevHelper      proc    far

                push    bp
                mov     bp, sp          ;set frame
                push    di              ;save index regs
                push    si

                ; get values from addresses
                les     di,  [bp + regadd] ;get address of start of structure
                mov     ax, es:[di].sio
                push    ax                 ;save es and si for last
                mov     ax, es:[di].eso
                push    ax                 ;save es and si for last
                mov     ax, es:[di].axo    ;ax-value
                mov     cx, es:[di].cxo    ;copy all values from structure to
                mov     dx, es:[di].dxo    ;the actual registers
                mov     bx, es:[di].bxo    ;before making the call
                mov     di, es:[di].dio
                pop     es
                pop     si                 ;restore registers for call

                call    [_pDevHelp]        ;call the device-helper routine

                ;return the values to the calling program
                push    ax              ;save ax
                lahf                    ;copy flags into ah register
                mov     cl, ah          ;copy to cl
                xor     ch, ch          ;zero upper byte
                pop     ax              ;restore ax
                push    es              ;save these
                push    di              ;for last loading
                les     di, [bp + regadd]  ;get address of start of structure
                mov     es:[di].cflag, cx  ;return carryflag value
                mov     es:[di].axo, ax  ;return value of ax
                mov     es:[di].bxo, bx  ;return bx-value
                mov     es:[di].dxo, dx  ;return dx-value
                pop     ax
                mov     es:[di].dio, ax  ;return di
                pop     ax               ;and es
                mov     es:[di].eso, ax
                mov     ax, cx           ;get carry flag into AX for return
                and     ax, 1            ;only carry bit is returned

                pop     si               ;restore real si and di
                pop     di
                pop     bp               ;and base pointer
                ret

_DevHelper      endp

_TEXT           ends

INITCODE        segment dword public 'CODE'     ;Initialization code
INITCODE        ends


;*****************************************************************************
;*                                Groups                                     *
;*****************************************************************************

DGROUP          group   HEADER, _DATA, CONST, _BSS, END_DATA
CGROUP          group   _TEXT, INITCODE

        end

