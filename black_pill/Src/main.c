#include "main.h"


int main(void)
{
	SCB->CPACR |= (0xF << 20);
	/* === Настройка системного тактирования === */
	initClockingmod();
	// InitGPIO_C();
	// initTim3();
	config_clock_and_output_PC();
	// configTim1();
	config_Tim1_PWM();
	uint16_t* table = init_table();

	while (1)
	{

		for (int i = 0; i < 100; i++)
			{
				TIM1->CCR1 = table[i];
				delay(200000);
			}

		for (int i = 99; i >= 0; i--)
			{
				TIM1->CCR1 = table[i];
				delay(200000);
			}
		// for( volatile uint32_t i = 0; i < 1000000; i++){}; // Задержка для видимости мигания
		// TIM1_CUSTOM->EGR |= TIM_EGR_UG; // Генерируем событие обновления для загрузки предделителя и автоперезагрузки	
	}
}


/* === Callback при переполнении таймера === */
void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM1_CUSTOM->SR & TIM_SR_UIF)
	{
		TIM1_CUSTOM->SR &= ~TIM_SR_UIF;
		// toggle_pc13_mod();
	
	}
}

/* === Обработка ошибок === */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
