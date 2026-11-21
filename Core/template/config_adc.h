/**
 * @brief ʹ��CubeMX����STM32G474��ADC�����ڶ�ʱ������ģ���źŵĲ���
 * @note CubeMX ����
 * 1. Pinout & Configuration -> ADC1 -> Mode
 *   - IN1: IN1 Single-ended
 *   - IN2: IN2 Single-ended
 *   ...
 *   - Vrefint Channel [*]
 * 2. Pinout & Configuration -> ADC1 -> Configuration -> Parameter Settings
 *   - ADC_Settings
 *     - Continuous Conversion Mode: disabled
 *   - ADC_Regular_ConversionMode
 *     - Number of Conversion: n
 *     - Rank 1
 *       - Channel: Channel Vrefint
 *       - Sample Time: 640.5 Cycles
 *     - Rank 2
 *       - Channel: Channel 1
 *       - Sample Time: 640.5 Cycles
 *     - Rank 3
 *       - Channel: Channel 2
 *       - Sample Time: 640.5 Cycles
 *     ...
 *     - External Trigger Conversion Source: High Resolution Timer Trigger 1 event
 *     - External Trigger Conversion Edge: rising
 * 3. ADC1 -> Configuration -> DMA Settings
 *   - Add
 *   - DMA Request: ADC1
 *   - DMA Request Settings
 *     - Mode: Circular
 *     - Data Width: Half Word,Half Word(16-bit)
 * 4. HRTIM1 -> Configuration -> Timer Master(���߱��)
 *   - Compare Unit 3:
 *     - Compare Unit 3 Configuration: Enable
 *     - Compare Value: 2000
 * 5. HRTIM1 -> Configuration -> ADC Triggers Configuration
 *   - ADC Trigger 1:
 *     - ADC Trigger 1 Configuration: Enable
 *     - Update Trigger Source: Master Timer
 *     - Trigger Sources Selection: 1
 *       - 1st Trigger Source: master compare 3
 * 6.NVIC ע�⼸��DMA����ռ���ȼ���һ�£�����Ὰ̬
 */
#ifndef __CONFIG_ADC_H__
#define __CONFIG_ADC_H__

#include "mbase.h"
#include "stm32g474xx.h"
#include "stm32g4xx_hal.h"
#include "adc.h"

/********************* USER CONFIG BEGIN *************************/

/**
 * @brief ADCͨ������
 *
 * @note ��ѹ�ο�ͨ��������ADC1��ͨ����
 */
#define ADC1_CHANNEL_NUM 5
#define ADC2_CHANNEL_NUM 0
#define ADC3_CHANNEL_NUM 0
#define ADC4_CHANNEL_NUM 0
#define ADC5_CHANNEL_NUM 0

/**
 * @brief ADC DMA����������
 * @arg uint8_t -> byte
 * @arg uint16_t -> half word
 * @arg uint32_t -> word
 */
typedef uint16_t ADC_DMA_Buffer_t;

/**
 * @brief ��ѹ�ο�ͨ����DMA��������λ��
 */
#define VREFINT_RAW_POS 0

/********************* USER CONFIG END ***************************/

/**
 * @brief ADC DMA������
 */

#define ADC_DMA1_BUFFER_OFFSET (0)
#define ADC_DMA2_BUFFER_OFFSET (ADC_DMA1_BUFFER_OFFSET + ADC1_CHANNEL_NUM)
#define ADC_DMA3_BUFFER_OFFSET (ADC_DMA2_BUFFER_OFFSET + ADC2_CHANNEL_NUM)
#define ADC_DMA4_BUFFER_OFFSET (ADC_DMA3_BUFFER_OFFSET + ADC3_CHANNEL_NUM)
#define ADC_DMA5_BUFFER_OFFSET (ADC_DMA4_BUFFER_OFFSET + ADC4_CHANNEL_NUM)

#define ADC_DMA_BUFFER_SIZE (ADC_DMA5_BUFFER_OFFSET + ADC5_CHANNEL_NUM)

extern ADC_DMA_Buffer_t adc_dma_buffer[ADC_DMA_BUFFER_SIZE];


/**
 * @brief ADC���
 */
#define ADC1_HANDLER (&hadc1)
#define ADC2_HANDLER (&hadc2)
#define ADC3_HANDLER (&hadc3)
#define ADC4_HANDLER (&hadc4)
#define ADC5_HANDLER (&hadc5)


/**
 * @brief ����ADCУ׼
 */
void CADC_Calibration_Start(void);


/**
 * @note ��ѹ�ο�ͨ��У׼��ַ: VREFINT_CAL_ADDR (const uint16_t*)
 * @note ��ѹ�ο�ͨ����ѹֵ��VREFINT_CAL_VREF
 */
#define VREFINT_CAL (*VREFINT_CAL_ADDR)
extern Float_t VDDA_mv;
void CADC_Calibrate_VDDA(void);


/**
 * @brief ����У׼���ݸ���ADCУ׼�������ȵ���CADC_Calibrate_VDDA()
 * @param raw ԭʼ����
 * @return У׼���ADC�����ϵĵ�ѹֵ(mV)
 */
Float_t CADC_Calibrate_mv(ADC_DMA_Buffer_t raw);


/**
 * @brief ����/ֹͣADC DMA����
 */
void CADC_Start_DMA(void);
void CADC_Stop_DMA(void);

/**
 * @brief ADC�жϻص���������
 */
typedef void (*ADC_IT_cbk)(void);

typedef struct {
    ADC_IT_cbk conv_cplt_cbk;
    ADC_IT_cbk conv_half_cplt_cbk;
} ADC_IT_Callbacks_t;

/**
 * @brief ע��ADC�ص�
 * @param adc_it_callbacks ADC�ص��ṹ��ָ��
 */
void ADC_IT_Callbacks_Register(ADC_IT_Callbacks_t* adc_it_callbacks);

#endif
