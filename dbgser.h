/*!****************************************************************************
 * @file
 * dbgser.h
 *
 * @brief
 * Serial output for debugging (debugger serial port)
 *
 * @date  11.02.2022
 ******************************************************************************/

#ifndef DBGSER_H_
#define DBGSER_H_

/*- Exported functions -------------------------------------------------------*/
void vWriteDbgSer(const unsigned char* pucData, unsigned uLen);
void vPrintDbgSer(const char* pszStr);

#endif /* DBGSER_H_ */
