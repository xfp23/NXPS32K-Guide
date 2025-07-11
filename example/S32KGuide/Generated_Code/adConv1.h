/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : adConv1.h
**     Project     : S32KGuide
**     Processor   : S32K144_100
**     Component   : adc
**     Version     : Component SDK_S32K1xx_15, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_S32K1xx_15
**     Compiler    : GNU C Compiler
**     Date/Time   : 2025-07-08, 16:42, # CodeGen: 8
**     Contents    :
**         ADC_DRV_InitConverterStruct       - void ADC_DRV_InitConverterStruct(adc_converter_config_t *const config);
**         ADC_DRV_ConfigConverter           - void ADC_DRV_ConfigConverter(const uint32_t instance,const...
**         ADC_DRV_GetConverterConfig        - void ADC_DRV_GetConverterConfig(const uint32_t...
**         ADC_DRV_Reset                     - void ADC_DRV_Reset(const uint32_t instance);
**         ADC_DRV_InitHwCompareStruct       - void ADC_DRV_InitHwCompareStruct(adc_compare_config_t *const config);
**         ADC_DRV_ConfigHwCompare           - void ADC_DRV_ConfigHwCompare(const uint32_t instance,const...
**         ADC_DRV_GetHwCompareConfig        - void ADC_DRV_GetHwCompareConfig(const uint32_t instance,adc_compare_config_t...
**         ADC_DRV_InitHwAverageStruct       - void ADC_DRV_InitHwAverageStruct(adc_average_config_t *const config);
**         ADC_DRV_ConfigHwAverage           - void ADC_DRV_ConfigHwAverage(const uint32_t instance,const...
**         ADC_DRV_GetHwAverageConfig        - void ADC_DRV_GetHwAverageConfig(const uint32_t instance,adc_average_config_t...
**         ADC_DRV_InitChanStruct            - void ADC_DRV_InitChanStruct(adc_chan_config_t *const config);
**         ADC_DRV_ConfigChan                - void ADC_DRV_ConfigChan(const uint32_t instance,const uint8_t chanIndex,const...
**         ADC_DRV_GetChanConfig             - void ADC_DRV_GetChanConfig(const uint32_t instance,const uint8_t...
**         ADC_DRV_SetSwPretrigger           - void ADC_DRV_SetSwPretrigger(const uint32_t instance,const...
**         ADC_DRV_WaitConvDone              - void ADC_DRV_WaitConvDone(const uint32_t instance);
**         ADC_DRV_GetConvCompleteFlag       - bool ADC_DRV_GetConvCompleteFlag(const uint32_t instance,const uint8_t...
**         ADC_DRV_GetChanResult             - void ADC_DRV_GetChanResult(const uint32_t instance,const uint8_t...
**         ADC_DRV_AutoCalibration           - void ADC_DRV_AutoCalibration(const uint32_t instance);
**         ADC_DRV_InitUserCalibrationStruct - void ADC_DRV_InitUserCalibrationStruct(adc_calibration_t *const config);
**         ADC_DRV_ConfigUserCalibration     - void ADC_DRV_ConfigUserCalibration(const uint32_t instance,const...
**         ADC_DRV_GetUserCalibration        - void ADC_DRV_GetUserCalibration(const uint32_t instance,adc_calibration_t...
**         ADC_DRV_GetInterruptNumber        - IRQn_Type ADC_DRV_GetInterruptNumber(const uint32_t instance);
**         ADC_DRV_ClearLatchedTriggers      - void ADC_DRV_ClearLatchedTriggers(const uint32_t instance,const...
**         ADC_DRV_ClearTriggerErrors        - void ADC_DRV_ClearTriggerErrors(const uint32_t instance);
**         ADC_DRV_GetTriggerErrorFlags      - uint32_t ADC_DRV_GetTriggerErrorFlags(const uint32_t instance);
**
**     Copyright 1997 - 2015 Freescale Semiconductor, Inc. 
**     Copyright 2016-2017 NXP 
**     All Rights Reserved.
**     
**     THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
**     IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
**     OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
**     IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
**     INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
**     SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
**     HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
**     STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
**     IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
**     THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file adConv1.h
** @version 01.00
*/         
/*!
**  @addtogroup adConv1_module adConv1 module documentation
**  @{
*/         
#ifndef adConv1_H
#define adConv1_H
/* MODULE adConv1. */

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.5, global macro not referenced
 * There are global macros defined to be used by the integrator and another one used as include guard.
 */

/* Include inherited beans */
#include "clockMan1.h"
#include "Cpu.h"
#include "adc_driver.h"

/*! @brief Device instance number */
#define INST_ADCONV1 0U

/*! @brief Configuration declaration */
extern const adc_converter_config_t adConv1_ConvConfig1;
    
/*! @brief Configuration declaration */
extern const adc_chan_config_t adConv1_ChnConfig0;
extern const adc_chan_config_t adConv1_ChnConfig1;
    
/*! @brief Configuration declaration */
extern const adc_compare_config_t adConv1_HwCompConfig0;
    
/*! @brief Configuration declaration */
extern const adc_average_config_t adConv1_HwAvgConfig0;
    
#endif
/* ifndef adConv1_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the Freescale S32K series of microcontrollers.
**
** ###################################################################
*/
