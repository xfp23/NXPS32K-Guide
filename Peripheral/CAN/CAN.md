# NXP S32K系列 CAN 使用指南（基于 S32DS）

## 版权

© 2025 . 未经允许不得复制、修改或分发。文档所有权属于 [小风的藏书阁](https://t.me/xfp2333)

---

# 一、CAN初始化

### 1.1 添加 CAN\_pal 组件

打开 `Components`菜单选择 `CAN_pal`组件并添加。

### 1.2 配置 Configuration 选项卡

| 选项                       | 含义          | 参数说明                        |
| ------------------------ | ----------- | --------------------------- |
| Number of buffers        | 终端箱数量       | 指定支持的发送/接收 buffer 数         |
| Operation Mode           | 工作模式        | Normal / Loopback / Disable |
| PE Clock                 | 内部时钟        | 用于 CAN 操作时钟源设置              |
| Enable FD                | 是否启用 CAN FD | 选 false 表示使用经典 CAN          |
| Buffer payload size      | 事件数据长度      | 选 CAN\_PAYLOAD\_SIZE\_8 等   |
| Bitrate to time segments | 配置传速和时间段    | 自定义时间段配置                    |

### 1.3 配置 Bitrate Configuration 选项卡

| 选项                        | 含义        |
| ------------------------- | --------- |
| Propagation segment       | 传播段       |
| Phase segment 1           | 随后段 1     |
| Phase segment 2           | 随后段 2     |
| Prescaler Division Factor | 预分频因子     |
| Resync jump width         | 重合跳距宽     |
| Bitrate \[kbits/s]        | CAN 的传送速率 |

### 1.4 初始化调用函数

```c
CAN_Init(&can_pal1_instance, &can_pal1_Config0);
```

---

# 二、CAN 接收配置 (RX)

### 2.1 接收数据结构体

```c
typedef struct {
    uint32_t cs;         // 状态信息
    uint32_t id;         // 数据包 ID
    uint8_t data[64];    // 数据内容
    uint8_t length;      // 数据长度（单位: 字节）
} can_message_t;

// ID 类型
typedef enum {
    CAN_MSG_ID_STD,      // 11位标准 ID
    CAN_MSG_ID_EXT       // 29位扩展 ID
} can_msg_id_type_t;
```

### 2.2 配置接收终端箱

```c
status_t CAN_ConfigRxBuff(
  const can_instance_t * const instance,
  uint32_t buffIdx,
  const can_buff_config_t *config,
  uint32_t acceptedId);
```

* `instance` : CAN 实例指针
* `buffIdx` : 终端箱编号 (唯一)
* `config` : 配置结构体
* `acceptedId` : 要接收的 CAN ID

### 2.3 设置接收标识符的掩码

```c
status_t CAN_SetRxFilter(
  const can_instance_t * const instance,
  can_msg_id_type_t idType,
  uint32_t buffIdx,
  uint32_t mask);
```

* 掩码: 用来决定哪些 ID 与 acceptedId 匹配才能接收
* 11位 ID 最大值 0x7FF
* 掩码为 0x00 表示不进行匹配过滤，接收所有 ID

### 2.4 接收终端箱配置示例

```c
can_buff_config_t Rx_buffCfg = {
    .enableFD = false,
    .enableBRS = false,
    .fdPadding = 0U,
    .idType = CAN_MSG_ID_STD,
    .isRemote = false
};

CAN_ConfigRxBuff(&can_pal1_instance, 0, &Rx_buffCfg, 0x100);
CAN_SetRxFilter(&can_pal1_instance, CAN_MSG_ID_STD, 0, 0x7FF);
```

> 注: buffer index (终端箱编号) 必须唯一，可以从 0\~最多 buffer 数-1 (省略说明)

### 2.5 注册事件回调函数

```c
typedef void (*can_callback_t)(uint32_t instance,
                                can_event_t eventType,
                                uint32_t objIdx,
                                void *driverState);

status_t CAN_InstallEventCallback(
    const can_instance_t * const instance,
    can_callback_t callback,
    void *callbackParam);

status_t CAN_Receive(
    const can_instance_t * const instance,
    uint32_t buffIdx,
    can_message_t *message);
```

回调示例:

```c
void CAN_EventHandler(uint32_t canInstance, can_event_t eventType, uint32_t mailboxIndex, void *userData)
{
    if (canInstance == 0)
    {
        CAN_Receive(&can_pal1_instance, mailboxIndex, &recvMsg_CAN0); // 重新注册
        switch (eventType)
        {
        case CAN_EVENT_RX_COMPLETE:
            // user code
            break;
        case CAN_EVENT_TX_COMPLETE:
            // user code
            break;
        default:
            break;
        }
    }
}

CAN_InstallEventCallback(&can_pal1_instance, &CAN_EventHandler, NULL);
```

### 2.6 不用事件，直接阻塞接收:

```c
status_t CAN_ReceiveBlocking(
    const can_instance_t * const instance,
    uint32_t buffIdx,
    can_message_t *message,
    uint32_t timeout);
```

示例:

```c
void UserTools_CANReceive(uint8_t *buffer, uint32_t expected_id, uint16_t mailboxIndex, size_t size)
{
    can_message_t message = {0};
    if (CAN_ReceiveBlocking(&can_pal1_instance, mailboxIndex, &message, 100) != STATUS_SUCCESS)
        return;
    if (message.id != expected_id)
        return;
    memcpy(buffer, message.data, (message.length < size) ? message.length : size);
}
```

---

# 三、CAN 发送配置 (TX)

### 3.1 配置发送终端箱

```c
status_t CAN_ConfigTxBuff(
  const can_instance_t * const instance,
  uint32_t buffIdx,
  const can_buff_config_t *config);
```

示例:

```c
can_buff_config_t Tx_buffCfg = {
    .enableFD = false,
    .enableBRS = false,
    .fdPadding = 0U,
    .idType = CAN_MSG_ID_STD,
    .isRemote = false
};

CAN_ConfigTxBuff(&can_pal1_instance, 1, &Tx_buffCfg);
```

### 3.2 发送数据示例

```c
status_t CAN_Send(
    const can_instance_t * const instance,
    uint32_t buffIdx,
    const can_message_t *message);

status_t CAN_SendBlocking(
    const can_instance_t * const instance,
    uint32_t buffIdx,
    const can_message_t *message,
    uint32_t timeout);
```

示例函数:

```c
void UserTools_CANTransmit(uint8_t *buffer, uint32_t id, uint16_t mailboxIndex, size_t size)
{
    if (size > 8) size = 8;
    can_message_t Tx_msg = {
        .cs = CAN_MSG_ID_STD,
        .id = id
    };
    memcpy(Tx_msg.data, buffer, size);
    CAN_SendBlocking(&can_pal1_instance, mailboxIndex, &Tx_msg, 100);
}
```

---

# 四、附录

## 终端箱 bufferIndex 范围

* 通常为 `0 ~ NumberOfBuffers - 1`
* 发送和接收需要分配不同 buffer index

## 掩码匹配策略描述

* 掩码为 `0x7FF` 表示完全匹配 (11 位 ID)
* 例: 掩码=0x7F0，配置ID=0x120，则仅在高 8 位匹配时才符合
* 多ID相似，可利用掩码进行分组接收

---
