/*!***************************************************************************
 * @file
 * hw_adc.c
 *
 * @brief
 * Low-level ADC setup
 *
 * @date  24.02.2022
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include "ch32v10x.h"
#include "hw_adc.h"


/*- Macros -------------------------------------------------------------------*/
/*! ADC power-on delay in SysTick timer counts                                */
#define ADC_TSTAB_TICKS               ((HSI_VALUE / 8) / 1000000) /* 1 us     */

/*! VDDA nominal voltage in mV                                                */
#define ADC_VDDA_NOM                  3300
//#define ADC_VDDA_NOM                5000

/*! Use calibration procedures (uncomment to activate)                        */
//#define USE_ADC_GAINOFFSET
//#define USE_ADC_CAL

/*! ADC gain and offset calibration values
 *  @{                                                                        */
#if defined(USE_ADC_CAL) && defined(USE_ADC_GAINOFFSET)
#if ADC_VDDA_NOM > 3900
/* 5V nominal VDDA                                                            */
#define ADC_CAL_GAIN                  4
#define ADC_CAL_OFFSET                -8
#else
/* 3.3V nominal VDDA                                                          */
#define ADC_CAL_GAIN                  6
#define ADC_CAL_OFFSET                -12
#endif /* ADC_VDDA_NOM */
#endif /* USE_ADC_CAL && USE_ADC_GAINOFFSET */
/*! @}                                                                        */

/*! ADC resolution in bits                                                    */
#define ADC_RES_BITS                  12

/*! ADC maximum output conversion value (12 bits)                             */
#define ADC_MAX_VAL                   ((1 << ADC_RES_BITS) - 1)

/*! Selected ADC sample time for software-triggered conversion                */
#define ADC_SAMPLE_TIME               ADC_SampleTime_239Cycles5


/*- Private variables --------------------------------------------------------*/
#ifdef USE_ADC_CAL
/*! ADC calibration value                                                     */
static uint16_t uiCalibrationValue;
#endif /* USE_ADC_CAL */


/*- Private functions --------------------------------------------------------*/
/*!****************************************************************************
 * @brief
 * Power-on delay (tSTAB), uses SysTick @ HCLK/8
 *
 * @date  24.02.2022
 ******************************************************************************/
static void vWait_tSTAB(void)
{
  uint32_t ulStartTime = SysTick_GetValueLow();
  uint32_t ulNow = ulStartTime;
  while (ulNow - ulStartTime < ADC_TSTAB_TICKS) ulNow = SysTick_GetValueLow();
}

#ifdef USE_ADC_CAL
/*!****************************************************************************
 * @brief
 * Apply calibration function to measured conversion value
 *
 * @note Original method seems to introduce additional offset and has been
 * deactivated.
 *
 * @param[in] uiConvVal   Measured raw conversion value
 * @return  (uint16_t)  Compensated conversion value
 * @date  24.02.2022
 ******************************************************************************/
static uint16_t uiApplyCalibration(uint16_t uiConvVal)
{
  /* Adjust with measured cal offset                      */
  int32_t lCalOffs = uiConvVal + uiCalibrationValue;

#ifdef USE_ADC_GAINOFFSET
  /* Adjust with ADC gain and offset                      */
  int32_t lGainOffs = ADC_CAL_GAIN * lCalOffs / 1000 + ADC_CAL_OFFSET;

  /* Calculate and saturate output conversion value       */
  int32_t lCalcVal = lCalOffs - lGainOffs;
  if      (lCalcVal < 0)           return 0;
  else if (lCalcVal > ADC_MAX_VAL) return ADC_MAX_VAL;
  else                             return (uint16_t)lCalcVal;
#else
  /* Saturate output conversion value                     */
  if      (lCalOffs < 0)           return 0;
  else if (lCalOffs > ADC_MAX_VAL) return ADC_MAX_VAL;
  else                             return (uint16_t)lCalOffs;
#endif /* USE_ADC_GAINOFFSET */
}
#endif /* USE_ADC_CAL */


/*!****************************************************************************
 * @brief
 * Initialise ADC peripheral
 *
 * @date  24.02.2022
 ******************************************************************************/
void vInitHW_ADC(void)
{
  /* Activate peripheral clock supply                     */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /* Set up base peripheral for software-triggered conver-
   * sion of a single channel                             */
  ADC_InitTypeDef sInit = {
    .ADC_ExternalTrigConv = ADC_ExternalTrigConv_None,
    .ADC_NbrOfChannel = 1
  };
  ADC_Init(ADC1, &sInit);

  /* Enable temperature sensor channel and wake up ADC from
   * power-down mode                                      */
  ADC_TempSensorVrefintCmd(ENABLE);
  ADC_Cmd(ADC1, ENABLE);

  /* Power-on delay                                       */
  vWait_tSTAB();

#ifdef USE_ADC_CAL
  /* Run calibration sequence                             */
  uiCalibrationValue = Get_CalibrationValue(ADC1);
#endif /* USE_ADC_CAL */
}

/*!****************************************************************************
 * @brief
 * Start software-triggered conversion and get compensated conversion value in
 * unit of millivolts.
 *
 * @param[in] ucChannel Selected ADC channel to start conversion on
 * @return  (uint16_t)  Compensated conversion value in mV
 * @date  24.02.2022
 ******************************************************************************/
uint16_t uiHW_GetAdcConversionValue_mV(uint8_t ucChannel)
{
  /* Configure selected channel and start software trig-
   * gered conversion                                     */
  ADC_RegularChannelConfig(ADC1, ucChannel, 1, ADC_SAMPLE_TIME);
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);

  /* Wait for conversion to finish and fetch result from
   * RDATA register                                       */
  while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) != SET);
  uint16_t uiConvVal = ADC_GetConversionValue(ADC1);

  /* Apply calibration compensation and convert to milli-
   * volts                                                */
#ifdef USE_ADC_CAL
  return (ADC_VDDA_NOM * uiApplyCalibration(uiConvVal)) >> ADC_RES_BITS;
#else
  return (ADC_VDDA_NOM * uiConvVal) >> ADC_RES_BITS;
#endif /* USE_ADC_CAL */
}
