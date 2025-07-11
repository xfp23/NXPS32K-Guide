#include "flag.h"

UserCommon_t UserCommon = {0};

can_message_t recvMsg_CAN0 = {0};

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

    FTM_DRV_ClearStatusFlags(INST_FLEXTIMER_MC1, FTM_TIME_OVER_FLOW_FLAG); // 清除溢出标志位
}

/**
 * @brief PTC端口的中断回调函数
 */
void PTC_EXT_IRQ()
{
    uint32_t bit_mask = PINS_DRV_GetPortIntFlag(PORTC) & ((1 << 12) | (1 << 13)); // 获取中断触发引脚

    if (bit_mask & (1 << 12))
    {
        /** User code begin */

        /** User code end */
    }

    if (bit_mask & (1 << 11))
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
void User_printf(const char *fmt, ...)
{
    char buffer[256] = {0};
    uint32_t bytesRemaining;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)buffer, fmt, ap);
    va_end(ap);

    LPUART_DRV_SendData(INST_LPUART1, (uint8_t *)buffer, strlen(buffer)); // 发送

    /* 添加超时机制 */
    uint32_t timeoutMs = 100;                    // 最大等待 100ms
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
void UART_RX_ISR(void *driverState, uart_event_t event, void *userData)
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
 *
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

lpi2c_master_state_t lpi2c1_MasterState;

/**
 * @brief I2C 写入多个字节到当前已配置的从设备地址
 *
 * @param buffer 要发送的数据指针
 * @param size   数据长度
 */
void UserTools_I2C_Writebytes(uint8_t *buffer, size_t size)
{
    if (buffer == NULL || size == 0)
        return;

    status_t status = LPI2C_DRV_MasterSendDataBlocking(
        INST_LPI2C1, // I2C实例编号
        buffer,      // 要发送的数据
        size,        // 数据长度
        true,        // 是否发送 STOP
        100U         // 超时时间
    );
    I2C_MasterSendDataBlocking(&i2c1_instance, buffer, size, true, 100);

    if (status != STATUS_SUCCESS)
    {
        // 处理错误，例如设置一个标志位
    }
}

/**
 * @brief 从当前已配置的从设备地址读取多个字节
 *
 * @param buffer 接收数据缓存区指针
 * @param size   要读取的字节数
 */
void UserTools_I2C_Readbytes(uint8_t *buffer, size_t size)
{
    if (buffer == NULL || size == 0)
        return;

    status_t status = LPI2C_DRV_MasterReceiveDataBlocking(
        INST_LPI2C1, // I2C实例编号
        buffer,      // 接收缓冲区
        size,        // 要接收的数据字节数
        true,        // 是否发送 STOP
        100U         // 超时时间
    );

    if (status != STATUS_SUCCESS)
    {
        // 处理错误，例如设置一个标志位
    }
}

/**
 * @brief
 *
 * @param buffer
 * @param size
 */
void UserTools_Spi_Writebytes(uint8_t *buffer, size_t size)
{
    if (buffer == NULL || size == 0)
        return;

    status_t status = LPSPI_DRV_MasterTransfer( // 非阻塞发送
        LPSPICOM1,                              // spi 实例编号
        buffer,                                 // 发送的buffer
        NULL,                                   // 不接收传NULL
        size                                    // 大小
    );
#if 0
   status_t status = SPI_MasterTransferBlocking( // 阻塞发送
        &spi1Instance, // 实例
        buffer, // buffer
        NULL, // 不接收传NULL
        size, // 大小
        100 // 超时时间
    );
#endif
}

/**
 * @brief
 *
 * @param buffer
 * @param size
 */
void UserTools_Spi_Readbytes(uint8_t *buffer, size_t size)
{
    if (buffer == NULL || size == 0)
        return;

    status_t status = LPSPI_DRV_MasterTransfer( // 非阻塞接收
        LPSPICOM1,                              // spi 实例编号
        NULL,                                   // 不发送传NULL
        buffer,                                 // 接收buffer
        size                                    // 大小
    );
#if 0
   status_t status = SPI_MasterTransferBlocking( // 阻塞接收
        &spi1Instance, // 实例
        NULL, // 不发送传NULL
        buffer, // 接收buffer
        size, // 大小
        100 // 超时时间
    );
#endif
}

/**
 * @brief
 *
 * @param txBuffer
 * @param tx_size
 * @param rxBuffer
 * @param rx_size
 */
void UserTools_Spi_WriteRead_bytes(uint8_t *txBuffer, size_t tx_size, uint8_t *rxBuffer, size_t rx_size)
{
    if ((txBuffer == NULL && tx_size > 0) || (rxBuffer == NULL && rx_size > 0))
        return; // 参数校验

    size_t transferSize = (tx_size > rx_size) ? tx_size : rx_size;

    status_t status = LPSPI_DRV_MasterTransfer(
        LPSPICOM1,   // SPI实例编号
        txBuffer,    // 发送buffer，如果没有发送数据，可以传NULL，但tx_size不能大于0
        rxBuffer,    // 接收buffer，如果不需要接收，传NULL
        transferSize // 传输总字节数，发送接收同时进行，长度取最大值
    );

#if 0
   status_t status = SPI_MasterTransferBlocking( // 阻塞发送
        &spi1Instance, // 实例
        txBuffer, // 发送buffer
        rxBuffer, // 接收buffer
        transferSize, // 大小
        100 // 超时时间
    );
#endif
    if (status != STATUS_SUCCESS)
    {
        // 错误处理
    }
}

