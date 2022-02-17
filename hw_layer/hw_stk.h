/*!****************************************************************************
 * @file
 * hw_stk.h
 *
 * @brief
 * Low-level SysTick configuration
 *
 * @date  17.02.2022
 ******************************************************************************/

#ifndef HW_STK_H_
#define HW_STK_H_

/*- Header files -------------------------------------------------------------*/
#include <stdint.h>


/*- Exported functions -------------------------------------------------------*/
void vInitHW_STK(void);
uint32_t ulGetTicks(void);

#endif /* HW_STK_H_ */
