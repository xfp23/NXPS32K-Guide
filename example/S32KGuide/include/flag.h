#ifndef FLAG_H
#define FLAG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "Cpu.h"


extern ftm_state_t UserState_FTM0;

extern void UserGuide_gpio();
extern void UserGuide_adc();
extern void Timer0_PeriodElapsedISR();

#ifdef __cplusplus
}
#endif

#endif
