/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "ILI9341_Touchscreen.h"

#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"

//#include "snow_tiger.h"
#include "string.h"

#include "stdlib.h"
#include "time.h"

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
volatile uint16_t Timer1 = 0;
uint8_t sect[512];

//char buffer1[512] =		"Saleae makes high-performance, dead-simple-to-use USB Logic Analyzers that can record both digital and analog, and decode protocols like SPI, I2C, Serial, 1-Wire, CAN."; //Буфер данных для записи/чтения
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void testLCD(void);
void testLCD_2(void);
void testLCD_3(void);

extern void SD_PowerOn(void);
extern uint8_t sd_ini(void);
extern uint8_t SD_Read_Block(uint8_t *buff, uint32_t lba);
extern uint8_t SD_Write_Block(uint8_t *buff, uint32_t lba);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
FRESULT ReadLongFile(void)

{
	uint16_t i = 0, i1 = 0;
	uint32_t ind = 0;
	uint32_t f_size = MyFile.fsize;
	sprintf(str1, "fsize: %lu\r\n", (unsigned long) f_size);
	HAL_UART_Transmit(&huart1, (uint8_t*) str1, strlen(str1), 0x1000);
	ind = 0;
	do
	{
		if (f_size < 512)
		{
			i1 = f_size;
		}
		else
		{
			i1 = 512;
		}
		f_size -= i1;
		f_lseek(&MyFile, ind);
		f_read(&MyFile, sect, i1, (UINT *) &bytesread);
		for (i = 0; i < bytesread; i++)
		{
			HAL_UART_Transmit(&huart1, sect + i, 1, 0x1000);
		}
		ind += i1;
	}
	while (f_size > 0);
	HAL_UART_Transmit(&huart1, (uint8_t*) "\r\n", 2, 0x1000);
	return FR_OK;

}

*/


void drawImageFromSD(uint16_t pixel) {
	static uint16_t width = 0, height = 0;
	ILI9341_Draw_Pixel(width, height, pixel);
	width++;
	if (width == 320) {
		width = 0;
		height++;
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
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_FATFS_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_Base_Start_IT(&htim2);


	ILI9341_Init(); //initial driver setup to drive ili9341
	HAL_Delay(1000);

/*
	//ILI9341_Fill_Screen(RED);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);

	//SD_Write_Block((uint8_t*)buffer1,0); //Запишем блок в буфер
	for (i = 0; i < 300; i++) {
		SD_Read_Block(sect, i); //Считаем блок из буфера
		for (int y = 0; y < 512; y+=2) {

			drawImageFromSD((*(sect+y)<<8)+*(sect+y+1));
//			HAL_UART_Transmit(&huart1, sect + y, 1, 0x1000);
//			HAL_UART_Transmit(&huart1, (uint8_t*) "\r\n", 2, 0x1000);
		}

	}

	*/

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		testLCD();
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

  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the CPU, AHB and APB busses clocks 
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
  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_1);
}

/* USER CODE BEGIN 4 */

