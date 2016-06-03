#include "stm32f10x.h"
#include "voilier.h"
#include "girouette.h"
#include "piloteRF.h"
#include "gpio.h"
#include "timer.h"
#include "Timer_Systick.h"

void Config_EXTI_Girouette(){
	// configure the interruption for Index
	// EXTI5
	EXTI->RTSR |= EXTI_RTSR_TR5;
	EXTI->IMR |= EXTI_IMR_MR5;
}

void Disable_EXTI_Girouette(){
	EXTI->RTSR &= ~EXTI_RTSR_TR5;
	EXTI->IMR &= ~EXTI_IMR_MR5;
}

// reset le conteur (=angle de la girouette) à 0
void EXTI9_5_GirouetteHandler(){
	TIM3->CNT = 0;
	Disable_EXTI_Girouette();
	EXTI->PR |= EXTI_PR_PR5;
}

TIM_TypeDef * TIM_SERVO;
int voie;
void initGirouette(void){
	TIM_SERVO=TIM4;
	voie=3;
	
	// initialisation des broches pour la lecture en mode floating input
	Port_IO_Init_InputPull(GPIOA, 5); // Index
	Port_IO_Init_Input(GPIOA, 6); // A
	Port_IO_Init_Input(GPIOA, 7); // B
	
	// configuration du timer pour la position (angle) de la girouette
	// on multiplie par quatre car il y a 4 cas différents où le timer s'incrémente
	Timer_Compare_Count(360*4, TIM3);
	
	// configuration de la mise à jour du servo-moteur de la voile
	// met à jour la position de la voile à chaque période
	
	// enable SysTick
	SysTick->CTRL |= SysTick_CTRL_ENABLE;
	// enable interruption
	SysTick->CTRL |= SysTick_CTRL_TICKINT;
	
	Systick_Prio_IT(6, UpdateGrandeVoile);
	// toutes les 10ms
	Systick_Period(10000);
	
	// init the PWM
	Port_IO_Init_Alt_Output( GPIOB, 8);
	Timer_1234_PWM_Init(TIM_SERVO, tempsPWM, 0.87f, 1, voie);
	Timer_Run(TIM_SERVO);
}

// remodifier la valeur de la PWM
void UpdateGrandeVoile(){
	// rapports PWM pour les positions max du servo moteur
	float rapport_max = 0.925f;
	float rapport_min = 0.82f;
	
	float angle_girouette, nouveau_rapport ;
	
	angle_girouette = absFloat(GetAngle());
	
	//Si angle_girouette compris entre 45 et 180° -> (theta - 45) / 135
	//Si angle_girouette compris entre 0 et 45 		-> 0
	if (angle_girouette < 45.0 ){
		nouveau_rapport = 0.0 ;
	}	else {
		nouveau_rapport = (angle_girouette - 45.0) / 135.0;
	}
	
	if (nouveau_rapport > 1.0)
		nouveau_rapport = 1.0;
	
	nouveau_rapport = (nouveau_rapport * (rapport_max-rapport_min)) + rapport_min;
	
	//changement angle servo : modification de la période PWM
	update_PWM(TIM_SERVO, nouveau_rapport, voie);
}

float GetAngle(void){
	float result = (float) TIM3->CNT;
	result /= 4;
	if (result >= 180)
		result = -(360 - result);
	return result;
}

float absFloat(float input){
	if (input < 0.0)
		return -input;
	else
		return input;
}
