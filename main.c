/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include <stdbool.h>
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
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Declaration of getVersion function
void getVersion();
// Store camera resolution information
uint8_t rx_res[10];
// Store camera block information
uint8_t rx_blk[20];
// Store camera frame width (316) & height (208) information
int framewidth = 0;
int frameheight = 0;
int frameminarea = 5000;
int framemaxarea = 7000;
int frameminwidth = 125;
int framemaxwidth = 175;
// Declaration of getResolution function
void getResolution(void);
// Declaration of setCameraBrightness function
void setCameraBrightness(int brightness);
// Store block's center based on x-coordinate
int blkcenterX = 0;
// Get block width & height
int blkwidth = 0;
int blkheight = 0;
int blkarea = 0;
// Declaration of getBlocks function
bool getBlocks(int sigmap, int maxBlocks);
// code assigning for 1 header byte, 1 direction bytes, 1 speed byte, 1 checksum
char uart_tx[4] = {0x7A, 0x00, 0x00, 0x00};
// Declaration for sending 4 bytes
void sendBytes(uint8_t dir, uint8_t spd);
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
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  getVersion();
  getResolution();
  //setCameraBrightness(40);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  bool check = getBlocks(1, 1);

	  // Determine speed
	  uint8_t speedbig = 30;
	  uint8_t speedbigger = 35;
	  uint8_t speed = 0;

	  if(check==1) {
		  // object at the center
		  if(blkcenterX > frameminwidth && blkcenterX < framemaxwidth) {
			  // stop condition
			  if(blkarea > frameminarea && blkarea < framemaxarea) {
				  sendBytes(0x09, speed);
			  }
			  // forward condition
			  else if(blkarea <= frameminarea) {
				  // Calculate speed
//				  if(blkarea >= 6000) {
//					  speed = speedsmall;
//				  }
//				  else {
//					  speed = speedbig;
//				  }
				  sendBytes(0x01, speedbig);
			  }
			  // reverse condition
			  else if(blkarea >= framemaxarea) {
				  // Calculate speed
//				  if(blkarea <= 8500) {
//					  speed = speedsmall;
//				  }
//				  else {
//					  speed = speedbig;
//				  }
				  sendBytes(0x02, speedbigger);
			  }
		  }
		  // object on the left
		  else if(blkcenterX <= frameminwidth) {
			  // side left condition
			  if(blkarea > frameminarea && blkarea < framemaxarea) {
				  sendBytes(0x07, speedbig);
			  }
			  // NW condition
			  else if(blkarea <= frameminarea) {
				  // Calculate speed
//				  if(blkarea >= 6000) {
//					  speed = speedsmall;
//				  }
//				  else {
//					  speed = speedbig;
//				  }
				  sendBytes(0x05, speedbig);
			  }
			  // SW condition
			  else if(blkarea >= framemaxarea) {
				  // Calculate speed
//  				  if(blkarea <= 8500) {
//  					  speed = speedsmall;
//  				  }
//  				  else {
//  					  speed = speedbig;
//  				  }
				  sendBytes(0x04, speedbig);
			  }
		  }
		  // object on the right
		  else if(blkcenterX >= framemaxwidth) {
			  // side right condition
			  if(blkarea > frameminarea && blkarea < framemaxarea) {
				  sendBytes(0x08, speedbig);
			  }
			  // NE condition
			  else if(blkarea <= frameminarea) {
				  // Calculate speed
//				  if(blkarea >= 6000) {
//					  speed = speedsmall;
//				  }
//				  else {
//					  speed = speedbig;
//				  }
				  sendBytes(0x03, speedbig);
			  }
			  // SE condition
			  else if(blkarea >= framemaxarea) {
				  // Calculate speed
//				  if(blkarea <= 8500) {
//					  speed = speedsmall;
//				  }
//				  else {
//					  speed = speedbig;
//				  }
				  sendBytes(0x06, speedbig);
			  }
		  }
		  // stop condition
		  else if(blkarea == 0) {
			  sendBytes(0x09, speed);
		  }
	  }
	  // stop condition
	  else {
		  sendBytes(0x09, speed);
	  }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 19200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
/*
 * \brief
 * 		To check connection between camera & cortex.
 *
 * */
void getVersion() {
	uint8_t tx_pixy2ver[] = {0xae, 0xc1, 0x0e, 0x00};
	uint8_t rx_ver[22];
	// Request to Camera/Pixy2
	HAL_UART_Transmit(&huart1, (uint8_t *)tx_pixy2ver, 4, 100);
	// Response from Camera/Pixy2
	HAL_UART_Receive(&huart1, rx_ver, sizeof(rx_ver), 100);

	int checksum, calchcksum = 0;
	checksum = rx_ver[5] * 256 + rx_ver[4];

	for(size_t i = 6; i < sizeof(rx_ver); ++i) {
	  calchcksum += rx_ver[i];
	}

	if(checksum == calchcksum) {

	}
}
/*
 * \brief
 * 		To get the frame size of the camera.
 * */
