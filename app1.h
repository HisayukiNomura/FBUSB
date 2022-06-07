/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app1.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP1_Initialize" and "APP1_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP1_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP1_H
#define _APP1_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "usb/usb_host_hid_keyboard.h"
#include "driver/driver_common.h"
// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum {
    /* Application pixels put*/
    APP1_STATE_INIT=0,
    APP1_STATE_OPEN_HOST_LAYER,
    APP1_STATE_WAIT_FOR_HOST_ENABLE,
    APP1_STATE_HOST_ENABLE_DONE,
    APP1_STATE_WAIT_FOR_DEVICE_ATTACH,
    APP1_STATE_DEVICE_ATTACHED,
    APP1_STATE_READ_HID,
    APP1_STATE_DEVICE_DETACHED,
    APP1_STATE_CHANGE_DEVICE_PARAMETERS,
    APP1_USART_STATE_DRIVER_OPEN,
    APP1_USART_STATE_CHECK_FOR_STRING_TO_SEND,
    APP1_USART_STATE_DRIVER_WRITE,
    APP1_STATE_ERROR
    } APP1_STATES;

typedef struct
{
    /* Application last data buffer */
    USB_HOST_HID_KEYBOARD_DATA data;

} APP1_DATA_LAST_DATA;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{

    /* USB Application's current state*/
    APP1_STATES state;
    /* USART Application task state */
    // APP1_STATES usartTaskState;          
    /* Unique handle to USB HID Host Keyboard driver */
    USB_HOST_HID_KEYBOARD_HANDLE handle;
    /* Unique handle to USART driver */
    DRV_HANDLE usartDriverHandle;
    /* Number of bytes written by the USART write*/
    uint32_t nBytesWritten;
    /* Size of the buffer to be written */
    uint32_t stringSize;
    /* Buffer used for USART writing */
    uint8_t string[64];
    /* Holds the current offset in the string buffer */
    uint16_t currentOffset;
    /* Flag used to determine if data is to be written to USART */
    bool stringReady;
    /* Flag used to select CAPSLOCK sequence */
    bool capsLockPressed;
    /* Flag used to select SCROLLLOCK sequence */
    bool scrollLockPressed;
    /* Flag used to select NUMLOCK sequence */
    bool numLockPressed;
    /* Holds the output Report*/
    uint8_t outputReport;
    /* Application current data buffer */
    USB_HOST_HID_KEYBOARD_DATA data;
    /* Application last data buffer */
    APP1_DATA_LAST_DATA lastData;
} APP1_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP1_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP1_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP1_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP1_Initialize ( void );


/*******************************************************************************
  Function:
    void APP1_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP1_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP1_Tasks( void );
 bool isUSBKBConnected(void);


//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP1_H */

/*******************************************************************************
 End of File
 */

