/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED3_Pin GPIO_PIN_13
#define LED3_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_15
#define LED1_GPIO_Port GPIOC
#define ADC_UIN_Pin GPIO_PIN_0
#define ADC_UIN_GPIO_Port GPIOA
#define ADC_UOUT_Pin GPIO_PIN_1
#define ADC_UOUT_GPIO_Port GPIOA
#define ADC_IIN_Pin GPIO_PIN_2
#define ADC_IIN_GPIO_Port GPIOA
#define ADC_IOUT_Pin GPIO_PIN_3
#define ADC_IOUT_GPIO_Port GPIOA
#define KEY1_ADD_Pin GPIO_PIN_0
#define KEY1_ADD_GPIO_Port GPIOB
#define KEY1_ADD_EXTI_IRQn EXTI0_IRQn
#define KEY2_SUB_Pin GPIO_PIN_1
#define KEY2_SUB_GPIO_Port GPIOB
#define KEY2_SUB_EXTI_IRQn EXTI1_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
