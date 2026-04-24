/*
 * initFlash.c
 *
 *  Created on: 18 окт. 2025 г.
 *      Author: esaul
 */

#include "main.h"

/*=== init flash===*/

void InitFlash(void){
	FLASH->ACR |= FLASH_ACR_PRFTEN; //Позволяет флешу заранее подгружать следующие слова
	FLASH->ACR |= FLASH_ACR_ICEN; //Включает кеш инструкций (чтение команд из Flash)
	FLASH->ACR |= FLASH_ACR_DCEN;//Включает кеширование данных (операции с переменными, массивами в Flash)
	/*===Запуск тактирования Flash===*/
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	FLASH->ACR |= FLASH_ACR_LATENCY_3WS;// в зависимости от частоты MCU
    // 3. Проверим, применилось ли значение (опционально)
    while ((FLASH->ACR & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_3WS);
}

