/*******************************************************************************
  System Interrupts File

  Company:
    Microchip Technology Inc.

  File Name:
    interrupt_a.S

  Summary:
    Raw ISR definitions.

  Description:
    This file maps all the interrupt vectors to their corresponding
    implementations. If a particular module interrupt is used, then its ISR
    definition can be found in corresponding PLIB source file. If a module
    interrupt is not used, then its ISR implementation is mapped to dummy
    handler.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <xc.h>
#include "ISR_Support.h"

   .extern  CORE_TIMER_Handler

   .section .vector_0,code, keep
   .equ     __vector_dispatch_0, IntVectorCORE_TIMER_Handler
   .global  __vector_dispatch_0
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorCORE_TIMER_Handler

IntVectorCORE_TIMER_Handler:
    la    $26,  _CORE_TIMER_Handler
    jr    $26
    nop
    .end    IntVectorCORE_TIMER_Handler

   .section .CORE_TIMER_Handler_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _CORE_TIMER_Handler

_CORE_TIMER_Handler:
    portSAVE_CONTEXT
    la    s6,  CORE_TIMER_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end    _CORE_TIMER_Handler
   .extern  TIMER_5_Handler

   .section .vector_20,code, keep
   .equ     __vector_dispatch_20, IntVectorTIMER_5_Handler
   .global  __vector_dispatch_20
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorTIMER_5_Handler

IntVectorTIMER_5_Handler:
    la    $26,  _TIMER_5_Handler
    jr    $26
    nop
    .end    IntVectorTIMER_5_Handler

   .section .TIMER_5_Handler_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _TIMER_5_Handler

_TIMER_5_Handler:
    portSAVE_CONTEXT
    la    s6,  TIMER_5_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end    _TIMER_5_Handler
   .extern  USB_1_Handler

   .section .vector_30,code, keep
   .equ     __vector_dispatch_30, IntVectorUSB_1_Handler
   .global  __vector_dispatch_30
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorUSB_1_Handler

IntVectorUSB_1_Handler:
    la    $26,  _USB_1_Handler
    jr    $26
    nop
    .end    IntVectorUSB_1_Handler

   .section .USB_1_Handler_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _USB_1_Handler

_USB_1_Handler:
    portSAVE_CONTEXT
    la    s6,  USB_1_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end    _USB_1_Handler
   .extern  CHANGE_NOTICE_Handler

   .section .vector_34,code, keep
   .equ     __vector_dispatch_34, IntVectorCHANGE_NOTICE_Handler
   .global  __vector_dispatch_34
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorCHANGE_NOTICE_Handler

IntVectorCHANGE_NOTICE_Handler:
    la    $26,  _CHANGE_NOTICE_Handler
    jr    $26
    nop
    .end    IntVectorCHANGE_NOTICE_Handler

   .section .CHANGE_NOTICE_Handler_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _CHANGE_NOTICE_Handler

_CHANGE_NOTICE_Handler:
    portSAVE_CONTEXT
    la    s6,  CHANGE_NOTICE_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end    _CHANGE_NOTICE_Handler

