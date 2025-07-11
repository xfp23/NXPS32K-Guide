/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : can_pal1.c
**     Project     : S32KGuide
**     Processor   : S32K144_100
**     Component   : can_pal
**     Version     : Component SDK_S32K1xx_15, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_S32K1xx_15
**     Compiler    : GNU C Compiler
**     Date/Time   : 2025-07-11, 10:19, # CodeGen: 39
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
** @file can_pal1.c
** @version 01.00
*/         
/*!
**  @addtogroup can_pal1_module can_pal1 module documentation
**  @{
*/         
#include "can_pal1.h"

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.1, types should be explicitly specified  
 * This rule applies only for C90 standard. Defining other integer types that are 
 * permitted in bit-field declarations is allowed in C99 standard.
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 8.4, external symbol defined without a prior declaration  
 * This symbols are declared in the generated configuration structures.  
 *
 * @section [global]
 * Violates MISRA 2012 Required Rule 1.3, occurrence of undefined symbol
 * This module is used only with defined values based on no of can interfaces and features enabled.
 *  
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 8.7, External variable could be made static.
 * The external variables will be used in other source files, with the same initialized values.
 *
 */


/*! @brief PAL instance information */
const can_instance_t can_pal1_instance = {CAN_INST_TYPE_FLEXCAN, 0U};

/*! @brief User configuration structure */
const can_user_config_t can_pal1_Config0 = {
    .maxBuffNum = 16UL,
    .mode = CAN_NORMAL_MODE,
    .peClkSrc = CAN_CLK_SOURCE_OSC,
    .enableFD = false,
    .payloadSize = CAN_PAYLOAD_SIZE_8,
    .nominalBitrate = {
        .propSeg = 7,
        .phaseSeg1 = 4,
        .phaseSeg2 = 1,
        .preDivider = 0,
        .rJumpwidth = 1
    },
    .dataBitrate = {
        .propSeg = 7,
        .phaseSeg1 = 4,
        .phaseSeg2 = 1,
        .preDivider = 0,
        .rJumpwidth = 1
    },
    .extension = NULL,
};
/* END can_pal1. */
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
