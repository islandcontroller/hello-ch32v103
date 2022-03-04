/*!****************************************************************************
 * @file
 * hw_init.c
 *
 * @brief
 * Top-Level hardware Init module
 *
 * @date  11.02.2022
 * @date  24.02.2022  Added ADC init
 * @date  03.03.2022  Added I2C2 init
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include <string.h>
#include "ch32v10x.h"
#include "hw_stk.h"
#include "hw_gpio.h"
#include "hw_usart1.h"
#include "hw_tim3.h"
#include "hw_adc.h"
#include "hw_i2c2.h"


/*!****************************************************************************
 * @brief
 * Top-level call to initialise all used hardware modules
 *
 * @date  11.02.2022
 * @date  24.02.2022  Added ADC init
 * @date  03.03.2022  Added I2C2 init
 ******************************************************************************/
void vInitHW(void)
{
  vInitHW_STK();
  vInitHW_GPIO();
  vInitHW_USART1();
  vInitHW_TIM3();
  vInitHW_ADC();
  vInitHW_I2C2();

  SystemCoreClockUpdate();
}
