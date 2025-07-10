# NXP S32K系列 (S32DS环境)指南

## 版权

© 2025 . 未经许可不得复制、修改或分发。 此文献为 [小風的藏書閣](https://t.me/xfp2333) 所有。

# SPI通信 (LPSPI模块)

---

## 初始化

1. 在`pin_mux`中选择`LPSPI`功能并配置对应引脚。

2. 在`Components`中新增`lpspi`模块，配置参数：

| 选项             | 说明      | 参数含义        |
| -------------- | ------- | ----------- |
| Baud rate      | 波特率     | SPI通信速率     |
| SS             | 片选      | 片选引脚使能与否    |
| SS polarity    | 片选极性    | 高电平有效或低电平有效 |
| Bits/frame     | 每帧位数    | 一般为8位       |
| Phase          | 时钟相位    | CPHA        |
| Clock polarity | 时钟极性    | CPOL        |
| Transfer type  | 传输类型    | 阻塞/非阻塞      |
| RX DMA channel | 接收DMA通道 | 如使用DMA则配置   |
| TX DMA channel | 发送DMA通道 | 如使用DMA则配置   |
| Callback       | 回调函数    | 非阻塞模式下使用    |

3. 调用初始化函数：

```c
LPSPI_DRV_MasterInit(LPSPICOM1, &lpspiCom1State, &lpspiCom1_MasterConfig0);
```

---

## 发送与接收

### 发送函数示例

```c
// 非阻塞发送
status_t status = LPSPI_DRV_MasterTransfer(
    LPSPICOM1,  // SPI实例编号
    buffer,     // 发送缓冲区
    NULL,       // 不接收数据，传NULL
    size        // 发送字节数
);

// 阻塞发送
status_t status = SPI_MasterTransferBlocking(
    &spi1Instance,
    buffer,
    NULL,
    size,
    100 // 超时时间(ms)
);
```

### 接收函数示例

```c
// 非阻塞接收
status_t status = LPSPI_DRV_MasterTransfer(
    LPSPICOM1,
    NULL,       // 不发送，传NULL
    buffer,     // 接收缓冲区
    size        // 接收字节数
);

// 阻塞接收
status_t status = SPI_MasterTransferBlocking(
    &spi1Instance,
    NULL,
    buffer,
    size,
    100 // 超时时间(ms)
);
```

---

## 用户自定义API示例

```c
void UserTools_Spi_Writebytes(uint8_t *buffer, size_t size)
{
    if (buffer == NULL || size == 0)
        return;

    status_t status = LPSPI_DRV_MasterTransfer(
        LPSPICOM1,
        buffer,
        NULL,
        size
    );

    // 也可以使用阻塞版本，注释中示例
}

void UserTools_Spi_Readbytes(uint8_t *buffer, size_t size)
{
    if (buffer == NULL || size == 0)
        return;

    status_t status = LPSPI_DRV_MasterTransfer(
        LPSPICOM1,
        NULL,
        buffer,
        size
    );
}

void UserTools_Spi_WriteRead_bytes(uint8_t *txBuffer, size_t tx_size, uint8_t *rxBuffer, size_t rx_size)
{
    if ((txBuffer == NULL && tx_size > 0) || (rxBuffer == NULL && rx_size > 0))
        return;

    size_t transferSize = (tx_size > rx_size) ? tx_size : rx_size;

    status_t status = LPSPI_DRV_MasterTransfer(
        LPSPICOM1,
        txBuffer,
        rxBuffer,
        transferSize
    );

    if (status != STATUS_SUCCESS)
    {
        // 错误处理
    }
}
```

---

## 重要说明

* SPI是全双工通信，发送与接收是同时进行的，`transferSize`应为发送与接收中较大的字节数。
* 若发送和接收字节数不同，多余部分一般由从设备丢弃或由主设备发送无意义数据（如0xFF）。
* 非阻塞API调用后，可以通过回调函数或状态查询获知传输完成情况。
* 阻塞API调用则会等待传输结束或超时。

---

## 相关API原型示例

```c
status_t LPSPI_DRV_MasterTransfer(
    uint32_t instance,
    const uint8_t * sendBuffer,
    uint8_t * receiveBuffer,
    uint16_t transferByteCount
);

status_t SPI_MasterTransferBlocking(
    spi_master_state_t * spiState,
    const uint8_t * sendBuffer,
    uint8_t * receiveBuffer,
    size_t transferByteCount,
    uint32_t timeoutMs
);
```

---

以上即为NXP S32K系列LPSPI通信的简明使用指南，适合快速上手S32DS环境下SPI驱动开发。
