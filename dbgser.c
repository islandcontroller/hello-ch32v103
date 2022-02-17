/*!****************************************************************************
 * @file
 * dbgser.h
 *
 * @brief
 * Serial output for debugging (debugger serial port)
 *
 * @date  11.02.2022
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include "ch32v10x.h"
#include "dbgser.h"


/*!****************************************************************************
 * @brief
 * Write data to serial debug output
 *
 * @param[in] *pucData    Data buffer
 * @param[in] uLen        Data length in bytes
 * @date  12.02.2022
 ******************************************************************************/
void vWriteDbgSer(const unsigned char* pucData, unsigned uLen)
{
  for (unsigned i = 0; i < uLen; ++i)
  {
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
    USART_SendData(USART1, pucData[i]);
  }
}

/*!****************************************************************************
 * @brief
 * Print null-terminated string to serial debug output
 *
 * @param[in] *pszStr     Null-terminated string
 * @date  12.02.2022
 ******************************************************************************/
void vPrintDbgSer(const char* pszStr)
{
  while (*pszStr != '\0')
  {
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
    USART_SendData(USART1, *pszStr++);
  }
}
