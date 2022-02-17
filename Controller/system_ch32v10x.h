/*!****************************************************************************
 * @file
 * system_ch32v10x.c
 *
 * @brief
 * Early system initialisation
 *
 * @date  16.02.2022
 ******************************************************************************/

#ifndef SYSTEM_CH32V10X_H_
#define SYSTEM_CH32V10X_H_

/*- Header files -------------------------------------------------------------*/
#include <stdint.h>


/*- Global variables ---------------------------------------------------------*/
/*! Calculated SYSCLK frequency in Hz                                         */
extern volatile uint32_t SystemCoreClock;


/*- Exported methods ---------------------------------------------------------*/
void SystemInit(void);
void SystemCoreClockUpdate(void);

#endif /* SYSTEM_CH32V10X_H_ */



