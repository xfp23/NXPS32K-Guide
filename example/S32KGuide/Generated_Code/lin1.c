/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : lin1.c
**     Project     : S32KGuide
**     Processor   : S32K144_100
**     Component   : lin
**     Version     : Component SDK_S32K1xx_15, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_S32K1xx_15
**     Compiler    : GNU C Compiler
**     Date/Time   : 2025-07-11, 15:03, # CodeGen: 40
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
** @file lin1.c
** @version 01.00
*/         
/*!
**  @addtogroup lin1_module lin1 module documentation
**  @{
*/         

/*!
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.
 * The external variables will be used in other source files, with the same initialized values.
 */

/* MODULE lin1. */

#include <stdbool.h>
#include <stddef.h>
#include "lin1.h"


/*! lin1 configuration structure */

lin_user_config_t lin1_InitConfig0 = {
    .baudRate = 9600U,
    .nodeFunction = (bool)MASTER,
    .autobaudEnable = false,
    .timerGetTimeIntervalCallback = lin1TimerGetTimeIntervalCallback0,
    .classicPID = NULL,
    .numOfClassicPID = 255U,
};

/*! Driver state structure */
lin_state_t lin1_State;

/* END lin1. */
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
