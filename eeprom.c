/*!****************************************************************************
 * @file
 * eeprom.c
 *
 * @brief
 * AT24C64 EEPROM access via I2C2
 *
 * @date  03.03.2022
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include "ch32v10x.h"
#include "eeprom.h"


/*- Macros -------------------------------------------------------------------*/
/*! @brief AT24C64 I2C Device Address                                         */
#define EEPROM_ADDR                   0xA0


/*!****************************************************************************
 * @brief
 * Blocking read of data from EEPROM
 *
 * @param[out] *aucBuffer Buffer for received data
 * @param[in] uAddress    Start address for read operation
 * @param[in] uLength     Number of bytes to be read
 * @date  03.03.2022
 ******************************************************************************/
void vReadEeprom(unsigned char* aucBuffer, unsigned uAddress, unsigned uLength)
{
  /* Dummy write to set start address                     */
  while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) != RESET);
  I2C_AcknowledgeConfig(I2C2, ENABLE);
  I2C_GenerateSTART(I2C2, ENABLE);

  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(I2C2, EEPROM_ADDR, I2C_Direction_Transmitter);

  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  I2C_SendData(I2C2, uAddress >> 16);
  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  I2C_SendData(I2C2, uAddress);
  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Read consecutive bytes                               */
  I2C_GenerateSTART(I2C2, ENABLE);

  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(I2C2, EEPROM_ADDR, I2C_Direction_Receiver);

  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  while (uLength-- > 1)
  {
    I2C_AcknowledgeConfig(I2C2, ENABLE);
    while (I2C_GetFlagStatus(I2C2, I2C_FLAG_RXNE) != SET);
    *aucBuffer++ = I2C_ReceiveData(I2C2);
  }
  I2C_AcknowledgeConfig(I2C2, DISABLE);
  while (I2C_GetFlagStatus(I2C2, I2C_FLAG_RXNE) != SET);
  *aucBuffer++ = I2C_ReceiveData(I2C2);

  /* End of transfer                                      */
  I2C_GenerateSTOP(I2C2, ENABLE);
}

/*!****************************************************************************
 * @brief
 * Blocking write of data to EEPROM
 *
 * @note
 * Maximum block length is EEPROM page size. Data block shall not cross page
 * border.
 *
 * @param[in] *aucBuffer  Buffer containing write data
 * @param[in] uAddress    Start address for write operation
 * @param[in] uLength     Number of bytes to be written
 * @date  03.03.2022
 ******************************************************************************/
void vWriteEeprom(const unsigned char* aucBuffer, unsigned uAddress, unsigned uLength)
{
  /* Set start address                                    */
  while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY) != RESET);
  I2C_GenerateSTART(I2C2, ENABLE);

  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
  I2C_Send7bitAddress(I2C2, EEPROM_ADDR, I2C_Direction_Transmitter);

  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  I2C_SendData(I2C2, uAddress >> 16);
  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  I2C_SendData(I2C2, uAddress);
  while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Write data                                           */
  while (uLength-- > 0)
  {
    I2C_SendData(I2C2, *aucBuffer++);
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  }

  /* End of transfer                                      */
  I2C_GenerateSTOP(I2C2, ENABLE);
}
