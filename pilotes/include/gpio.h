#ifndef GPIO_H
#define GPIO_H

#include "stm32f10x.h"

void Port_IO_Init_Ports(char PortA, char PortB, char PortC);
void Port_IO_Init_ADC(GPIO_TypeDef * Port, u8 Broche);
char Port_IO_Init_Output( GPIO_TypeDef * Port, u8 Broche);
void Port_IO_Init_OpenDrain(GPIO_TypeDef * Port, u8 Broche);
void Port_IO_Init_Alt_Output(GPIO_TypeDef * Port, u8 Broche);
char Port_IO_Init_Input( GPIO_TypeDef * Port, u8 Broche);
void Port_IO_Init_InputPull(GPIO_TypeDef * Port, u8 Broche);
void Port_IO_Set(GPIO_TypeDef * Port, u8 Broche);
void Port_IO_Reset(GPIO_TypeDef * Port, u8 Broche);
void Port_IO_Blink(GPIO_TypeDef * Port, u8 Broche);
unsigned int Port_IO_Read(GPIO_TypeDef * Port, u8 Broche);

#endif
