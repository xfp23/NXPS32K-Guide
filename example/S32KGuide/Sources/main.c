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
#include "pins_driver.h"
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
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

    /** 初始化GPIO */
PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

/** 初始化ADC */
ADC_DRV_ConfigConverter(INST_ADCONV1, &adConv1_ConvConfig1); // 初始化ADC
ADC_DRV_AutoCalibration(INST_ADCONV1); // 校准ADC

/** 初始化定时器 */
FTM_DRV_Init(INST_FLEXTIMER_MC1, &flexTimer_mc1_InitConfig,&UserState_FTM0/* ftm_state_t * state */);
INT_SYS_InstallHandler(FTM0_Ovf_Reload_IRQn,&Timer0_PeriodElapsedISR,(isr_t*)0u);
INT_SYS_EnableIRQ(FTM0_Ovf_Reload_IRQn);    // 使能timer0中断
FTM_DRV_InitCounter(INST_FLEXTIMER_MC1, &flexTimer_mc1_TimerConfig);    // 初始化timer0的计数器
FTM_DRV_CounterStart(INST_FLEXTIMER_MC1);   // 启动timer0的计数器



while(1)
{
  UserGuide_gpio();
  UserGuide_adc();
//	if(PINS_DRV_ReadPin(PTC,13) == 0) // 读取单个引脚
//	{
//		PINS_DRV_WritePin(PTE,13,0);
//	}
//	PINS_DRV_SetPullSel();
//PINS_DRV_SetMuxModeSel();
//PINS_DRV_SetPinIntSel();
//PINS_DRV_GetPinIntSel();
//PINS_DRV_ClearPinIntFlagCmd();
//PINS_DRV_EnableDigitalFilter();
//PINS_DRV_DisableDigitalFilter();
//PINS_DRV_ConfigDigitalFilter();
//PINS_DRV_GetPortIntFlag();
//PINS_DRV_ClearPortIntFlagCmd();
//PINS_DRV_SetGlobalPinControl();
//PINS_DRV_SetGlobalIntControl();
//PINS_DRV_GetPinsDirection();
//PINS_DRV_SetPinDirection();
//PINS_DRV_SetPinsDirection();
//PINS_DRV_SetPortInputDisable();
//PINS_DRV_GetPortInputDisable();
//PINS_DRV_WritePin();
//PINS_DRV_WritePins();
//PINS_DRV_GetPinsOutput();
//PINS_DRV_SetPins();
//PINS_DRV_ClearPins();
//PINS_DRV_TogglePins();
//PINS_DRV_ReadPins();
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
