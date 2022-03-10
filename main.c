/*!****************************************************************************
 * @file
 * main.c
 *
 * @brief
 * "Hello World"-like demo project for the CH32V103 RISC-V MCU.
 *
 * This project contains a simple set of modules to get the MCU running in a
 * minimal configuration:
 *  - Polling-mode serial I/O on USART1 (connected to WCH-Link VCP)
 *  - SysTick enabled and using empty dummy interrupt handler
 *  - TIM3 PWM output to LED
 *  - ADC1 internal temperature sensor and Vrefint readout
 *  - I2C2 for 24C64 EEPROM read/write
 * All project files are also available online at:
 *  https://github.com/islandcontroller/hello-ch32v103
 *
 * @date  11.02.2022
 * @date  14.02.2022  Added core information printout
 * @date  17.02.2022  Added clocks, ESig information printout
 * @date  17.02.2022  Added LED animation
 * @date  23.02.2022  Added remote echo and serial input commands processing
 * @date  24.02.2022  Added temperature sensor and Vrefint info printout
 * @date  03.03.2022  Modified to used printf(), putchar() and getchar()
 * @date  04.03.2022  Added EEPROM demo
 * @date  10.03.2022  Added information block readout; Disabled EEPROM demo for
 *                    default configuration
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ch32v10x.h"
#include "hw_init.h"
#include "hw_adc.h"
#include "syscalls.h"
#include "dbgser.h"
#include "led.h"
#include "eeprom.h"


/*- Macros -------------------------------------------------------------------*/
/*! @brief Enable 24C64 EEPROM demo                                           */
//#define USE_EEPROM_DEMO

/*! @brief Number of bytes to be read for EEPROM hexdump                      */
#define EEPROM_NUM_BYTES              256

/*! @brief EEPROM Hexdump items per row                                       */
#define HEXDUMP_ROW_ITEMS             16


/*- Private variables --------------------------------------------------------*/
/*! String lookup for XLEN definition field                                   */
const char* const apszMisaMxl[4] = {
  "0 - (undefined)",
  "1 - 32-bit XLEN",
  "2 - 64-bit XLEN",
  "3 - 128-bit XLEN"
};

/*! Lookup table for ISA Extensions                                           */
const char* const apszMisaExt[26] = {
  "A - Atomic extension",
  "B - (reserved) Bit-Manipulation extension",
  "C - Compressed extension",
  "D - Double-precision floating-point extension",
  "E - RV32E base ISA",
  "F - Single-precision floating-point extension",
  "G - Additional standard extensions present",
  "H - Hypervisor extension",
  "I - RV32I/RV64I/RV128I base ISA",
  "J - (reserved) Dynamically Translated Languages extension",
  "K - (reserved)",
  "L - (reserved) Decimal Floating-Point extension",
  "M - Integer Multiply/Divide extension",
  "N - User-level interrupts supported",
  "O - (reserved)",
  "P - (reserved) Packed-SIMD extension",
  "Q - Quad-precision floating-point extension",
  "R - (reserved)",
  "S - Supervisor mode implemented",
  "T - (reserved) Transactional Memory extension",
  "U - User mode implemented",
  "V - (reserved) Vector extension",
  "W - (reserved)",
  "X - Non-standard extensions present",
  "Y - (reserved)",
  "Z - (reserved)"
};

#ifdef USE_EEPROM_DEMO
/*! EEPROM demo data to be programmed                                         */
const char* const pszEepromData = "CH32V103 I2C Demo";
#endif /* USE_EEPROM_DEMO */


/*- Private functions --------------------------------------------------------*/
/*!****************************************************************************
 * @brief
 * Print plain-text info from MISA

 * @date  12.02.2022
 * @date  03.03.2022  Modified to use printf()
 ******************************************************************************/
static void vPrintCoreInfo(void)
{
  uint32_t ulMArchId = __get_MARCHID();
  uint32_t ulMVendorId = __get_MVENDORID();
  uint32_t ulMImpId = __get_MIMPID();
  uint32_t ulMISA = __get_MISA();

  printf(
    "-- Core Information ------------------------------\r\n"
  );

  /* Print register values                                */
  printf("MARCHID:   0x%08lX\r\n", ulMArchId);
  printf("MIMPID:    0x%08lX\r\n", ulMImpId);
  printf("MVENDORID: 0x%08lX\r\n", ulMVendorId);
  printf("MISA:      0x%08lX\r\n", ulMISA);

  /* Print MXL configuration                              */
  unsigned uMxl = (ulMISA >> 30) & 0x3UL;
  printf("  MXL:\r\n    %s\r\n", apszMisaMxl[uMxl]);

  /* Print extensions information                         */
  printf("  Extensions:\r\n");
  for (unsigned i = 0; i < 26; ++i)
  {
    if (ulMISA & (1UL << i)) printf("    %s\r\n", apszMisaExt[i]);
  }
}

