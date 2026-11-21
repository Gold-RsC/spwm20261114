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
#include "adc.h"
#include "dma.h"
#include "hrtim.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "config_hrtim.h"
#include "config_adc.h"
#include "cv_cc.h"
#include "kalman.h"
#include "print_adapt.h"
#include "spwm.h"
// #include "oled.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

typedef struct {
    float voltage;
    float current;
    float power;
    Sin_Analyzer_t sin_analyzer;
} Power_t;
Power_t in, out;

Kalman_Filter_t kalman[5];

SPWM_t spwm;
float pwm;

const int FREQ      = 20000;
const int HALF_FREQ = FREQ / 2;
const int REP       = 5000;
const int HALF_REP  = REP / 2;

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void set_pwm(float pwm) {
    CHRTIM_Compare_Set(HRTIM_TIMERINDEX_MASTER, HRTIM_COMPAREUNIT_1, HALF_REP * (1 - pwm));
    CHRTIM_Compare_Set(HRTIM_TIMERINDEX_MASTER, HRTIM_COMPAREUNIT_2, HALF_REP * (1 + pwm));
}
bool flag = 0;
void ADC_IT_cbks(void) {
    // 校准电压和电流，得到引脚上的电压值(mv)
    CADC_Calibrate_VDDA();
    in.voltage  = CADC_Calibrate_mv(adc_dma_buffer[1]);
    out.voltage = CADC_Calibrate_mv(adc_dma_buffer[2]);
    in.current  = CADC_Calibrate_mv(adc_dma_buffer[3]);
    out.current = CADC_Calibrate_mv(adc_dma_buffer[4]);

    //
    flag = 1;
    // 拟合
    in.voltage  = in.voltage * 1.0f + 0.0f;
    out.voltage = out.voltage * 1.0f + 0.0f;
    in.current  = (in.current - 1650) * 1.0f + 0.0f;
    out.current = (out.current - 1650) * 1.0f + 0.0f;

    // 计算功率
    in.power  = in.voltage * in.current;
    out.power = out.voltage * out.current;


    //
    //        // 控制输出
    //        float pwm = CV_CC_Update(&cv_cc, out.voltage, out.current, target_voltage, target_current);
    //        set_pwm(pwm);
}
float Ui_min = 9999;
float Ii_min = 9999;
float Uo_min = 9999;
float Io_min = 9999;

float Ui_max = 0;
float Ii_max = 0;
float Uo_max = 0;
float Io_max = 0;
void repetition_event_cbk(HRTIM_Timer_Index_t TimerIdx) {
    if (TimerIdx == HRTIM_TIMERINDEX_MASTER) {
        static int i = 0;

        pwm = SPWM_Update_Open(&spwm);
        set_pwm(pwm);
        // CADC_Start_DMA();
        // CADC_Calibrate_VDDA();
        // in.voltage  = CADC_Calibrate_mv(adc_dma_buffer[1]);
        // out.voltage = CADC_Calibrate_mv(adc_dma_buffer[2]);
        // in.current  = CADC_Calibrate_mv(adc_dma_buffer[3]);
        // out.current = CADC_Calibrate_mv(adc_dma_buffer[4]);

        // Ui_max = max(Ui_max, in.voltage);
        // Ii_max = max(Ii_max, in.current);
        // Uo_max = max(Uo_max, out.voltage);
        // Io_max = max(Io_max, out.current);
        // Ui_min = min(Ui_min, in.voltage);
        // Ii_min = min(Ii_min, in.current);
        // Uo_min = min(Uo_min, out.voltage);
        // Io_min = min(Io_min, out.current);


        if (i >= HALF_FREQ) {
            HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
            i = 0;
        }
        ++i;
    }
}
void cmp4_cbk(HRTIM_Timer_Index_t TimerIdx) {
    if (TimerIdx == HRTIM_TIMERINDEX_MASTER) {
        static int i = 0;

        ++i;
        if (i < HALF_FREQ) {
            return;
        }
        i = 0;
    }
}


bool flag_key1 = false;
bool flag_key2 = false;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == KEY1_ADD_Pin) {
        flag_key1 = true;
    }
    else if (GPIO_Pin == KEY2_SUB_Pin) {
        flag_key2 = true;
    }
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

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
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_HRTIM1_Init();
    MX_UART5_Init();
    MX_I2C1_Init();
    /* USER CODE BEGIN 2 */


    ADC_IT_Callbacks_t cbks = {.conv_cplt_cbk = ADC_IT_cbks, .conv_half_cplt_cbk = NULL};
    ADC_IT_Callbacks_Register(&cbks);

    CADC_Calibration_Start();
    CADC_Start_DMA();

    CHRTIM_Timer_Start(HRTIM_TIMERID_MASTER | HRTIM_TIMERID_TIMER_A | HRTIM_TIMERID_TIMER_B);
    CHRTIM_Output_Start(HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2 | HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TB2);


    SPWM_Init(&spwm, (Float_t)FREQ, GRID_FREQUENCY, 0.8);

    HRTIM_IT_Callbacks_t hrtimcbks = {.repetition_event_cbk = repetition_event_cbk, .compare4_cbk = cmp4_cbk};
    CHRTIM_IT_Callbacks_Register(&hrtimcbks);

    CHRTIM_Master_Enable_IT(HRTIM_MDIER_MREPIE | HRTIM_MDIER_MCMP4IE);

    // Sin_Analyzer_Init(&in.sin_analyzer, FREQ, 40, 60);
    // Sin_Analyzer_Init(&out.sin_analyzer, FREQ, 40, 60);

    // HAL_Delay(1000);
    // OLED_Init();
    // OLED_Clear_all();
    // OLED_Update();


    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    	Float_t a[]={1,2,3};
        Vofa_JustFloat((Float_t*)&a, 3);

        // OLED_Print_16x8_px(0, 0, "Ui_min: %f", Ui_min);
        // OLED_Print_16x8_px(0, 16, "Ui_max: %f", Ui_max);
        // OLED_Print_16x8_px(0, 32, "Uo_min: %f", Uo_min);
        // OLED_Print_16x8_px(0, 48, "Uo_max: %f", Uo_max);

        // OLED_Update();
        // OLED_Clear_all();
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM            = RCC_PLLM_DIV2;
    RCC_OscInitStruct.PLL.PLLN            = 25;
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ            = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR            = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
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
void assert_failed(uint8_t* file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
