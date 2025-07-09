#include "flag.h"

UserCommon_t UserCommon = {0};

/**
 * @brief 指南gpio教程
 *
 */
void UserGuide_gpio()
{
    uint8_t state = 0;
    PINS_DRV_WritePin(PTE, 12, 1);            // PTE12写入高电平
    PINS_DRV_WritePins(PTE, 1 << 12);         // 给端口输出按掩码写入
    state = PINS_DRV_ReadPins(PTE) & 1 << 12; // 读出整个引脚状态然后掩码查看指定引脚
}


ftm_state_t UserState_FTM0 = {0};
/**
 * @brief 指南adc教程
 * @note 此函数code1 为ADC单次转换
 */
void UserGuide_adc()
{

    /** user code1 begin */
    uint16_t Mcu_adc = 0x00;
    ADC_DRV_ConfigChan(INST_ADCONV1, 0u, &adConv1_ChnConfig0);
    ADC_DRV_WaitConvDone(INST_ADCONV1);
    ADC_DRV_GetChanResult(INST_ADCONV1, 0u, &Mcu_adc);
    /** user code1 end */

    /** user code2 begin */
    
    /** user code2 end */
}


/**
 * @brief 
 * @note
 * 
 */
void UserGuide_Timer()
{
    
}

/**
 * @brief 定时器FTM0中断回调
 * @note
 */
void Timer0_PeriodElapsedISR()
{

    FTM_DRV_ClearStatusFlags(INST_FLEXTIMER_MC1, FTM_TIME_OVER_FLOW_FLAG);  // 清除溢出标志位
}

/**
 * @brief PTC端口的中断回调函数
 */
void PTC_EXT_IRQ()
{
	uint32_t bit_mask = PINS_DRV_GetPortIntFlag(PORTC) & ((1 << 12) | (1 << 13)); // 获取中断触发引脚

	if(bit_mask & (1 << 12))
	{
		/** User code begin */

		/** User code end */
	}

	if(bit_mask & (1 << 11))
	{

	}

	/** 清除中断标志 */
	PINS_DRV_ClearPortIntFlagCmd(PORTC);
}

/**
 * @brief 用户串口发送函数
 * 
 * @param fmt 
 * @param ... 
 */
void User_printf(const char* fmt, ...)
{
    char buffer[256] = {0};
    uint32_t bytesRemaining;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char*)buffer, fmt, ap);
    va_end(ap);

    LPUART_DRV_SendData(INST_LPUART1, (uint8_t *)buffer, strlen(buffer)); // 发送

    /* 添加超时机制 */
    uint32_t timeoutMs = 100; // 最大等待 100ms
    uint32_t startTime = OSIF_GetMilliseconds(); // 获取当前时间戳

    while (LPUART_DRV_GetTransmitStatus(INST_LPUART1, &bytesRemaining) != STATUS_SUCCESS)
    {
        if ((OSIF_GetMilliseconds() - startTime) > timeoutMs)
        {
            // 超时退出，必要时可以打印错误或置标志位
            break;
        }
    }
}

/**
 * @brief 串口接收中断回调函数
 * @note 
 */
void UART_RX_ISR (void *driverState, uart_event_t event, void *userData)
 {
	uint32_t inst = (uint32_t)userData;
#if 0
    static uint32_t bytesRemaining = 0;
    static uint8_t buffer[256] = {0};
    static uint16_t  bufferIdx = 0;
    status_t status = 0;
    uint8_t triger;
	(void) driverState;
	(void) userData;

	if(LPUART_DRV_GetReceiveStatus(INST_LPUART1, &bytesRemaining) != STATUS_BUSY)
	{
		status = LPUART_DRV_GetReceiveStatus(INST_LPUART1, &bytesRemaining); //串口数据接收成功
		if(status == STATUS_SUCCESS)
		{
			/* 为接收到的字符串添加终止符 */
			bufferIdx++;
			triger =1;
			buffer[bufferIdx] = 0U;
			/* 将接收缓存文件复制到rxdata数组 */
            // memset(UserCommon.Uart1_rxdata,0,UART_BUFFERSIZE);
			memcpy(UserCommon.Uart1_rxdata,buffer,bufferIdx);
			memset(buffer,0,sizeof(buffer));
			//LPUART_DRV_SendDataBlocking(INST_LPUART1, buffer, bufferIdx, 100UL);
			/* Reset the buffer index to start a new reception */
			bufferIdx = 0U;
			/*完成操作后清除中断标志位并重新开始接收串口数据*/
			LPUART_DRV_ReceiveData(INST_LPUART1, buffer, 1U);
		}
		else
		{
			//error handle
			LPUART_DRV_ReceiveData(INST_LPUART1, buffer, 1U);
		}
	   }

	/*串口接收缓存满事件*/
	if(event==UART_EVENT_RX_FULL)
		{
		/*判断是否收到换行符*/
		if ((buffer[bufferIdx] != '\n') && (bufferIdx != (UART_BUFFERSIZE - 2U)))
			{
			/*每接收一字节数据,就将储存位置指向数组的下一位,直到接收到换行符*/
				bufferIdx++;
				LPUART_DRV_SetRxBuffer(INST_LPUART1, &buffer[bufferIdx], 1U);
			}
		}
#else

    if (inst == INST_LPUART1)
    {
        if (event == UART_EVENT_END_TRANSFER)
        {
            // DMA接收完成（相当于IDLE中断）
            UserCommon.flag.isUartReceive = ON;

            // 重新启动DMA接收
            LPUART_DRV_ReceiveData(INST_LPUART1, UserCommon.Uart1_rxdata, UART_BUFFERSIZE);
        }
        else if (event == UART_EVENT_ERROR)
        {
            // 错误处理
        }
    }
#endif
}


/**
 * @brief DMA接收回调
 */
void UART_RX_DMA_Callback(void *driverState, uart_event_t event, void *userData)
{
    uint32_t inst = (uint32_t)userData;
    if (inst == INST_LPUART1)
	{
	    if (event == UART_EVENT_END_TRANSFER)
	    {
	        // DMA接收完成
	        UserCommon.flag.isUartReceive = ON;

	        // 重新启动接收（实现循环）

	    }
	    else if (event == UART_EVENT_ERROR)
	    {
	        // 错误处理
	    }
	}

}