/*!****************************************************************************
 * @brief
 * Print plain-text system core clock information
 *
 * @note
 * Run SystemCoreClockUpdate() at least once before calling this function to
 * ensure HCLK value recalculation.
 *
 * @date  14.02.2022
 * @date  03.03.2022  Modified to use printf()
 ******************************************************************************/
static void vPrintSysCoreClk(void)
{
  printf(
    "-- Clocks ----------------------------------------\r\n"
  );

  unsigned uKHz = SystemCoreClock / 1000;
  unsigned uMHz = uKHz / 1000;
  unsigned uKHzRem = uKHz % 1000;
  printf("f_HCLK = %d.%03d MHz\r\n", uMHz, uKHzRem);
}

/*!****************************************************************************
 * @brief
 * Print flash size and device ID information
 *
 * @date  17.02.2022
 * @date  03.03.2022  Modified to use printf()
 ******************************************************************************/
static void vPrintEsigInfo(void)
{
  const volatile uint16_t* puiFlSize = (const void*)0x1FFFF7E0UL;
  const volatile uint32_t* pulUID = (const void*)0x1FFFF7E8UL;

  printf(
    "-- ESIG ------------------------------------------\r\n"
  );

  printf("FLASH Size: %d KB\r\n", *puiFlSize);
  printf("Unique ID: %08lX %08lX %08lX\r\n", pulUID[2], pulUID[1], pulUID[0]);
}

/*!****************************************************************************
 * @brief
 * Print analog inputs info
 *
 * @date  24.02.2022
 * @date  03.03.2022  Modified to use printf()
 ******************************************************************************/
static void vPrintAnalogInfo(void)
{
  /* Temperature sensor values                            */
  int32_t lVoltageTS = uiHW_GetAdcConversionValue_mV(ADC_Channel_TempSensor);
  int32_t lTemperature = TempSensor_Volt_To_Temper(lVoltageTS);
  printf("Temp sensor: %ld mV, %ld degC", lVoltageTS, lTemperature);
  if ((lTemperature < 10) || (lTemperature > 50)) printf(" (invalid?)");

  /* Internal voltage reference                           */
  int32_t lVoltageVref = uiHW_GetAdcConversionValue_mV(ADC_Channel_Vrefint);
  printf("\r\nVrefint: %ld mV\r\n", lVoltageVref);
}

/*!****************************************************************************
 * @brief
 * Print Hexdump of memory buffer
 *
 * @param[in] *pBuffer    Data buffer
 * @param[in] uLen        Number of bytes to display
 * @param[in] uBaseAdr    Base address for row counters
 * @date  10.03.2022
 ******************************************************************************/
static void vPrintHexDump(const uint8_t* pBuffer, unsigned uLen, unsigned uBaseAdr)
{
  for (unsigned uRow = 0; uRow < uLen / HEXDUMP_ROW_ITEMS; ++uRow)
  {
    /* Address or Offset                                  */
    unsigned uRowAddr = uRow * HEXDUMP_ROW_ITEMS;
    printf("%08x  ", uRowAddr + uBaseAdr);

    /* Byte columns                                       */
    for (unsigned uCol = 0; uCol < HEXDUMP_ROW_ITEMS; ++uCol)
    {
      unsigned char ucData = pBuffer[uRowAddr + uCol];
      printf("%02x ", ucData);
      if (uCol == (HEXDUMP_ROW_ITEMS / 2 - 1)) putchar(' ');
    }

    /* ASCII text representation                          */
    putchar(' ');
    for (unsigned uCol = 0; uCol < HEXDUMP_ROW_ITEMS; ++uCol)
    {
      unsigned char ucData = pBuffer[uRowAddr + uCol];
      putchar(isprint(ucData) ? ucData : '.');
    }
    printf("\r\n");
  }
}

#ifdef USE_EEPROM_DEMO
/*!****************************************************************************
 * @brief
 * Print EEPROM hexdump
 *
 * @date  04.03.2022
 * @date  10.03.2022  Moved hexdump printout into separate routine
 ******************************************************************************/
static void vPrintEepromData(void)
{
  unsigned char aucBuffer[EEPROM_NUM_BYTES];

  /* Time readout into buffer                             */
  printf("Reading EEPROM... ");
  unsigned uStart = SysTick_GetValueLow();
  vReadEeprom(aucBuffer, 0, EEPROM_NUM_BYTES);
  unsigned uDuration = (SysTick_GetValueLow() - uStart) / ((HSI_VALUE / 8) / 1000);
  printf("done. Read %d bytes in %d ms.\r\n", EEPROM_NUM_BYTES, uDuration);

  /* Hexdump printout                                     */
  vPrintHexDump(aucBuffer, EEPROM_NUM_BYTES, 0);
}
#endif /* USE_EEPROM_DEMO */

