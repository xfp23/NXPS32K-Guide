/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : can_pal1.h
**     Project     : S32KGuide
**     Processor   : S32K144_100
**     Component   : can_pal
**     Version     : Component SDK_S32K1xx_15, Driver 01.00, CPU db: 3.00.000
**     Repository  : SDK_S32K1xx_15
**     Compiler    : GNU C Compiler
**     Date/Time   : 2025-07-11, 10:19, # CodeGen: 39
**     Contents    :
**         CAN_Init                     - status_t CAN_Init(const can_instance_t * const instance, const...
**         CAN_Deinit                   - status_t CAN_Deinit(const can_instance_t * const instance);
**         CAN_SetBitrate               - status_t CAN_SetBitrate(const can_instance_t * const instance,...
**         CAN_GetBitrate               - status_t CAN_GetBitrate(const can_instance_t * const instance,...
**         CAN_ConfigTxBuff             - status_t CAN_ConfigTxBuff(const can_instance_t * const instance, uint32_t...
**         CAN_ConfigRemoteResponseBuff - status_t CAN_ConfigRemoteResponseBuff(const can_instance_t * const instance,...
**         CAN_ConfigRxBuff             - status_t CAN_ConfigRxBuff(const can_instance_t * const instance, uint32_t...
**         CAN_Send                     - status_t CAN_Send(const can_instance_t * const instance, uint32_t buffIdx,...
**         CAN_SendBlocking             - status_t CAN_SendBlocking(const can_instance_t * const instance, uint32_t...
**         CAN_Receive                  - status_t CAN_Receive(const can_instance_t * const instance, uint32_t buffIdx,...
**         CAN_ReceiveBlocking          - status_t CAN_ReceiveBlocking(const can_instance_t * const instance, uint32_t...
**         CAN_AbortTransfer            - status_t CAN_AbortTransfer(const can_instance_t * const instance, uint32_t...
**         CAN_SetRxFilter              - status_t CAN_SetRxFilter(const can_instance_t * const instance,...
**         CAN_GetTransferStatus        - status_t CAN_GetTransferStatus(const can_instance_t * const instance,...
**         CAN_InstallEventCallback     - status_t CAN_InstallEventCallback(const can_instance_t * const instance,...
**         CAN_GetDefaultConfig         - void CAN_GetDefaultConfig(can_instance_t * instance, can_user_config_t *...
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
** @file can_pal1.h
** @version 01.00
*/         
/*!
**  @addtogroup can_pal1_module can_pal1 module documentation
**  @{
*/         
#ifndef can_pal1_H
#define can_pal1_H

/**
 * @page misra_violations MISRA-C:2012 violations
 *
 * @section [global]
 * Violates MISRA 2012 Advisory Rule 2.3, should not contain unused type declarations 
 * These definitions are used by the driver. 
 */
 
/* MODULE can_pal1. */

/* Include inherited beans */
#include "clockMan1.h"
#include "dmaController1.h"
#include "Cpu.h"

/*! @brief PAL instance information */
extern const can_instance_t can_pal1_instance;

/*! @brief User configuration structure */
extern const can_user_config_t can_pal1_Config0;

#endif
/* ifndef __can_pal1_H */
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

