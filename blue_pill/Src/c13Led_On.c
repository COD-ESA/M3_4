/*
 * c13Led_On.c
 *
 *  Created on: Nov 25, 2025
 *      Author: esaul
 */

#include "main.h"

void led13_on(void)
{
	//затактировать APB2 для GPIOC
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	//НАСТРОЙКА ПИНА С13 НА ВЫХОД
	GPIOC->CRH &= ~(GPIO_CRH_CNF13_Msk | GPIO_CRH_MODE13_Msk);
	GPIOC->CRH |= GPIO_CRH_MODE13_0;// Push-Pull

	//Включаю свет-диод
	GPIOC->BSRR = (1<<(13+16));
}

