#include "timer.h"
#include "clock.h"
#include "gpio.h"
#include "girouette.h"
#include <stdlib.h>

#define PWM_PERIOD 20000

void (*IT2_function) (void);
void (*IT3_function) (void);
void (*IT4_function) (void);


/**
* Configure les Timers 1, 2, 3 et 4
* @param Timer Pointeur vers le jeu de registres (de type TIM_TypeDef ) du
* timer considéré
* @param Duree_us Intervalle de temps exprimé en µs entre
* deux débordements successifs
* @return Le durée véritable qui a été configurée
**/
void EnableTimerClock(TIM_TypeDef *Timer){
	CLOCK_Configure();

	switch ((int)Timer) {
			 case (int)TIM1 : 
							RCC->APB2ENR |= RCC_APB2ENR_TIM1EN ;
				break;
			 case (int)TIM2 :
							RCC->APB1ENR  |= RCC_APB1ENR_TIM2EN; // Enable TIM2
				break;
			 case (int)TIM3 :
							RCC->APB1ENR  |= RCC_APB1ENR_TIM3EN ; // Enable TIM3
				break;
			 case (int)TIM4 :
							RCC->APB1ENR  |= RCC_APB1ENR_TIM4EN; // Enable TIM4
				break;
	 }
}

float Timer_1234_Init(TIM_TypeDef *Timer, float Duree_us ){
	float tRef;
	float iARR;
	float iPSC;
	
	EnableTimerClock(Timer);
     
  tRef = (((float)1)/(((float)CLOCK_GetTIMCLK(Timer))/(float)1000000));
	
	iARR = (float)(65535);	// Compte jusqu'au max
  iPSC = (Duree_us/(tRef*iARR)); // (+1)
    
	iARR = (Duree_us/(tRef*(((int)iPSC)+1)));
  Timer->PSC = (int) iPSC;		// Prescale 
	Timer->ARR = (int) iARR;	// Compte jusqu'a iARR
	//Timer->CR1 |= 0b0010000100;
	// certaines versions de Keil ne supportent apparemment pas la notation binaire
	Timer->CR1 |= 0x84;
									// CEN = 0 (on lance pas le timer maintenant),
									// UDIS = 0 (activer les event update)
									// URS = 1 (overflow/underflow generate update)
									// OPM = 0 (Counter is not stopped at update event)
									// DIR = 0 Counter used as upcounter.
	                                // CMS = 00 Edge-aligned mode. The counter counts up or down depending on the direction bit
									// ARPE = 1 Timer_ARR register is buffered.
									// CKD = 00 tDTS = tCK_INT
	 
	 return (tRef)*((float)((int)iARR))*((float)((int)iPSC+1));
}

void Timer_Run(TIM_TypeDef *Timer){
	Timer->CR1 |= TIM_CR1_CEN;
}

void Timer_Stop(TIM_TypeDef *Timer){
	Timer->CR1 &= ~TIM_CR1_CEN;
}

void TIM2_IRQHandler (void){
	IT2_function();
	TIM2->SR &= (~TIM_SR_UIF);   // Nettoyer le drapeau de mise à jour
}

void TIM3_IRQHandler (void){
	IT3_function();
	TIM3->SR &= (~TIM_SR_UIF);   // Nettoyer le drapeau de mise à jour
}

void TIM4_IRQHandler (void){
	IT4_function();
	TIM4->SR &= (~TIM_SR_UIF);   // Nettoyer le drapeau de mise à jour
}

/**
* Configure les Timers 1, 2, 3 et 4
* @param Timer : Pointeur vers le jeu de registres (de type TIM_TypeDef ) du
* timer considéré
* @param Duree_us : Intervalle de temps exprimé en µs entre
* deux débordements : successifs
* @param rapport : Duty cycle
* @param mode : Edge aligned mode / Center aligned mode
* @param voie : voie de sortie du signal PWM sur le timer (4 par timer)
* @return La durée véritable qui a été configurée
**/

