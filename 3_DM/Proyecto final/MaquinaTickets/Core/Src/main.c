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
#define NUM_FILAS 		4
#define NUM_COLUMNAS 	3
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
static void BCD_inicializarCEx (void);
static void BCD_apagarDisplay (void);
static void BCD_encenderDisplay (int);
static void BCD_enviarDigito (int, int, int, int);
static void BCD_enviarLetra (char, int);

static char Matrix_leerTecla(void);
static void Matrix_inicializarFilas(void);

static int char_to_int(char);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

static void Matrix_inicializarFilas (void)
{
	HAL_GPIO_WritePin(FILA2_GPIO_Port, FILA2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(FILA3_GPIO_Port, FILA3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(FILA4_GPIO_Port, FILA4_Pin, GPIO_PIN_RESET);
}

static int char_to_int (char tecla) {
	return ((int)tecla - (int)'0');
}

static char Matrix_leerTecla (void)
{
	/*
	 * Sistema de sondeo (polling) en el que vamos instroduciendo 1 lógico por fila
	 * y leyendo al mismo tiempo cada columna. Según la combinación de fila y columna
	 * se ha pulsado la tecla correspondiente.
	 */

	/* Comenzamos inicializando las columnas a 0 por si guardan algún valor */
	for (int fila = 4; fila > 1; fila--) {
		/* Activamos 1 fila en cada bucle */
		HAL_GPIO_WritePin(FILA4_GPIO_Port, FILA4_Pin, (fila == 4));

		HAL_GPIO_WritePin(FILA3_GPIO_Port, FILA3_Pin, (fila == 3));

		HAL_GPIO_WritePin(FILA2_GPIO_Port, FILA2_Pin, (fila == 2));

		/* Leemos columnas */
		if (fila == 4 && HAL_GPIO_ReadPin(COL1_GPIO_Port, COL1_Pin)) {
			Matrix_inicializarFilas();
			return 'A';
		} else if (fila == 4 && HAL_GPIO_ReadPin(COL2_GPIO_Port, COL2_Pin)) {
			Matrix_inicializarFilas();
			return '7';
		} else if (fila == 4 && HAL_GPIO_ReadPin(COL3_GPIO_Port, COL3_Pin)) {
			Matrix_inicializarFilas();
			return '4';
		} else if (fila == 4 && HAL_GPIO_ReadPin(COL4_GPIO_Port, COL4_Pin)) {
			Matrix_inicializarFilas();
			return '1';
		}

		if (fila == 3 && HAL_GPIO_ReadPin(COL1_GPIO_Port, COL1_Pin)) {
			Matrix_inicializarFilas();
			return '0';
		} else if (fila == 3 && HAL_GPIO_ReadPin(COL2_GPIO_Port, COL2_Pin)) {
			Matrix_inicializarFilas();
			return '8';
		} else if (fila == 3 && HAL_GPIO_ReadPin(COL3_GPIO_Port, COL3_Pin)) {
			Matrix_inicializarFilas();
			return '5';
		} else if (fila == 3 && HAL_GPIO_ReadPin(COL4_GPIO_Port, COL4_Pin)) {
			Matrix_inicializarFilas();
			return '2';
		}
		
		if (fila == 2 && HAL_GPIO_ReadPin(COL1_GPIO_Port, COL1_Pin)) {
			Matrix_inicializarFilas();
			return 'B';
		} else if (fila == 2 && HAL_GPIO_ReadPin(COL2_GPIO_Port, COL2_Pin)) {
			Matrix_inicializarFilas();
			return '9';
		} else if (fila == 2 && HAL_GPIO_ReadPin(COL3_GPIO_Port, COL3_Pin)) {
			Matrix_inicializarFilas();
			return '6';
		} else if (fila == 2 && HAL_GPIO_ReadPin(COL4_GPIO_Port, COL4_Pin)) {
			Matrix_inicializarFilas();
			return '3';
		}
		
		/* Volvemos a la fila inicial */
		if (fila == 2) {
			fila = 5;
		}
	}

	return 0;

}

static void BCD_inicializarCEx (void)
{
	HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CE3_GPIO_Port, CE3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CE4_GPIO_Port, CE4_Pin, GPIO_PIN_SET);

//	HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
//	HAL_Delay(50);
//	HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
//	HAL_Delay(50);
}

/*
 * TURN OFF DISPLAY X FUNCTION
 * display: display a apagar
 */

static void BCD_apagarDisplay (void)
{
	HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SIGNO_GPIO_Port, SIGNO_Pin, GPIO_PIN_RESET);

	/* Habilitamos todos los CEx */
	HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CE3_GPIO_Port, CE3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CE4_GPIO_Port, CE4_Pin, GPIO_PIN_RESET);
	
	/* Volvemos a deshabilitar los CEx */
	HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CE3_GPIO_Port, CE3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CE4_GPIO_Port, CE4_Pin, GPIO_PIN_SET);

	// DA ERROR EN HAL_Delay (??)
