/*!***************************************************************************
 * @file
 * hw_gpio.c
 *
 * @brief
 * Low-level GPIO setup
 *
 * @date  11.02.2022
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include "ch32v10x.h"
#include "hw_iodefs.h"
#include "hw_gpio.h"


/*!****************************************************************************
 * @brief
 * Activate clocks and initialise GPIO pins
 *
 * @date  11.02.2022
 * @date  17.02.2022  Added TIM3CH1 output mapping
 * @date  23.02.2022  Modified USART pin mappings; added RX path
 ******************************************************************************/
void vInitHW_GPIO(void)
{
  /* Enable peripheral clock supplies for used GPIO       */
  RCC_APB2PeriphClockCmd(GPIO_USED_PERIPH, ENABLE);

  /* Initialise Pins                                      */
  GPIO_InitTypeDef sInitUSART1RTX = {
    .GPIO_Pin = USART1TX_GPIO_Pin | USART1RX_GPIO_Pin,
    .GPIO_Mode = USART1RTX_GPIO_Mode,
    .GPIO_Speed = GPIO_Speed_2MHz
  };
  GPIO_Init(USART1RTX_GPIO_Port, &sInitUSART1RTX);
  GPIO_InitTypeDef sInitTIM3CH1 = {
    .GPIO_Pin = TIM3CH1_GPIO_Pin,
    .GPIO_Mode = TIM3CH1_GPIO_Mode,
    .GPIO_Speed = GPIO_Speed_2MHz
  };
  GPIO_Init(TIM3CH1_GPIO_Port, &sInitTIM3CH1);
}
