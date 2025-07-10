以下是你所需的完整外部中断章节文档，已补全函数说明与细节，保持与整体风格一致：

---

# 外部中断配置与使用（NXP S32K，S32DS环境）

## 版权

© 2025 . 未经许可不得复制、修改或分发。 此文献为 [小風的藏書閣](https://t.me/xfp2333) 所有。

---

## 1. 配置 GPIO 引脚

在配置中断前，需确保中断引脚已正确初始化为 GPIO 输入。具体引脚初始化方式请参考文档：[GPIO初始化指南](./gpio.md)。

---

## 2. 设置中断触发条件

使用 `PINS_DRV_SetPinIntSel()` 函数为目标引脚配置中断类型：

```c
/* 设置中断触发条件 */
PINS_DRV_SetPinIntSel(PORTC, 12, PORT_INT_RISING_EDGE);  // PTC12 上升沿触发中断
PINS_DRV_SetPinIntSel(PORTC, 13, PORT_INT_FALLING_EDGE); // PTC13 下降沿触发中断
PINS_DRV_SetPinIntSel(PORTB, 2,  PORT_INT_RISING_EDGE);  // PTB2 上升沿触发中断
```

### 可选中断触发模式（枚举类型）

```c
typedef enum
{
    PORT_DMA_INT_DISABLED  = 0x0U,  /*!< 禁用中断或DMA请求 */
    PORT_DMA_RISING_EDGE   = 0x1U,  /*!< 上升沿触发 DMA 请求 */
    PORT_DMA_FALLING_EDGE  = 0x2U,  /*!< 下降沿触发 DMA 请求 */
    PORT_DMA_EITHER_EDGE   = 0x3U,  /*!< 任意边沿触发 DMA 请求 */
    PORT_INT_LOGIC_ZERO    = 0x8U,  /*!< 引脚为逻辑0时触发中断 */
    PORT_INT_RISING_EDGE   = 0x9U,  /*!< 上升沿触发中断 */
    PORT_INT_FALLING_EDGE  = 0xAU,  /*!< 下降沿触发中断 */
    PORT_INT_EITHER_EDGE   = 0xBU,  /*!< 任意边沿触发中断 */
    PORT_INT_LOGIC_ONE     = 0xCU   /*!< 引脚为逻辑1时触发中断 */
} port_interrupt_config_t;
```

---

## 3. 安装并使能中断服务函数

使用 `INT_SYS_InstallHandler()` 安装对应的端口中断服务函数，再通过 `INT_SYS_EnableIRQ()` 使能中断：

```c
/* 安装中断处理函数 */
INT_SYS_InstallHandler(PORTC_IRQn, &PTC_EXT_IRQ, NULL);  // PTC12 和 PTC13 共用 PORTC 中断
INT_SYS_InstallHandler(PORTB_IRQn, &PTB_EXT_IRQ, NULL);  // PTB2 的 PORTB 中断

/* 启用中断 */
INT_SYS_EnableIRQ(PORTC_IRQn);
INT_SYS_EnableIRQ(PORTB_IRQn);
```

---

## 4. 实现中断回调函数

```c
/**
 * @brief PTC 中断处理函数
 * @note 处理 PTC12 和 PTC13 中断
 */
void PTC_EXT_IRQ(void)
{
    uint32_t buttonsPressed = PINS_DRV_GetPortIntFlag(PORTC) & ((1 << 12) | (1 << 13)); // 获取中断触发引脚

    if (buttonsPressed != 0)
    {
        if (buttonsPressed & (1 << 12))
        {
            PTC12_KEY1 = 1;
            PINS_DRV_TogglePins(PTD, 1 << 16);  // 示例操作：翻转 PD16
        }

        if (buttonsPressed & (1 << 13))
        {
            PTC13_KEY2 = 1;
            PINS_DRV_TogglePins(PTD, 1 << 15);  // 示例操作：翻转 PD15
        }
    }

    /* 清除中断标志位 */
    PINS_DRV_ClearPortIntFlagCmd(PORTC);
}
```

---

## 5. 函数原型说明

### `PINS_DRV_SetPinIntSel`

```c
void PINS_DRV_SetPinIntSel(PORT_Type * const base,
                           uint32_t pin,
                           port_interrupt_config_t intConfig);
```

| 参数名         | 类型                        | 说明              |
| ----------- | ------------------------- | --------------- |
| `base`      | `PORT_Type*`              | 指定端口（如 `PORTC`） |
| `pin`       | `uint32_t`                | 引脚号（0 \~ 31）    |
| `intConfig` | `port_interrupt_config_t` | 中断触发类型枚举        |

---

### `INT_SYS_InstallHandler`

```c
void INT_SYS_InstallHandler(IRQn_Type irqNumber,
                            const isr_t handler,
                            isr_t* const oldHandler);
```

| 参数名          | 类型          | 说明                    |
| ------------ | ----------- | --------------------- |
| `irqNumber`  | `IRQn_Type` | 中断向量号（如 `PORTC_IRQn`） |
| `handler`    | `isr_t`     | 中断函数指针                |
| `oldHandler` | `isr_t*`    | 可选旧函数指针（通常填 `NULL`）   |

---

### `INT_SYS_EnableIRQ`

```c
void INT_SYS_EnableIRQ(IRQn_Type irqNumber);
```

| 参数名         | 类型          | 说明                    |
| ----------- | ----------- | --------------------- |
| `irqNumber` | `IRQn_Type` | 中断向量号（如 `PORTC_IRQn`） |

---

### `PINS_DRV_GetPortIntFlag`

```c
uint32_t PINS_DRV_GetPortIntFlag(PORT_Type * const base);
```

返回当前端口的中断标志寄存器（32位位掩码）。

---

### `PINS_DRV_ClearPortIntFlagCmd`

```c
void PINS_DRV_ClearPortIntFlagCmd(PORT_Type * const base);
```

清除所有已触发的端口中断标志位。

---

## 总结

* 同一个 `PORTx` 的所有引脚共用一个中断向量（如 `PORTC_IRQn` 负责 `PTC0 ~ PTC31` 所有中断）。
* 无法对多个引脚同时调用 `PINS_DRV_SetPinIntSel()` 设置，需要单独配置。
* 中断服务函数需手动读取标志位判断是哪个引脚产生中断。
* 不同端口（如 PORTB、PORTC）各自有独立的中断向量，必须分别配置。

---

如需进一步自动分发中断逻辑、处理多个引脚事件、或支持消抖等功能，可在 ISR 中引入状态机或事件队列机制处理。

需要我再加个“消抖逻辑示例”吗？
