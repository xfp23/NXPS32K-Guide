# NXP S32K系列 (S32DS环境)指南

## 版权

© 2025 . 未经许可不得复制、修改或分发。 此文献为 [小风的藏书阁](https://t.me/xfp2333) 所有。

# ADC单次转换

## ADC初始化

1. **双击项目中的 **\`\`** 选项**，弹出 `Components Library` 对话框。

2. 在库中找到 `adc`，双击添加到工程中。

3. 在工程目录中的 `Components` 区，点击刚添加的 `adConvX` 组件（如 `adConv1`），切换到右侧的 `Properties` 选项卡。

   **⚠ 注意**：

   * `Component Name` 决定了后续生成代码中 ADC 相关变量的名称，建议保持简洁有辨识度，例如 `adc1`。

4. 在 `Device` 下拉中选择对应的 ADC 模块（如 `ADC0` 或 `ADC1`）， 然后切换到 `Configurations` 选项卡，点击 `+` 添加一个配置项，勾选启用该配置项，并根据需求填写下方各参数：

---

### ADC 主配置参数说明表

| 选项                                | 含义           | 说明                                                    |
| --------------------------------- | ------------ | ----------------------------------------------------- |
| **Clock Divide**                  | 时钟分频         | 配置输入时钟的分频系数，影响 ADC 的转换速度。一般为 `Divide by 1/2/4/...` 等。 |
| **Sample Time**                   | 采样周期         | 指定 ADC 每次采样的采样时间（影响精度和速度）。单位通常为 ADC 时钟周期。             |
| **Resolution**                    | 分辨率          | ADC 的转换精度，可选 `8-bit`、`10-bit`、`12-bit` 等。             |
| **Input Clock**                   | 输入时钟来源       | 设置 ADC 模块使用的时钟源（如 `SOSCDIV2_CLK`、`FIRC_DIV2_CLK` 等）。  |
| **Trigger for Control channel 0** | 控制通道 0 的触发来源 | 设置 ADC 转换的触发模式（如 `Software Trigger`、`PDB Trigger` 等）。 |
| **PreTrigger Select**             | 预触发通道选择      | 指定用于控制预转换的通道，一般配合外部触发使用。                              |
| **Trigger Select**                | 触发选择         | 设定主触发源通道编号或逻辑。                                        |
| **DMA Enable**                    | DMA 使能       | 若勾选，则 ADC 完成采样后可自动将结果通过 DMA 传输。                       |
| **Voltage Reference**             | 电压参考选择       | 设置 ADC 的参考电压源，常见选项有 `VDDA`、`VREFH`、内部参考等。             |
| **Continuous Conversion**         | 连续转换模式       | 勾选后会连续不断采样；若用于单次转换，请关闭此项。                             |
| **Internal Supply Monitoring**    | 内部电源监测       | 用于开启对 Vref/Vdd 等内部电源信号的监测采样。                          |

---

5. 切换到 `Channel Configurations` 选项卡，点击左上角 `+` 添加一个 ADC 通道配置。

6. 在下方参数配置区，设置该通道的 `Input Channel`（输入通道编号），对应实际接入信号的引脚通道，如 `ADC0_SE4`。

   其他参数如触发方式、中断控制等可按默认保留，后续可在代码中进一步配置。

7. 完成以上步骤后，点击右上角 `Update Code` 按钮生成对应初始化代码（如 `Adc_InitConfig()` 和 `Adc_StartGroupConversion()` 等），这些函数可在 `main.c` 中调用以启动 ADC 功能。

8. 然后展开`Components`中的`adConvx`选择拖放初始化函数:

```c
ADC_DRV_ConfigConverter(INST_ADCONV1, &adConv1_ConvConfig1); // 初始化ADC
ADC_DRV_AutoCalibration(INST_ADCONV1); // 校准ADC
```

---

## 开始转换

---

### 1. `ADC_DRV_ConfigChan`

配置 ADC 的通道控制参数（如输入通道编号、中断、采样通道等）。

```c
status_t ADC_DRV_ConfigChan(uint32_t instance,
                            uint8_t chanIndex,
                            const adc_chan_config_t *config);
```

#### 参数说明：

| 参数名         | 类型                          | 说明                                           |
| ----------- | --------------------------- | -------------------------------------------- |
| `instance`  | `uint32_t`                  | ADC 实例号（如 `INST_ADCONV1` 对应 `ADC0`）。         |
| `chanIndex` | `uint8_t`                   | 控制通道索引，常用 `0u`。                              |
| `config`    | `const adc_chan_config_t *` | 通道配置结构体指针（SDK 会自动生成，如 `adConv1_ChnConfig0`）。 |

#### 示例：

```c
ADC_DRV_ConfigChan(INST_ADCONV1, 0u, &adConv1_ChnConfig0);
```

---

### 2. `ADC_DRV_WaitConvDone`

阻塞等待 ADC 转换完成（软件触发场景用）。

```c
status_t ADC_DRV_WaitConvDone(uint32_t instance);
```

#### 参数说明：

| 参数名        | 类型         | 说明                        |
| ---------- | ---------- | ------------------------- |
| `instance` | `uint32_t` | ADC 实例号（如 `INST_ADCONV1`） |

#### 示例：

```c
ADC_DRV_WaitConvDone(INST_ADCONV1);
```

> ⚠ 若你使用中断或 DMA，通常不应调用此函数。

---

### 3. `ADC_DRV_GetChanResult`

获取某个控制通道的采样值。

```c
status_t ADC_DRV_GetChanResult(uint32_t instance,
                                uint8_t chanIndex,
                                adc_result_t *result);
```

#### 参数说明：

| 参数名         | 类型               | 说明                                                     |
| ----------- | ---------------- | ------------------------------------------------------ |
| `instance`  | `uint32_t`       | ADC 实例号。                                               |
| `chanIndex` | `uint8_t`        | 控制通道编号（如 `0u`）。                                        |
| `result`    | `adc_result_t *` | 存储结果的变量指针，类型为 `uint16_t`（实际 typedef 为 `adc_result_t`）。 |

#### 示例：

```c
    uint16_t Mcu_adc = 0x00;

    ADC_DRV_ConfigChan(INST_ADCONV1, 0u, &adConv1_ChnConfig0);
    ADC_DRV_WaitConvDone(INST_ADCONV1);
    ADC_DRV_GetChanResult(INST_ADCONV1, 0u, &Mcu_adc);

    // 取同一个ADC的另外通道结果,单次转换只能轮流配置通道
    ADC_DRV_ConfigChan(INST_ADCONV1, 0u, &adConv1_ChnConfig1);
    ADC_DRV_WaitConvDone(INST_ADCONV1);
    ADC_DRV_GetChanResult(INST_ADCONV1, 0u, &Mcu_adc);
```
