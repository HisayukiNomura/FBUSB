/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "i2cLCD.h"
#include "config/default/peripheral/tmr/plib_tmr5.h"
#include "config/default/peripheral/gpio/plib_gpio.h"
#include "config/default/library/i2cbb/i2c_bb.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */
int global_data;


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

void I2C_BB_Callback(uintptr_t context);

/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */

#define LCD_ADDR    0b0111110

#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80


// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00


static uint8_t buffer[4];

uint8_t     numLines;
uint8_t     numColumns;


  bool is5V;
  uint8_t contrast;
  uint8_t displayfunction;
  uint8_t displaycontrol;
  uint8_t displaymode;
  uint8_t waitDefault;
  
  

void LCDInit(uint8_t a_numLines, uint8_t a_numColumns, int8_t a_Contrast, bool a_is5V,uint8_t waitTick) 
{
    numLines = a_numLines;
    numColumns = a_numColumns;
    is5V = a_is5V;
    contrast = a_Contrast;
    waitDefault = waitTick;
    
    
    I2C_BB_CallbackRegister(I2C_BB_Callback, (uintptr_t)NULL);    

    
    LCDWriteCmdByteSync(0x38,waitDefault);      // Function Set 8 bit、2Line、Standard Height、 Normal Instruction
    LCDWriteCmdByteSync(0x38,waitDefault);
    LCDWriteCmdByteSync(0x39,waitDefault);      // Function Set 8 bit、、2Line、Standard Height、 Extension Instruction
    LCDWriteCmdByteSync(0x14,waitDefault);
    LCDWriteCmdByteSync(0x70 | (contrast & 0x0f) ,waitDefault);          // コントラスト（下位４ビット)
                
    LCDWriteCmdByteSync((is5V ? 0x58 : 0x5C ) | ((a_Contrast >> 4) & 0x03),waitDefault);   // 5V駆動の場合は0x58,3.3Vの時は0x58
    LCDWriteCmdByteSync(0x6C,waitDefault);    
    return;
}

bool isCallBack = true;
void I2C_BB_Callback(uintptr_t context)
{
    volatile I2CBB_ERROR  err = I2C_BB_ErrorGet();
    if(err == I2CBB_ERROR_NONE) {
            //LED_ORANGE_Clear();
    } else {
            //LED_ORANGE_Set();
    }
    isCallBack  = true;
}




void LCDWriteCmdByteSync(uint8_t dat , uint8_t wait)

{
    isCallBack  = false;
    buffer[0] = 0x00;
    buffer[1] = dat;
    bool bRet;
    vTaskDelay(10);
    do {
        bRet= I2C_BB_Write(LCD_ADDR, buffer,2);
        if (bRet == false) vTaskDelay(10);
    } while (bRet == false);
    
        
    if (wait != 0) {
        vTaskDelay(wait);
    }
    while (  I2C_BB_IsBusy() == true || isCallBack == false) { 
        vTaskDelay(10);
    }
    
}
void LCDWriteDataBytesSync(unsigned char *buf , uint8_t wait) 
{
    bool bRet;
    volatile uint8_t i;
    size_t len = strlen((const char *)buf);
    for (i = 0 ; i < len;i++) {
        isCallBack  = false;
        buffer[0] = 0x40;
        buffer[1] = (uint8_t)buf[i];

        //while ( I2C_BB_IsBusy() == true) { vTaskDelay(10);}        

        //vTaskDelay(10);
        do {
            bRet = I2C_BB_Write(LCD_ADDR, buffer,2);
            if (bRet == false) vTaskDelay(10);
        } while (bRet == false);
        
        while (  I2C_BB_IsBusy() == true || isCallBack == false) {vTaskDelay(10);}
    }
    
    if (wait != 0) {
        vTaskDelay(wait);
    }    
}

void LCDClear(void)
{
     LCDWriteCmdByteSync(LCD_CLEARDISPLAY,200);
}

void LCDHome(void)
{
     LCDWriteCmdByteSync( LCD_RETURNHOME,20);
    
}
void LCDLocate(uint8_t col, uint8_t row)
{
    uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    if ( row >= numLines ) {
      row = numLines-1;    // we count rows starting w/0
    }
    LCDWriteCmdByteSync(LCD_SETDDRAMADDR | (col + row_offsets[row]),1);
}

void LCDDisplay(bool onoff) {
    if (onoff) {
        displaycontrol |=  LCD_DISPLAYON;
    } else {
        displaycontrol &=  ~LCD_DISPLAYON;
    }
    LCDWriteCmdByteSync(LCD_DISPLAYCONTROL | displaycontrol,1);
}

// Turns the underline cursor on/off
void LCDCursor(bool onoff) {
    if (onoff) {
        displaycontrol |=  LCD_CURSORON;
    } else {
        displaycontrol &=  ~LCD_CURSORON;
    }    
    LCDWriteCmdByteSync(LCD_DISPLAYCONTROL | displaycontrol,1);
}

// Turn on and off the blinking cursor
void LCDCurorBlink(bool onoff) {
    if (onoff) {
        displaycontrol |=  LCD_BLINKON;
    } else {
        displaycontrol &=  ~LCD_BLINKON;
    }
    LCDWriteCmdByteSync(LCD_DISPLAYCONTROL | displaycontrol,1);
}

void LCDWriteText(unsigned char *buf)
{
//    LCDWriteCmdByteSync(0x38,10);
//    LCDWriteCmdByteSync(0x0C,10);
//    LCDWriteCmdByteSync(0x01,10);
    // vTaskDelay(2);
                
    LCDWriteDataBytesSync(buf,0);
   
}


/* *****************************************************************************
 End of File
 */
