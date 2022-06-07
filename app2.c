/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app2.c

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

#include "app2.h"
#include "KeyCodeDef.h"
#include "app1.h"
#include "config/default/peripheral/gpio/plib_gpio.h"
#include "i2cLCD.h"
#include "KeyCodeDef.h"
#include "mylib.h"
#include "config/default/peripheral/evic/plib_evic.h"

#include "config/default/FreeRTOSConfig.h"
#include "third_party/rtos/FreeRTOS/Source/include/timers.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
#define LED_USE_FOR_CONNECTION true             // LED�́A�ڑ���Ԃ��������߂Ɏg����BLED���f�o�b�O�p�ɂ��鎞��false�ɂ���B
// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP2_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/




APP2_DATA app2Data;

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


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************
static uint8_t kvWk =0;

void Out2DataPort(uint8_t SelVal)
{
    uint32_t kv = KeyIdxValue();
#if  0
    // ��������0.5��S
    if (SelVal == 0) {
        kv = (kv >> 4) | 0xFFFFFF00;
    } else {
        kv = kv | 0xFFFFFF00;
    }
     uint32_t LB = LATB & 0xFFFFFF00;
    LB |= kv;
    LATB = LB;
#else
    kvWk = (kvWk | kv);
    // ���������� 1��S
    if (SelVal == 0) {          // SELECT �� H�Ȃ̂ŏ��4bit���o��
        if (kv & 0b10000000) DATA1_Set(); else DATA1_Clear();
        if (kv & 0b01000000) DATA2_Set(); else DATA2_Clear();
        if (kv & 0b00100000) DATA3_Set(); else DATA3_Clear();
        if (kv & 0b00010000) DATA4_Set(); else DATA4_Clear();
    } else {                    // SELECT��L�Ȃ̂ŁA����4bit���o��
        if (kv & 0b00001000) DATA1_Set(); else DATA1_Clear();
        if (kv & 0b00000100) DATA2_Set(); else DATA2_Clear();
        if (kv & 0b00000010) DATA3_Set(); else DATA3_Clear();
        if (kv & 0b00000001) DATA4_Set(); else DATA4_Clear();
    }

    
#endif  
    
}

// RESET�M���̎󂯎��ɂ�INT3���g���B
// �����A�|�[�g�ύX���荞��CNEN���g�������A���荞�݂�������Ȃ������B�N���b�N��20MHz
// �ɂ���ƁA��荞�݂͌������B
// RESET�̃p���X��11��S�����Ȃ��A�N���b�N���x����CNEN�ł͕ύX�����Ȃ��悤���B
// INT3�ɂ���Ɩ��͔������Ȃ������B
// ���̌�A�V�X�e���N���b�N��20Mhz�i�Z�����b�N�̎��g���j�ɂ����CNEN�ł����������B
// SELECT�́AHIGH-110��S�@LOW-40��S���炢����̂ŁACNEN�ł���肱�ڂ��Ȃ��B�j
// 
// RESET��RESET�̊Ԃ́A37mS�i37000��S�j
// RESET�̃X�g���[�u��10��S
// SELECT�́A�ŏ��� L ��60��S�A�@H��110��S�A����ȍ~��L��40��S

uint8_t  iFCConnect = 0;                // �t�@�~�R���ؒf�J�E���g�B�^�X�N�̒��ł��̃J�E���^�𑝂₵�Ă����B
bool isFCConnected;

static bool isStartRound = false;              // RESET����̈�A�̃V�[�P���X�������Ă��邩�̃t���O
static uint8_t SelectCount = 0;                 // SELECT�p���X�̐�

static void RESET_Handler(GPIO_PIN pin, uintptr_t context)
{
    if (RESET_Get()==0) {
        kvWk  = 0;

        //LED_GREEN_Set();
        bool isSwapped = SwapKeyMap();                           // �o�b�N�ōX�V���ꂽ�L�[�e�[�u����ǂݏo���悤�Ƀt�����g�Ɏ����Ă���
        if (isSwapped == false) {
            LED_ORANGE_Toggle();
        }
        KeyIdxReset();                          // �L�[�̃C���f�b�N�X���O�ɂ���
        isStartRound  = true;
        SelectCount = 0;
        uint8_t SelVal = SELECT_Get();          // �ŏ��́i1�s�ڂ́j�f�[�^���o�X�ɏo�͂���
        Out2DataPort(SelVal);
        //LED_GREEN_Clear();
        iFCConnect = 0;                         // ���Z�b�g�M�����m�F�o������A�t�@�~�R���ؒf�J�E���^���[���ɂ���
        isFCConnected = true;
    }
}

/*
 * GPIO(SELECT)�̕ύX�n���h���B
 * SELECT�́A�G�b�W���荞�݂ł���K�v������AINT�ɂ���Ɩʓ|�ɂȂ�B���̂��߁A�|�[�g
 * �ύX�̊��荞�݂ɂ����B�����A�Ȃ񂩎��Ȃ����Ƃ�����A������ƕs���B
 *  
 */