void getResolution(void) {
	uint8_t tx_pixy2res[] = {0xae, 0xc1, 0x0c, 0x01, 0x7a};
	HAL_UART_Transmit(&huart1, (uint8_t *)tx_pixy2res, 5, 100);
	HAL_UART_Receive(&huart1, rx_res, sizeof(rx_res), 100);

	int checksum, calchcksum = 0;
	checksum = rx_res[5] * 256 + rx_res[4];

	for(size_t i = 6; i < sizeof(rx_res); ++i) {
	  calchcksum += rx_res[i];
	}

	if(checksum == calchcksum) {
	  // Get camera frame width (316) & height (208)
	  framewidth = rx_res[7] * 256 + rx_res[6];
	  frameheight = rx_res[9] * 256 + rx_res[8];
	}
}
/*
 * \brief
 * 		To set the camera brightness.
 *
 * \param brightness
 * 		The camera brightness. Value can be between 0 to 255.
 *
 * */
void setCameraBrightness(int brightness) {
	uint8_t tx_pixy2set[] = {0xae, 0xc1, 0x10, 0x01, brightness};
	uint8_t rx_set[10];
	HAL_UART_Transmit(&huart1, (uint8_t *)tx_pixy2set, 5, 100);
	HAL_UART_Receive(&huart1, rx_set, sizeof(rx_set), 100);

	int checksum, calchcksum = 0;
	checksum = rx_set[5] * 256 + rx_set[4];

	for(size_t i = 6; i < sizeof(rx_set); ++i) {
	  calchcksum += rx_set[i];
	}

	if(checksum == calchcksum) {

	}
}
/*
 * \brief
 * 		To retrieve information on the signature detected by the camera.
 *
 * \param sigmap
 * 		The specified signature to receive data from.
 *
 * \param maxBlocks
 * 		The restriction on the number of blocks to receive data from.
 * */
bool getBlocks(int sigmap, int maxBlocks) {
	uint8_t tx_pixy2blk[] = {0xae, 0xc1, 0x20, 0x02, sigmap, maxBlocks};
	HAL_UART_Transmit(&huart1, (uint8_t *)tx_pixy2blk, 6, 100);
	HAL_UART_Receive(&huart1, rx_blk, sizeof(rx_blk), 100);

	int checksum, calchcksum = 0;
	checksum = rx_blk[5] * 256 + rx_blk[4];

	for(size_t i = 6; i < sizeof(rx_blk); ++i) {
	  calchcksum += rx_blk[i];
	}

	if(checksum == calchcksum) {
		// Get block's center based on x-coordinate
		blkcenterX = rx_blk[9] * 256 + rx_blk[8];

		// Get block width & height
		blkwidth = rx_blk[13] * 256 + rx_blk[12];
		blkheight = rx_blk[15] * 256 + rx_blk[14];
		blkarea = blkwidth * blkheight;
		return 1;
	}
	return 0;
}
/*
 * \brief
 * 		To transmit 4 bytes to P1 on the directions and speed
 *
 * \param dir
 * 		The specified direction.
 *
 * \param spd
 * 		The specified speed.
 *
 * */
void sendBytes(uint8_t dir, uint8_t spd) {
	uart_tx[1] = dir;
	uart_tx[2] = spd;
	uart_tx[3] = (uart_tx[1] ^ uart_tx[2]) ^ 0x7F;

	/* testing purpose
	char uart_buf[50];
	int uart_buf_len;
	uart_buf_len = sprintf(uart_buf, "Header: %d\r\n", uart_tx[0]);
	HAL_UART_Transmit(&huart2, &uart_buf, uart_buf_len, 100);

	uart_buf_len = sprintf(uart_buf, "Direction: %d\r\n", uart_tx[1]);
	HAL_UART_Transmit(&huart2, &uart_buf, uart_buf_len, 100);

	uart_buf_len = sprintf(uart_buf, "Speed: %d\r\n", uart_tx[2]);
	HAL_UART_Transmit(&huart2, &uart_buf, uart_buf_len, 100);

	uart_buf_len = sprintf(uart_buf, "Checksum: %d\r\n", uart_tx[3]);
	HAL_UART_Transmit(&huart2, &uart_buf, uart_buf_len, 100);
	*/

	HAL_UART_Transmit(&huart3, &uart_tx[0], 1, 100);
	HAL_UART_Transmit(&huart3, &uart_tx[1], 1, 100);
	HAL_UART_Transmit(&huart3, &uart_tx[2], 1, 100);
	HAL_UART_Transmit(&huart3, &uart_tx[3], 1, 100);
	HAL_Delay(100);
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

