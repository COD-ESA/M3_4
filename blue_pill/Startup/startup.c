/*Мой самописный файл startup 18.03.26*/
#include "main.h"
#include <stdint.h>

// Определение макроса для запрета прерываний (ассемблерная команда CPSID I)
#define __disable_irq() __asm volatile ("cpsid i" : : : "memory")

extern int main(void);
extern uint32_t _estack, _sidata, _sdata, _edata, _sbss, _ebss;

void Reset_Handler();
void Default_Handler();
void __attribute__((weak)) Error_Handler();

void __attribute__((noreturn)) NMI_Handler();
void NMI_Handler()   __attribute__((weak, alias("Default_Handler")));

void __attribute__((noreturn)) HardFault_Handler();
void HardFault_Handler()   __attribute__((weak, alias("Default_Handler")));
//И так далее со всеми векторами которые будут использоваться МК

typedef void (*ISR_Handler)(void);

__attribute__((section(".isr_vector"), used))
const ISR_Handler vectors[] = {
    (ISR_Handler)&_estack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    Error_Handler,
};


void Reset_Handler(void)
{
    uint32_t *pSource = &_sidata;
    uint32_t *pDest   = &_sdata;

    while (pDest < &_edata)
        *pDest++ = *pSource++;

    pDest = &_sbss;
    while (pDest < &_ebss)
        *pDest++ = 0;

    main();

    while (1);
}

void Error_Handler(void)
{
    __disable_irq();

    /* Мигалка для визуальной диагностики (упрощенно) */
    while (1)
    {
        //затактировать APB2 для GPIOC
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

        //НАСТРОЙКА ПИНА С13 НА ВЫХОД
        GPIOC->CRH &= ~(GPIO_CRH_CNF13_Msk | GPIO_CRH_MODE13_Msk);
        GPIOC->CRH |= GPIO_CRH_MODE13_0;// Push-Pull

        //Включаю свет-диод
        if(GPIOC->ODR & (1 << 13)) {
            GPIOC->BRR = (1 << 13); // Сбросить пин PC13
        } else {
            GPIOC->BSRR = (1 << (13 + 16)); // Установить пин PC13
        }

        GPIOC->BSRR = (1<<(13+16));
        // Инвертируем состояние пина PC13
        // Здесь должен быть код обращения к регистрам GPIO напрямую или через HAL
        // GPIOC->ODR ^= (1 << 13); 
        
        for (volatile int i = 0; i < 100000; i++); // Грубая задержка
    }
}

void __attribute__((naked, noreturn)) Default_Handler(){
    while(1);
}