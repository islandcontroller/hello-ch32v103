/*!****************************************************************************
 * @file
 * ch32v10x_it.c
 *
 * @brief
 * Application-defined interrupt handler definitions
 *
 * @note
 * In order to use the hardware stack feature, optimisation must be enabled in
 * the GNU RISC-V Cross C Compiler settings (e.g. -Og)
 *
 * @date  11.02.2022
 * @date  17.02.2022  Added SysTick dummy handler
 * @date  03.03.2022  Added optimisation hint attributes
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include "ch32v10x.h"


/*!****************************************************************************
 * @brief
 * Hard fault handler
 *
 * @date  11.02.2022
 * @date  03.03.2022  Added optimisation hint attribute
 ******************************************************************************/
RV_INTERRUPT void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/*!****************************************************************************
 * @brief
 * SysTick interrupt handler
 *
 * @date  17.02.2022
 * @date  03.03.2022  Added optimisation hint attribute
 ******************************************************************************/
RV_INTERRUPT void SysTick_Handler(void)
{
}
