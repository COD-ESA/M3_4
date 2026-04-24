#include <main.h>


void init_clock_MCU(void)
{
  // SystemInit() is called from startup file before main()
  // Additional clock configuration can be done here if needed
  RCC->CR |= RCC_CR_HSEON; // Enable HSE
    while(!(RCC->CR & RCC_CR_HSERDY)); // Wait until HSE is ready
    // Configure Flash latency and prefetch buffer
    FLASH->ACR |= FLASH_ACR_PRFTBE; // Enable Prefetch Buffer
    FLASH->ACR &= ~FLASH_ACR_LATENCY;   // Clear latency bits
    FLASH->ACR |= FLASH_ACR_LATENCY_2;  // Set 2 wait states for 72 MHz 
    // Configure PLL
    RCC->CFGR &= ~RCC_CFGR_PLLXTPRE; // HSE not divided
    RCC->CFGR |= RCC_CFGR_PLLSRC;    // HSE as PLL source

    RCC->CFGR &= ~RCC_CFGR_PLLMULL;  // Clear PLLMUL bits
    RCC->CFGR |= RCC_CFGR_PLLMULL9;  // PLL input clock x 9 = 72 MHz
    RCC->CR |= RCC_CR_PLLON;         // Enable PLL
    while(!(RCC->CR & RCC_CR_PLLRDY)); // Wait until PLL is ready
    // Configure AHB, APB1, APB2 prescalers 
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1; // AHB = SYSCLK
    RCC->CFGR &= ~RCC_CFGR_PPRE1; // Clear AP
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // APB1 = HCLK/2
    RCC->CFGR &= ~RCC_CFGR_PPRE2; // Clear APB2
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; // APB2
    // Select PLL as system clock source
    RCC->CFGR &= ~RCC_CFGR_SW; // Clear SW bits
    RCC->CFGR |= RCC_CFGR_SW_PLL; // Select PLL as system clock
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); // Wait until PLL is used as system clock
}
