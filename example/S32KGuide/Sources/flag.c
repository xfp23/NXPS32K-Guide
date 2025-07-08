#include "flag.h"

/**
 * @brief 指南gpio教程
 *
 */
void UserGuide_gpio()
{
    uint8_t state = 0;
    PINS_DRV_WritePin(PTE, 12, 1);            // PTE12写入高电平
    PINS_DRV_WritePins(PTE, 1 << 12);         // 给端口输出按掩码写入
    state = PINS_DRV_ReadPins(PTE) & 1 << 12; // 读出整个引脚状态然后掩码查看指定引脚
}

/**
 * @brief 指南adc教程
 * @note 此函数code1 为ADC单次转换
 */
void UserGuide_adc()
{

    /** user code1 begin */
    uint16_t Mcu_adc = 0x00;
    ADC_DRV_ConfigChan(INST_ADCONV1, 0u, &adConv1_ChnConfig0);
    ADC_DRV_WaitConvDone(INST_ADCONV1);
    ADC_DRV_GetChanResult(INST_ADCONV1, 0u, &Mcu_adc);
    /** user code1 end */

    /** user code2 begin */
    
    /** user code2 end */
}
