/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app1.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app1.h"
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "config/default/peripheral/gpio/plib_gpio.h"
#include "i2cLCD.h"
#include "KeyCodeDef.h"
#include "mylib.h"
#include "config/default/peripheral/evic/plib_evic.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP1_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP1_DATA app1Data;



// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/

// キーボードが接続されているかを返す
inline bool isUSBKBConnected(void)
{
    return app1Data.state == APP1_STATE_READ_HID;
}





// USBホストのイベントハンドラ
USB_HOST_EVENT_RESPONSE APP_USBHostEventHandler (USB_HOST_EVENT event, void * eventData, uintptr_t context)

{
    switch(event) {
        case USB_HOST_EVENT_DEVICE_UNSUPPORTED:{
            break;
        }
        default: {
            break;
        }
    }
    return USB_HOST_EVENT_RESPONSE_NONE;
}



void APP_USBHostHIDKeyboardEventHandler(USB_HOST_HID_KEYBOARD_HANDLE handle, 
                                        USB_HOST_HID_KEYBOARD_EVENT event, void * pData)
{   
    switch (event) {

        case USB_HOST_HID_KEYBOARD_EVENT_ATTACH:{
            app1Data.handle = handle;
            app1Data.state =  APP1_STATE_DEVICE_ATTACHED;
            app1Data.nBytesWritten = 0;
            app1Data.stringReady = false;
            memset(&app1Data.string, 0, sizeof(app1Data.string));
            memset(&app1Data.lastData, 0, sizeof(app1Data.lastData));
            app1Data.stringSize = 0;
            app1Data.capsLockPressed = false;
            app1Data.scrollLockPressed = false;
            app1Data.numLockPressed = false;
            app1Data.outputReport = 0;
            break;
        }
        case USB_HOST_HID_KEYBOARD_EVENT_DETACH:{
            app1Data.handle = handle;
            app1Data.state = APP1_STATE_DEVICE_DETACHED;
            app1Data.nBytesWritten = 0;
            app1Data.stringReady = false;
            //app1Data.usartTaskState = APP1_USART_STATE_CHECK_FOR_STRING_TO_SEND;
            memset(&app1Data.string, 0, sizeof(app1Data.string));
            memset(&app1Data.lastData, 0, sizeof(app1Data.lastData));
            app1Data.stringSize = 0;
            app1Data.capsLockPressed = false;
            app1Data.scrollLockPressed = false;
            app1Data.numLockPressed = false;
            app1Data.outputReport = 0;
            break;
        }
        case USB_HOST_HID_KEYBOARD_EVENT_REPORT_RECEIVED:{
            app1Data.handle = handle;
            app1Data.state = APP1_STATE_READ_HID;
            /* Keyboard Data from device */
            memcpy(&app1Data.data, pData, sizeof(app1Data.data));
            break;
        }
        default:
            break;
    }
    return;
}




// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP1_Initialize ( void )

  Remarks:
    See prototype in app1.h.
 */

void APP1_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app1Data.state = APP1_STATE_INIT;

    InitKeyMap();

    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    //TMR5_InterruptEnable();
    //TMR5_Start();
    
}



/******************************************************************************
  Function:
    void APP1_Tasks ( void )

  Remarks:
    See prototype in app1.h.
 */
SemaphoreHandle_t a;