void testLCD(void) {

	srand(time(NULL));

	//----------------------------------------------------------PERFORMANCE TEST
	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	ILI9341_Draw_Text("FPS TEST, 40 loop 2 screens", 10, 10, BLACK, 1, WHITE);
	HAL_Delay(2000);
	ILI9341_Fill_Screen(WHITE);

	uint32_t Timer_Counter = 0;
	for (uint32_t j = 0; j < 2; j++) {
		HAL_TIM_Base_Start(&htim1);
		for (uint16_t i = 0; i < 10; i++) {
			ILI9341_Fill_Screen(WHITE);
			ILI9341_Fill_Screen(BLACK);
		}

		//20.000 per second!
		HAL_TIM_Base_Stop(&htim1);
		Timer_Counter += __HAL_TIM_GET_COUNTER(&htim1);
		__HAL_TIM_SET_COUNTER(&htim1, 0);
	}
	Timer_Counter /= 2;

	char counter_buff[30];
	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	sprintf(counter_buff, "Timer counter value: %d", Timer_Counter * 2);
	ILI9341_Draw_Text(counter_buff, 10, 10, BLACK, 1, WHITE);

	double seconds_passed = 2 * ((float) Timer_Counter / 20000);
	sprintf(counter_buff, "Time: %.3f Sec", seconds_passed);
	ILI9341_Draw_Text(counter_buff, 10, 30, BLACK, 2, WHITE);

	double timer_float = 20 / (((float) Timer_Counter) / 20000); //Frames per sec

	sprintf(counter_buff, "FPS:  %.2f", timer_float);
	ILI9341_Draw_Text(counter_buff, 10, 50, BLACK, 2, WHITE);
	double MB_PS = timer_float * 240 * 320 * 2 / 1000000;
	sprintf(counter_buff, "MB/S: %.2f", MB_PS);
	ILI9341_Draw_Text(counter_buff, 10, 70, BLACK, 2, WHITE);
	double SPI_utilized_percentage = (MB_PS / (6.25)) * 100; //50mbits / 8 bits
	sprintf(counter_buff, "SPI Utilized: %.2f", SPI_utilized_percentage);
	ILI9341_Draw_Text(counter_buff, 10, 90, BLACK, 2, WHITE);
	HAL_Delay(10000);

	static uint16_t x = 0;
	static uint16_t y = 0;

	char Temp_Buffer_text[40];

	//----------------------------------------------------------COUNTING MULTIPLE SEGMENTS
	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	ILI9341_Draw_Text("Counting multiple segments at once", 10, 10, BLACK, 1,
	WHITE);
	HAL_Delay(2000);
	ILI9341_Fill_Screen(WHITE);

	for (uint16_t i = 0; i <= 10; i++) {
		sprintf(Temp_Buffer_text, "Counting: %d", i);
		ILI9341_Draw_Text(Temp_Buffer_text, 10, 10, BLACK, 2, WHITE);
		ILI9341_Draw_Text(Temp_Buffer_text, 10, 30, BLUE, 2, WHITE);
		ILI9341_Draw_Text(Temp_Buffer_text, 10, 50, RED, 2, WHITE);
		ILI9341_Draw_Text(Temp_Buffer_text, 10, 70, GREEN, 2, WHITE);
		ILI9341_Draw_Text(Temp_Buffer_text, 10, 90, BLACK, 2, WHITE);
		ILI9341_Draw_Text(Temp_Buffer_text, 10, 110, BLUE, 2, WHITE);
		ILI9341_Draw_Text(Temp_Buffer_text, 10, 130, RED, 2, WHITE);
		ILI9341_Draw_Text(Temp_Buffer_text, 10, 150, GREEN, 2, WHITE);
		ILI9341_Draw_Text(Temp_Buffer_text, 10, 170, WHITE, 2, BLACK);
		ILI9341_Draw_Text(Temp_Buffer_text, 10, 190, BLUE, 2, BLACK);
		ILI9341_Draw_Text(Temp_Buffer_text, 10, 210, RED, 2, BLACK);
	}

	HAL_Delay(1000);

	//----------------------------------------------------------COUNTING SINGLE SEGMENT
	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	ILI9341_Draw_Text("Counting single segment", 10, 10, BLACK, 1, WHITE);
	HAL_Delay(2000);
	ILI9341_Fill_Screen(WHITE);

	for (uint16_t i = 0; i <= 100; i++) {
		sprintf(Temp_Buffer_text, "Counting: %d", i);
		ILI9341_Draw_Text(Temp_Buffer_text, 10, 10, BLACK, 3, WHITE);
	}

	HAL_Delay(1000);

	//----------------------------------------------------------ALIGNMENT TEST
	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	ILI9341_Draw_Text("Rectangle alignment check", 10, 10, BLACK, 1, WHITE);
	HAL_Delay(2000);
	ILI9341_Fill_Screen(WHITE);

	ILI9341_Draw_Hollow_Rectangle_Coord(50, 50, 100, 100, BLACK);
	ILI9341_Draw_Filled_Rectangle_Coord(20, 20, 50, 50, BLACK);
	ILI9341_Draw_Hollow_Rectangle_Coord(10, 10, 19, 19, BLACK);
	HAL_Delay(1000);

	//----------------------------------------------------------LINES EXAMPLE
	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	ILI9341_Draw_Text("Randomly placed and sized", 10, 10, BLACK, 1, WHITE);
	ILI9341_Draw_Text("Horizontal and Vertical lines", 10, 20, BLACK, 1, WHITE);
	HAL_Delay(2000);
	ILI9341_Fill_Screen(WHITE);

	for (uint32_t i = 0; i < 30000; i++) {
		uint32_t random_num = 0;
		uint16_t xr = 0;
		uint16_t yr = 0;
		uint16_t radiusr = 0;
		uint16_t colourr = 0;
		random_num = rand() % 320;
		xr = random_num;
		random_num = rand() % 320;
		yr = random_num;
		random_num = rand() % 320;
		radiusr = random_num;
		random_num = rand() % 0xffff;
		colourr = random_num;

		xr &= 0x01FF;
		yr &= 0x01FF;
		radiusr &= 0x001F;
		//ili9341_drawpixel(xr, yr, WHITE);
		ILI9341_Draw_Horizontal_Line(xr, yr, radiusr, colourr);
		ILI9341_Draw_Vertical_Line(xr, yr, radiusr, colourr);
	}

	HAL_Delay(1000);

	//----------------------------------------------------------HOLLOW CIRCLES EXAMPLE
	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	ILI9341_Draw_Text("Randomly placed and sized", 10, 10, BLACK, 1, WHITE);
	ILI9341_Draw_Text("Circles", 10, 20, BLACK, 1, WHITE);
	HAL_Delay(2000);
	ILI9341_Fill_Screen(WHITE);

	for (uint32_t i = 0; i < 3000; i++) {
		uint32_t random_num = 0;
		uint16_t xr = 0;
		uint16_t yr = 0;
		uint16_t radiusr = 0;
		uint16_t colourr = 0;
		random_num = rand() % 320;
		xr = random_num;
		random_num = rand() % 320;
		yr = random_num;
		random_num = rand() % 320;
		radiusr = random_num;
		random_num = rand() % 0xffff;
		colourr = random_num;

		xr &= 0x01FF;
		yr &= 0x01FF;
		radiusr &= 0x001F;
		//ili9341_drawpixel(xr, yr, WHITE);
		ILI9341_Draw_Hollow_Circle(xr, yr, radiusr * 2, colourr);
	}
	HAL_Delay(1000);

	//----------------------------------------------------------FILLED CIRCLES EXAMPLE
	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	ILI9341_Draw_Text("Randomly placed and sized", 10, 10, BLACK, 1, WHITE);
	ILI9341_Draw_Text("Filled Circles", 10, 20, BLACK, 1, WHITE);
	HAL_Delay(2000);
	ILI9341_Fill_Screen(WHITE);

	for (uint32_t i = 0; i < 1000; i++) {
		uint32_t random_num = 0;
		uint16_t xr = 0;
		uint16_t yr = 0;
		uint16_t radiusr = 0;
		uint16_t colourr = 0;
		random_num = rand() % 320;
		xr = random_num;
		random_num = rand() % 320;
		yr = random_num;
		random_num = rand() % 320;
		radiusr = random_num;
		random_num = rand() % 0xffff;
		colourr = random_num;

		xr &= 0x01FF;
		yr &= 0x01FF;
		radiusr &= 0x001F;
		//ili9341_drawpixel(xr, yr, WHITE);
		ILI9341_Draw_Filled_Circle(xr, yr, radiusr / 2, colourr);
	}
	HAL_Delay(1000);

	//----------------------------------------------------------HOLLOW RECTANGLES EXAMPLE
	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	ILI9341_Draw_Text("Randomly placed and sized", 10, 10, BLACK, 1, WHITE);
	ILI9341_Draw_Text("Rectangles", 10, 20, BLACK, 1, WHITE);
	HAL_Delay(2000);
	ILI9341_Fill_Screen(WHITE);

	for (uint32_t i = 0; i < 20000; i++) {
		uint32_t random_num = 0;
		uint16_t xr = 0;
		uint16_t yr = 0;
		uint16_t radiusr = 0;
		uint16_t colourr = 0;
		random_num = rand() % 320;
		xr = random_num;
		random_num = rand() % 320;
		yr = random_num;
		random_num = rand() % 320;
		radiusr = random_num;
		random_num = rand() % 0xffff;
		colourr = random_num;

		xr &= 0x01FF;
		yr &= 0x01FF;
		radiusr &= 0x001F;
		//ili9341_drawpixel(xr, yr, WHITE);
		ILI9341_Draw_Hollow_Rectangle_Coord(xr, yr, xr + radiusr, yr + radiusr,
				colourr);
	}
	HAL_Delay(1000);

	//----------------------------------------------------------FILLED RECTANGLES EXAMPLE
	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	ILI9341_Draw_Text("Randomly placed and sized", 10, 10, BLACK, 1, WHITE);
	ILI9341_Draw_Text("Filled Rectangles", 10, 20, BLACK, 1, WHITE);
	HAL_Delay(2000);
	ILI9341_Fill_Screen(WHITE);

	for (uint32_t i = 0; i < 20000; i++) {
		uint32_t random_num = 0;
		uint16_t xr = 0;
		uint16_t yr = 0;
		uint16_t radiusr = 0;
		uint16_t colourr = 0;
		random_num = rand() % 320;
		xr = random_num;
		random_num = rand() % 320;
		yr = random_num;
		random_num = rand() % 320;
		radiusr = random_num;
		random_num = rand() % 0xffff;
		colourr = random_num;

		xr &= 0x01FF;
		yr &= 0x01FF;
		radiusr &= 0x001F;
		//ili9341_drawpixel(xr, yr, WHITE);
		ILI9341_Draw_Rectangle(xr, yr, radiusr, radiusr, colourr);
	}
	HAL_Delay(1000);

	//----------------------------------------------------------INDIVIDUAL PIXEL EXAMPLE

	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	ILI9341_Draw_Text("Slow draw by selecting", 10, 10, BLACK, 1, WHITE);
	ILI9341_Draw_Text("and adressing pixels", 10, 20, BLACK, 1, WHITE);
	HAL_Delay(2000);
	ILI9341_Fill_Screen(WHITE);

	x = 0;
	y = 0;
	while (y < 240) {
		while ((x < 320) && (y < 240)) {

			if (x % 2) {
				ILI9341_Draw_Pixel(x, y, BLACK);
			}

			x++;
		}

		y++;
		x = 0;
	}

	x = 0;
	y = 0;

	while (y < 240) {
		while ((x < 320) && (y < 240)) {

			if (y % 2) {
				ILI9341_Draw_Pixel(x, y, BLACK);
			}

			x++;
		}

		y++;
		x = 0;
	}
	HAL_Delay(2000);

	//----------------------------------------------------------INDIVIDUAL PIXEL EXAMPLE
	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	ILI9341_Draw_Text("Random position and colour", 10, 10, BLACK, 1, WHITE);
	ILI9341_Draw_Text("500000 pixels", 10, 20, BLACK, 1, WHITE);
	HAL_Delay(2000);
	ILI9341_Fill_Screen(WHITE);

	for (uint32_t i = 0; i < 500000; i++) {
		uint32_t random_num = 0;
		uint16_t xr = 0;
		uint16_t yr = 0;
		random_num = rand() % 240;
		xr = random_num;
		random_num = rand() % 320;
		yr = random_num;
		uint16_t color = rand() % 0xffff;

		xr &= 0x01FF;
		yr &= 0x01FF;
		ILI9341_Draw_Pixel(xr, yr, color);
	}
	HAL_Delay(2000);

	//----------------------------------------------------------565 COLOUR EXAMPLE, Grayscale
	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	ILI9341_Draw_Text("Colour gradient", 10, 10, BLACK, 1, WHITE);
	ILI9341_Draw_Text("Grayscale", 10, 20, BLACK, 1, WHITE);
	HAL_Delay(2000);

	for (uint16_t i = 0; i <= (320); i++) {
		uint16_t Red = 0;
		uint16_t Green = 0;
		uint16_t Blue = 0;

		Red = i / (10);
		Red <<= 11;
		Green = i / (5);
		Green <<= 5;
		Blue = i / (10);

		uint16_t RGB_color = Red + Green + Blue;
		ILI9341_Draw_Rectangle(i, x, 1, 240, RGB_color);

	}
	HAL_Delay(2000);

}

