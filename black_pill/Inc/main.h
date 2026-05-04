/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H


#include  "stm32f411xe.h"
#include  "struct_gpio.h"
#include "stdio.h"


// void InitGPIO_C(void);
// void gpio_toggle_pin(void);
// void initTim3(void);
void InitFlash(void);
// int initClocking_Debug(void);
void initClockingmod(void);
void config_clock_and_output_PC(void);
void toglle_pc13(void);
void toggle_pc13_mod(void);
void configTim1(void);
void config_Tim1_PWM(void);
uint16_t* init_table(void);
void delay(volatile uint32_t t);


#ifdef __cplusplus
extern "C" {
#endif

void Error_Handler(void);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
