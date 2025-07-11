#ifndef FLAG_H
#define FLAG_H

/**
 * @file flag.h
 * @author xfp23
 * @brief
 * @version 0.1
 * @date 2025-07-09
 *
 * @copyright Copyright (c) 2025 
 *
 */
#include "Cpu.h"
#include "stdarg.h"
#include "string.h"
#include "osif.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define UART_BUFFERSIZE (256)

#define ON (1)
#define OFF (0)

#define USE_WDT (1)

typedef struct
{
uint8_t isUartReceive :1; // 是否接收到完整数据
uint8_t isReceive_bits :7; // 保留位
}User_flag_t;

typedef struct
{
    uint8_t Uart1_rxdata[UART_BUFFERSIZE];
    User_flag_t flag; // 标志
} UserCommon_t;

extern ftm_state_t UserState_FTM0;
extern UserCommon_t UserCommon;
extern lpi2c_master_state_t lpi2c1_MasterState;
extern can_message_t recvMsg_CAN0;

extern void UserGuide_gpio();
extern void UserGuide_adc();
extern void Timer0_PeriodElapsedISR();
extern void PTC_EXT_IRQ();
extern void UART_RX_ISR(void *driverState, uart_event_t event, void *userData);
extern void UART_RX_DMA_Callback(void *driverState, uart_event_t event, void *userData);
extern void CAN_EventHandler(uint32_t canInstance,can_event_t eventType,uint32_t mailboxIndex,void *userData);

#ifdef __cplusplus
}
#endif

#endif
