/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

void LCD_enviarByte(uint8_t comando, uint8_t byte);
void LCD_inicializacion(uint8_t banco, uint8_t entrada_caracteres);
void LCD_ocupado(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  // Configuramos R/W del LCD como solo escritura
  // HAL_GPIO_WritePin (GPIOA, GPIO_PIN_1, GPIO_PIN_SET);

  // Inicializamos el display usando el banco 0 y modo escritura por la dcha
  LCD_inicializacion(SMM, SCMR);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DATSER_GPIO_Port, DATSER_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CHIPSEL_Pin|COMDAT_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DATSER_Pin */
  GPIO_InitStruct.Pin = DATSER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DATSER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CHIPSEL_Pin COMDAT_Pin LD2_Pin */
  GPIO_InitStruct.Pin = CHIPSEL_Pin|COMDAT_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : BUSY_Pin */
  GPIO_InitStruct.Pin = BUSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUSY_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/***********************************************************************
 * comando: será 1 si lo que se va a enviar al display es un comando
 * será 0 si lo que se va a enviar al display es un dato
 * byte: el comando o dato respectivo
 ***********************************************************************/
void LCD_enviarByte(uint8_t comando, uint8_t byte)
{
  // Comprobamos si se le envía un comando o un dato
  if (comando) {
    // Activamos pin para recepción de comando
    HAL_GPIO_WritePin(COMDAT_GPIO_Port, COMDAT_Pin, GPIO_PIN_SET);

    // Enviamos comando
    HAL_GPIO_WritePin(DATSER_GPIO_Port, DATSER_Pin, byte);   // *** DUDA, se pasa el byte directamente?
  } else {
    /* 1. Enviamos dato (Controlar bit a bit (?)).
     * 2. Modificamos puntero de datos.
     * 3. 
    // Activamos pin para recepción de dato
    HAL_GPIO_WritePin(COMDAT_GPIO_Port, COMDAT_Pin, GPIO_PIN_RESET);

    // Enviamos dato
    HAL_GPIO_WritePin(DATSER_GPIO_Port, DATSER_Pin, byte);   // *** DUDA, se pasa el byte directamente?

  }
}

/***********************************************************************
 * banco: sobre el cual se va a escribir por lo general el BANCO0
 * entrada_caracteres: si entran por la izquierda o por la derecha
 * al display. Por lo general comando SCMR.
 * La secuencia de incialización de comandos será:
 * Alimentar Display, SFF, BANCO, DISPLAYON, SCMR
 ***********************************************************************/
void LCD_inicializacion(uint8_t banco, uint8_t entrada_caracteres)
{
	/*******************************************
	 * 1. Alimentamos display.
	 * 2. Configuramos marco de frecuencia.
	 * 3. Usamos banco 0 (arriba).
   * 4. Encendemos display.
   * 5. Habilitamos modo de escrutra SCMR.
   **** RELOJ (?) **** 
	 *******************************************/

  // Ponemos pin CHIPSEL a nivel bajo mientras enviamos comandos
  HAL_GPIO_WritePin(CHIPSEL_GPIO_Port, CHIPSEL_Pin, GPIO_PIN_RESET);
  
  // Alimentamos display (Vcc y Gnd de placa ?)

  // Configuramos marco de frecuencia
  LCD_ocupado();   // Comprobamos si está ocupado
  LCD_enviarByte(COMMAND, SFF);

  // Habilitamos banco 0
  LCD_ocupado();   // Comprobamos si está ocupado
  LCD_enviarByte(COMMAND, banco);

  // Encendemos display
  LCD_ocupado();   // Comprobamos si está ocupado
  LCD_enviarByte(COMMAND, DISP_ON);

  // Habilitamos escritura por la derecha
  LCD_ocupado();   // Comprobamos si está ocupado
  LCD_enviarByte(COMMAND, entrada_caracteres);

  // Deshabilitamos el cursor bajo los caracteres
  LCD_ocupado();   // Comprobamos si está ocupado
  LCD_enviarByte(COMMAND, CLCURS);

  // Ponemos pin CHIPSEL a nivel alto cuando terminamos de enviar comandos
  LCD_ocupado();   // Por si no da tiempo a procesar el último comando
  HAL_GPIO_WritePin(CHIPSEL_GPIO_Port, CHIPSEL_Pin, GPIO_PIN_SET);
}

/***********************************************************************
 * detectar si el display admite comandos o datos comprobando la
 * señal BUSY
 ***********************************************************************/
void LCD_ocupado(void)
{
  while (!HAL_GPIO_ReadPin(BUSY_GPIO_Port, BUSY_Pin));
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
