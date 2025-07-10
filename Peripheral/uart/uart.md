# NXP S32K系列 (S32DS环境)指南

## 版权

© 2025 . 未经许可不得复制、修改或分发。 此文献为 [小風的藏書閣](https://t.me/xfp2333) 所有。

# UART

## 初始化

1. 在 `pin_mux` 中确认串口引脚

2. 在 `Components` 中选择串口组件进行配置，选择基本信息，如果需要 `DMA` 接收，那么就添加 DMA，在 `Transfer type` 类型选择 DMA

3. 如果选择了 DMA 后，要添加通道。

4. 调用函数

```c
// 初始化DMA
EDMA_DRV_Init(&dmaController1_State, &dmaController1_InitConfig0, edmaChnStateArray, edmaChnConfigArray, EDMA_CONFIGURED_CHANNELS_COUNT);

// 初始化串口
LPUART_DRV_Init(INST_LPUART1, &lpuart1_State, &lpuart1_InitConfig0);

// 注册回调函数
LPUART_DRV_InstallRxCallback(INST_LPUART1, UART_RX_ISR, (void *)INST_LPUART1);

// 开启接收
LPUART_DRV_ReceiveData(INST_LPUART1, UserCommon.Uart1_rxdata, UART_BUFFERSIZE);
```

## 接收与发送

### 1. 接收回调函数示例:

```c
void UART_RX_ISR (void *driverState, uart_event_t event, void *userData)
{
    uint32_t inst = (uint32_t)userData;

#if 0
    static uint32_t bytesRemaining = 0;
    static uint8_t buffer[256] = {0};
    static uint16_t bufferIdx = 0;
    status_t status = 0;
    uint8_t triger;
    (void) driverState;
    (void) userData;

    if (LPUART_DRV_GetReceiveStatus(INST_LPUART1, &bytesRemaining) != STATUS_BUSY)
    {
        status = LPUART_DRV_GetReceiveStatus(INST_LPUART1, &bytesRemaining);
        if (status == STATUS_SUCCESS)
        {
            bufferIdx++;
            triger = 1;
            buffer[bufferIdx] = 0U;
            memcpy(UserCommon.Uart1_rxdata, buffer, bufferIdx);
            memset(buffer, 0, sizeof(buffer));
            bufferIdx = 0U;
            LPUART_DRV_ReceiveData(INST_LPUART1, buffer, 1U);
        }
        else
        {
            LPUART_DRV_ReceiveData(INST_LPUART1, buffer, 1U);
        }
    }

    if (event == UART_EVENT_RX_FULL)
    {
        if ((buffer[bufferIdx] != '\n') && (bufferIdx != (UART_BUFFERSIZE - 2U)))
        {
            bufferIdx++;
            LPUART_DRV_SetRxBuffer(INST_LPUART1, &buffer[bufferIdx], 1U);
        }
    }

#else
    if (inst == INST_LPUART1)
    {
        if (event == UART_EVENT_END_TRANSFER)
        {
            UserCommon.flag.isUartReceive = ON;
            LPUART_DRV_ReceiveData(INST_LPUART1, UserCommon.Uart1_rxdata, UART_BUFFERSIZE);
        }
        else if (event == UART_EVENT_ERROR)
        {
            // 错误处理
        }
    }
#endif
}
```

### 2. 发送函数示例:

```c
/**
 * @brief 用户串口发送函数
 *
 * @param fmt 格式化字符串
 * @param ... 可变参数
 */
void User_printf(const char* fmt, ...)
{
    char buffer[256] = {0};
    uint32_t bytesRemaining;

    va_list ap;
    va_start(ap, fmt);
    vsprintf((char*)buffer, fmt, ap);
    va_end(ap);

    LPUART_DRV_SendData(INST_LPUART1, (uint8_t *)buffer, strlen(buffer));

    uint32_t timeoutMs = 100;
    uint32_t startTime = OSIF_GetMilliseconds();

    while (LPUART_DRV_GetTransmitStatus(INST_LPUART1, &bytesRemaining) != STATUS_SUCCESS)
    {
        if ((OSIF_GetMilliseconds() - startTime) > timeoutMs)
        {
            // 超时退出，可处理异常
            break;
        }
    }
}
```

## 官方API函数原型与参数说明

### LPUART\_DRV\_ReceiveData

```c
status_t LPUART_DRV_ReceiveData(uint32_t instance, uint8_t * rxBuff, uint32_t rxSize);
```

* `instance`：LPUART通道实例（如INST\_LPUART1）
* `rxBuff`：接收数据的缓冲区指针
* `rxSize`：接收的数据长度（DMA方式下表示一次DMA传输量）

### LPUART\_DRV\_SendData

```c
status_t LPUART_DRV_SendData(uint32_t instance, const uint8_t * txBuff, uint32_t txSize);
```

* `txBuff`：发送数据缓冲区
* `txSize`：发送字节数

### LPUART\_DRV\_InstallRxCallback

```c
status_t LPUART_DRV_InstallRxCallback(uint32_t instance, lpuart_callback_t function, void * parameter);
```

* `function`：串口接收回调函数
* `parameter`：用户自定义参数，可传串口编号等，回调时会原样传回

### OSIF\_GetMilliseconds

```c
uint32_t OSIF_GetMilliseconds(void);
```

* 返回当前系统运行的毫秒数，用于超时计时等场景

### EDMA\_DRV\_Init

```c
status_t EDMA_DRV_Init(edma_state_t *state,
                       const edma_user_config_t *userConfig,
                       edma_chn_state_t *chnStateArray[],
                       const edma_chn_config_t *chnConfigArray[],
                       uint8_t numberOfChannels);
```

* 用于初始化EDMA控制器。

---
