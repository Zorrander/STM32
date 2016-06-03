#include "piloteRF.h"
#include "timer.h"
#include "gpio.h"

/*
Quand PB6(ou 7) à 1 -> active comptage timer
Quand PB6(ou 7) à 0 -> fin comptage -> valeur entre 1 et 2 ms
valeur = valeur -1,5
>0 -> droite relais à 1 PA2
<0 -> gauche relais à 0
0 -> rien
*/

TIM_TypeDef * TIM_RF_CNT;

int timcnt;
char first;
int autoreload;
int commande;
int correction;
int commande_max;
float rapport;

/*
Valeur mini ? 1ms
Valeur neutre ? 1,50 ms
Valeur maxi ? 2,0ms
*/
void EXTI9_5_RFHandler(){
	if(Port_IO_Read(GPIOB, PIN_RF)){
		Timer_Run(TIM_RF_CNT);
	} else {
		timcnt = TIM_RF_CNT->CNT;
		Timer_Stop(TIM_RF_CNT);
		TIM_RF_CNT->CNT = 0;
		timcnt = (timcnt*TIM_RF_US)/autoreload;
		if (first>10){
			correction = timcnt;
			first=0;
		} else if (first!=0) {
			first+=1;
		} else {
			commande = timcnt - correction;
			if (commande > commande_max){
				commande_max=commande;
			} else if ((-commande) > commande_max){
				commande_max=(-commande);
			}
			if (commande > seuilCommande) {
				Port_IO_Set(GPIOA, 2);
				rapport = (float) commande/commande_max;
			} else if (commande < -seuilCommande) {
				Port_IO_Reset(GPIOA, 2);
				rapport = (float) (-commande)/commande_max;
			} else {
				rapport = 0;
			}
			update_PWM(TIM2, 1.0f-rapport, 2);
		}
	}
	EXTI->PR |= 0x1 << 6;
}


void Config_EXTI_RF(){
	// Configure mask of interrupt lines
	EXTI->IMR|=EXTI_IMR_MR6;
	// Configure mask of event lines
	//EXTI->EMR|=EXTI_EMR_MR6;
	// Configure trigger selection bits
	EXTI->RTSR|=EXTI_RTSR_TR6;	// rising
	EXTI->FTSR|=EXTI_FTSR_TR6;	// falling
	
	
	RCC->APB2ENR|= RCC_APB2ENR_AFIOEN;
	//Choose port b6 as input
	AFIO->EXTICR[1] &= ~(AFIO_EXTICR2_EXTI6);
	AFIO->EXTICR[1] |= AFIO_EXTICR2_EXTI6_PB;
}

void RF_Init(){
	// Timer pour compter
	TIM_RF_CNT=TIM1;
	
	first=1;
	correction=1500;
	commande_max=1;
	//Configure le pin du RF en entrée
	Timer_1234_Init(TIM_RF_CNT, TIM_RF_US);
	autoreload = TIM_RF_CNT->ARR;
	Port_IO_Init_InputPull(GPIOB, PIN_RF);
	
	//Configuration moteur 
	Port_IO_Init_Alt_Output(GPIOA, 1);
	Port_IO_Init_Output(GPIOA, 2);
	Port_IO_Set(GPIOA, 2);
	
	//Timer pour commander le moteur en PWM
	Timer_1234_PWM_Init(TIM2, tempsPWM, 0.0f, 1, 2);
	Timer_Run(TIM2);
}

void RF_Battery_Low(){
	
}

void RF_Angle_Max(){
	
}
