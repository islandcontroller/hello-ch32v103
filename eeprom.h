/*!****************************************************************************
 * @file
 * eeprom.h
 *
 * @brief
 * AT24C64 EEPROM access via I2C2
 *
 * @date  03.03.2022
 ******************************************************************************/

#ifndef EEPROM_H_
#define EEPROM_H_

/*- Macros -------------------------------------------------------------------*/
/*! Page size in Bytes                                                        */
#define EEPROM_PAGE_SIZE              32


/*- Exported functions -------------------------------------------------------*/
void vReadEeprom(unsigned char* aucBuffer, unsigned uAddress, unsigned uLength);
void vWriteEeprom(const unsigned char* aucBuffer, unsigned uAddress, unsigned uLength);

#endif /* EEPROM_H_ */
