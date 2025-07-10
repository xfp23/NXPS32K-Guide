# NXP S32K系列 (S32DS环境)指南

## 版权

© 2025 . 未经许可不得复制、修改或分发。 此文献为 [小风的藏书阁](https://t.me/xfp2333) 所有。

# I2C(I2C PAL) 抽象层指南

## 一、硬件配置

1. 打开 `pin_mux`，选择 `lpi2c` 选项卡选择 I2C 引脚

2. 选择 `Components`新增 `I2C_pal` 模块，根据选项卡设置参数

## 二、初始化代码

```c
// 当需要换个从设备通信时，必须重新初始化，因为在这里设置从地址
I2C_MasterInit(&i2c1_instance, &i2c1_MasterConfig0);
```

**I2C\_MasterInit 函数原型:**

```c
status_t I2C_MasterInit(
    i2c_master_state_t * instance,
    const i2c_master_user_config_t * userConfig
);
```

* `instance`：I2C 实例状态指针
* `userConfig`：用户配置结构，包括从设备地址、速率等

## 三、发送与接收

### 发送函数

```c
/**
 * @brief I2C 主机静态发送函数 (non-blocking)
 */
I2C_MasterSendData(&i2c1_instance, buffer, size, true);

/**
 * @brief I2C 主机阻塞发送函数 (blocking)
 */
I2C_MasterSendDataBlocking(&i2c1_instance, buffer, size, true, 100);
```

**I2C\_MasterSendDataBlocking 函数原型:**

```c
status_t I2C_MasterSendDataBlocking(
    const i2c_master_state_t * instance,
    const uint8_t * txBuff,
    uint32_t txSize,
    bool sendStop,
    uint32_t timeout
);
```

* `sendStop` ：是否发送 STOP 信号

### 接收函数

```c
/**
 * @brief I2C 主机静态接收函数 (non-blocking)
 */
I2C_MasterReceiveData(&i2c1_instance, buffer, size, true);

/**
 * @brief I2C 主机阻塞接收函数 (blocking)
 */
I2C_MasterReceiveDataBlocking(&i2c1_instance, buffer, size, true, 100);
```

## 四、用户实现字节函数示例

### 发送

```c
/**
 * @brief I2C 写入多个字节到当前已配置的从设备地址
 */
void UserTools_I2C_Writebytes(uint8_t* buffer, size_t size)
{
    if (buffer == NULL || size == 0) return;

    status_t status = I2C_MasterSendDataBlocking(&i2c1_instance, buffer, size, true, 100);

    if (status != STATUS_SUCCESS)
    {
        // 处理错误
    }
}
```

### 接收

```c
/**
 * @brief 从当前已配置的从设备地址读取多个字节
 */
void UserTools_I2C_Readbytes(uint8_t *buffer, size_t size)
{
    if (buffer == NULL || size == 0) return;

    status_t status = I2C_MasterReceiveDataBlocking(
        &i2c1_instance,          // I2C 实例
        buffer,                  // 接收缓冲区
        size,                    // 要接收的字节数
        true,                    // 发送 STOP
        100                      // 超时时间（ms）
    );

    if (status != STATUS_SUCCESS)
    {
        // 处理错误
    }
}
```

## 五、释放 I2C 资源

```c
I2C_MasterDeinit(&i2c1_instance);
```

### I2C\_MasterDeinit 函数原型:

```c
status_t I2C_MasterDeinit(i2c_master_state_t * instance);
```

* 给定实例释放资源，关闭引脚

---

如需添加 I2C 地址切换方法，可通过:

```c
I2C_DRV_MasterSetSlaveAddr(INST_LPI2C1, new_address, false);
```

---
