/*!****************************************************************************
 * @file
 * hw_init.c
 *
 * @brief
 * Top-Level hardware Init module
 *
 * @date  11.02.2022
 * @date  24.02.2022  Added ADC init
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include <string.h>
#include "ch32v10x.h"
#include "hw_stk.h"
#include "hw_gpio.h"
#include "hw_usart1.h"
#include "hw_tim3.h"
#include "hw_adc.h"


/*!****************************************************************************
 * @brief
 * Top-level call to initialise all used hardware modules
 *
 * @date  11.02.2022
 * @date  24.02.2022  Added ADC init
 ******************************************************************************/
void vInitHW(void)
{
  vInitHW_STK();
  vInitHW_GPIO();
  vInitHW_USART1();
  vInitHW_TIM3();
  vInitHW_ADC();

  SystemCoreClockUpdate();
}
