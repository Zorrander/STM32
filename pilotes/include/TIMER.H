#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"

float Timer_1234_Init(TIM_TypeDef *Timer, float Duree_us );
void Timer_Run(TIM_TypeDef *Timer);
void Timer_Stop(TIM_TypeDef *Timer);

float Timer_1234_PWM_Init(TIM_TypeDef *Timer, float Duree_us, float rapport, int mode, int voie);
void update_PWM(TIM_TypeDef *Timer, float rapport, int voie);

void Timer_Active_IT( TIM_TypeDef *Timer, u8 Priority, void (*IT_function) (void) );

void servo(void);
void init_Servo (void) ;

// pour la girouette: compare 2 pins et compte selon l'ordre
void Timer_Compare_Count(int arr, TIM_TypeDef *Timer);

#endif /* __TIMER_H */