//	HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
//	HAL_Delay(50);
//	HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
//	HAL_Delay(50);

}

static void BCD_encenderDisplay (int display)
{
	switch(display) {
	case 1:
		HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_SET);
		break;
	case 2:
		HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_SET);
		break;
	case 3:
		HAL_GPIO_WritePin(CE3_GPIO_Port, CE3_Pin, GPIO_PIN_SET);
		break;
	case 4:
		HAL_GPIO_WritePin(CE4_GPIO_Port, CE4_Pin, GPIO_PIN_SET);
		break;
	default:
		break;
	}
}

/*
 * SEND BYTE ON DISPLAY FUNCTION
 * character: caracter a mostrar
 * display: display en el que mostrar el caracter
 * punto: indica si se enciende o no el punto
 * signo: indica si se enciende o no el signo
 */

static void BCD_enviarDigito (int character, int display, int punto, int signo)
{

	switch (character) {
	case 0:
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 2:
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 3:
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 4:
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 5:
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 6:
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 7:
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 8:
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 9:
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	default:
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	}
	
	if (signo) {
		HAL_GPIO_WritePin(SIGNO_GPIO_Port, SIGNO_Pin, GPIO_PIN_SET);
	}

	if (display == 1) {
		HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_RESET);
		// HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_SET);
		// HAL_GPIO_WritePin(CE3_GPIO_Port, CE3_Pin, GPIO_PIN_SET);
		// HAL_GPIO_WritePin(CE4_GPIO_Port, CE4_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
		HAL_Delay(10);
		
		HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_SET);

//		BCD_apagarDisplay();
//
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
//		HAL_Delay(10);
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
//		HAL_Delay(10);

	} else if (display == 2) {
		// HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_RESET);
		// HAL_GPIO_WritePin(CE3_GPIO_Port, CE3_Pin, GPIO_PIN_SET);
		// HAL_GPIO_WritePin(CE4_GPIO_Port, CE4_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
		HAL_Delay(10);
		
		HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_SET);

//		BCD_apagarDisplay();
//
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
//		HAL_Delay(10);
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
//		HAL_Delay(10);

	} else if (display == 3) {

		if (punto) {
			HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_SET);
		}

		// HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_SET);
		// HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CE3_GPIO_Port, CE3_Pin, GPIO_PIN_RESET);
		// HAL_GPIO_WritePin(CE4_GPIO_Port, CE4_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
		HAL_Delay(10);
		
		HAL_GPIO_WritePin(CE3_GPIO_Port, CE3_Pin, GPIO_PIN_SET);

//		BCD_apagarDisplay();
//
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
//		HAL_Delay(10);
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
//		HAL_Delay(10);

	} else if (display == 4) {
		// HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_SET);
		// HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_SET);
		// HAL_GPIO_WritePin(CE3_GPIO_Port, CE3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CE4_GPIO_Port, CE4_Pin, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
		HAL_Delay(10);
		
		HAL_GPIO_WritePin(CE4_GPIO_Port, CE4_Pin, GPIO_PIN_SET);

//		BCD_apagarDisplay();
//
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
//		HAL_Delay(10);
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
//		HAL_Delay(10);

	}
}

