/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE, 
#include "definitions.h"                // SYS function prototypes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SWAP(type, a, b) ({                                             \
      type tmp = (a);                                                   \
      (a) = (b);                                                        \
      (b) = (tmp);                                                      \
    })

void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        SWAP(char , *(str+start), *(str+end));
        start++;
        end--;
    }
}
char* itoa(int num, char* str, int basNum)
{

    int i = 0;
    bool isNegative = false;
 
    // Handle 0 explicitly, otherwise empty string is printed for 0 
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    
  
    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && basNum == 10)
    {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0)
    {
        int rem = num % basNum;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/basNum;
    }
     
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
    str[i] = '\0'; // Append string terminator
     // Reverse the string
    reverse(str, i);
    return str;

}

char buf[3];
char* Byte2Hex(uint8_t num)
{
    buf[0] = (num>>4) > 9 ? ((num>>4)-10) + 'A' : (num>>4) + '0';
    buf[1] = (num & 0x0F) > 9 ? ((num & 0x0F)-10) + 'A' :(num & 0x0F) + '0';
    buf[2] = 0;
    return buf;
}

char* toBinStr(uint8_t val)
{
    static char ans[9];
    ans[9] = 0;
    for (uint8_t i = 0 ; i<8;i++) {
        if (0b10000000 & val) {
            ans[i] = '1';
        } else{
            ans[i] = '0';
        }
        val = val << 1;
    }
    return ans;
}