void APP1_Tasks ( void )
{

    
    //uint8_t counter = 0;
    //bool foundInLast = false;   //最後の検出
            
    /* Check the application's current state. */
    switch ( app1Data.state ) {
        /* Application's initial state. */
        case APP1_STATE_INIT:{

            USB_HOST_EventHandlerSet(APP_USBHostEventHandler, 0);
            USB_HOST_HID_KEYBOARD_EventHandlerSet(APP_USBHostHIDKeyboardEventHandler);
			USB_HOST_BusEnable(0);
			app1Data.state = APP1_STATE_WAIT_FOR_HOST_ENABLE;
            LCDInit(16,2,0x5f,false,10);
            LCDClear();
            LCDDisplay(true);
            LCDLocate(10,1);
            LCDWriteText((uint8_t *)"<INIT>");
            UseKeyMap();
            SwapKeyMap();
            UnuseKeyMap(); 
            break;
        }
		case APP1_STATE_WAIT_FOR_HOST_ENABLE:{
            /* Check if the host operation has been enabled */
            LCDLocate(10,1);
            LCDWriteText((uint8_t *)"<W-EN>");
            if(USB_HOST_BusIsEnabled(0)) {
                /* This means host operation is enabled. We can
                 * move on to the next state */
                app1Data.state = APP1_STATE_HOST_ENABLE_DONE;
            }
            break;
        }
        case APP1_STATE_HOST_ENABLE_DONE: {
            LCDLocate(10,1);
            LCDWriteText((uint8_t *)"<D-EN>");
            app1Data.stringSize = 64;
            app1Data.state = APP1_STATE_WAIT_FOR_DEVICE_ATTACH;
            break;
        }
        case APP1_STATE_WAIT_FOR_DEVICE_ATTACH: {
            /* Wait for device attach. The state machine will move
             * to the next state when the attach event
             * is received.  */
            LCDLocate(10,1);
            LCDWriteText((uint8_t *)"<W-AT>");
            break;
        }
        case APP1_STATE_DEVICE_ATTACHED: {
            /* Wait for device report */
            // LCDLocate(10,1);
            // LCDWriteText((uint8_t *)"<ATCH>");
            app1Data.stringReady = true;
            app1Data.stringSize = 64;
            app1Data.state = APP1_STATE_READ_HID;
            break;
        }

        case APP1_STATE_READ_HID:{                  // レポートが来た
            uint8_t count = 0;
            app1Data.stringReady = false;
            app1Data.stringSize = 64;
            //memset(&app1Data.string, 0, sizeof(app1Data.string));

            
            //　修飾キーの処理
            KeyInfo kiMod;
            bool setUnset;
            kiMod = findKey(0xE6);
            setUnset = app1Data.data.modifierKeysData.rightAlt;

            if (setUnset) keyMap[0][kiMod.Base.row] |= kiMod.Base.colmn; else keyMap[0][kiMod.Base.row] &= ~kiMod.Base.colmn;

            kiMod = findKey(0xE2);
            setUnset = app1Data.data.modifierKeysData.leftAlt;
            if (setUnset) keyMap[0][kiMod.Base.row] |= kiMod.Base.colmn; else keyMap[0][kiMod.Base.row] &= ~kiMod.Base.colmn;

            kiMod = findKey(0xE1);
            setUnset = app1Data.data.modifierKeysData.leftShift;
            if (setUnset) keyMap[0][kiMod.Base.row] |= kiMod.Base.colmn; else keyMap[0][kiMod.Base.row] &= ~kiMod.Base.colmn;
            kiMod = findKey(0xE5);
            setUnset = app1Data.data.modifierKeysData.rightShift;
            if (setUnset) keyMap[0][kiMod.Base.row] |= kiMod.Base.colmn; else keyMap[0][kiMod.Base.row] &= ~kiMod.Base.colmn;

            kiMod = findKey(0xE0);
            setUnset = app1Data.data.modifierKeysData.leftControl;
            if (setUnset) keyMap[0][kiMod.Base.row] |= kiMod.Base.colmn; else keyMap[0][kiMod.Base.row] &= ~kiMod.Base.colmn;
            kiMod = findKey(0xE4);
            setUnset = app1Data.data.modifierKeysData.rightControl;
            if (setUnset) keyMap[0][kiMod.Base.row] |= kiMod.Base.colmn; else keyMap[0][kiMod.Base.row] &= ~kiMod.Base.colmn;
            
            kiMod = findKey(0xE3);
            setUnset = app1Data.data.modifierKeysData.leftGui;
            if (setUnset) keyMap[0][kiMod.Base.row] |= kiMod.Base.colmn; else keyMap[0][kiMod.Base.row] &= ~kiMod.Base.colmn;
            kiMod = findKey(0xE7);
            setUnset = app1Data.data.modifierKeysData.rightGui;
            if (setUnset) keyMap[0][kiMod.Base.row] |= kiMod.Base.colmn; else keyMap[0][kiMod.Base.row] &= ~kiMod.Base.colmn;
            


            //　非修飾キーの処理
            // ここで、USBレポートの６つを処理する。https://www.usb.org/sites/default/files/hid1_11.pdf によると、
            // レポート中の６つのキーは、順番を問わない、ということだった。
            // なので、ファミコン側のスキャンが遅く、その間、複数回のレポートが来てしまうと、２つのキーの順番が逆に
            // なってしまうかもしれない。
            // これを避けるため、本当は、レポートをキューにして、ファミコン側のスキャンと比べる必要がある。
            // ただ、実装はかなり面倒になりそうなので、とりあえずバカみたいな実装で済ませてみる。
            
            
            for(count = 0; count < 6; count++) {
                // Non-modifier-key
                if(app1Data.data.nonModifierKeysData[count].event == USB_HID_KEY_PRESSED) {
                    KeyInfo ki =  findKey(app1Data.data.nonModifierKeysData[count].keyCode);
                    keyMap[0][ki.Base.row] |= ki.Base.colmn;
                }
            }
            UpdateUSBKeyReport();
           //LED_ORANGE_Clear();


#if 0
            
            /* We need to display only the non modifier keys */
            for(count = 0; count < 6; count++) {
                if(app1Data.data.nonModifierKeysData[count].event == USB_HID_KEY_PRESSED) {
                    //LCDLocate(10,1);
                    //LCDWriteText((uint8_t *)"<KEYP>");
                       
                    for(counter = 0; counter < 6; counter++) {
                        if((app1Data.lastData.data.nonModifierKeysData[counter].event == USB_HID_KEY_PRESSED) && ((app1Data.lastData.data.nonModifierKeysData[counter].keyCode == app1Data.data.nonModifierKeysData[count].keyCode))) {
                            break;
                        }
                    }
                    if(foundInLast == false) {
                        app1Data.stringReady = true;
                        APP_MapKeyToUsage(app1Data.data.nonModifierKeysData[count].keyCode);
                    } else {
                            /* Reset it it false for next iteration */
                        foundInLast = false;
                    }
                }
            }
                /* Store the present to future */
            memcpy(&app1Data.lastData.data, &app1Data.data, sizeof(app1Data.data));
            
            
            if(app1Data.usartTaskState == APP1_USART_STATE_CHECK_FOR_STRING_TO_SEND) {
                /* We can send Data to USART but need to check*/
                app1Data.stringSize = 64;
                memset(&app1Data.string, 0, sizeof(app1Data.string));
                /* We need to display only the non modifier keys */
                for(count = 0; count < 6; count++) {
                    if(app1Data.data.nonModifierKeysData[count].event == USB_HID_KEY_PRESSED) {
                        LCDLocate(10,1);
                        LCDWriteText((uint8_t *)"<KEYP>");
                       
                        for(counter = 0; counter < 6; counter++) {
                            if((app1Data.lastData.data.nonModifierKeysData[counter].event == USB_HID_KEY_PRESSED) && ((app1Data.lastData.data.nonModifierKeysData[counter].keyCode == app1Data.data.nonModifierKeysData[count].keyCode))) {
                               /*
                                 sysCount = SYS_TMR_SystemCountGet ();
                                if(200 <= 1000 * (sysCount - app1Data.lastData.data.nonModifierKeysData[counter].sysCount) / SYS_TMR_SystemCountFrequencyGet()) {
                                    foundInLast = false;
                                } else {
                                    foundInLast = true;
                                }
                                */
                                break;
                            }
                        }
                        if(foundInLast == false) {
                            app1Data.stringReady = true;
                            APP_MapKeyToUsage(app1Data.data.nonModifierKeysData[count].keyCode);
                        } else {

                            /* Reset it it false for next iteration */

                            foundInLast = false;
                        }
                    }
                }
                /* Store the present to future */
                memcpy(&app1Data.lastData.data, &app1Data.data, sizeof(app1Data.data));
            }
#endif

            break;

        }

        case APP1_STATE_DEVICE_DETACHED:
            LCDLocate(10,1);
            LCDWriteText((uint8_t *)"<DTCH>");
            
            app1Data.state = APP1_STATE_HOST_ENABLE_DONE;
            break;
        case APP1_STATE_ERROR:
            /* The application comes here when the demo
             * has failed. Provide LED indication .*/
//            BSP_LEDOn( APP_USB_LED_1 );
            LCDLocate(10,1);
            LCDWriteText((uint8_t *)"<ERR!>");
            
            break;
        default:
            break;
    }

}


/*******************************************************************************
 End of File
 */
