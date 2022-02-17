/*!****************************************************************************
 * @file
 * hw_init.c
 *
 * @brief
 * Top-Level hardware Init module
 *
 * @date  11.02.2022
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include <string.h>
#include "hw_stk.h"
#include "hw_gpio.h"
#include "hw_usart1.h"
#include "hw_tim3.h"


/*!****************************************************************************
 * @brief
 * Top-level call to initialise all used hardware modules
 *
 * @date  11.02.2022
 ******************************************************************************/
void vInitHW(void)
{
  vInitHW_STK();
  vInitHW_GPIO();
  vInitHW_USART1();
  vInitHW_TIM3();

  SystemCoreClockUpdate();
}
