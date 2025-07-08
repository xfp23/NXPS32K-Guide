# NXP S32K系列 (S32DS环境)指南

## 版权

© 2025 . 未经许可不得复制、修改或分发。 此文献为 [小风的藏书阁](https://t.me/xfp2333) 所有。

---

## 1.引脚初始化

1. **展开 ****\`\`**** 文件夹**，双击 `pin_mux`，打开图形化引脚配置界面。

2. 点击界面上方的 \`\` 标签页，选择所需的引脚，并设置方向（如 `Input`、`Output` 或 `InputOutput`）。

3. 若需配置更多 GPIO 相关属性，**右键第二列的引脚名称**，选择 \`\`，开始配置下方参数：

---

### GPIO 配置参数说明表

| 选项                                | 含义           | 参数说明                                                                      |
| --------------------------------- | ------------ | ------------------------------------------------------------------------- |
| **Interrupt Status Field**        | 中断状态标志（只读）   | 仅用于查看该引脚是否触发过中断。中断处理时需通过写 `1` 清除。                                         |
| **Interrupt Configuration Field** | 中断触发方式配置     | 设置该引脚的中断模式，例如：`Rising Edge`（上升沿）、`Falling Edge`（下降沿）、`Either Edge`（双边沿）等。 |
| **Pin Mux Field**                 | 引脚复用模式选择     | 设置引脚功能为 GPIO 或其他外设功能（如 UART、SPI、PWM 等）。常见配置为 `GPIO`。                      |
| **Lock Field**                    | 寄存器锁定        | 启用后，将锁定该引脚的配置寄存器，系统复位前不可更改。适用于安全敏感引脚。                                     |
| **Pull Enable Field**             | 上/下拉电阻使能     | 是否启用内部上拉/下拉电阻，需与 Pull Select 一起使用。默认关闭。                                   |
| **Pull Select Field**             | 上拉/下拉选择      | `Pull Up` 表示上拉电阻，`Pull Down` 表示下拉电阻。需在 Pull Enable 启用下才生效。                |
| **Initial Value Field**           | 初始电平值（仅输出有效） | 设置 GPIO 输出引脚在上电时的默认电平状态（高或低）。                                             |
| **Digital Filter Field**          | 数字滤波器启用      | 可抑制输入信号中的毛刺，适用于按键、脉冲类输入。启用后，需在其他配置中设定滤波参数。                                |

---

4. **配置完成后点击右上角 ****\`\`**** 按钮生成代码**，IDE 会自动生成初始化函数，例如 `PINS_DRV_Init()`。

5. 回到 `main.c`，**展开 ****\`\`**** 函数拖入 ****\`\`**** 函数中**，以完成引脚初始化。

---

### 示例：

```c
int main(void)
{
    /* 初始化 GPIO 引脚配置（由图形化工具生成） */
    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    while (1)
    {
        // 主循环逻辑
    }
}
```

---

## 2.API

### `PINS_DRV_SetPullSel`

配置上拉/下拉类型（Pull Select）。

```c
void PINS_DRV_SetPullSel(PORT_Type * const base, uint32_t pin, port_pull_config_t pullConfig);
```

* `pullConfig` 可为 `PORT_INTERNAL_PULL_UP` 或 `PORT_INTERNAL_PULL_DOWN`

### `PINS_DRV_SetMuxModeSel`

设置引脚复用模式。

```c
void PINS_DRV_SetMuxModeSel(PORT_Type * const base, uint32_t pin, port_mux_t mux);
```

### `PINS_DRV_SetPinIntSel`

配置引脚中断触发方式。

```c
void PINS_DRV_SetPinIntSel(PORT_Type * const base, uint32_t pin, port_interrupt_config_t intConfig);
```

### `PINS_DRV_GetPinIntSel`

获取引脚中断配置。

```c
port_interrupt_config_t PINS_DRV_GetPinIntSel(const PORT_Type * const base, uint32_t pin);
```

### `PINS_DRV_ClearPinIntFlagCmd`

清除引脚的中断标志位。

```c
void PINS_DRV_ClearPinIntFlagCmd(PORT_Type * const base, uint32_t pin);
```

### `PINS_DRV_EnableDigitalFilter`

启用某个引脚的数字滤波。

```c
void PINS_DRV_EnableDigitalFilter(PORT_Type * const base, uint32_t pin);
```

### `PINS_DRV_DisableDigitalFilter`

禁用某个引脚的数字滤波。

```c
void PINS_DRV_DisableDigitalFilter(PORT_Type * const base, uint32_t pin);
```

### `PINS_DRV_ConfigDigitalFilter`

配置端口的数字滤波参数。

```c
void PINS_DRV_ConfigDigitalFilter(PORT_Type * const base, const port_digital_filter_config_t * const config);
```

### `PINS_DRV_GetPortIntFlag`

获取端口中断状态位。

```c
uint32_t PINS_DRV_GetPortIntFlag(const PORT_Type * const base);
```

### `PINS_DRV_ClearPortIntFlagCmd`

清除整个端口的中断标志位。

```c
void PINS_DRV_ClearPortIntFlagCmd(PORT_Type * const base);
```

### `PINS_DRV_SetGlobalPinControl`

批量设置引脚控制寄存器值。

```c
void PINS_DRV_SetGlobalPinControl(PORT_Type * const base, uint16_t pins, uint16_t value);
```

### `PINS_DRV_SetGlobalIntControl`

批量设置全局中断控制。

```c
void PINS_DRV_SetGlobalIntControl(PORT_Type * const base, uint16_t pins, uint16_t value);
```

### `PINS_DRV_GetPinsDirection`

获取引脚方向配置。

```c
pins_channel_type_t PINS_DRV_GetPinsDirection(const GPIO_Type * const base);
```

### `PINS_DRV_SetPinDirection`

设置单个引脚方向。

```c
void PINS_DRV_SetPinDirection(GPIO_Type * const base, pins_channel_type_t pin, pins_level_type_t direction);
```

### `PINS_DRV_SetPinsDirection`

设置多个引脚方向（位掩码）。

```c
void PINS_DRV_SetPinsDirection(GPIO_Type * const base, pins_channel_type_t pins);
```

### `PINS_DRV_SetPortInputDisable`

禁用输入缓冲器。

```c
void PINS_DRV_SetPortInputDisable(GPIO_Type * const base, pins_channel_type_t pins);
```

### `PINS_DRV_GetPortInputDisable`

获取输入禁用状态。

```c
pins_channel_type_t PINS_DRV_GetPortInputDisable(GPIO_Type * const base);
```

### `PINS_DRV_WritePin`

写入单个引脚输出值。

```c
void PINS_DRV_WritePin(GPIO_Type * const base, pins_channel_type_t pin, pins_level_type_t value);
```

### `PINS_DRV_WritePins`

写入整个端口的输出值。

```c
void PINS_DRV_WritePins(GPIO_Type * const base, pins_channel_type_t pins);
```

### `PINS_DRV_GetPinsOutput`

获取当前输出寄存器状态。

```c
pins_channel_type_t PINS_DRV_GetPinsOutput(const GPIO_Type * const base);
```

### `PINS_DRV_SetPins`

将多个引脚设为高电平。

```c
void PINS_DRV_SetPins(GPIO_Type * const base, pins_channel_type_t pins);
```

### `PINS_DRV_ClearPins`

将多个引脚设为低电平。

```c
void PINS_DRV_ClearPins(GPIO_Type * const base, pins_channel_type_t pins);
```

### `PINS_DRV_TogglePins`

翻转引脚电平。

```c
void PINS_DRV_TogglePins(GPIO_Type * const base, pins_channel_type_t pins);
```

### `PINS_DRV_ReadPins`

读取整个 GPIO 端口输入电平。

```c
pins_channel_type_t PINS_DRV_ReadPins(const GPIO_Type * const base);
```

**示例**：

```c
if (PINS_DRV_ReadPins(PTC) & (1 << 13)) {
    // PTC13 为高电平
}
```

## 3. 工程示例
**[源码文件]()**
