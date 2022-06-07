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
#define LED_USE_FOR_CONNECTION true             // LEDは、接続状態を示すために使われる。LEDをデバッグ用にする時はfalseにする。
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
    // こっちは0.5μS
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
    // こっちだと 1μS
    if (SelVal == 0) {          // SELECT が Hなので上位4bitを出力
        if (kv & 0b10000000) DATA1_Set(); else DATA1_Clear();
        if (kv & 0b01000000) DATA2_Set(); else DATA2_Clear();
        if (kv & 0b00100000) DATA3_Set(); else DATA3_Clear();
        if (kv & 0b00010000) DATA4_Set(); else DATA4_Clear();
    } else {                    // SELECTがLなので、下位4bitを出力
        if (kv & 0b00001000) DATA1_Set(); else DATA1_Clear();
        if (kv & 0b00000100) DATA2_Set(); else DATA2_Clear();
        if (kv & 0b00000010) DATA3_Set(); else DATA3_Clear();
        if (kv & 0b00000001) DATA4_Set(); else DATA4_Clear();
    }

    
#endif  
    
}

// RESET信号の受け取りにはINT3を使う。
// 当初、ポート変更割り込みCNENを使ったが、割り込みがかからなかった。クロックを20MHz
// にすると、取り込みは減った。
// RESETのパルスは11μSしかなく、クロックが遅いとCNENでは変更が取れないようだ。
// INT3にすると問題は発生しなかった。
// その後、システムクロックを20Mhz（セラロックの周波数）にするとCNENでも反応した。
// SELECTは、HIGH-110μS　LOW-40μSくらいあるので、CNENでも取りこぼさない。）
// 
// RESETとRESETの間は、37mS（37000μS）
// RESETのストローブは10μS
// SELECTは、最初の L が60μS、　Hが110μS、それ以降のLは40μS

uint8_t  iFCConnect = 0;                // ファミコン切断カウント。タスクの中でこのカウンタを増やしていく。
bool isFCConnected;

static bool isStartRound = false;              // RESETからの一連のシーケンスが動いているかのフラグ
static uint8_t SelectCount = 0;                 // SELECTパルスの数

static void RESET_Handler(GPIO_PIN pin, uintptr_t context)
{
    if (RESET_Get()==0) {
        kvWk  = 0;

        //LED_GREEN_Set();
        bool isSwapped = SwapKeyMap();                           // バックで更新されたキーテーブルを読み出しようにフロントに持ってくる
        if (isSwapped == false) {
            LED_ORANGE_Toggle();
        }
        KeyIdxReset();                          // キーのインデックスを０にする
        isStartRound  = true;
        SelectCount = 0;
        uint8_t SelVal = SELECT_Get();          // 最初の（1行目の）データをバスに出力する
        Out2DataPort(SelVal);
        //LED_GREEN_Clear();
        iFCConnect = 0;                         // リセット信号が確認出来たら、ファミコン切断カウンタをゼロにする
        isFCConnected = true;
    }
}

/*
 * GPIO(SELECT)の変更ハンドラ。
 * SELECTは、エッジ割り込みである必要があり、INTにすると面倒になる。そのため、ポート
 * 変更の割り込みにした。ただ、なんか取れないこともあり、ちょっと不安。
 *  
 */


static void GPIO_Change_Handler(GPIO_PIN pin, uintptr_t context)
{
    if (isStartRound == false) {                // RESETからのシーケンスが始まっていなければ何もしない
        return;
    }
    if (pin == SELECT_PIN) {  
        uint8_t SelVal = SELECT_Get();
        // SELECT信号は、0の時には下位４ビット、１の時には上位４ビットをポートに出力
        // する。この繰り返し。信号の立下りで、行を１つ増やしていく。
        if (SelVal == 0) {      // 立下りの場合
            KeyIdxInc();
       } else {
            SelectCount++;      // 立ち上がった時はカウントをあげる。
            if (SelectCount == 9) {
                isStartRound = false;
            }
       }
        // 信号が立ち上がっても立ち下がっても、ここでデータバスに信号を出力する。
        // 信号が立ち下がった場合には、インデックスは１つ増えているはず。
        Out2DataPort(SelVal);
    }
}

/*
 *一定時間ごとのタイマー。FCとの接続監視とLEDの点滅を行う。
 */
uint8_t BlinkCounter=0;

void ATimerCallback( TimerHandle_t xTimer )
{
    if (iFCConnect >=5) {
        isFCConnected = false;      // FCとの信号切れ
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
 はじめは、このタスクで色々やろうと思ってたが、実際には速度の都合上、割り込み処理
 * の中で（しかも気合を入れて）やらないと間に合わなかった。このタスクはいらなかったな…
 */

void APP2_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( app2Data.state )
    {
        /* Application's initial state. */
        case APP2_STATE_INIT:
        {
            // ピン変化割り込みでRESETを受け取ろうとしたが、RESETのストローブが急峻なのか、
            // リセットで割り込みを受けることができなかった。（SELECTは問題ない）
            // 仕方ないので、RESETは外部割込み（INT3)で受け取るように変更した。
            
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
                // 特にやることもない
            }
            if (isUSBKBConnected()==false) {        // USBキーボードが接続されている
                // 特にやることもない

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
