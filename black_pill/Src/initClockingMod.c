/*
 * initClockingMod.c
 *
 *  Created on: 19 окт. 2025 г.
 *      Author: esaul
 */

#include "main.h"

void initClockingmod(void){

	uint32_t volatile timeout;
	/* 1) enable PWR clock and set VOS = scale1, wait VOSRDY */
    // RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    // (void)RCC->APB1ENR;

    /* set VOS = 1 (Scale1) in PWR_CR (bit VOS) */
    PWR->CR |= PWR_CR_VOS;
    /* wait VOSRDY (PWR->CSR VOSRDY bit) */
    timeout = 10000;
    while (((PWR->CSR & PWR_CSR_VOSRDY) == 0) && --timeout) { __NOP(); }
	
	/*===Запуск тактирования от HSE кварца===*/
	RCC->CR &= ~RCC_CR_HSEBYP;
	RCC->CR |= RCC_CR_HSEON; //включаю тактирование от HSE (основной кварц)
	while(!(RCC->CR & RCC_CR_HSERDY)); // жду стабилизирования кварца после запуска

	/*===Запуск тактирования Flash===*/
	InitFlash();

	/*===Настроить PLL===*/
	RCC->CR &= ~RCC_CR_PLLON;//выключаю PLL и жду его выключения, иначе в PLLCFGR нельзя записать новых значений
	while (RCC->CR & RCC_CR_PLLRDY);

	// Очищаем все значимые поля перед записью
	RCC->PLLCFGR &= ~(RCC_PLLCFGR_PLLM | RCC_PLLCFGR_PLLN | RCC_PLLCFGR_PLLP | RCC_PLLCFGR_PLLQ | RCC_PLLCFGR_PLLSRC);

	/*===HSE = 25 MHz=== (смотри частоту кварца на плате)*/
	/*===SYSCLK = 96 MHz===*/
	RCC->PLLCFGR |= (
		RCC_PLLCFGR_PLLSRC_HSE 			|
		(25U << RCC_PLLCFGR_PLLM_Pos) 	| // Вход 1 MHz
		(384U << RCC_PLLCFGR_PLLN_Pos)  | // VCO 384 MHz (намного стабильнее)
		(1U << RCC_PLLCFGR_PLLP_Pos)    | // PLLP = 4 (384 / 4 = 96 MHz)
		(8U << RCC_PLLCFGR_PLLQ_Pos)	  // PLLQ = 8 (384 / 8 = 48 MHz для USB)
	);
			
	//AHB system clock not divided (prescaler) - HCLK будет тактироваться той же частотой с AHB
	//HCLK будет тактироваться частотой AHB = 96MHz
	//APB1 48MHz (НЕ БОЛЕЕ 50МГц)

	/* set prescalers: AHB=1, APB1=2, APB2=1 */
	RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PPRE2_DIV1;		
	/*===Включаю PLL===*/
	RCC->CR |= RCC_CR_PLLON;//
	while(!(RCC->CR & RCC_CR_PLLRDY));

	/*===Перевожу систему на тактирование от PLL===*/
	RCC->CFGR &= ~RCC_CFGR_SW;//
	RCC->CFGR |= RCC_CFGR_SW_PLL; // ШИНА AHB ТАКТИРОВАТЬСЯ БУДЕТ ОТ PLL 96MHz
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN; //ВКЛ КЛОК USB OTG FS
	timeout = 1000000;
	while(--timeout){ __NOP();};
	RCC->AHB2RSTR |= RCC_AHB2RSTR_OTGFSRST; // сделать reset USB
	timeout = 1000000;
	while(--timeout){ __NOP();};
	RCC->AHB2RSTR &= ~RCC_AHB2RSTR_OTGFSRST;// ОТКЛЮЧИТЬ reset USB
   /*=== ДИНАМИЧЕСКАЯ ПРОВЕРКА ===*/

//    uint32_t sysclk = SystemCoreClock; // SYSCLK в Hz
//    uint32_t usbclk = sysclk / ((RCC->PLLCFGR & RCC_PLLCFGR_PLLQ) >> RCC_PLLCFGR_PLLQ_Pos);

//    // Проверяем SYSCLK и USB
//    if(sysclk != 96000000U){
//        // ошибка SYSCLK
//        while(1);
//    }

//    if(usbclk != 48000000U){
//        // ошибка USB
//        while(1);
//    }
}

