/*!****************************************************************************
 * @file
 * main.c
 *
 * @brief
 * "Hello World"-like demo project for the CH32V103 RISC-V MCU.
 *
 * This project contains a simple set of modules to get the MCU running in a
 * minimal configuration:
 *  - Serial output on USART1 (connected to WCH-Link VCP)
 *  - SysTick enabled and using empty dummy interrupt handler
 *  - TIM3 PWM output to LED
 * All project files are also available online at:
 *  https://github.com/islandcontroller/hello-ch32v103
 *
 * @date  11.02.2022
 * @date  14.02.2022  Added core information printout
 * @date  17.02.2022  Added clocks, ESig information printout
 * @date  17.02.2022  Added LED animation
 ******************************************************************************/


/*- Header files -------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "ch32v10x.h"
#include "hw_init.h"
#include "dbgser.h"
#include "led.h"


/*- Macros -------------------------------------------------------------------*/
/*! @brief Output line buffer size in bytes (incl. trailing zero)             */
#define LINE_BUF_LEN                  53


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


/*- Private functions --------------------------------------------------------*/
/*!****************************************************************************
 * @brief
 * Print plain-text info from MISA

 * @date  12.02.2022
 ******************************************************************************/
static void vPrintCoreInfo(void)
{
  char acLine[LINE_BUF_LEN];

  uint32_t ulMArchId = __get_MARCHID();
  uint32_t ulMVendorId = __get_MVENDORID();
  uint32_t ulMImpId = __get_MIMPID();
  uint32_t ulMISA = __get_MISA();

  vPrintDbgSer(
    "-- Core Information ------------------------------\r\n"
  );

  /* Print register values                                */
  sprintf(acLine, "MARCHID:   0x%08lX\r\n", ulMArchId);
  vPrintDbgSer(acLine);
  sprintf(acLine, "MIMPID:    0x%08lX\r\n", ulMImpId);
  vPrintDbgSer(acLine);
  sprintf(acLine, "MVENDORID: 0x%08lX\r\n", ulMVendorId);
  vPrintDbgSer(acLine);
  sprintf(acLine, "MISA:      0x%08lX\r\n", ulMISA);
  vPrintDbgSer(acLine);

  /* Print MXL configuration                              */
  unsigned uMxl = (ulMISA >> 30) & 0x3UL;
  vPrintDbgSer("  MXL:\r\n    ");
  vPrintDbgSer(apszMisaMxl[uMxl]);
  vPrintDbgSer("\r\n");

  /* Print extensions information                         */
  vPrintDbgSer("  Extensions:\r\n");
  for (unsigned i = 0; i < 26; ++i)
  {
    if (ulMISA & (1UL << i))
    {
      vPrintDbgSer("    ");
      vPrintDbgSer(apszMisaExt[i]);
      vPrintDbgSer("\r\n");
    }
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
 ******************************************************************************/
static void vPrintSysCoreClk(void)
{
  char acLine[LINE_BUF_LEN];

  vPrintDbgSer(
    "-- Clocks ----------------------------------------\r\n"
  );

  unsigned uKHz = SystemCoreClock / 1000;
  unsigned uMHz = uKHz / 1000;
  unsigned uKHzRem = uKHz % 1000;
  sprintf(acLine, "f_HCLK = %d.%03d MHz\r\n", uMHz, uKHzRem);
  vPrintDbgSer(acLine);
}

/*!****************************************************************************
 * @brief
 * Print flash size and device ID information
 *
 * @param[in] uiFlSize    Flash memory size from ESIG
 * @param[in] aulUID      Unique device ID from ESIG
 * @date  17.02.2022
 ******************************************************************************/
static void vPrintEsigInfo(void)
{
  const volatile uint16_t* puiFlSize = (const void*)0x1FFFF7E0UL;
  const volatile uint32_t* pulUID = (const void*)0x1FFFF7E8UL;

  char acLine[LINE_BUF_LEN];

  vPrintDbgSer(
    "-- ESIG ------------------------------------------\r\n"
  );

  sprintf(acLine, "FLASH Size: %d KB\r\n", *puiFlSize);
  vPrintDbgSer(acLine);
  sprintf(acLine, "Unique ID: %08lX %08lX %08lX\r\n", pulUID[2], pulUID[1], pulUID[0]);
  vPrintDbgSer(acLine);
}


/*!****************************************************************************
 * @brief
 * Main program entry point
 *
 * @date  11.02.2022
 * @date  14.02.2022  Added core information printout
 * @date  17.02.2022  Added clocks, ESig information printout
 * @date  17.02.2022  Added LED animation
 ******************************************************************************/
int main(void)
{
  vInitHW();
  vInitLed();

  vPrintDbgSer(
    "\x1b[2J"
    "--------------------------------------------------\r\n"
    "        ##                                        \r\n"
    "\r\n"
    "      ##    ######                                \r\n"
    "     ##  ##                                       \r\n"
    "    ##  ##                              17.02.2022\r\n"
    "   ##    ######  ######                  @islandc_\r\n"
    "--------------------------------------------------\r\n"
    " MCU:  WCH CH32V103R8T6                           \r\n"
    " Core: RISC-V3A (RV32IMAC)                        \r\n"
    "--------------------------------------------------\r\n"
    "\r\n"
  );
  vPrintCoreInfo();
  vPrintDbgSer("\r\n");
  vPrintSysCoreClk();
  vPrintDbgSer("\r\n");
  vPrintEsigInfo();

  while (1)
  {
    vPollLed();
  }
}
