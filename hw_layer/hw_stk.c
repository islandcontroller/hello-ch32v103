/*!****************************************************************************
 * @file
 * hw_stk.c
 *
 * @brief
 * Low-level SysTick configuration
 *
 * @date  17.02.2022
 * @date  18.02.2022  Modified STK access functions
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include "ch32v10x.h"


/*!****************************************************************************
 * @brief
 * Enable SysTick-Timer
 *
 * @date  17.02.2022
 * @date  18.02.2022  Modified access functions
 * @date  24.02.2022  Changed SysTick naming convention
 ******************************************************************************/
void vInitHW_STK(void)
{
  SysTick_Cmd(ENABLE);
}
