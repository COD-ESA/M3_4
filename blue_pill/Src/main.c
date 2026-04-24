/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

uint32_t array_1[] = {1,2,3,4,5};

int main(void)
{

  init_clock_MCU();

  led13_on();

  while (1)
  {
    for (volatile int i = 0; i < 1000000; i++){__asm__ volatile ("nop");}; // Грубая задержка

    if (GPIOC->ODR & (1 << 13)) {
        GPIOC->BSRR = (1 << (13 + 16)); // RESET (выключить)
    } else {
        GPIOC->BSRR = (1 << 13);        // SET (включить)
    }
  }

}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }

}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif /* USE_FULL_ASSERT */
