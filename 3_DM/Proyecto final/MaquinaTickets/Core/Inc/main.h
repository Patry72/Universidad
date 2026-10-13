/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

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
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define DISP5_Pin GPIO_PIN_0
#define DISP5_GPIO_Port GPIOC
#define DISP4_Pin GPIO_PIN_1
#define DISP4_GPIO_Port GPIOC
#define DISP0_Pin GPIO_PIN_0
#define DISP0_GPIO_Port GPIOA
#define DISP1_Pin GPIO_PIN_1
#define DISP1_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define DISP2_Pin GPIO_PIN_4
#define DISP2_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define SIGNO_Pin GPIO_PIN_7
#define SIGNO_GPIO_Port GPIOA
#define FILA3_Pin GPIO_PIN_5
#define FILA3_GPIO_Port GPIOC
#define DISP3_Pin GPIO_PIN_0
#define DISP3_GPIO_Port GPIOB
#define COL3_Pin GPIO_PIN_1
#define COL3_GPIO_Port GPIOB
#define COL2_Pin GPIO_PIN_2
#define COL2_GPIO_Port GPIOB
#define CE1_Pin GPIO_PIN_10
#define CE1_GPIO_Port GPIOB
#define FILA2_Pin GPIO_PIN_6
#define FILA2_GPIO_Port GPIOC
#define CE4_Pin GPIO_PIN_7
#define CE4_GPIO_Port GPIOC
#define COL4_Pin GPIO_PIN_8
#define COL4_GPIO_Port GPIOC
#define CE2_Pin GPIO_PIN_8
#define CE2_GPIO_Port GPIOA
#define CE3_Pin GPIO_PIN_9
#define CE3_GPIO_Port GPIOA
#define COL1_Pin GPIO_PIN_11
#define COL1_GPIO_Port GPIOA
#define FILA4_Pin GPIO_PIN_12
#define FILA4_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define DISP7_Pin GPIO_PIN_4
#define DISP7_GPIO_Port GPIOB
#define DISP6_Pin GPIO_PIN_5
#define DISP6_GPIO_Port GPIOB
#define DISPCLK_Pin GPIO_PIN_6
#define DISPCLK_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
