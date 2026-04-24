#include "main.h"


int main(void)
{
	/* === Настройка системного тактирования === */
	initClockingmod();
	// InitGPIO_C();
	// initTim3();
//	HAL_TIM_Base_Start_IT(&htim3);
	config_clock_and_output_PC();
	configTim1();
	while (1)
	{

	}
}


/* === Callback при переполнении таймера === */
void TIM1_UP_TIM10_IRQnHandler(void)
{
	if(TIM1_CUSTOM->SR & TIM_SR_UIF)
	{
		TIM1_CUSTOM->SR &= ~TIM_SR_UIF;
		toggle_pc13_mod();
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
