/**
 * @file stm32f103xx_adc.c
 * @brief Header file for STM32F103xx ADC
 * @details This file contains functions to configure and control the ADC peripheral
 *          on STM32F103xx microcontrollers.
 *
 * @author Vedant A. Rokad
 * @date 3rd February 2024
 * @board STM32F103C8T6 Bluepill
 */
 
#include "stm32f103xx_adc.h"

/**
 * @brief Function to control the peripheral clock of ADC
 * @param pADCx Pointer to the ADC peripheral
 * @param EnorDi Enable or disable the peripheral clock (ENABLE: enable, DISABLE: disable)
 * @retval None
 */
void ADC_PeriClockControl(ADC_TypeDef *pADCx,uint32_t EnorDi){
		if(EnorDi == ENABLE)
		{			
			if(pADCx == ADC1) 
			{
				RCC->APB2ENR |= (RCC_APB2ENR_ADC1EN);
			}
			else if(pADCx == ADC2)
			{
				RCC->APB2ENR |= (RCC_APB2ENR_ADC2EN);				
			}
		}
		else
		{
			if(pADCx == ADC1) 
			{
				RCC->APB2ENR |= ~(RCC_APB2ENR_ADC1EN);
			}
			else if(pADCx == ADC2)
			{
				RCC->APB2ENR |= ~(RCC_APB2ENR_ADC2EN);				
			}
		}
}


/**
 * @brief Function to initialize ADC for single channel conversion
 * @param pADCHandler Pointer to ADC handle structure
 * @retval None
 */
void ADC_Init_SingleChannel(ADC_Handle_t *pADCHandler){

	ADC_PeriClockControl(pADCHandler->pADCx,ENABLE);
/**
	* @brief: Configure Data Align
	*/
	if(pADCHandler->ADCConfig.ADC_DataAlign == ADC_DataAlign_RIGHT){
			pADCHandler->pADCx->CR2 &= ~(ADC_CR2_ALIGN);
	}
	else{
			pADCHandler->pADCx->CR2 |= (ADC_CR2_ALIGN);
	}

/**
	* @brief: Configure Continuious Mode
	*/	
	pADCHandler->pADCx->CR2 |= (ADC_CR2_CONT) ;

/**
	* @brief: Configure External Event on to the Software event on
	*/		
	pADCHandler->pADCx->CR2 |= (ADC_CR2_EXTSEL);

/**
	* @brief: Configure Sample Cycle
	*/			
	if(pADCHandler->ADCConfig.ADC_Channel <= ADC_Channel_9){
		pADCHandler->pADCx->SMPR2 |= (uint32_t)(pADCHandler->ADCConfig.ADC_Channel*3);
	}
	else{
		pADCHandler->pADCx->SMPR2 |= (uint32_t)((pADCHandler->ADCConfig.ADC_Channel-10)*3);
	}
	
/**
	* @brief: Configure Sequence
	*/				
	pADCHandler->pADCx->SQR3 |= (pADCHandler->ADCConfig.ADC_Channel);
	
}


/**
 * @brief Function to start ADC conversion
 * @param pADCHandler Pointer to ADC handle structure
 * @retval None
 */
void ADC_StartConversion(ADC_Handle_t *pADCHandler){
		
		pADCHandler->pADCx->CR2 |= (ADC_CR2_RSTCAL);
		while((pADCHandler->pADCx->CR2) & (ADC_CR2_RSTCAL));
		pADCHandler->pADCx->CR2 |= (ADC_CR2_CAL);
		while((pADCHandler->pADCx->CR2) & (ADC_CR2_CAL));
		
	
		pADCHandler->pADCx->CR2 |= (ADC_CR2_ADON) ;
		pADCHandler->pADCx->CR2 |= (ADC_CR2_EXTTRIG) ;
		pADCHandler->pADCx->CR2 |= (ADC_CR2_SWSTART) ;	
}

/**
 * @brief Function to read ADC conversion result
 * @param pADCHandler Pointer to ADC handle structure
 * @retval 16-bit ADC conversion result
 */
uint16_t ADC_ReadConversion(ADC_Handle_t *pADCHandler){
	return (uint16_t)(pADCHandler->pADCx->DR & (0xffff));
}