static void BCD_enviarLetra (char character, int display)
{
	switch (character) {
	case'0':
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 'Z':
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 'U':
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 'M':
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 'O':
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 'C':
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 'L':
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 'A':
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 'T':
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 'E':
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	case 'H':
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	default:
		HAL_GPIO_WritePin(DISP0_GPIO_Port, DISP0_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP1_GPIO_Port, DISP1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP2_GPIO_Port, DISP2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP3_GPIO_Port, DISP3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP4_GPIO_Port, DISP4_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP5_GPIO_Port, DISP5_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP6_GPIO_Port, DISP6_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DISP7_GPIO_Port, DISP7_Pin, GPIO_PIN_RESET);
		break;
	}

	if (display == 1) {
		HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_RESET);
		// HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_SET);
		// HAL_GPIO_WritePin(CE3_GPIO_Port, CE3_Pin, GPIO_PIN_SET);
		// HAL_GPIO_WritePin(CE4_GPIO_Port, CE4_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
		HAL_Delay(10);
		
		HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_SET);

//		BCD_apagarDisplay();
//
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
//		HAL_Delay(10);
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
//		HAL_Delay(10);

	} else if (display == 2) {
		// HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_RESET);
		// HAL_GPIO_WritePin(CE3_GPIO_Port, CE3_Pin, GPIO_PIN_SET);
		// HAL_GPIO_WritePin(CE4_GPIO_Port, CE4_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
		HAL_Delay(10);
		
		HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_SET);

