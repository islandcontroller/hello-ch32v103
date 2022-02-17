/*!****************************************************************************
 * @file
 * ch32v10x_it.h
 *
 * @brief
 * Application-defined interrupt handler definitions
 *
 * @date  11.02.2022
 * @date  17.02.2022  Added SysTick dummy handler
 ******************************************************************************/

#ifndef CH32V10X_IT_H_
#define CH32V10X_IT_H_

/*- Exported functions -------------------------------------------------------*/
void HardFault_Handler(void);
void SysTick_Handler(void);

#endif /* CH32V10X_IT_H_ */
