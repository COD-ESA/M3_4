/*configTIM1.c*/

#include "main.h"

void configTim1(void)
{   
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // Включаем тактирование TIM1

    TIM1_CUSTOM->CR1 |= TIM_CR1_DIR; // Счёт вниз
    // TIM1_CUSTOM->CR1 &= ~TIM_CR1_DIR; // Счёт вверх

    /*частота настроена на 96МГц*/
    TIM1_CUSTOM->PSC = 9600 - 1; // Предделитель для получения 10 кГц (96 МГц / 9600 = 10 кГц)
    TIM1_CUSTOM->ARR = 10000 - 1; // Автоперезагрузка для получения 1 Гц (10 кГц / 10000 = 1 Гц)
    TIM1_CUSTOM->EGR |= TIM_EGR_UG; // Генерируем событие обновления для загрузки предделителя и автоперезагрузки
    TIM1_CUSTOM->DIER |= TIM_DIER_UIE; // Разрешаем прерывание по обновлению (переполнению)

    TIM1_CUSTOM->SR &= ~TIM_SR_UIF; // Сбрасываем флаг обновления, чтобы избежать немедленного срабатывания прерывания

    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn); // Разрешаем прерывание от TIM1 Update и TIM10
    TIM1_CUSTOM->CR1 |= TIM_CR1_CEN; // Включаем таймер
}