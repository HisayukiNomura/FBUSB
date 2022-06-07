/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    USBキーコードと任天堂キーマップの変換テーブル

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef __keycodedef_h__
#define __keycodedef_h__
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#ifdef __cplusplus
extern "C" {
#endif
    
extern SemaphoreHandle_t mtxKeyMap;

extern uint8_t  keyMap[2][10];     // ０行目は使われない。

typedef union  {
    struct {
        uint8_t row;
        uint8_t colmn;
    } Base;
    uint16_t dat;
} KeyInfo;
extern int ScanCount;

void InitKeyMap(void);

KeyInfo findKey(uint8_t key);
void TblPrt(void);
bool SwapKeyMap(void);
void KeyIdxReset(void);
uint8_t KeyIdxInc(void);
uint8_t GetKeyMapIdx(void);
uint8_t KeyIdxValue(void);
void UpdateUSBKeyReport(void);
void SetKeyMode(uint8_t kym);

inline void UseKeyMap()
{
    xSemaphoreTake(mtxKeyMap,portMAX_DELAY);
}
inline void UnuseKeyMap()
{
    xSemaphoreGive(mtxKeyMap);    
}

#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
