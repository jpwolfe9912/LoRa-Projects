/** @file 		seeed-e5-mini_bsp.c
 *  @brief 
 *
 *
 *  @author 	jpwolfe
 *  @date 		Oct 9, 2022
 */
#include "seeed-e5-mini_bsp.h"

void
BSP_LED_Init(void)
{
	RCC->AHB2ENR |=	RCC_AHB2ENR_GPIOBEN;

	GPIOB->MODER &=	~GPIO_MODER_MODE5;
	GPIOB->MODER |=	GPIO_MODER_MODE5_0;
}

void
BSP_LED_Off(void)
{
	GPIOB->BSRR	|= GPIO_BSRR_BR5;
}

void
BSP_LED_On(void)
{
	GPIOB->BSRR	|= GPIO_BSRR_BS5;
}

void
BSP_LED_Toggle(void)
{
	GPIOB->ODR	^= GPIO_ODR_OD5;
}
