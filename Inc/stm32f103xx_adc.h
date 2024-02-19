/**
 * @file stm32f103xx_adc.h
 * @brief Header file for STM32F103xx ADC
 * @details This file contains functions to configure and control the ADC peripheral
 *          on STM32F103xx microcontrollers.
 *
 * @author Vedant A. Rokad
 * @date 3rd February 2024
 * @board STM32F103C8T6 Bluepill
 */
 
#include "stm32f10x.h"

/**
 * @defgroup ADC_Configuration ADC Configuration
 * @{
 */

/** @brief ADC data alignment */
#define ADC_DataAlign_RIGHT 0 /**< Right alignment for ADC data */
#define ADC_DataAlign_LEFT 1  /**< Left alignment for ADC data */

/** @brief ADC sample time */
#define ADC_SampleTime_1_5_Cycle   0 /**< 1.5 ADC clock cycles */
#define ADC_SampleTime_7_5_Cycle   1 /**< 7.5 ADC clock cycles */
#define ADC_SampleTime_13_5_Cycle  2 /**< 13.5 ADC clock cycles */
#define ADC_SampleTime_28_5_Cycle  3 /**< 28.5 ADC clock cycles */
#define ADC_SampleTime_41_5_Cycle  4 /**< 41.5 ADC clock cycles */
#define ADC_SampleTime_55_5_Cycle  5 /**< 55.5 ADC clock cycles */
#define ADC_SampleTime_71_5_Cycle  6 /**< 71.5 ADC clock cycles */
#define ADC_SampleTime_239_5_Cycle 7 /**< 239.5 ADC clock cycles */

/** @brief ADC channels */
#define ADC_Channel_0  0  /**< ADC channel 0 */
#define ADC_Channel_1  1  /**< ADC channel 1 */
#define ADC_Channel_2  2  /**< ADC channel 2 */
#define ADC_Channel_3  3  /**< ADC channel 3 */
#define ADC_Channel_4  4  /**< ADC channel 4 */
#define ADC_Channel_5  5  /**< ADC channel 5 */
#define ADC_Channel_6  6  /**< ADC channel 6 */
#define ADC_Channel_7  7  /**< ADC channel 7 */
#define ADC_Channel_8  8  /**< ADC channel 8 */
#define ADC_Channel_9  9  /**< ADC channel 9 */
#define ADC_Channel_10 10 /**< ADC channel 10 */
#define ADC_Channel_11 11 /**< ADC channel 11 */
#define ADC_Channel_12 12 /**< ADC channel 12 */
#define ADC_Channel_13 13 /**< ADC channel 13 */
#define ADC_Channel_14 14 /**< ADC channel 14 */
#define ADC_Channel_15 15 /**< ADC channel 15 */
#define ADC_Channel_16 16 /**< ADC channel 16 */
#define ADC_Channel_17 17 /**< ADC channel 17 */

/** @} */

/**
 * @struct ADC_Config_t
 * @brief Structure to hold ADC configuration parameters
 */
typedef struct {
    uint32_t ADC_DataAlign;    /**< Data alignment for ADC */
    uint32_t ADC_Channel;      /**< ADC channel */
    uint32_t ADC_SampleTime;   /**< Sample time for ADC */
} ADC_Config_t;

/**
 * @struct ADC_Handle_t
 * @brief Structure to hold ADC handle and configuration
 */
typedef struct {
    ADC_TypeDef *pADCx;        /**< Pointer to ADC peripheral */
    ADC_Config_t ADCConfig;    /**< ADC configuration */
} ADC_Handle_t;

/**
 * @brief Function to control the peripheral clock of ADC
 * @param pADCx Pointer to the ADC peripheral
 * @param EnorDi Enable or disable the peripheral clock (1: enable, 0: disable)
 * @retval None
 */
void ADC_PeriClockControl(ADC_TypeDef *pADCx, uint32_t EnorDi);

/**
 * @brief Function to initialize ADC for single channel conversion
 * @param pADCHandler Pointer to ADC handle structure
 * @retval None
 */
void ADC_Init_SingleChannel(ADC_Handle_t *pADCHandler);

/**
 * @brief Function to start ADC conversion
 * @param pADCHandler Pointer to ADC handle structure
 * @retval None
 */
void ADC_StartConversion(ADC_Handle_t *pADCHandler);

/**
 * @brief Function to read ADC conversion result
 * @param pADCHandler Pointer to ADC handle structure
 * @retval 16-bit ADC conversion result
 */
uint16_t ADC_ReadConversion(ADC_Handle_t *pADCHandler);