/**
 * @brief CAN事件回调
 *
 * @param canInstance 实例编号
 * @param eventType 事件类型
 * @param mailboxIndex
 * @param userData
 */
void CAN_EventHandler(uint32_t canInstance, can_event_t eventType, uint32_t mailboxIndex, void *userData)
{
    if (canInstance == 0)
    {
        CAN_Receive(&can_pal1_instance, mailboxIndex, &recvMsg_CAN0); // 接收报文并重新注册回调函数
        switch (eventType)
        {
        case CAN_EVENT_RX_COMPLETE: // rx完成
            /**  user code begin */

            /** user code end */
            break;
        case CAN_EVENT_TX_COMPLETE: // tx完成
            /**  user code begin */

            /** user code end */
            break;
        default:
            break;
        }
    }
}

/**
 * @brief
 *
 * @param buffer
 * @param id
 * @param mailboxIndex
 * @param size
 */
void UserTools_CANTransmit(uint8_t *buffer, uint32_t id, uint16_t mailboxIndex, size_t size)
{
    if (size > 8)
        size = 8; // 限制最大长度

    can_message_t Tx_msg = {
        .cs = CAN_MSG_ID_STD,
        .id = id};

    memcpy(Tx_msg.data, buffer, size);
#if 0
    CAN_Send(&can_pal1_instance, mailboxIndex, &Tx_msg); // 非阻塞发送
#else
    CAN_SendBlocking(&can_pal1_instance, mailboxIndex, &Tx_msg, 100); // 阻塞发送
#endif
}

/**
 * @brief
 *
 * @param buffer
 * @param expected_id
 * @param mailboxIndex
 * @param size
 */
void UserTools_CANReceive(uint8_t *buffer, uint32_t expected_id, uint16_t mailboxIndex, size_t size)
{
    can_message_t message = {0};

#if 1
    if (CAN_ReceiveBlocking(&can_pal1_instance, mailboxIndex, &message, 100) != STATUS_SUCCESS)
        return; // 超时或错误
#else
    CAN_Receive(&can_pal1_instance, mailboxIndex, &message);
#endif
    // 可选：检查是否收到目标 ID（注意扩展帧可能会误判）
    if (message.id != expected_id)
        return;

    // 安全拷贝
    size_t copyLen = (message.length < size) ? message.length : size;
    memcpy(buffer, message.data, copyLen);
}

#if 0
/**
 * @brief lin总线校验ID
 *
 * @param id
 * @return uint8_t
 */
uint8_t lin_calc_protected_id(uint8_t id)
{
    id &= 0x3F; // 只保留低6位
    uint8_t p0 = ((id >> 0) & 0x01) ^ ((id >> 1) & 0x01) ^
                 ((id >> 2) & 0x01) ^ ((id >> 4) & 0x01);

    uint8_t p1 = ~(((id >> 1) & 0x01) ^ ((id >> 3) & 0x01) ^
                   ((id >> 4) & 0x01) ^ ((id >> 5) & 0x01)) &
                 0x01;

    return (p1 << 7) | (p0 << 6) | id;
}
#endif

void LIN_EventHandler(uint32_t instance, lin_state_t *linState)
{
    lin_callback_t Current = linState->Callback;

    if (instance == INST_LIN1)
    {
        switch (linState->currentEventId)
        {
        case LIN_PID_OK: // 接收到有效的帧标识符
            LIN_DRV_SetTimeoutCounter(INST_LIN1, 500);
            /** user code begin */

            /** user code end */

            break;
        case LIN_PID_ERROR: // 帧标识符（PID）错误
            LIN_DRV_GoToSleepMode(INST_LIN1);
            /** user code begin */

            /** user code end */

            break;
        case LIN_WAKEUP_SIGNAL: // 收到唤醒信号

            /** user code begin */

            /** user code end */
            break;
        case LIN_RX_COMPLETED: // 接收完成
            LIN_DRV_GotoIdleState(INST_LIN1);
            /** user code begin */

            /** user code end */
            break;
        case LIN_TX_COMPLETED: // 发送完成
            LIN_DRV_GotoIdleState(INST_LIN1);
            /** user code begin */

            /** user code end */

            break;
        case LIN_CHECKSUM_ERROR: // 出错
        case LIN_READBACK_ERROR:
        case LIN_FRAME_ERROR:
        case LIN_RECV_BREAK_FIELD_OK:
            LIN_DRV_SetTimeoutCounter(INST_LIN1, 500);

            /** user code begin */

            /** user code end */

            break;

        default:
            break;
        }
    }
}

#define TIMER_COMPARE_VAL (uint16_t)(2000U)
static uint16_t timerOverflowInterruptCount = 0U;

uint32_t lin1TimerGetTimeIntervalCallback0(uint32_t *ns)
{
    static uint32_t previousCountValue = 0UL;
    uint32_t counterValue;

    counterValue = FTM_DRV_CounterRead(INST_FLEXTIMER_MC2);
    *ns = ((uint32_t)(counterValue + timerOverflowInterruptCount * TIMER_COMPARE_VAL - previousCountValue)) * 1000U / 4U;
    timerOverflowInterruptCount = 0UL;
    previousCountValue = counterValue;
    return 0UL;
}

ftm_state_t LINState_FTM2 = {0};
void LIN_TimeoutEventHandler()
{
    /* LIN总线超时服务*/
    LIN_DRV_TimeoutService(INST_LIN1);
    /* Increment overflow count */
    timerOverflowInterruptCount++;
    FTM_DRV_ClearStatusFlags(INST_FLEXTIMER_MC2, (uint32_t)FTM_TIME_OVER_FLOW_FLAG);
}
