#ifndef __PILOTERF_H
#define __PILOTERF_H

#include "stm32f10x.h"

#define PIN_RF 6
#define TIM_RF_US 3000
#define tempsPWM 10000
#define seuilCommande 20

void RF_Init(void);
void Config_EXTI_RF(void);
void EXTI9_5_RFHandler(void);

void RF_Angle_Max(void);
void RF_Battery_Low(void);

#endif /* __PILOTERF_H */
