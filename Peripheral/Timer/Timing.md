# NXP S32K系列 (S32DS环境)指南

## 版权

© 2025 . 未经许可不得复制、修改或分发。 此文献为 [小風的藏書閣](https://t.me/xfp2333) 所有。

# 定时器周期性中断

## 初始化定时器

1. 双击 `Components` 添加 `ftm_mc` 组件，进行定时器配置。

2. 选择定时器编号，并配置以下选项卡：

---

### **FTM module Clock Setup**

| 选项                     | 含义     | 说明                      |
| ---------------------- | ------ | ----------------------- |
| FTM clock source       | 时钟源选择  | 如 FIRC, SOSC, SPLL 等    |
| Clock config index     | 时钟配置索引 | 一般为 0，表示使用默认系统时钟配置      |
| Power                  | 电源控制   | 是否使能 FTM 模块电源           |
| Clock source prescaler | 分频因子   | 设定定时器时钟源的预分频（如 1, 2, 4） |

---

### **FTM global configuration**

| 选项                       | 含义     | 说明                |
| ------------------------ | ------ | ----------------- |
| Debug Mode               | 调试模式   | Debug 时是否继续计数     |
| Timer operation mode     | 工作模式   | 设置为定时器或 PWM 工作模式等 |
| Timer overflow interrupt | 溢出中断   | 勾选以启用溢出中断（周期中断）   |
| Initialization trigger   | 初始化触发源 | 可选项，可默认保留         |

---

### **Timer configuration**

| 选项            | 含义  | 说明                         |
| ------------- | --- | -------------------------- |
| Initial value | 初始值 | 计数器的起始计数值，通常为 0            |
| Final value   | 结束值 | 计数器的最大值，当计数达到此值触发中断（即 ARR） |

> 💡 注意：定时周期 = (Prescaler \* (FinalValue + 1)) / TimerClockHz

---

## 代码示例及中断流程

### 1. 初始化定时器

```c
ftm_state_t UserState_FTM0 = {0};
FTM_DRV_Init(INST_FLEXTIMER_MC1, &flexTimer_mc1_InitConfig, &UserState_FTM0);
```

#### 函数原型：

```c
status_t FTM_DRV_Init(uint32_t instance,
                      const ftm_user_config_t * config,
                      ftm_state_t * state);
```

| 参数名      | 类型                            | 说明                             |
| -------- | ----------------------------- | ------------------------------ |
| instance | uint32\_t                     | FTM 实例号，如 `INST_FLEXTIMER_MC1` |
| config   | const ftm\_user\_config\_t \* | FTM 初始化配置结构体                   |
| state    | ftm\_state\_t \*              | 用户定义的状态变量（需全局）                 |

---

### 2. 安装中断处理函数

```c
INT_SYS_InstallHandler(FTM0_Ovf_Reload_IRQn, &Timer0_PeriodElapsedISR, (isr_t*)0u);
```

#### 函数原型：

```c
void INT_SYS_InstallHandler(IRQn_Type irqNumber,
                            isr_t handler,
                            isr_t * oldHandler);
```

| 参数名        | 类型         | 说明                           |
| ---------- | ---------- | ---------------------------- |
| irqNumber  | IRQn\_Type | 中断号，如 `FTM0_Ovf_Reload_IRQn` |
| handler    | isr\_t     | 用户定义的中断函数指针                  |
| oldHandler | isr\_t \*  | 旧中断处理函数（如不使用传 NULL）          |

---

### 3. 使能中断

```c
INT_SYS_EnableIRQ(FTM0_Ovf_Reload_IRQn);
```

#### 函数原型：

```c
void INT_SYS_EnableIRQ(IRQn_Type irqNumber);
```

| 参数名       | 类型         | 说明  |
| --------- | ---------- | --- |
| irqNumber | IRQn\_Type | 中断号 |

---

### 4. 启动定时器计数器

```c
FTM_DRV_InitCounter(INST_FLEXTIMER_MC1, &flexTimer_mc1_TimerConfig);
FTM_DRV_CounterStart(INST_FLEXTIMER_MC1);
```

#### 函数原型：

```c
status_t FTM_DRV_InitCounter(uint32_t instance,
                              const ftm_timer_param_t * timer);
status_t FTM_DRV_CounterStart(uint32_t instance);
```

| 参数名      | 类型                            | 说明       |
| -------- | ----------------------------- | -------- |
| instance | uint32\_t                     | FTM 实例号  |
| timer    | const ftm\_timer\_param\_t \* | 计数器配置结构体 |

---

### 5. 中断处理函数实现

```c
void Timer0_PeriodElapsedISR(void)
{
    // 清除中断标志位
    FTM_DRV_ClearStatusFlags(INST_FLEXTIMER_MC1, FTM_TIME_OVER_FLOW_FLAG);
    // 用户逻辑
}
```

#### 清除标志函数原型：

```c
void FTM_DRV_ClearStatusFlags(uint32_t instance,
                               uint32_t statusMask);
```

| 参数名        | 类型        | 说明                                  |
| ---------- | --------- | ----------------------------------- |
| instance   | uint32\_t | FTM 实例号                             |
| statusMask | uint32\_t | 状态标志位掩码，如 `FTM_TIME_OVER_FLOW_FLAG` |

---