//		BCD_apagarDisplay();
//
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
//		HAL_Delay(10);
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
//		HAL_Delay(10);

	} else if (display == 3) {
		// HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_SET);
		// HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CE3_GPIO_Port, CE3_Pin, GPIO_PIN_RESET);
		// HAL_GPIO_WritePin(CE4_GPIO_Port, CE4_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
		HAL_Delay(10);
		
		HAL_GPIO_WritePin(CE3_GPIO_Port, CE3_Pin, GPIO_PIN_SET);

//		BCD_apagarDisplay();
//
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
//		HAL_Delay(10);
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
//		HAL_Delay(10);

	} else if (display == 4) {
		// HAL_GPIO_WritePin(CE1_GPIO_Port, CE1_Pin, GPIO_PIN_SET);
		// HAL_GPIO_WritePin(CE2_GPIO_Port, CE2_Pin, GPIO_PIN_SET);
		// HAL_GPIO_WritePin(CE3_GPIO_Port, CE3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CE4_GPIO_Port, CE4_Pin, GPIO_PIN_RESET);

		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
		HAL_Delay(10);
		
		HAL_GPIO_WritePin(CE4_GPIO_Port, CE4_Pin, GPIO_PIN_SET);

//		BCD_apagarDisplay();
//
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
//		HAL_Delay(10);
//		HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
//		HAL_Delay(10);

	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	BCD_inicializarCEx();
	BCD_apagarDisplay();
	Matrix_inicializarFilas();



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

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  char tecla;
	  int seleccionado = 0;   // Indica si se ha seleccionado producto
	  int introducido = 0;    // Indica si se ha introducido el dinero
	  int parte_entera;   // Parte entera del dinero
	  int parte_decimal;  // Parte decimal del dinero
	  double dinero;
	  double precio;
	  int producto;
	  double prueba = (double)1 + (double)2;
	  double prueba2 = prueba / 2;

	  int fin = 0;   ////////////////////////////////////////////////
	  int signo = 0;
	  double devuelta;
	
	  BCD_enviarLetra('H', 4);
	  BCD_enviarLetra('O', 3);
	  BCD_enviarLetra('L', 2);
	  BCD_enviarLetra('A', 1);
	  
	  HAL_Delay(2000);
	  
	  /* MOSTRAMOS PRODUCTOS */

	  do {

		  /* Primer producto: ZUMO */
		  BCD_enviarLetra('Z', 4);
		  BCD_enviarLetra('U', 3);
		  BCD_enviarLetra('M', 2);
		  BCD_enviarLetra('O', 1);

		  tecla = Matrix_leerTecla();

		  if (tecla == 'B') {
			  seleccionado = 1;
			  producto = 1;
			  break;
		  } else if (tecla == 'A') {
			/* Seguundoo producto: té */
			BCD_enviarLetra('T', 4);
			BCD_enviarLetra('E', 3);
			BCD_enviarLetra('0', 2);
			BCD_enviarLetra('0', 1);
		  }

		  tecla = Matrix_leerTecla();

		  if (tecla == 'B') {
			  seleccionado = 1;
			  producto = 2;
			  break;
		  } else if (tecla == 'A') {
			/* Tercer producto: COLA */
			BCD_enviarLetra('C', 4);
			BCD_enviarLetra('O', 3);
			BCD_enviarLetra('L', 2);
			BCD_enviarLetra('A', 1);
		  }

		  tecla = Matrix_leerTecla();

		  if (tecla == 'B') {
			  seleccionado = 1;
			  producto = 3;
			  break;
		  }

	  } while (!seleccionado);

	  /* MOSTRAMOS PRECIO SEGÚN PRODUCTO SELECCIONADO */
	  /* HACER FUNCIÓN PARA MOSTRAR PERCIO A PARTIR DE SU VARIABLE */
	  if (producto == 1) {
		  /* 0,95 */
		  BCD_enviarDigito(0, 4, 0, 0);
		  BCD_enviarDigito(0, 3, 1, 0);
		  BCD_enviarDigito(9, 2, 0, 0);
		  BCD_enviarDigito(5, 1, 0, 0);

		  devuelta = 0.95;   ////////////////////////////////////////////////////////
	  } else if (producto == 2) {
		  /* 0,80 */
		  BCD_enviarDigito(0, 4, 0, 0);
		  BCD_enviarDigito(0, 3, 1, 0);
		  BCD_enviarDigito(8, 2, 0, 0);
		  BCD_enviarDigito(0, 1, 0, 0);

		  devuelta = 0.80;   ////////////////////////////////////////////////////////
	  } else if (producto == 3) {
		  /* 1,20 */
		  BCD_enviarDigito(0, 4, 0, 0);
		  BCD_enviarDigito(1, 3, 1, 0);
		  BCD_enviarDigito(2, 2, 0, 0);
		  BCD_enviarDigito(0, 1, 0, 0);

		  devuelta = 1.20;   ////////////////////////////////////////////////////////
	  }

	  HAL_Delay(1000);

	  /* ESPERAMOS A INTRODUCIR EL DINERO */
	  do {
		  while (!introducido) {
			  int digitos = 0;   // Nº de dígitos a introducir por el usuario (deben ser 4)

			  tecla = Matrix_leerTecla();
			  parte_entera = char_to_int(tecla) * 10;

			  /* Comprobamos que se ha pulsado un número */
			  if (tecla >= '0' || tecla <= '9') {
				  ++digitos;
			  }

			  HAL_Delay(500);

			  tecla = Matrix_leerTecla();
			  parte_entera += char_to_int(tecla);

			  /* Comprobamos que se ha pulsado un número */
			  if (tecla >= '0' || tecla <= '9') {
				  ++digitos;
			  }

			  HAL_Delay(500);

			  tecla = Matrix_leerTecla();
			  parte_decimal = char_to_int(tecla) * 10;

			  /* Comprobamos que se ha pulsado un número */
			  if (tecla >= '0' || tecla <= '9') {
				  ++digitos;
			  }

			  HAL_Delay(500);

			  tecla = Matrix_leerTecla();
			  parte_decimal += char_to_int(tecla);

			  /* Comprobamos que se ha pulsado un número */
			  if (tecla >= '0' || tecla <= '9') {
				  ++digitos;
			  }

			  HAL_Delay(500);

			  if (digitos == 4) {
				  introducido = 1;
			  }
		  }

		  // Pruebo a mostrar dinero introducido
		  BCD_enviarDigito(parte_entera / 10, 4, 0, 0);
		  BCD_enviarDigito(parte_entera % 10, 3, 1, 0);
		  BCD_enviarDigito(parte_decimal / 10, 2, 0, 0);
		  BCD_enviarDigito(parte_decimal % 10, 1, 0, 0);

		  HAL_Delay(2000);

		  /* Obtenemos el dinero introducido para calcular la devuelta */
		  dinero = (double)parte_entera * 100 + (double)parte_decimal;
		  dinero = dinero / 100;

		  /* MOSTRAMOS DINERO QUE FALTA O DEVUELTA */

		  if (producto == 1) {   // ZUMO
			  devuelta = devuelta - dinero;
		  } else if (producto == 2) {   // TE
			  devuelta = devuelta - dinero;
		  } else if (producto == 3) {   // COLA
			  devuelta = devuelta - dinero;
		  }

		  if (devuelta <= 0) {
			  devuelta *= (-1);   // Quitamos signo
			  signo = 1;
			  fin = 1;
		  } else {
			  introducido = 0;   // Tenemos que volver a introducir el dinero que falta
		  }

		  parte_entera = devuelta;
		  parte_decimal = (devuelta * 100);
		  parte_decimal = parte_decimal % 100;

		  BCD_enviarDigito(parte_entera / 10, 4, 0, signo);
		  BCD_enviarDigito(parte_entera % 10, 3, 1, signo);
		  BCD_enviarDigito(parte_decimal / 10, 2, 0, signo);
		  BCD_enviarDigito(parte_decimal % 10, 1, 0, signo);

	  } while (!fin);

	  HAL_Delay(5000);

	  HAL_GPIO_WritePin(SIGNO_GPIO_Port, SIGNO_Pin, GPIO_PIN_RESET);

	  HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_RESET);
	  HAL_Delay(10);
	  HAL_GPIO_WritePin(DISPCLK_GPIO_Port, DISPCLK_Pin, GPIO_PIN_SET);
	  HAL_Delay(10);

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
  HAL_GPIO_WritePin(GPIOC, DISP5_Pin|DISP4_Pin|FILA3_Pin|FILA2_Pin
                          |CE4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DISP0_Pin|DISP1_Pin|DISP2_Pin|LD2_Pin
                          |SIGNO_Pin|CE2_Pin|CE3_Pin|FILA4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, DISP3_Pin|CE1_Pin|DISP7_Pin|DISP6_Pin
                          |DISPCLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DISP5_Pin DISP4_Pin FILA3_Pin FILA2_Pin
                           CE4_Pin */
  GPIO_InitStruct.Pin = DISP5_Pin|DISP4_Pin|FILA3_Pin|FILA2_Pin
                          |CE4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : DISP0_Pin DISP1_Pin DISP2_Pin LD2_Pin
                           SIGNO_Pin CE2_Pin CE3_Pin FILA4_Pin */
  GPIO_InitStruct.Pin = DISP0_Pin|DISP1_Pin|DISP2_Pin|LD2_Pin
                          |SIGNO_Pin|CE2_Pin|CE3_Pin|FILA4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DISP3_Pin CE1_Pin DISP7_Pin DISP6_Pin
                           DISPCLK_Pin */
  GPIO_InitStruct.Pin = DISP3_Pin|CE1_Pin|DISP7_Pin|DISP6_Pin
                          |DISPCLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : COL3_Pin COL2_Pin */
  GPIO_InitStruct.Pin = COL3_Pin|COL2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : COL4_Pin */
  GPIO_InitStruct.Pin = COL4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(COL4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : COL1_Pin */
  GPIO_InitStruct.Pin = COL1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(COL1_GPIO_Port, &GPIO_InitStruct);

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
