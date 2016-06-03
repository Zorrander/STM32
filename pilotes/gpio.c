#include "gpio.h"

// va alimenter chaque port spécifié en fréquences
void Port_IO_Init_Ports(char PortA, char PortB, char PortC)
{
	if (PortA == 1)
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPAEN;
	if (PortB == 1)
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPBEN;
	if (PortC == 1)
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPCEN;
}

// analog input
void Port_IO_Init_ADC(GPIO_TypeDef * Port, u8 Broche){
	if (Port == GPIOA)
			(RCC->APB2ENR)|= RCC_APB2ENR_IOPAEN;
	else if (Port == GPIOB)
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPBEN;
	else if (Port == GPIOC)
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPCEN;
	if(Broche > 7){
		// efface la zone concernée
		Port->CRH = Port->CRH & ~(0xF << (Broche - 8) * 4);
		// met la valeur desirée
		Port->CRH = (Port->CRH |(0x00 << ((Broche-8)*4)));
	}
	else{
		// efface la zone concernée
		Port->CRL = Port->CRL & ~(0xF << Broche * 4);
		// met la valeur désirée
		Port->CRL = (Port->CRL | (0x00 << (Broche*4)));
	}
}

// output push-pull
char Port_IO_Init_Output( GPIO_TypeDef * Port, u8 Broche){
	if (Port == GPIOA)
			(RCC->APB2ENR)|= RCC_APB2ENR_IOPAEN;
	else if (Port == GPIOB)
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPBEN;
	else if (Port == GPIOC)
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPCEN;
	
	if (Broche > 7){
		// efface la zone concernée
		Port->CRH = Port->CRH & ~(0xF << (Broche - 8) * 4);
		// inscrit les nouvelles valeurs
		Port->CRH = Port->CRH |(0x1 << (Broche - 8) * 4);
	}else{			
		// efface la zone concernée
		Port->CRL = Port->CRL & ~(0xF << Broche * 4);
		// inscrit les nouvelles valeurs
		Port->CRL = Port->CRL |(0x1 << Broche * 4);
	}
	return 0;
}

void Port_IO_Init_OpenDrain(GPIO_TypeDef *Port, u8 Broche){
	if (Port == GPIOA)
			(RCC->APB2ENR)|= RCC_APB2ENR_IOPAEN;
	else if (Port == GPIOB)
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPBEN;
	else if (Port == GPIOC)
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPCEN;
	
	if (Broche > 7){
		// efface la zone concernée
		Port->CRH = Port->CRH & ~(0xF << (Broche - 8) * 4);
		// inscrit les nouvelles valeurs
		Port->CRH = Port->CRH |(0x5 << (Broche - 8) * 4);
	}else{			
		// efface la zone concernée
		Port->CRL = Port->CRL & ~(0xF << Broche * 4);
		// inscrit les nouvelles valeurs
		Port->CRL = Port->CRL |(0x5 << Broche * 4);
	}
}

// input pull up/down
void Port_IO_Init_InputPull(GPIO_TypeDef * Port, u8 Broche){
	if (Port == GPIOA)
			(RCC->APB2ENR)|= RCC_APB2ENR_IOPAEN;
	else if (Port == GPIOB)
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPBEN;
	else if (Port == GPIOC)
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPCEN;
	
	if (Broche > 7){
		// efface la zone concernée
		Port->CRH = Port->CRH & ~(0xF << (Broche - 8) * 4);
		// inscrit les nouvelles valeurs
		Port->CRH = Port->CRH |(0x8 << (Broche - 8) * 4);
	}else{			
		// efface la zone concernée
		Port->CRL = Port->CRL & ~(0xF << Broche * 4);
		// inscrit les nouvelles valeurs
		Port->CRL = Port->CRL |(0x8 << Broche * 4);
	}
}

// floating input
char Port_IO_Init_Input( GPIO_TypeDef * Port, u8 Broche){
	if (Port == GPIOA)
			(RCC->APB2ENR)|= RCC_APB2ENR_IOPAEN;
	else if (Port == GPIOB)
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPBEN;
	else if (Port == GPIOC)
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPCEN;
	
	if (Broche > 7){
		// efface la zone concernée
		Port->CRH = Port->CRH & ~(0xF << (Broche - 8) * 4);
		// inscrit les nouvelles valeurs
		Port->CRH = Port->CRH |(0x4 << (Broche - 8) * 4);
	}else{			
		// efface la zone concernée
		Port->CRL = Port->CRL & ~(0xF << Broche * 4);
		// inscrit les nouvelles valeurs
		Port->CRL = Port->CRL |(0x4 << Broche * 4);
	}
	return 0;
}

void Port_IO_Init_Alt_Output(GPIO_TypeDef *Port, u8 Broche){
	if (Port == GPIOA)
			(RCC->APB2ENR)|= RCC_APB2ENR_IOPAEN;
	else if (Port == GPIOB)
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPBEN;
	else if (Port == GPIOC)
		(RCC->APB2ENR)|= RCC_APB2ENR_IOPCEN;
	
	if (Broche > 7){
		// efface la zone concernée
		Port->CRH = Port->CRH & ~(0xF << (Broche - 8) * 4);
		// inscrit les nouvelles valeurs
		Port->CRH = Port->CRH |(0x9 << (Broche - 8) * 4);
	}else{			
		// efface la zone concernée
		Port->CRL = Port->CRL & ~(0xF << Broche * 4);
		// inscrit les nouvelles valeurs
		Port->CRL = Port->CRL |(0x9 << Broche * 4);
	}
}

void Port_IO_Set(GPIO_TypeDef * Port, u8 Broche){
	Port->BSRR = 0x1 << Broche;
}

void Port_IO_Reset(GPIO_TypeDef * Port, u8 Broche){
	Port->BSRR = 0x1 << (Broche + 16);
}

void Port_IO_Blink(GPIO_TypeDef * Port, u8 Broche){
	Port->ODR = Port->ODR ^ (0x1 << Broche);
}

unsigned int Port_IO_Read(GPIO_TypeDef * Port, u8 Broche){
	return (0x1 << Broche) & Port->IDR;
}
