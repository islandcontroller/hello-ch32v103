/*!****************************************************************************
 * @file
 * hw_iodefs.h
 *
 * @brief
 * I/O Pinout definitions
 *
 * @date  11.02.2022
 * @date  17.02.2022  Added TIM3CH1 output mapping
 * @date  23.02.2022  Added USART1RX mapping, combined RX/TX defines
 * @date  03.03.2022  Fixed USART1RX mode configuration to Input w/ Pull-Up
 ******************************************************************************/

#ifndef HW_IODEFS_H_
#define HW_IODEFS_H_

/*- Header files -------------------------------------------------------------*/
#include "ch32v10x.h"


/*- Macros -------------------------------------------------------------------*/
/*! @brief Used GPIO peripherals bitmask                                      */
#define GPIO_USED_PERIPH              ( RCC_APB2Periph_GPIOA | 0UL )

/*! @brief PA6: Timer 3 Channel 1 PWM Output to LED
 *  @{                                                                        */
#define TIM3CH1_GPIO_Port             GPIOA
#define TIM3CH1_GPIO_Pin              GPIO_Pin_6
#define TIM3CH1_GPIO_Mode             GPIO_Mode_AF_PP
/*! @}                                                                        */

/*! @brief PA9/10: USART1 RX/TX to debugger serial port
 *  @{                                                                        */
#define USART1RTX_GPIO_Port           GPIOA
#define USART1TX_GPIO_Pin             GPIO_Pin_9
#define USART1RX_GPIO_Pin             GPIO_Pin_10
#define USART1TX_GPIO_Mode            GPIO_Mode_AF_PP
#define USART1RX_GPIO_Mode            GPIO_Mode_IPU
/*! @}                                                                        */

#endif /* HW_IODEFS_H_ */
