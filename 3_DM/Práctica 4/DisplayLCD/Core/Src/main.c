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

void LCD_enviarByte (uint8_t comando, uint8_t byte);
void LCD_inicializacion (int banco, int entrada_caracteres);
void LCD_ocupado (void);
void LCD_displayOFF (int mode);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/***********************************************************************
 * mode: será 0 si queremos apagarlo y 1 si queremos encenderlo
 ***********************************************************************/
void LCD_display_ON_OFF (int mode)
{
  if (mode == 0) {
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_SET);
  } else if (mode == 1) {
    HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);
  }
}

/***********************************************************************
 * comando: será 1 si lo que se va a enviar al display es un comando
 * será 0 si lo que se va a enviar al display es un dato
 * byte: el comando o dato respectivo
 ***********************************************************************/
void LCD_enviarByte(uint8_t comando, uint8_t byte)
{
  // Ponemos pin CHIPSEL a nivel bajo mientras enviamos comandos
	HAL_GPIO_WritePin(CHIPSEL_GPIO_Port, CHIPSEL_Pin, GPIO_PIN_RESET);

	// IMPORTANTE !! comprobar si está ocupado antes de enviar
	LCD_ocupado();

	// Comprobamos si se le envía comando o dato
	if (comando) {
		// Activamos pin para recepción de comando
		HAL_GPIO_WritePin(COMDAT_GPIO_Port, COMDAT_Pin, GPIO_PIN_SET);

	} else {
		// Activamos pin para recepción de dato
		HAL_GPIO_WritePin(COMDAT_GPIO_Port, COMDAT_Pin, GPIO_PIN_RESET);
	}

	// Enviamos comando o dato (bit a bit)

	for (int i = 1; i <= 8; i++) {
		// Aplicamos máscara para quedarnos con el msb


		if (byte & 0x80) {   // bit a 1
			HAL_GPIO_WritePin(DATSER_GPIO_Port, DATSER_Pin, GPIO_PIN_SET);
		} else {   // bit a 0
			HAL_GPIO_WritePin(DATSER_GPIO_Port, DATSER_Pin, GPIO_PIN_RESET);
		}

		HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(SCK_GPIO_Port, SCK_Pin, GPIO_PIN_SET);
		HAL_Delay(10);

		byte <<= 1;
	}

	// Ponemos pin CHIPSEL a nivel alto al finalizar el envío de comandos
	HAL_GPIO_WritePin(CHIPSEL_GPIO_Port, CHIPSEL_Pin, GPIO_PIN_SET);
}

/***********************************************************************
 * banco: sobre el cual se va a escribir por lo general el BANCO0
 * entrada_caracteres: si entran por la izquierda (0) o por la derecha (1)
 * al display. Por lo general comando SCMR.
 * La secuencia de incialización de comandos será:
 * Alimentar Display, SFF, BANCO, DISPLAYON, SCMR
 ***********************************************************************/
void LCD_inicializacion(int banco, int entrada_caracteres)
{
  //Configuramos marco de frecuencia
  LCD_enviarByte(COMMAND, SFF);

  // Habilitamos banco: 0 -> banco 0;  1 -> banco 1;  2 -> bancos 0 y 1 (Todos en SYNC OUTPUT)
  // *** DUDA: usar banco0 = 0x1A y banco1= 0x1B ó ambos=0x1F *** //
  if (banco == 0) {
    LCD_enviarByte(COMMAND, BANCO0);
  }

  if (banco == 1) {
    LCD_enviarByte(COMMAND, BANCO1);
  }

  if (banco == 2) {
    LCD_enviarByte(COMMAND, BANCO_0_1);
  }

  // Encendemos display
  LCD_enviarByte(COMMAND, DISP_ON);

  LCD_enviarByte(COMMAND, SANDM);

  // Habilitamos escritura por la derecha
  if (entrada_caracteres == 0) {
    LCD_enviarByte(COMMAND, SCML);
  }

  if (entrada_caracteres == 1) {
    LCD_enviarByte(COMMAND, SCMR);
  }
}

/***********************************************************************
 * detectar si el display admite comandos o datos comprobando la
 * señal BUSY
 ***********************************************************************/
void LCD_ocupado(void)
{
	while (!HAL_GPIO_ReadPin(BUSY_GPIO_Port, BUSY_Pin));   // GPIO_PIN_1 = BUSY_Pin
}

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

  LCD_display_ON_OFF(0);
  HAL_Delay(3000);
  LCD_display_ON_OFF(1);

  LCD_inicializacion(2, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

	  // Enviamos cadena como dato (DATA)
	  HAL_Delay(10);
	  LCD_enviarByte(DATA, 'A');
	  HAL_Delay(10);
	  LCD_enviarByte(DATA, 'L');
	  HAL_Delay(10);
	  LCD_enviarByte(DATA, 'O');
	  HAL_Delay(10);
	  LCD_enviarByte(DATA, 'H');

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
  HAL_GPIO_WritePin(GPIOC, DATSER_Pin|SCK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CHIPSEL_Pin|COMDAT_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RESET_GPIO_Port, RESET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DATSER_Pin SCK_Pin */
  GPIO_InitStruct.Pin = DATSER_Pin|SCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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

  /*Configure GPIO pin : RESET_Pin */
  GPIO_InitStruct.Pin = RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RESET_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
