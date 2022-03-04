/*!****************************************************************************
 * @file
 * hw_i2c2.c
 *
 * @brief
 * Low-level initialisation for I2C2 (24C64 EEPROM)
 *
 * @date  03.03.2022
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include "ch32v10x.h"
#include "hw_i2c2.h"


/*!****************************************************************************
 * @brief
 * Activate and configure I2C2 peripheral in master mode
 *
 * @date  03.03.2022
 ******************************************************************************/
void vInitHW_I2C2(void)
{
  /* Enable peripheral clock                              */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

  /* Reset peripheral                                     */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);

  /* Configure I2C peripheral                             */
  I2C_InitTypeDef sInit = {
    .I2C_ClockSpeed = 100000,
    .I2C_Mode = I2C_Mode_I2C,
    .I2C_DutyCycle = I2C_DutyCycle_2,
    .I2C_Ack = I2C_Ack_Enable,
    .I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit
  };
  I2C_Init(I2C2, &sInit);
  I2C_Cmd(I2C2, ENABLE);
}