static void GPIO_Change_Handler(GPIO_PIN pin, uintptr_t context)
{
    if (isStartRound == false) {                // RESET����̃V�[�P���X���n�܂��Ă��Ȃ���Ή������Ȃ�
        return;
    }
    if (pin == SELECT_PIN) {  
        uint8_t SelVal = SELECT_Get();
        // SELECT�M���́A0�̎��ɂ͉��ʂS�r�b�g�A�P�̎��ɂ͏�ʂS�r�b�g���|�[�g�ɏo��
        // ����B���̌J��Ԃ��B�M���̗�����ŁA�s���P���₵�Ă����B
        if (SelVal == 0) {      // ������̏ꍇ
            KeyIdxInc();
       } else {
            SelectCount++;      // �����オ�������̓J�E���g��������B
            if (SelectCount == 9) {
                isStartRound = false;
            }
       }
        // �M���������オ���Ă������������Ă��A�����Ńf�[�^�o�X�ɐM�����o�͂���B
        // �M�����������������ꍇ�ɂ́A�C���f�b�N�X�͂P�����Ă���͂��B
        Out2DataPort(SelVal);
    }
}

/*
 *��莞�Ԃ��Ƃ̃^�C�}�[�BFC�Ƃ̐ڑ��Ď���LED�̓_�ł��s���B
 */
uint8_t BlinkCounter=0;

void ATimerCallback( TimerHandle_t xTimer )
{
    if (iFCConnect >=5) {
        isFCConnected = false;      // FC�Ƃ̐M���؂�
    } else {
        iFCConnect++;
    }
    BlinkCounter++;
    if (BlinkCounter == 0xFF) {
        BlinkCounter = 0;
    }
    
#if LED_USE_FOR_CONNECTION == true
    if (isFCConnected) {
       LED_GREEN_Set();
    } else if ((BlinkCounter  & 0x07) == 0) {
       LED_GREEN_Toggle();
    }
    if (isUSBKBConnected()) {
        LED_ORANGE_Set();
    } else if ((BlinkCounter & 0x7) == 0) {
        LED_ORANGE_Toggle();
    }    
#endif
    
}
TimerHandle_t   hTimer = 0;
BaseType_t TimerStarted;


/*******************************************************************************
  Function:
    void APP2_Initialize ( void )

  Remarks:
    See prototype in app2.h.
 */
void APP2_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    if (hTimer == 0) {
        hTimer = xTimerCreate("Hertbeat", pdMS_TO_TICKS( 100 ),pdTRUE,0,ATimerCallback);
        if (hTimer != 0) {
                TimerStarted = xTimerStart(hTimer, 0 ); 
        }
    }

}


/******************************************************************************
  Function:
    void APP2_Tasks ( void )

  Remarks:
 �͂��߂́A���̃^�X�N�ŐF�X��낤�Ǝv���Ă����A���ۂɂ͑��x�̓s����A���荞�ݏ���
 * �̒��Łi�������C�������āj���Ȃ��ƊԂɍ���Ȃ������B���̃^�X�N�͂���Ȃ������ȁc
 */

void APP2_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app2Data.state )
    {
        /* Application's initial state. */
        case APP2_STATE_INIT:
        {
            // �s���ω����荞�݂�RESET���󂯎�낤�Ƃ������ARESET�̃X�g���[�u���}�s�Ȃ̂��A
            // ���Z�b�g�Ŋ��荞�݂��󂯂邱�Ƃ��ł��Ȃ������B�iSELECT�͖��Ȃ��j
            // �d���Ȃ��̂ŁARESET�͊O�������݁iINT3)�Ŏ󂯎��悤�ɕύX�����B
            
            bool appInitialized = true;
            app2Data.state = APP2_STATE_INIT;

           
            GPIO_PinInterruptCallbackRegister(RESET_PIN, RESET_Handler, 0);
            GPIO_PinInterruptCallbackRegister(SELECT_PIN, GPIO_Change_Handler, 0);
            SELECT_InterruptEnable();
            RESET_InterruptEnable();
    
            DATA1_Clear();
            DATA2_Clear();
            DATA3_Clear();
            DATA4_Clear();
            


            
            if (appInitialized)
            {
                app2Data.state = APP2_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP2_STATE_SERVICE_TASKS:
        {
            #if LED_USE_FOR_CONNECTION == true
            if (isFCConnected == false) {
                // ���ɂ�邱�Ƃ��Ȃ�
            }
            if (isUSBKBConnected()==false) {        // USB�L�[�{�[�h���ڑ�����Ă���
                // ���ɂ�邱�Ƃ��Ȃ�

            }
            #endif

            break;
        }
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}


/*******************************************************************************
 End of File
 */
