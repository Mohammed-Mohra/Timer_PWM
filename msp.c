/*
 * msp.c
 *
 *  Created on: Jan 24, 2022
 *      Author: sa
 */
#include "stm32f4xx_hal.h"


void HAL_MspInit(void)
{
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_MspInit could be implemented in the user file
   */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	SCB->SHCSR |= 0x7 << 16;	//enable memory fault, bus fault and usage fault

	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);

}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	/* In the low level initialization of the peripherals
		Normally just enable the clock and the interrupts and configure the pins like muxing if there is communication
*/
	__HAL_RCC_TIM6_CLK_ENABLE();
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
	//HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);

}
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
 //	first enable the clock for the timer 2
	__HAL_RCC_TIM2_CLK_ENABLE();

	//Second Enable the interrupts for the timer 2
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);

	//Third mux all the gpios peripherals and enable the clock for the gpios

	__HAL_RCC_GPIOA_CLK_ENABLE();

	//Channel 1 PA0

	GPIO_InitTypeDef	gpioa_channels;
	gpioa_channels.Alternate = GPIO_AF1_TIM2;
	gpioa_channels.Mode = GPIO_MODE_AF_PP;
	gpioa_channels.Pin = GPIO_PIN_0;
	gpioa_channels.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &gpioa_channels);
/*
	// Channel 2 PA1
	gpioa_channels.Pin = GPIO_PIN_1;
	//gpioa_channels.Alternate = GPIO_AF1_TIM2;
	gpioa_channels.Mode = GPIO_MODE_OUTPUT_PP;
	gpioa_channels.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &gpioa_channels);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);

	// Channel 3 PA2
	gpioa_channels.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOA, &gpioa_channels);
	// Channel 4 PA3
	gpioa_channels.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpioa_channels);
*/
}
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
	// FirstEnable the clock of the timer
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	// then mux the required gpio required for the timer
	// PA0 -> Channel1
	GPIO_InitTypeDef	gpioa_channel1;
	gpioa_channel1.Alternate = GPIO_AF1_TIM2;
	gpioa_channel1.Mode = GPIO_MODE_AF_PP;
	gpioa_channel1.Pin = GPIO_PIN_0;
	gpioa_channel1.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &gpioa_channel1);

	// Now lets enable the interrupts cause we will work in interrupt mode
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);

}


void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
	/* In the low level initialization of the peripherals
		Normally just enable the clock and the interrupts and configure the pins like muxing if there is communication
*/
	__HAL_RCC_TIM2_CLK_ENABLE();

	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef	ds_line;
	ds_line.Mode = GPIO_MODE_AF_PP;
	ds_line.Pin = GPIO_PIN_3;
	ds_line.Pull = GPIO_PULLUP;
	ds_line.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOB, &ds_line);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	//HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 15, 0);

}

