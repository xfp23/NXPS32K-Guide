# NXP S32K系列 (S32DS环境)指南 (未验证)

## 版权

© 2025. 未经许可不得复制、修改或分发。此文献为 [小風的藏書閣](https://t.me/xfp2333) 所有。

---

# ADC 连续转换（Continuous Conversion）

---

## 📘 简介

**连续转换模式**指的是 ADC 一次启动后，不需用户软件再次触发，ADC模块会不断采样并转换指定通道上的模拟信号。适用于：

* 轮询方式定时采样
* 使用 DMA 连续搬运数据
* 背景信号监测场景

---

## 🛠 步骤概览

1. 添加并配置 ADC 组件
2. 启用 `Continuous Conversion` 选项
3. 配置 ADC 通道
4. 启动转换（手动或硬件触发）
5. 可选：结合 DMA 实现高效搬运

---

## 🔧 配置步骤

### 1. 添加 `adc` 组件

在 `.mex` 配置中：

* 添加 `adc`（如 `adConv0`）
* 右侧属性中进行如下配置：

---

### 2. 启用连续转换

在 `Configurations` 页签中：

| 项目                        | 设置建议                        |
| ------------------------- | --------------------------- |
| Input Clock               | FIRC/SOSC 等                 |
| Resolution                | 12-bit                      |
| Clock Divide              | Divide by 4 等               |
| Trigger Source            | **Software Trigger**（或 PDB） |
| **Continuous Conversion** | ✅ 勾选                        |
| DMA Enable                | 可选，若结合 DMA 使用               |
| Voltage Reference         | VDDA / VREFH 等              |

---

### 3. 添加通道配置

在 `Channel Configuration` 页签中：

* 添加通道（如 `ADC0_SE4`）
* 设置对应输入通道编号
* 可设置中断或 DMA 使能（按需）

---

## 🧱 示例代码

### 初始化

```c
// 1. 校准
ADC_DRV_AutoCalibration(INST_ADCONV0);

// 2. 初始化ADC
ADC_DRV_ConfigConverter(INST_ADCONV0, &adConv0_ConvConfig0);

// 3. 配置通道
ADC_DRV_ConfigChan(INST_ADCONV0, 0u, &adConv0_ChnConfig0);

// 4. 启动转换
ADC_DRV_StartGroupConversion(INST_ADCONV0);
```

---

### 获取结果（阻塞方式）

```c
adc_result_t adc_result;

while (1)
{
    if (ADC_DRV_GetChanResult(INST_ADCONV0, 0u, &adc_result) == STATUS_SUCCESS)
    {
        // adc_result 保存转换结果
    }
}
```

---

## ⚙ 配合 DMA 使用（推荐）

若启用了 DMA：

* 初始化 `edma` 控制器
* 配置 DMA 搬运目标数组
* 启动后，无需调用 `GetChanResult`，数据自动搬运

```c
uint16_t adc_dma_buffer[1]; // 每个通道一个位置

// 启动连续采样 + DMA
ADC_DRV_StartGroupConversion(INST_ADCONV0);
// 结果会不断填入 adc_dma_buffer[0]
```

---

## 🔍 注意事项

| 项目      | 建议或说明                                           |
| ------- | ----------------------------------------------- |
| 采样速率    | 受限于 ADC Clock / Clock Divider / Sample Time     |
| 多通道     | **连续转换模式建议只采单通道**，否则需要软件切换通道                    |
| 多通道连续采样 | 推荐使用 PDB + DMA 配合实现                             |
| CPU读取方式 | 若不使用DMA，可通过 `GetChanResult` + `WaitConvDone` 实现 |
| 溢出      | 若读取不及时可能会导致采样结果丢失，可启用中断或 DMA                    |

---

## ✅ 示例：轮询获取电压值

```c
#include "adc_driver.h"

void Adc_ContinuousInit(void)
{
    ADC_DRV_AutoCalibration(INST_ADCONV0);
    ADC_DRV_ConfigConverter(INST_ADCONV0, &adConv0_ConvConfig0);
    ADC_DRV_ConfigChan(INST_ADCONV0, 0u, &adConv0_ChnConfig0);
    ADC_DRV_StartGroupConversion(INST_ADCONV0);
}

uint16_t Adc_ReadVoltage(void)
{
    adc_result_t result;
    if (ADC_DRV_GetChanResult(INST_ADCONV0, 0u, &result) == STATUS_SUCCESS)
    {
        return result;
    }
    return 0xFFFF; // 错误码
}
```

---

## 📎 总结

| 场景      | 推荐方式                 |
| ------- | -------------------- |
| 单通道轮询采样 | 启用 Continuous + 软件读取 |
| 多通道轮询采样 | 使用 PDB 或软件切换通道       |
| 单/多通道高速 | 启用 DMA + Continuous  |
