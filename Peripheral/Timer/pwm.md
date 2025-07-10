# NXP S32K系列 (S32DS环境)指南

## 版权

© 2025 . 未经许可不得复制、修改或分发。 此文献为 [小風的藏書閣](https://t.me/xfp2333) 所有。

# PWM Generate (pwm\_pal模块)

## 初始化

1. 打开`pin_mux`切换上方`View Mode`选项卡到`pins`

2. 选择对应引脚的`Selected Function`列选择`Ftmx/ftm_ch/x`选择对应定时器的通道号

3. 新增`pwm_pal`模块，`Device`处选择跟刚才对应的定时器编号

4. 配置`pwm_channels`选项卡

---

| 选项                 | 说明       | 参数                                         |
| ------------------ | -------- | ------------------------------------------ |
| channel            | PWM输出通道号 | 具体定时器通道，如 0、1、2等                           |
| Alignment          | PWM计数模式  | Edge Aligned（边沿对齐）、Center Aligned（中心对齐）    |
| Period             | PWM周期    | 以时钟周期数计数，如设置为1500表示PWM周期为1500个时钟周期         |
| Duty               | 初始占空比    | 0\~Period之间的值，通常设置为Period的百分比，如750表示50%占空比 |
| Polarity           | PWM极性    | Active high（高电平有效）、Active low（低电平有效）       |
| Clock source       | 时钟源      | 选择定时器的时钟来源                                 |
| Clock prescaler    | 分频系数    | 定时器分频
| Deadtime prescaler | 死区时间分频   | 用于死区时间配置，适用于互补PWM                          |

5. 调用初始化代码

```c
PWM_Init(&pwm_pal1Instance, &pwm_pal1Configs);
```

## 启动PWM

通过设置占空比启动PWM输出：

```c
PWM_UpdateDuty(&pwm_pal1Instance, pwm_pal1Configs.pwmChannels->channel, 100); 
// 100表示占空比100%，实际值应该在0~Period之间，根据需要换算
```

## 停止PWM

将占空比设置为0，PWM停止输出高电平：

```c
PWM_UpdateDuty(&pwm_pal1Instance, pwm_pal1Configs.pwmChannels->channel, 0);
```

## 删除PWM驱动

释放PWM资源，停止PWM：

```c
PWM_Deinit(&pwm_pal1Instance);
```

## 用户自定义函数更新占空比

示例函数，将百分比占空比转换为计数周期内的对应计数值并更新：

```c
void UserTools_UpdateDuty(float dutyPercent)
{
    uint32_t dutyCount = (uint32_t)(pwm_pal1Configs.pwmChannels->period * (dutyPercent / 100.0f));
    PWM_UpdateDuty(&pwm_pal1Instance, pwm_pal1Configs.pwmChannels->channel, dutyCount);
}
```

---

## 频率与占空比计算

### PWM频率计算

PWM频率由定时器时钟频率和PWM周期（Period）决定：

$$
f_{PWM} = \frac{f_{timer}}{Period + 1}
$$

* **$f_{timer}$**: 定时器时钟频率（Hz），通常是定时器模块的输入时钟频率
* **Period**: PWM周期计数值，设置于配置中（寄存器的ARR值）

示例：

* 定时器时钟频率为 48 MHz
* 设置 Period = 1499 （计数0\~1499，计数周期为1500）

则：

$$
f_{PWM} = \frac{48,000,000}{1500} = 32,000 \text{ Hz}
$$

### 占空比计算

占空比为高电平时间占PWM周期的比例，计算公式：

$$
Duty\% = \frac{Duty\_Count}{Period + 1} \times 100\%
$$

* **Duty\_Count**: PWM高电平持续时间对应的计数值

设置PWM时，实际传入的`duty`参数是`Duty_Count`，需要根据占空比百分比计算。

---

## 官方API原型与参数说明

```c
/**
 * @brief 初始化PWM实例
 * @param instance 指向PWM实例结构体指针
 * @param config 指向PWM全局配置结构体指针
 * @return status_t 返回状态码，成功返回STATUS_SUCCESS
 */
status_t PWM_Init(const pwm_instance_t *const instance, const pwm_global_config_t *config);

/**
 * @brief 更新PWM通道占空比
 * @param instance 指向PWM实例结构体指针
 * @param channel PWM通道号
 * @param duty 占空比计数值（0~Period）
 * @return status_t 返回状态码
 */
status_t PWM_UpdateDuty(const pwm_instance_t *const instance, uint8_t channel, uint32_t duty);

/**
 * @brief 反初始化PWM，释放资源
 * @param instance 指向PWM实例结构体指针
 * @return status_t 返回状态码
 */
status_t PWM_Deinit(const pwm_instance_t *const instance);
```

---