/*!****************************************************************************
 * @brief
 * Print User Sel. and Vendor Config. Words
 *
 * @date  10.03.2022
 ******************************************************************************/
static void vPrintInfoBlockWords(void)
{
  const void* pUserSelWord = (const void*)0x1FFFF800UL;
  const void* pVendConfWord = (const void*)0x1FFFF880UL;

  /* Hexdump printout                                     */
  printf("User selection word:\r\n");
  vPrintHexDump(pUserSelWord, 128, (unsigned)pUserSelWord);
  printf("Vendor configuration word:\r\n");
  vPrintHexDump(pVendConfWord, 128, (unsigned)pVendConfWord);
}

/*!****************************************************************************
 * @brief
 * Serial input processing
 *
 * @date  23.02.2022
 * @date  24.02.2022  Changed NVIC naming convention
 * @date  24.02.2022  Added analog inputs readout command; modified help text
 * @date  03.03.2022  Modified to use printf(), putchar(), getchar()
 * @date  04.03.2022  Added EEPROM readout demo
 * @date  10.03.2022  Added information block readout command
 ******************************************************************************/
static void vPollSerial(void)
{
  /* Early exit, if no data is available                  */
  if (!bIsDbgSerAvailable()) return;

  /* Fetch character and print remote echo                */
  char c = getchar();
  printf("%c\r\n", c);

  /* Process command                                      */
  switch (c)
  {
    case '?':
      /* Show available commands                          */
      printf(
        "Available Commands:\r\n"
        "  ?    Show this help\r\n"
        "  a    Print analog inputs info\r\n"
#ifdef USE_EEPROM_DEMO
        "  e    Read EEPROM\r\n"
#endif /* USE_EEPROM_DEMO */
        "  i    Read information block\r\n"
        "  r    Reboot system\r\n"
      );
      break;

    case 'a':
      /* Print analog inputs info                         */
      vPrintAnalogInfo();
      break;

#ifdef USE_EEPROM_DEMO
    case 'e':
      /* Print EEPROM hexdump                             */
      vPrintEepromData();
      break;
#endif /* USE_EEPROM_DEMO */

    case 'i':
      /* Read information block                           */
      vPrintInfoBlockWords();
      break;

    case 'r':
      /* Reboot command                                   */
      PFIC_SystemReset();
      break;

    default:
      fprintf(stderr, "Unknown command. Press \"?\" to show available commands.\r\n");
  }

  /* Input prompt                                         */
  putchar('>');
  fflush(stdout);
}


/*!****************************************************************************
 * @brief
 * Main program entry point
 *
 * @date  11.02.2022
 * @date  14.02.2022  Added core information printout
 * @date  17.02.2022  Added clocks, ESig information printout
 * @date  17.02.2022  Added LED animation
 * @date  23.02.2022  Added serial input processing
 * @date  24.02.2022  Added help prompt
 * @date  03.03.2022  Modified to use printf()
 * @date  03.03.2022  Moved escape sequence into dbgser macro
 * @date  04.03.2022  Added EEPROM programming
 ******************************************************************************/
int main(void)
{
  vInitHW();
  vInitLed();

  /* Init syscalls retargeting                            */
  vInitSyscalls();

  /* Print system info                                    */
  printf(
    VT100_CLEAR_TERM
    "--------------------------------------------------\r\n"
    "        ##                                        \r\n"
    "\r\n"
    "      ##    ######                                \r\n"
    "     ##  ##                                       \r\n"
    "    ##  ##                                        \r\n"
    "   ##    ######  ######                  @islandc_\r\n"
    "--------------------------------------------------\r\n"
    " MCU:  WCH CH32V103R8T6                           \r\n"
    " Core: RISC-V3A (RV32IMAC)                        \r\n"
    "--------------------------------------------------\r\n"
    "\r\n"
  );
  vPrintCoreInfo();
  printf("\r\n");
  vPrintSysCoreClk();
  printf("\r\n");
  vPrintEsigInfo();
#ifdef USE_EEPROM_DEMO
  printf("\r\nWriting EEPROM... ");
  vWriteEeprom((const unsigned char*)pszEepromData, 0, strlen(pszEepromData));
  printf("done.");
#endif /* USE_EEPROM_DEMO */
  printf("\r\nPress \"?\" to show available commands.\r\n>");
  fflush(stdout);

  /* Main program loop                                    */
  while (1)
  {
    vPollLed();
    vPollSerial();
  }
}