void testLCD_2(void) {
	srand(time(NULL));

	//----------------------------------------------------------FILLED CIRCLES EXAMPLE
	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);

	for (uint32_t i = 0; i < 10000; i++) {

		uint32_t random_num = 0;
		uint16_t xr = 0;
		uint16_t yr = 0;
		uint16_t radiusr = 0;
		uint16_t colourr = 0;
		random_num = rand() % 320;
		xr = random_num;
		random_num = rand() % 320;
		yr = random_num;
		random_num = rand() % 1000;
		radiusr = random_num;
		random_num = rand() % 0xffff;
		colourr = random_num;

		xr &= 0x01FF;
		yr &= 0x01FF;
		radiusr &= 0x001F;
		//ili9341_drawpixel(xr, yr, WHITE);
		ILI9341_Draw_Filled_Circle(xr, yr, radiusr / 2, colourr);
	}
	HAL_Delay(500);
}

void testLCD_3(void) {

	//----------------------------------------------------------TOUCHSCREEN EXAMPLE
	ILI9341_Fill_Screen(WHITE);
	ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
	ILI9341_Draw_Text("Touchscreen", 10, 10, BLACK, 2, WHITE);
	ILI9341_Draw_Text("Touch to draw", 10, 30, BLACK, 2, WHITE);
	ILI9341_Set_Rotation(SCREEN_VERTICAL_1);

	while (1) {

		if (TP_Touchpad_Pressed()) {

			uint16_t x_pos = 0;
			uint16_t y_pos = 0;

			//HAL_GPIO_WritePin(GPIOB, LD3_Pin | LD2_Pin, GPIO_PIN_SET);

			uint16_t position_array[2];

			if (TP_Read_Coordinates(position_array) == TOUCHPAD_DATA_OK) {
				x_pos = position_array[0];
				y_pos = position_array[1];
				ILI9341_Draw_Filled_Circle(x_pos, y_pos, 2, BLACK);

				ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
				char counter_buff[30];
				sprintf(counter_buff, "POS X: %.3d", x_pos);
				ILI9341_Draw_Text(counter_buff, 10, 80, BLACK, 2, WHITE);
				sprintf(counter_buff, "POS Y: %.3d", y_pos);
				ILI9341_Draw_Text(counter_buff, 10, 120, BLACK, 2, WHITE);
				ILI9341_Set_Rotation(SCREEN_VERTICAL_1);
			}

			//ILI9341_Draw_Pixel(x_pos, y_pos, BLACK);

		} else {
			//HAL_GPIO_WritePin(GPIOB, LD3_Pin | LD2_Pin, GPIO_PIN_RESET);
		}

	}
}

void testLCD_4(void) {

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim2) {
		Timer1++;
	}
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
	 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
