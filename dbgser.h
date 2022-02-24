/*!****************************************************************************
 * @file
 * dbgser.h
 *
 * @brief
 * Serial output for debugging (debugger serial port)
 *
 * @date  11.02.2022
 * @date  23.02.2022  Added single-char write and blocking read
 ******************************************************************************/

#ifndef DBGSER_H_
#define DBGSER_H_

/*- Header files -------------------------------------------------------------*/
#include <stdbool.h>


/*- Exported functions -------------------------------------------------------*/
void vWriteDbgSer(const unsigned char* pucData, unsigned uLen);
void vPrintDbgSer(const char* pszStr);
void vPutCharDbgSer(char cData);
bool bIsDbgSerAvailable(void);
char cGetCharDbgSer(void);

#endif /* DBGSER_H_ */