float Timer_1234_PWM_Init(TIM_TypeDef *Timer, float Duree_us, float rapport, int mode, int voie){
	float iARR;
	float iCCR1;
	
	// calcul de la valeur de l'ARR et du PSC du Timer selon la duree voulue
	Timer_1234_Init(Timer, Duree_us);
	
	// Activer le TIM1
	if (Timer == TIM1)
		Timer->BDTR |= TIM_BDTR_MOE;

	/*
	*Configuration de la fréquence du signal PWM
	*/
	iARR = (float) Timer->ARR;
	iCCR1 = iARR * rapport;
	iCCR1 = (float)((int)iCCR1);
	
	//la voie

	/*
	*Choix du mode sur chaque voie
	*4 chanels sur chaque timers :
	*TIMx_CCMR1 -> chanels 1 et 2
	*TIMx_CCMR2 -> chanels 3 et 4
	*/

	//1 : donner la direction du channel CC1S
	//2 : choix du mode OC1M 110 = mode 1 OC1M 111 = mode 2
	//3 : preload enable
	//4 : capture/compare enable register et polarité
	switch (voie) {
		case 1:
			Timer->CCR1 = (int) iCCR1;
			if (mode==1) {
				Timer->CCMR1  &= (~TIM_CCMR1_CC1S);
				Timer->CCMR1 |= TIM_CCMR1_OC1M;
				Timer->CCMR1 &= (~TIM_CCMR1_OC1M_0);
			} else if (mode==2) {
			  Timer->CCMR1  |= (TIM_CCMR1_CC1S);
				Timer->CCMR1 |= TIM_CCMR1_OC1M;
			}
			
			Timer->CCMR1  |= TIM_CCMR1_OC1PE;
			Timer->CCER   |= TIM_CCER_CC1E  ;
			Timer->CCER   |= TIM_CCER_CC1P  ;
			break;
		case 2:
			Timer->CCR2 = (int) iCCR1;
			if (mode==1) {
        Timer->CCMR1 &= (~TIM_CCMR1_CC2S);
				Timer->CCMR1 |= TIM_CCMR1_OC2M;
				Timer->CCMR1 &= (~TIM_CCMR1_OC2M_0);
			} else if (mode==2) {
			  Timer->CCMR1  |= (TIM_CCMR1_CC2S);
				Timer->CCMR1 |= TIM_CCMR1_OC2M;
			}
			
			Timer->CCMR1  |= TIM_CCMR1_OC2PE;
			Timer->CCER   |= TIM_CCER_CC2E  ;
			Timer->CCER   |= TIM_CCER_CC2P  ;
			break;
		case 3:
			Timer->CCR3 = (int) iCCR1;
		  if (mode==1) {
        Timer->CCMR2  &= (~TIM_CCMR2_CC3S);
				Timer->CCMR2 |= TIM_CCMR2_OC3M;
				Timer->CCMR2 &= (~TIM_CCMR2_OC3M_0);
			} else if (mode==2) {
			  Timer->CCMR2  |= (TIM_CCMR2_CC3S);
				Timer->CCMR2 |= TIM_CCMR2_OC3M;
			}
			Timer->CCMR2  |= TIM_CCMR2_OC3PE;
			Timer->CCER   |= TIM_CCER_CC3E  ;
			Timer->CCER   |= TIM_CCER_CC3P  ;
			break;
		case 4:
			Timer->CCR4 = (int) iCCR1;
		  if (mode==1) {
				Timer->CCMR2  &= (~TIM_CCMR2_CC4S);
				Timer->CCMR2 |= TIM_CCMR2_OC4M;
				Timer->CCMR2 &= (~TIM_CCMR2_OC4M_0);
			}else if (mode==2) {
			  Timer->CCMR2  |= (TIM_CCMR2_CC4S);
				Timer->CCMR2 |= TIM_CCMR2_OC4M;
			}
			Timer->CCMR2  |= TIM_CCMR2_OC4PE;
			Timer->CCER   |= TIM_CCER_CC4E  ;
			Timer->CCER   |= TIM_CCER_CC4P  ;
			break;
	}
	
	return iCCR1/iARR;
}

void update_PWM(TIM_TypeDef *Timer, float rapport, int voie){
	float iARR;
	float iCCR1;
	
    /*
    *Configuration de la fréquence du signal PWM
    */
	iARR = (float) Timer->ARR;
	iCCR1 = iARR * rapport;

	switch (voie){
		case 1:
			Timer->CCR1 = (int) iCCR1;
			break;
		case 2:
			Timer->CCR2 = (int) iCCR1;
			break;
		case 3:
			Timer->CCR3 = (int) iCCR1;
			break;
		case 4:
			Timer->CCR4 = (int) iCCR1;
			break;
	}
}

void Timer_Compare_Count(int arr, TIM_TypeDef *Timer){
	
	EnableTimerClock(Timer);
	
	// on utilise les entrées TI1 et TI2 pour comparer
	Timer->CCMR1 |= TIM_CCMR1_CC1S_0;
	Timer->CCMR1 |= TIM_CCMR1_CC2S_0;
	Timer->CCER &= ~(TIM_CCER_CC1P);
	Timer->CCER &= ~(TIM_CCER_CC2P);
	
		// on efface la zone SMS du registre SMCR pour l'initialiser
	Timer->SMCR &= ~(0x7);
	Timer->SMCR |= 0x3;
	
	// limite du comptage
	// -1 pour le décalage?
	Timer->ARR = arr - 1;
	
	// active le compteur
	Timer->CR1 |= 0x1;
}


/**
* Configure la routine d'interruption d'un Timer
* @param Timer Pointeur vers le jeu de registres (de type TIM_TypeDef ) du
* timer considéré
* @param Priority Niveau de priorité de l'interruption
* @param IT_function Pointeur sur la fonction qui sera exécutée dans le routine
d'interruption
**/
void Timer_Active_IT( TIM_TypeDef *Timer, u8 Priority, void (*IT_function) (void) ){
	
	// Configuration IT
	switch ((int)Timer) {
         case (int)TIM2 :
            NVIC->ISER[0] |= 1<<TIM2_IRQn;
			IT2_function = IT_function;
			NVIC->IP[TIM2_IRQn]=Priority<<4;
					break;
         case (int)TIM3 :
             NVIC->ISER[0] |= 1<<TIM3_IRQn;
			IT3_function = IT_function;
			NVIC->IP[TIM3_IRQn]=Priority<<4;
					break;
         case (int)TIM4 :
            NVIC->ISER[0] |= 1<<TIM4_IRQn;
			IT4_function = IT_function;
			NVIC->IP[TIM4_IRQn]=Priority<<4;
					break;
		}
	Timer->DIER = 1; // Activation de l'interruption de mise à jour
}
