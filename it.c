/*
 * it.c
 *
 *  Created on: Jan 24, 2022
 *      Author: sa
 */
#include	"main.h"
extern TIM_HandleTypeDef	tim2;
extern TIM_HandleTypeDef	tim6;

void SysTick_Handler()
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM2_IRQHandler()
{
	HAL_TIM_IRQHandler(&tim2);

}


void TIM6_DAC_IRQHandler()
{
	HAL_TIM_IRQHandler(&tim6);

}
