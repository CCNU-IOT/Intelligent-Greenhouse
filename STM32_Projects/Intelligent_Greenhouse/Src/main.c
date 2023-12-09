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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../Inc/adc.h"
#include "uart.h"
#include "key.h"
#include "relay.h"
#include "tim.h"
#include "spi.h"
#include "rgb.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
    // #define __WS2812__
    // #define __KEY_RELAY__
    // #define __AUTO_RELAY__
    // #define __UART_SENSER__
     #define __NEW_SENSER__
    // #define __PWM__
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

  /* USER CODE BEGIN 2 */
#ifdef __WS2812__
  /*------------------基于spi实现的ws2812灯带变色-------------------*/
    HAL_SPI_MspInit(&spi_rgb_strip_light_handle);
    spi_rgb_strip_light_init();
  /*------------------基于spi实现的ws2812灯带变色-------------------*/
#endif /*__WS2812__*/

#ifdef __KEY_RELAY__
  /*------------------按键控制水泵-------------------*/
  /*------按下KEY_UP开启水泵，按下其余按钮关闭水泵-----*/
    key_init_it();	
    Relay_Init();
  /*------------------按键控制水泵-------------------*/
#endif /*__KEY_RELAY__*/

#ifdef __PWM__
  /*------------------输出PWM波（由PE9输出，可接在示波器上观察）-------------------*/
  // 初始化定时器，设置PWM占空比为50%（可根据需要修改）
    WS2812B_pwm_tim_init(50);
  /*------------------输出PWM波（由PE9输出，可接在示波器上观察）-------------------*/
#endif /*__PWM__*/

#ifdef __NEW_SENSER__
  float humidity; // 土壤湿度值，用于接受ADC数据
  ADC_Init();
#endif /*__NEW_SENSER__*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    MX_GPIO_Init();
    //HAL_Delay(1000);
    /* USER CODE END WHILE */
    /* USER CODE BEGIN 3 */

#ifdef __AUTO_RELAY__
	  // Relay_On();
	  // HAL_Delay(1000);
    // Relay_Off();
    // HAL_Delay(1000);
#endif /*__AUTO_RELAY__*/

#ifdef __UART_SENSER__
    //   // 土壤湿度值
		// if(sensor_read())
		// 	printf("干燥\r\n");
		// else
		// 	printf("潮湿\r\n");
      
		// humidity = (float)(ADC_getAverage(20,1))*(3.3/4096);// 采集20次，通道1，参考电压为3.3v
		// printf("rain：%.2f\r\n",humidity);
		// Delay_Ms(1000);
#endif /*__UART_SENSER__*/

#ifdef __NEW_SENSER__
humidity = (float)(ADC_getAverage(5))*(3.3/4096); // 读取5次ADC值求平均
printf("rain：%.2f\r\n",humidity);
#endif /*__NEW_SENSER__*/

#ifdef __WS2812__
    spi_rgb_strip_light_display_color(RED);
    HAL_Delay(500);

    spi_rgb_strip_light_display_color(GREEN);
    HAL_Delay(500);

    spi_rgb_strip_light_display_color(BLUE);
    HAL_Delay(500);

    spi_rgb_strip_light_display_color(YELLOW);
    HAL_Delay(500);

    spi_rgb_strip_light_display_color(PURPLE);
    HAL_Delay(500);
#endif /*__WS2812__*/
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
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
