/*******************************************************************************
  GPIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.h

  Summary:
    GPIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (GPIO) module.

*******************************************************************************/

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

#ifndef PLIB_GPIO_H
#define PLIB_GPIO_H

#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************


/*** Macros for DATA4 pin ***/
#define DATA4_Set()               (LATBSET = (1<<0))
#define DATA4_Clear()             (LATBCLR = (1<<0))
#define DATA4_Toggle()            (LATBINV= (1<<0))
#define DATA4_OutputEnable()      (TRISBCLR = (1<<0))
#define DATA4_InputEnable()       (TRISBSET = (1<<0))
#define DATA4_Get()               ((PORTB >> 0) & 0x1)
#define DATA4_PIN                  GPIO_PIN_RB0

/*** Macros for DATA3 pin ***/
#define DATA3_Set()               (LATBSET = (1<<1))
#define DATA3_Clear()             (LATBCLR = (1<<1))
#define DATA3_Toggle()            (LATBINV= (1<<1))
#define DATA3_OutputEnable()      (TRISBCLR = (1<<1))
#define DATA3_InputEnable()       (TRISBSET = (1<<1))
#define DATA3_Get()               ((PORTB >> 1) & 0x1)
#define DATA3_PIN                  GPIO_PIN_RB1

/*** Macros for DATA2 pin ***/
#define DATA2_Set()               (LATBSET = (1<<2))
#define DATA2_Clear()             (LATBCLR = (1<<2))
#define DATA2_Toggle()            (LATBINV= (1<<2))
#define DATA2_OutputEnable()      (TRISBCLR = (1<<2))
#define DATA2_InputEnable()       (TRISBSET = (1<<2))
#define DATA2_Get()               ((PORTB >> 2) & 0x1)
#define DATA2_PIN                  GPIO_PIN_RB2

/*** Macros for DATA1 pin ***/
#define DATA1_Set()               (LATBSET = (1<<3))
#define DATA1_Clear()             (LATBCLR = (1<<3))
#define DATA1_Toggle()            (LATBINV= (1<<3))
#define DATA1_OutputEnable()      (TRISBCLR = (1<<3))
#define DATA1_InputEnable()       (TRISBSET = (1<<3))
#define DATA1_Get()               ((PORTB >> 3) & 0x1)
#define DATA1_PIN                  GPIO_PIN_RB3

/*** Macros for SDA pin ***/
#define SDA_Set()               (LATBSET = (1<<4))
#define SDA_Clear()             (LATBCLR = (1<<4))
#define SDA_Toggle()            (LATBINV= (1<<4))
#define SDA_OutputEnable()      (TRISBCLR = (1<<4))
#define SDA_InputEnable()       (TRISBSET = (1<<4))
#define SDA_Get()               ((PORTB >> 4) & 0x1)
#define SDA_PIN                  GPIO_PIN_RB4

/*** Macros for SCL pin ***/
#define SCL_Set()               (LATASET = (1<<4))
#define SCL_Clear()             (LATACLR = (1<<4))
#define SCL_Toggle()            (LATAINV= (1<<4))
#define SCL_OutputEnable()      (TRISACLR = (1<<4))
#define SCL_InputEnable()       (TRISASET = (1<<4))
#define SCL_Get()               ((PORTA >> 4) & 0x1)
#define SCL_PIN                  GPIO_PIN_RA4

/*** Macros for SELECT pin ***/
#define SELECT_Set()               (LATBSET = (1<<7))
#define SELECT_Clear()             (LATBCLR = (1<<7))
#define SELECT_Toggle()            (LATBINV= (1<<7))
#define SELECT_OutputEnable()      (TRISBCLR = (1<<7))
#define SELECT_InputEnable()       (TRISBSET = (1<<7))
#define SELECT_Get()               ((PORTB >> 7) & 0x1)
#define SELECT_PIN                  GPIO_PIN_RB7
#define SELECT_InterruptEnable()   (CNENBSET = (1<<7))
#define SELECT_InterruptDisable()  (CNENBCLR = (1<<7))

/*** Macros for RESET pin ***/
#define RESET_Set()               (LATBSET = (1<<8))
#define RESET_Clear()             (LATBCLR = (1<<8))
#define RESET_Toggle()            (LATBINV= (1<<8))
#define RESET_OutputEnable()      (TRISBCLR = (1<<8))
#define RESET_InputEnable()       (TRISBSET = (1<<8))
#define RESET_Get()               ((PORTB >> 8) & 0x1)
#define RESET_PIN                  GPIO_PIN_RB8
#define RESET_InterruptEnable()   (CNENBSET = (1<<8))
#define RESET_InterruptDisable()  (CNENBCLR = (1<<8))

