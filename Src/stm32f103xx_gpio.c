/**
  ******************************************************************************
  * @file    		stm32f103xx_gpio.c
  * @author  		Vedant A. Rokad
  * @processor 	ARM Cortex-M3
	* @controller STM32F103C8T6
  * @date    		26-Feb-2022
  * @brief   		Peripheral_Driver Source file
  ******************************************************************************
 **/


#include "stm32f103xx_gpio.h"


/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//																GPIO_PeriClockControl                                //
//																															  										 //
/////////////////////////////////////////////////////////////////////////////////////////
void GPIO_PeriClockControl(GPIO_TypeDef *pGPIOx , uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			RCC->APB2ENR |=(1<<2);
		}
		if(pGPIOx == GPIOB)
		{
			RCC->APB2ENR |=(1<<3);
		}
		if(pGPIOx == GPIOC)
		{
			RCC->APB2ENR |=(1<<4);
		}
		if(pGPIOx == GPIOD)
		{
			RCC->APB2ENR |=(1<<5);
		}

	}

	else if(EnorDi == DISABLE)
	{
		if(pGPIOx == GPIOA)
		{
			RCC->APB2ENR &= ~(1<<2);
		}
		if(pGPIOx == GPIOB)
		{
			RCC->APB2ENR &= ~(1<<3);
		}
		if(pGPIOx == GPIOC)
		{
			RCC->APB2ENR &= ~(1<<4);
		}
		if(pGPIOx == GPIOD)
		{
			RCC->APB2ENR &= ~(1<<5);
		}

	}
}


/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//																		GPIO_Init                                				 //
//																															  										 //
/////////////////////////////////////////////////////////////////////////////////////////
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	
	if(pGPIOHandle->GPIO_PinConfig.GPIO_Pintype == GPIO_PIN_TYPE_NORMAL)
	{
		uint32_t temp1 , temp2;
		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
		temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
		if(temp1 == 0)
		{
			pGPIOHandle->pGPIOx->CRL &= ~(0xF<<(temp2*4));
			pGPIOHandle->pGPIOx->CRL |= (pGPIOHandle->GPIO_PinConfig.GPIO_ModeInOut<<(temp2*4));
			pGPIOHandle->pGPIOx->CRL |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode<<((temp2*4)+2));		
		}

		else
		{
			pGPIOHandle->pGPIOx->CRH &= ~(0xF<<(temp2*4));
			pGPIOHandle->pGPIOx->CRH |= (pGPIOHandle->GPIO_PinConfig.GPIO_ModeInOut<<(temp2*4));
			pGPIOHandle->pGPIOx->CRH |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode<<((temp2*4)+2));
		}
	}
	else if(pGPIOHandle->GPIO_PinConfig.GPIO_Pintype == GPIO_PIN_TYPE_INTERRUPT)
	{
		/*Enable AFIO in RCC*/
		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
		
		/*Set IMR*/
		EXTI->IMR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		
		/*Set Edge trigger*/
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinTypeMode == GPIO_MODE_IT_FT)
		{
			EXTI->FTSR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR &= ~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinTypeMode == GPIO_MODE_IT_RT)
		{
			EXTI->RTSR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR &= ~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinTypeMode == GPIO_MODE_IT_RFT)
		{
			EXTI->FTSR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		
		uint8_t var1 =0 , var2 =0;
		var1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber/4;
		var2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber%4;

		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		AFIO->EXTICR[var1] |= (portcode<<(var2*4));
		
		
	}

}


/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//														GPIO_IRQ_Interrupt_ENConfig                              //
//																															  										 //
/////////////////////////////////////////////////////////////////////////////////////////
void GPIO_IRQ_Interrupt_ENConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		NVIC_EnableIRQ(IRQNumber);
	}
	else
	{
		NVIC_DisableIRQ(IRQNumber);
	}
}


void GPIO_IRQ_PR_Config(uint8_t IRQNumber, uint8_t IRQ_Priority)
{
	NVIC_SetPriority(IRQNumber,IRQ_Priority);
}

void GPIO_IRQHandling(uint8_t PinNumber)
{
		if(EXTI->PR & (1<<PinNumber))
		{
			EXTI->PR |= (1<<PinNumber);
		}
}



/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//																		GPIO_PinEnDn           			                     //
//																															  										 //
/////////////////////////////////////////////////////////////////////////////////////////
void GPIO_PinEnDn(GPIO_TypeDef *pGPIOx,uint32_t PinNumber,uint32_t EnorDi)
{
		if(EnorDi == ENABLE)
		{
			GPIO_PeriClockControl(pGPIOx,ENABLE);
			GPIO_Handle_t gpio;
			gpio.pGPIOx = pGPIOx;
			gpio.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT_PP;
			gpio.GPIO_PinConfig.GPIO_ModeInOut = GPIO_OUT_50MHZ;
			gpio.GPIO_PinConfig.GPIO_PinNumber = PinNumber;
			GPIO_Init(&gpio);
			pGPIOx->ODR &= ~(1 << PinNumber);
		}
		else
		{
			GPIO_PeriClockControl(pGPIOx,ENABLE);
			GPIO_Handle_t gpio;
			gpio.pGPIOx = pGPIOx;
			gpio.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT_PP;
			gpio.GPIO_PinConfig.GPIO_ModeInOut = GPIO_OUT_50MHZ;
			gpio.GPIO_PinConfig.GPIO_PinNumber = PinNumber;
			GPIO_Init(&gpio);
			pGPIOx->ODR |= (1 << PinNumber);
		}
}

void GPIO_Set(GPIO_TypeDef *pGPIOx , uint32_t PinNumber)
{
	pGPIOx->ODR &= ~(1 << PinNumber);
}
void GPIO_ReSet(GPIO_TypeDef *pGPIOx , uint32_t PinNumber)
{
	pGPIOx->ODR |= (1 << PinNumber);
}

