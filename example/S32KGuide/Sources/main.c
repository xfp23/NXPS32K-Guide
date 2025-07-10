/* ###################################################################
**     Filename    : main.c
**     Processor   : S32K1xx
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/
/*!
**  @addtogroup main_module main module documentation
**  @{
*/
/* MODULE main */

/* Including necessary module. Cpu.h contains other modules needed for compiling.*/
#include "Cpu.h"
#include "flag.h"

volatile int exit_code = 0;

/* User includes (#include below this line is not maintained by Processor Expert) */

/*!
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
/* Write your local variable definition here */

/*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
  PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
  /*** End of Processor Expert internal initialization.                    ***/
  CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT, g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
  CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);
  /** 初始化GPIO */
  PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

  /** 初始化ADC */
  ADC_DRV_ConfigConverter(INST_ADCONV1, &adConv1_ConvConfig1); // 初始化ADC
  ADC_DRV_AutoCalibration(INST_ADCONV1);                       // 校准ADC
                                                               //  SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));  // Enable CP10 and CP11 full access

  /** 初始化定时器 */
  FTM_DRV_Init(INST_FLEXTIMER_MC1, &flexTimer_mc1_InitConfig, &UserState_FTM0 /* ftm_state_t * state */);
  INT_SYS_InstallHandler(FTM0_Ovf_Reload_IRQn, &Timer0_PeriodElapsedISR, (isr_t *)0u);
  INT_SYS_EnableIRQ(FTM0_Ovf_Reload_IRQn);                             // 使能timer0中断
  FTM_DRV_InitCounter(INST_FLEXTIMER_MC1, &flexTimer_mc1_TimerConfig); // 初始化timer0的计数器
  FTM_DRV_CounterStart(INST_FLEXTIMER_MC1);                            // 启动timer0的计数器

  ///** 设置外部中断 *

  /* 设置中断触发条件 */
  PINS_DRV_SetPinIntSel(PORTC, 12, PORT_INT_RISING_EDGE);  // PTC12 上升沿触�??????????????
  PINS_DRV_SetPinIntSel(PORTC, 13, PORT_INT_FALLING_EDGE); // PTC13 下降沿触�??????????????
  /* 安装中断处理函数 */
  INT_SYS_InstallHandler(PORTC_IRQn, &PTC_EXT_IRQ, NULL);
  /* 启用中断 */
  INT_SYS_EnableIRQ(PORTC_IRQn);

  /** DMA */
  EDMA_DRV_Init(&dmaController1_State, &dmaController1_InitConfig0, edmaChnStateArray, edmaChnConfigArray, EDMA_CONFIGURED_CHANNELS_COUNT);

  /** UART init  */
  LPUART_DRV_Init(INST_LPUART1, &lpuart1_State, &lpuart1_InitConfig0);
  LPUART_DRV_InstallRxCallback(INST_LPUART1, UART_RX_ISR, (void *)INST_LPUART1);
  LPUART_DRV_ReceiveData(INST_LPUART1, UserCommon.Uart1_rxdata, UART_BUFFERSIZE);
  OSIF_TimeDelay(100); // 100ms 延时

  /**I2C pal Init */
  //  LPI2C_DRV_MasterInit(INST_LPI2C1, &lpi2c1_MasterConfig0,&lpi2c1_MasterState);
  I2C_MasterInit(&i2c1_instance, &i2c1_MasterConfig0); // I2C_MasterInit(&i2c1_instance,);
  LPI2C_DRV_MasterInit(INST_LPI2C1, &lpi2c1_MasterConfig0, &lpi2c1_MasterState);
  /** PWM init */
  PWM_Init(&pwm_pal1Instance, &pwm_pal1Configs);
  PWM_UpdateDuty(&pwm_pal1Instance, pwm_pal1Configs.pwmChannels->channel, pwm_pal1Configs.pwmChannels->period * (50 / 100)); // 占空比根据周期计�????????????

  /** spi init
   */
  LPSPI_DRV_MasterInit(LPSPICOM1, &lpspiCom1State, &lpspiCom1_MasterConfig0);
  // LPSPI_DRV_MasterTransfer(LPSPICOM1,);

#if USE_WDT
  /** WDOG init */
  WDOG_DRV_Init(INST_WATCHDOG1, &watchdog1_Config0);
#endif

  while (1)
  {
#if USE_WDT
    WDOG_DRV_Trigger(INST_WATCHDOG1); // 喂狗
#endif
    UserGuide_gpio();
    UserGuide_adc();
  }
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the NXP S32K series of microcontrollers.
**
** ###################################################################
*/