/*** Macros for LED_GREEN pin ***/
#define LED_GREEN_Set()               (LATBSET = (1<<13))
#define LED_GREEN_Clear()             (LATBCLR = (1<<13))
#define LED_GREEN_Toggle()            (LATBINV= (1<<13))
#define LED_GREEN_OutputEnable()      (TRISBCLR = (1<<13))
#define LED_GREEN_InputEnable()       (TRISBSET = (1<<13))
#define LED_GREEN_Get()               ((PORTB >> 13) & 0x1)
#define LED_GREEN_PIN                  GPIO_PIN_RB13

/*** Macros for LED_ORANGE pin ***/
#define LED_ORANGE_Set()               (LATBSET = (1<<15))
#define LED_ORANGE_Clear()             (LATBCLR = (1<<15))
#define LED_ORANGE_Toggle()            (LATBINV= (1<<15))
#define LED_ORANGE_OutputEnable()      (TRISBCLR = (1<<15))
#define LED_ORANGE_InputEnable()       (TRISBSET = (1<<15))
#define LED_ORANGE_Get()               ((PORTB >> 15) & 0x1)
#define LED_ORANGE_PIN                  GPIO_PIN_RB15


// *****************************************************************************
/* GPIO Port

  Summary:
    Identifies the available GPIO Ports.

  Description:
    This enumeration identifies the available GPIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/

typedef enum
{
    GPIO_PORT_A = 0,
    GPIO_PORT_B = 1,
} GPIO_PORT;

// *****************************************************************************
/* GPIO Port Pins

  Summary:
    Identifies the available GPIO port pins.

  Description:
    This enumeration identifies the available GPIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/

typedef enum
{
    GPIO_PIN_RA0 = 0,
    GPIO_PIN_RA1 = 1,
    GPIO_PIN_RA2 = 2,
    GPIO_PIN_RA3 = 3,
    GPIO_PIN_RA4 = 4,
    GPIO_PIN_RB0 = 16,
    GPIO_PIN_RB1 = 17,
    GPIO_PIN_RB2 = 18,
    GPIO_PIN_RB3 = 19,
    GPIO_PIN_RB4 = 20,
    GPIO_PIN_RB5 = 21,
    GPIO_PIN_RB7 = 23,
    GPIO_PIN_RB8 = 24,
    GPIO_PIN_RB9 = 25,
    GPIO_PIN_RB10 = 26,
    GPIO_PIN_RB11 = 27,
    GPIO_PIN_RB13 = 29,
    GPIO_PIN_RB14 = 30,
    GPIO_PIN_RB15 = 31,

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
    GPIO_PIN_NONE = -1

} GPIO_PIN;

typedef  void (*GPIO_PIN_CALLBACK) ( GPIO_PIN pin, uintptr_t context);

void GPIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t GPIO_PortRead(GPIO_PORT port);

void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value);

uint32_t GPIO_PortLatchRead ( GPIO_PORT port );

void GPIO_PortSet(GPIO_PORT port, uint32_t mask);

void GPIO_PortClear(GPIO_PORT port, uint32_t mask);

void GPIO_PortToggle(GPIO_PORT port, uint32_t mask);

void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortInterruptEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortInterruptDisable(GPIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: Local Data types and Prototypes
// *****************************************************************************
// *****************************************************************************

typedef struct {

    /* target pin */
    GPIO_PIN                 pin;

    /* Callback for event on target pin*/
    GPIO_PIN_CALLBACK        callback;

    /* Callback Context */
    uintptr_t               context;

} GPIO_PIN_CALLBACK_OBJ;

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void GPIO_PinWrite(GPIO_PIN pin, bool value)
{
    GPIO_PortWrite((GPIO_PORT)(pin>>4), (uint32_t)(0x1) << (pin & 0xF), (uint32_t)(value) << (pin & 0xF));
}

static inline bool GPIO_PinRead(GPIO_PIN pin)
{
    return (bool)(((GPIO_PortRead((GPIO_PORT)(pin>>4))) >> (pin & 0xF)) & 0x1);
}

static inline bool GPIO_PinLatchRead(GPIO_PIN pin)
{
    return (bool)((GPIO_PortLatchRead((GPIO_PORT)(pin>>4)) >> (pin & 0xF)) & 0x1);
}

static inline void GPIO_PinToggle(GPIO_PIN pin)
{
    GPIO_PortToggle((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinSet(GPIO_PIN pin)
{
    GPIO_PortSet((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinClear(GPIO_PIN pin)
{
    GPIO_PortClear((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInputEnable(GPIO_PIN pin)
{
    GPIO_PortInputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinOutputEnable(GPIO_PIN pin)
{
    GPIO_PortOutputEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInterruptEnable(GPIO_PIN pin)
{
    GPIO_PortInterruptEnable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

static inline void GPIO_PinInterruptDisable(GPIO_PIN pin)
{
    GPIO_PortInterruptDisable((GPIO_PORT)(pin>>4), 0x1 << (pin & 0xF));
}

bool GPIO_PinInterruptCallbackRegister(
    GPIO_PIN pin,
    const   GPIO_PIN_CALLBACK callBack,
    uintptr_t context
);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GPIO_H
