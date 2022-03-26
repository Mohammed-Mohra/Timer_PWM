/*
 * PWM_main.c
 *
 *  Created on: Jan 29, 2022
 *      Author: sa
 */




#include "stm32f4xx_hal.h"
#include <string.h>
#include "main.h"
#include "it.h"
TIM_HandleTypeDef	tim2;
TIM_HandleTypeDef	tim6;
uint32_t compare_reg_val = 0;
uint32_t tim_count[2] = {0};
uint8_t count = 0;

uint8_t time_period_flag = FALSE;
void Error_Handler();
void SystemClockConfig(uint8_t systick_clk_val);
void tim_init();
void gpio_led_init();
int main(void)
{
	uint16_t brightness = 0;
	HAL_Init();

	SystemClockConfig(hse_no_div);
	tim_init();
	gpio_led_init();
	if(HAL_TIM_PWM_Start(&tim2, TIM_CHANNEL_1) != HAL_OK)
		Error_Handler();


	while(1)
	{
		while(brightness < 1998)
		{
			__HAL_TIM_SET_COMPARE(&tim2 ,TIM_CHANNEL_1, brightness);
			brightness+=10;
			HAL_Delay(1);
		}

		while(brightness > 0)
		{
			__HAL_TIM_SET_COMPARE(&tim2 ,TIM_CHANNEL_1, brightness);
			brightness-=10;
			HAL_Delay(1);
		}

	}


	while(1);
	return 0;
}


void gpio_led_init()
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef	gpio_led;
	gpio_led.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_led.Pin = GPIO_PIN_12;
	gpio_led.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &gpio_led);
}



void tim_init()
{
	tim2.Instance = TIM2;
	tim2.Init.Prescaler = 76;
	tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	tim2.Init.Period = 2000-1;
	// There is the question whether we should configure the active channel in the Timer
	if(HAL_TIM_PWM_Init(&tim2) != HAL_OK)
		Error_Handler();

	TIM_OC_InitTypeDef sConfig;
	sConfig.OCMode =  TIM_OCMODE_PWM1;
	sConfig.Pulse = (1999*11)/100;
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH; // Get to know what it means


	if(HAL_TIM_PWM_ConfigChannel(&tim2, &sConfig, TIM_CHANNEL_1)!= HAL_OK)
		Error_Handler();



}


void SystemClockConfig(uint8_t systick_clk_val)
{
	// Configuring the Clock of the device to HSE
	RCC_ClkInitTypeDef	rcc_clk_init;
	rcc_clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |\
			RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 ;


	RCC_OscInitTypeDef 	rcc_hse;
	memset(&rcc_hse,0,sizeof(rcc_hse));
	rcc_hse.OscillatorType = RCC_OSCILLATORTYPE_HSE ;
	//rcc_hse.HSIState = RCC_HSI_ON;
	rcc_hse.HSEState = RCC_HSE_ON;
	rcc_hse.HSICalibrationValue = 16;

	switch(systick_clk_val)
	{
	case SYS_CLK_50MH:
		rcc_hse.PLL.PLLM = 8;
		rcc_hse.PLL.PLLN = 100;
		rcc_hse.PLL.PLLP = RCC_PLLP_DIV2;
		rcc_hse.PLL.PLLSource = RCC_PLLSOURCE_HSE;
		rcc_hse.PLL.PLLState = RCC_PLL_ON;

		rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
		rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

		if(HAL_RCC_OscConfig(&rcc_hse) != HAL_OK)
			Error_Handler();

		if(HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_ACR_LATENCY_1WS) != HAL_OK)
			Error_Handler();
		break;
	case SYS_CLK_84MH:
		// To enable the microcontroller to work in full capacity u have to enable certain settings in power mode
		// First enabel the clock for

		rcc_hse.PLL.PLLM = 8;
		rcc_hse.PLL.PLLN = 84;
		rcc_hse.PLL.PLLP = RCC_PLLP_DIV2;
		rcc_hse.PLL.PLLSource = RCC_PLLSOURCE_HSE;
		rcc_hse.PLL.PLLState = RCC_PLL_ON;

		rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
		rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
		rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

		if(HAL_RCC_OscConfig(&rcc_hse) != HAL_OK)
			Error_Handler();

		if(HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_ACR_LATENCY_2WS) != HAL_OK)
			Error_Handler();
		break;
	case SYS_CLK_120MH:
		rcc_hse.PLL.PLLM = 8;
		rcc_hse.PLL.PLLN = 120;
		rcc_hse.PLL.PLLP = RCC_PLLP_DIV2;
		rcc_hse.PLL.PLLSource = RCC_PLLSOURCE_HSI;
		rcc_hse.PLL.PLLState = RCC_PLL_ON;

		rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV2;
		rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
		rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV2;
		rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

		if(HAL_RCC_OscConfig(&rcc_hse) != HAL_OK)
			Error_Handler();

		if(HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_ACR_LATENCY_4WS) != HAL_OK)
			Error_Handler();
		break;

	case hse_no_div:

		rcc_clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
		rcc_clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
		rcc_clk_init.APB2CLKDivider = RCC_HCLK_DIV1;
		rcc_clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;

		if(HAL_RCC_OscConfig(&rcc_hse) != HAL_OK)
			Error_Handler();

		if(HAL_RCC_ClockConfig(&rcc_clk_init, FLASH_ACR_LATENCY_1WS) != HAL_OK)
			Error_Handler();
		break;
	}


	HAL_SYSTICK_Config(HAL_RCC_GetSysClockFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
}


void Error_Handler()
{
	while(1);
}
