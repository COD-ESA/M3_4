/*configPWM.c*/

/*
 - подать тактирование на порт
 - настроить вывод на альтернативную функцию
 - выход А8 имеет поддержку Т1_CH1

*/
#include "main.h"

void delay(volatile uint32_t t)
{
    while (t--)
    {
        __asm__("nop");
    }
}

void config_Tim1_PWM(void)
{
    /*затактировал порт А*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    (void)RCC->AHB1ENR;
    
    /*push-pull только этот режим корректно работает с ШИМ*/
    GPIOA_CUSTOM->OTYPER &= ~(GPIO_OTYPER_OT8); 
    /*настройка скорости вывода на максимум*/
    GPIOA_CUSTOM->OSPEEDR |= GPIO_OSPEEDR_OSPEED8;

    /*настройка режима PUPDR*/
    GPIOA_CUSTOM->PUPDR &= ~(GPIO_PUPDR_PUPD8_Msk); // no pull

    /*Вкл альтернативный режим*/
    GPIOA_CUSTOM->MODER &= ~(GPIO_MODER_MODER8);
    GPIOA_CUSTOM->MODER |= (GPIO_MODER_MODER8_1);

    GPIOA_CUSTOM->AFRH &= ~(GPIO_AFRH_AFSEL8);
    GPIOA_CUSTOM->AFRH |= GPIO_AFRH_AFSEL8_0;

    /*НАСТРОЙКА ШИМ*/
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // Включаем тактирование модуля TIM1

    // TIM1->PSC = 96 - 1;     // делим до 1 мГц
    // TIM1->ARR = 1000 - 1;   // получаем 1 кГц

    // TIM1->PSC = 192 - 1;     // делим до 500 кГц
    // TIM1->ARR = 1000 - 1;   // получаем 500Гц
    
    // TIM1->PSC = 96 - 1;     // делим до 100 кГц
    // TIM1->ARR = 10000 - 1;   // получаем 100Гц

    TIM1->PSC = 100 - 1;     // делим до 50 кГц
    TIM1->ARR = 19200 - 1;   // получаем 50Гц
    // 3. скважность
    // TIM1->CCR1 = 500;

    // 4. PWM mode
    TIM1->CCMR1 &= ~TIM_CCMR1_OC1M;
    TIM1->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos);
    TIM1->CCMR1 |= TIM_CCMR1_OC1PE;

    // 5. включить канал на выход
    TIM1->CCER |= TIM_CCER_CC1E;

    // 6. главный выход (ОБЯЗАТЕЛЬНО)
    TIM1->BDTR |= TIM_BDTR_MOE;

    // 7. запуск
    TIM1->EGR |= TIM_EGR_UG;
    TIM1->CR1 |= TIM_CR1_CEN;
}

uint16_t table[100];

uint16_t* init_table(void)
{
    for (int i = 0; i < 100; i++)
    {
        float x = (float)i / 100.0f;
        float y = x * x; // простая гамма
        table[i] = (uint16_t)(y * TIM1->ARR);
    }
    return table;
}