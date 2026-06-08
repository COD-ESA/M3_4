/*Мой самописный файл startup 18.03.26*/
#include "main.h"
#include <stdint.h>

// Определение макроса для запрета прерываний (ассемблерная команда CPSID I)
#define __disable_irq() __asm volatile ("cpsid i" : : : "memory")

extern int main(void);
extern uint32_t _estack, _sidata, _sdata, _edata, _sbss, _ebss;

void Reset_Handler();
void Default_Handler();
void MemManage_Handler();
void TIM1_UP_TIM10_IRQHandler(void);
void __attribute__((weak)) Error_Handler();

void __attribute__((noreturn)) NMI_Handler();
void NMI_Handler()   __attribute__((weak, alias("Default_Handler")));

void __attribute__((noreturn)) HardFault_Handler();
void HardFault_Handler()   __attribute__((weak, alias("Default_Handler")));

void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));//weak-алиас на Default_Handler пока нет реализации, если появится реализация, то она будет использоваться вместо Default_Handler
//И так далее со всеми векторами которые будут использоваться МК

typedef void (*ISR_Handler)(void);

__attribute__((section(".isr_vector"), used))

const ISR_Handler vectors[] = {
    (ISR_Handler)&_estack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    0, // BusFault_Handler
    0, // UsageFault_Handler    
    0,0,0,0, // Reserved
    0, // SVC_Handler
    0, // DebugMon_Handler
    0, // Reserved
    0, // PendSV_Handler
    0, // SysTick_Handler

    /*IRQ0*/
    [TIM1_UP_TIM10_IRQn + 16] = TIM1_UP_TIM10_IRQHandler,
    Error_Handler,
    Default_Handler, 
    Default_Handler, 
    Default_Handler, 
    Default_Handler, 
    Default_Handler, 
    Default_Handler, 
    Default_Handler, 
    Default_Handler, 
    Default_Handler,
    Default_Handler,
    Default_Handler,
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

void __attribute__((naked, noreturn)) Default_Handler(){
    while(1);
}