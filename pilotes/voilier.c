#include "stm32f10x.h"
#include "voilier.h"
#include "girouette.h"
#include "timer.h"
#include "piloteRF.h"

#define EXTI9_5_INTERRUPT_PRIORITY 6

void EXTI9_5_IRQHandler(){
	if ((EXTI->PR & (1 << 5))>>5){
		EXTI9_5_GirouetteHandler();
	}
		
	if ((EXTI->PR & (1 << PIN_RF))>>PIN_RF){
		EXTI9_5_RFHandler();
	}
}

void Config_NVIC(){
	// Configuration IT
  NVIC->ISER[0] |= 1 << EXTI9_5_IRQn;
	NVIC->IP[EXTI9_5_IRQn] = EXTI9_5_INTERRUPT_PRIORITY<<4;	
}

void initEverything(){
	/* Girouette */
	initGirouette();
	RF_Init();
	
	Config_EXTI_RF();
	Config_EXTI_Girouette();
	Config_NVIC();
}
