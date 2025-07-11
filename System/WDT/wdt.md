# NXP S32K系列 (S32DS环境)指南

## 版权

© 2025 . 未经许可不得复制、修改或分发。
此文献为 [小風的藏書閣](https://t.me/xfp2333) 所有。

---

# WDT（硬件看门狗）指南

---

## 初始化步骤

### 1. 添加 Watchdog 组件

在 **Components** 面板中，添加 `watchdog` 组件（例如：`WATCHDOG1`）。

### 2. 配置 Watchdog 参数（Watchdog Configuration0）

| 选项                     | 说明                    | 推荐配置                                                                                                                         |
| ---------------------- | --------------------- | ---------------------------------------------------------------------------------------------------------------------------- |
| **Clock Source**       | 看门狗计数时钟源              | `WDOG_LPO_CLOCK`（默认，128 kHz，低功耗）<br>`WDOG_SIRC_CLOCK`（8 MHz / 128 KHz）<br>`WDOG_SOSC_CLOCK`（外部晶振）<br>`WDOG_BUS_CLOCK`（主系统时钟） |
| **Prescaler Enable**   | 是否使能预分频器              | 建议开启，用于延长溢出时间                                                                                                                |
| **Prescaler**          | 预分频值                  | 典型为`256`，实际分频因型号不同而异                                                                                                         |
| **Update Enable**      | 是否允许中途修改配置            | 若关闭，则初始化后无法更改配置                                                                                                              |
| **Interrupt Enable**   | 看门狗超时是否触发中断           | 通常关闭（使用复位），如需中断则开启                                                                                                           |
| **Interrupt Update**   | 中断参数是否可更新             | 建议关闭                                                                                                                         |
| **Window Mode Enable** | 是否启用窗口看门狗模式           | 默认关闭；若开启则必须在设定时间窗口内喂狗                                                                                                        |
| **Window Value**       | 窗口模式下早期喂狗禁止窗口         | 通常设为0（关闭窗口模式）                                                                                                                |
| **Timeout Value**      | 超时时间（计数周期）            | 依据所选时钟频率与需求设置                                                                                                                |
| **Wait Mode Enable**   | MCU 进入 Wait 模式时是否继续计数 | 视应用场景配置，通常开启                                                                                                                 |
| **Stop Mode Enable**   | MCU 停止模式是否继续计数        | 可根据是否需要深度省电设置                                                                                                                |
| **Debug Mode Enable**  | 调试模式下是否暂停计数           | 开启：调试时防止误复位                                                                                                                  |

---

### 3. 初始化代码

```c
WDOG_DRV_Init(INST_WATCHDOG1, &watchdog1_Config0);
```

---

## 喂狗函数（定期调用）

```c
WDOG_DRV_Trigger(INST_WATCHDOG1);  // 喂狗，重置计数器
```

建议定期在主循环或定时器中调用，确保系统正常运行。

---

## 时间计算

超时时间计算方式如下：

```
Watchdog Timeout = (Timeout Value × Prescaler) / Clock Frequency
```

### 例子：

* `Clock Source = WDOG_LPO_CLOCK = 128 kHz`
* `Prescaler = 256`
* `Timeout Value = 0x4000 (16384)`

计算得：

```
Timeout = (16384 × 256) / 128000 ≈ 32.768 秒
```

---

## API 函数原型与参数说明

### `WDOG_DRV_Init()`

```c
status_t WDOG_DRV_Init(uint32_t instance, const wdog_user_config_t * config);
```

* `instance`: 看门狗编号（如 `INST_WATCHDOG1`）
* `config`: 初始化配置结构体指针（自动生成）

返回值：状态码（`STATUS_SUCCESS` 表示成功）

---

### `WDOG_DRV_Trigger()`

```c
void WDOG_DRV_Trigger(uint32_t instance);
```

* `instance`: 看门狗编号（如 `INST_WATCHDOG1`）

功能：立即向看门狗写入触发值，重置计数器，防止系统复位。

---

### `WDOG_DRV_Deinit()`

```c
status_t WDOG_DRV_Deinit(uint32_t instance);
```

* 停止看门狗功能（仅当 `UpdateEnable == true` 时有效）
* 通常不推荐在正常系统运行时调用

---

## 小贴士

* 看门狗是防止程序死锁的重要机制，建议在**主循环、定时器中断、任务调度末尾**处喂狗。
* 初始化后默认会自动启动看门狗。
* 若想开发板调试时不被复位，请开启 Debug Mode（调试暂停喂狗）。
