#include "main.h"


#define LUT_SIZE 128

uint16_t pwm_table[LUT_SIZE];

/*генерируемая таблица значений для PWM - "дыхание"*/
void init_table_for_PWM_DMA(void)
{
    for (int i = 0; i < LUT_SIZE; i++)
    {
        // 0 → 127 → 0 (дыхание)
        int x = (i < LUT_SIZE/2) ? i : (LUT_SIZE - i);

        // квадрат для “плавности”
        uint32_t y = x * x;

        pwm_table[i] = (y * (TIM1->ARR)) / ((LUT_SIZE/2)*(LUT_SIZE/2));
    }
}

void config_Tim1_PWM_DMA(void)
{   
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
        
    DMA2_Stream5->CR &= ~DMA_SxCR_EN;
    while (DMA2_Stream5->CR & DMA_SxCR_EN);

    DMA2_Stream5->CR = 0;

    // канал 6
    DMA2_Stream5->CR |= (6 << DMA_SxCR_CHSEL_Pos);

    // память → периферия
    DMA2_Stream5->CR |= DMA_SxCR_DIR_0;

    // циклический режим
    DMA2_Stream5->CR |= DMA_SxCR_CIRC;

    // инкремент памяти
    DMA2_Stream5->CR |= DMA_SxCR_MINC;

    // размер 16 бит
    DMA2_Stream5->CR |= DMA_SxCR_MSIZE_0;
    DMA2_Stream5->CR |= DMA_SxCR_PSIZE_0;

    // адреса
    DMA2_Stream5->PAR = (uint32_t)&(TIM1->CCR1);// куда кладем данные
    DMA2_Stream5->M0AR = (uint32_t)pwm_table; //откуда берем данные

    // количество элементов
    DMA2_Stream5->NDTR = LUT_SIZE;

    TIM1->DIER |= TIM_DIER_UDE; // Update DMA request

    TIM1->CR1 |= TIM_CR1_ARPE; //Включаю теневой регистр (Shadow Register)

    DMA2_Stream5->CR |= DMA_SxCR_EN; // включаем
}