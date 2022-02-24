/*!***************************************************************************
 * @file
 * hw_adc.h
 *
 * @brief
 * Low-level ADC setup
 *
 * @date  24.02.2022
 ******************************************************************************/

#ifndef HW_ADC_H_
#define HW_ADC_H_

/*- Header files -------------------------------------------------------------*/
#include <stdint.h>


/*- Exported functions -------------------------------------------------------*/
void vInitHW_ADC(void);
uint16_t uiHW_GetAdcConversionValue_mV(uint8_t ucChannel);

#endif /* HW_ADC_H_ */
