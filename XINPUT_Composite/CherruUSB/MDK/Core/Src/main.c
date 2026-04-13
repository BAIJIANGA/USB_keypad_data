/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "usb.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "KEY.h"
KEY_U8 k_buf[10];
KEY_U8 k_value[2];
KEY_U8 k_now_state[2];
KEY_U8 k_old_state[2];
uint8_t report1[20] = {0x00,20};
uint8_t report[40] = {0x00,0};
void ka_h(KEY_U32 a)
{
    // switch (a)
    // {
    //   case 0 : HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);break;
    //   case 1 : HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_5);break;
    //   default : break;
    // }

    switch (a)
    {
//      case 0 : report[2] &= 0xFE;report[16] &= 0xFE;   break;
//      case 1 : report[2] &= 0xFD;report[16] &= 0xFD; break;
      case 0 : report[1] &= 0xFE;report[8] &= 0xFE;report1[3] &= (~(0x01 << 2));break;
      case 1 : report[1] &= 0xFD;report[8] &= 0xFD;break;
      default : break;
    }
}
void ka_l(KEY_U32 a)
{
    switch (a)
    {
      case 0 : report[1] |= 0x01;report[8] |= 0x01;report1[3] |= (0x01 << 2);break;
      case 1 : report[1] |= (0x02);report[8] |= (0x02);break;
      default : break;
    }
    // switch (a)
    // {
    //   case 0 : HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_5);break;
    //   case 1 : HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_5);break;
    //   default : break;
    // }
}
KEY_HandleTypeDef key_handler = 
{
    k_value,
    k_buf,
    k_now_state,
    k_old_state,
    ka_h,
    ka_l,
};
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
void hid_keyboard_init(uint8_t busid, uintptr_t reg_base);
void hid_xbox_init(uint8_t busid, uintptr_t reg_base);
void hid_xbox_deinit(uint8_t busid);
int usb_dc_init(uint8_t busid);
extern uint8_t out_flag;
extern volatile uint8_t read_buffer[64];
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
void usbd_port_config(uint8_t state)
{
 GPIO_InitTypeDef GPIO_Initure;
 __HAL_RCC_GPIOA_CLK_ENABLE(); /* ʹ�� PORTA ʱ�� */
 if (state)
 {
 USB->CNTR &= ~(1 << 1); /* PWDN = 0, �˳��ϵ�ģʽ */
 /* PA11 ����ģʽ����,���ù��� */
 GPIO_Initure.Pin = GPIO_PIN_11;
 GPIO_Initure.Mode = GPIO_MODE_AF_PP; 
 GPIO_Initure.Pull = GPIO_PULLUP; 
 GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
 HAL_GPIO_Init(GPIOA, &GPIO_Initure);
 /* PA12 ����ģʽ����,���ù��� */
 GPIO_Initure.Pin = GPIO_PIN_12;
 GPIO_Initure.Mode = GPIO_MODE_AF_PP; 
 GPIO_Initure.Pull = GPIO_PULLUP; 
 GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
 HAL_GPIO_Init(GPIOA, &GPIO_Initure);
 }
 else
 {
 USB->CNTR |= 1 << 1; /* PWDN = 1, ����ϵ�ģʽ */
 /* PA11 ����ģʽ����,������� */
 GPIO_Initure.Pin = GPIO_PIN_11;
 GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; 
 GPIO_Initure.Pull = GPIO_PULLUP; 

 GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
 HAL_GPIO_Init(GPIOA, &GPIO_Initure);
 /* PA12 ����ģʽ����,������� */
 GPIO_Initure.Pin = GPIO_PIN_12;
 HAL_GPIO_Init(GPIOA, &GPIO_Initure);
 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET); /* PA11 = 0 */
 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET); /* PA12 = 0 */
 }
}
volatile uint8_t Flag_ex = 0;
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
	return ch;
}
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
  MX_USART1_UART_Init();
  MX_USB_PCD_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  usbd_port_config(0);
  HAL_Delay(100);
  usbd_port_config(1);
  hid_keyboard_init(0,USB_BASE);
  setvbuf(stdout, NULL, _IONBF, 0);
  HAL_Delay(20);
  HAL_TIM_Base_Start_IT(&htim7);
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
#ifdef USE_FULL_ASSERT
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
