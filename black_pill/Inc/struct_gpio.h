#include "core_cm4.h"
#include <stdint.h>

typedef struct
{
    __IO uint32_t MODER;
    __IO uint32_t OTYPER;
    __IO uint32_t OSPEEDR;   
    __IO uint32_t PUPDR;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t LCKR;
    __IO uint32_t AFRL;
    __IO uint32_t AFRH;
} CUSTOM_GPIO_TypeDef;


typedef struct
{
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t SMCR;
    __IO uint32_t DIER;
    __IO uint32_t SR;
    __IO uint32_t EGR;
    __IO uint32_t CCMR1;
    __IO uint32_t CCMR2;
    __IO uint32_t CCER;
    __IO uint32_t CNT;
    __IO uint32_t PSC;
    __IO uint32_t ARR;
    __IO uint32_t RCR;
    __IO uint32_t CCR1;
    __IO uint32_t CCR2;
    __IO uint32_t CCR3;
    __IO uint32_t CCR4;
    __IO uint32_t BDTR;
    __IO uint32_t DCR;
    __IO uint32_t DMAR;
} CUCTOM_TIM1_TypeDef;


/*!< Peripheral memory map */
#define BEGINDING                    (0X40000000UL)                 /*АДРЕС НАЧАЛА МОДУЛЕЙ ПЕРЕФЕРИИ*/
#define APB1_BASE                    (BEGINDING + 0x00000000UL)              /*АДРЕС НАЧАЛА ШИНЫ APB1*/    
#define APB2_BASE                    (BEGINDING + 0x00010000UL)        /*АДРЕС НАЧАЛА ШИНЫ APB2*/

#define BEGINDING_AHB1               (BEGINDING + 0x00020000UL)        /*АДРЕС НАЧАЛА ШИНЫ AHB1*/ 
#define GPIOC_BASE_CUSTOM            (BEGINDING_AHB1 + 0x00000800UL)     /*АДРЕС НАЧАЛА ПОРТА GPIOC*/
#define TIM1_BASE_CUSTOM             (APB2_BASE + 0x00000000UL)        /*АДРЕС НАЧАЛА TIM1*/  

#define GPIOC_CUSTOM                 ((CUSTOM_GPIO_TypeDef *) GPIOC_BASE_CUSTOM)

/*APB2 peripherals*/
#define TIM1_CUSTOM                  ((CUCTOM_TIM1_TypeDef *) TIM1_BASE_CUSTOM)