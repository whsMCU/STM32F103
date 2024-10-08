/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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


void hwInit(void);
void SystemClock_Config(void);

can_msg_t msg;

uint32_t pre_time = 0;

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  hwInit();
  pre_time = micros();
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(micros()-pre_time >= 500000)
	  {
	    pre_time = micros();
		  gpioPinToggle(LED);
		  canMsgWrite(_DEF_CAN1, &msg);
	  }

//    if (uartAvailable(_DEF_UART1) > 0)
//    {
//      uint8_t rx_data;
//      rx_data = uartRead(_DEF_UART1);
//
//      uartPrintf(_DEF_UART1, "Rx : 0x%X\r\n", rx_data);
//    }
    cliMain();
  }
  /* USER CODE END 3 */
}

void hwInit(void)
{
  #ifdef _USE_HW_RTC
    rtcInit();
  #endif
//  usbInit();
  cliInit();
//  logInit();
//  logOpen(HW_LOG_CH, 115200);
//  logPrintf("\r\n[ Firmware Begin... ]\r\n");

  tim_Init();
  tim_Begin(_DEF_TIM3);
  gpioInit();
  uartInit();

  canInit();
  canOpen(_DEF_CAN1, CAN_NORMAL, CAN_500K);

  msg.id_type = CAN_STD;
  msg.dlc     = CAN_DLC_2;
  msg.id      = 0x321;
  msg.length  = 2;
  msg.data[0] = 1;
  msg.data[1] = 2;

  canMsgWrite(_DEF_CAN1, &msg);

  cliOpen(_DEF_UART1, 115200);
  lcdInit();

  ssd1306_SetCursor(0, 0);
  ssd1306_WriteString("Hello", Font_11x18, White);
  ssd1306_UpdateScreen();
  HAL_Delay(1000);

  ssd1306_SetCursor(64, 0);
  ssd1306_WriteString("World", Font_11x18, White);
  ssd1306_UpdateScreen();
  HAL_Delay(1000);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  //RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
