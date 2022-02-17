/*!****************************************************************************
 * @file
 * system_ch32v10x.c
 *
 * @brief
 * Early system initialisation
 *
 * @date  16.02.2022
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include "ch32v10x.h"
#include "system_ch32v10x.h"


/*- Compiler options ---------------------------------------------------------*/
#pragma GCC push_options
#pragma GCC optimize("O1")


/*- Global variables ---------------------------------------------------------*/
/*! Calculated SYSCLK frequency in Hz                                         */
volatile uint32_t SystemCoreClock;


/*- Private variables --------------------------------------------------------*/
/*! AHB Prescaler Shift value look-up table for HPRE config                   */
static const uint8_t aucPrescShf[16] = { [0 ... 7] = 0, 1, 2, 3, 4, 6, 7, 8, 9 };


/*- Private functions --------------------------------------------------------*/
/*!****************************************************************************
 * @brief
 * Determine SYSCLK base clock frequency for PLL configuration
 *
 * @param[in] uCfgR0      Cached RCC_CFGR0 value
 * @return  (unsigned)  Base clock frequency in Hz
 * @date  16.02.2022
 ******************************************************************************/
static inline unsigned uCalcPllBaseClk(unsigned uCfgR0)
{
  unsigned uInptFreq;
  unsigned uMul = ((uCfgR0 & RCC_PLLMULL) >> 18) + 2;

  switch (uCfgR0 & RCC_PLLSRC)
  {
    case RCC_PLLSRC_HSE:      uInptFreq = HSE_VALUE >> !!(uCfgR0 & RCC_PLLXTPRE_HSE_Div2); break;
    case RCC_PLLSRC_HSI_Div2: uInptFreq = HSI_VALUE;                                       break;
    default: __builtin_unreachable();
  }

  return uInptFreq * uMul;
}


/*!****************************************************************************
 * @brief
 * Early system init
 *
 * Initialises minimum-config for clocking tree
 *
 *        8 MHz                             8 MHz        1 MHz
 *   HSI ------> SYSCLK ---[ /1 ]---> HCLK ---+---[ /8 ]--------------> SysTick
 *                                            |          8 MHz
 *                                            +---[ /1 ]--------------> PCLK1
 *                                            |          8 MHz
 *                                            +---[ /1 ]---+----------> PCLK2
 *                                                         |        4 MHz
 *                                                         +--[ /2 ]--> ADC
 *
 * References:
 *  [1] CH32xRM "CH32x103 Reference Manual", p. 19ff
 *      URL: http://www.wch-ic.com/downloads/CH32xRM_PDF.html
 *
 * @date  16.02.2022
 ******************************************************************************/
void SystemInit(void)
{
  /* Enable HSI and switch SYSCLK to it                   */
  RCC->CTLR |= RCC_HSION;
  RCC->CFGR0 &= ~(RCC_CFGR0_MCO | RCC_ADCPRE | RCC_PPRE2 | RCC_PPRE1 | RCC_HPRE | RCC_SW);

  /* Power down and de-configure PLL and HSE              */
  RCC->CTLR &= ~(RCC_PLLON | RCC_CSSON | RCC_HSEON);
  RCC->CTLR &= ~RCC_HSEBYP;
  RCC->CFGR0 &= ~(RCC_USBPRE | RCC_PLLMULL | RCC_PLLXTPRE | RCC_PLLSRC);

  /* Disable RCC interrupts and clear interrupt flags     */
  RCC->INTR &= ~(RCC_PLLRDYIE | RCC_HSERDYIE | RCC_HSIRDYIE | RCC_LSERDYIE | RCC_LSIRDYIE);
  RCC->INTR |= RCC_CSSC | RCC_PLLRDYC | RCC_HSERDYC | RCC_HSIRDYC | RCC_LSERDYC | RCC_LSIRDYC;

  /* Update System Core clock frequency variable          */
  SystemCoreClock = HSI_VALUE;
}

/*!****************************************************************************
 * @brief
 * Calculate HCLK from current configuration
 *
 * @date  16.02.2022
 ******************************************************************************/
void SystemCoreClockUpdate(void)
{
  unsigned uConfigVal = RCC->CFGR0;
  unsigned uBaseClk;

  /* Determine SYSCLK base clock frequency                */
  switch (uConfigVal & RCC_SWS)
  {
    case RCC_SWS_HSI: uBaseClk = HSI_VALUE; break;
    case RCC_SWS_HSE: uBaseClk = HSE_VALUE; break;
    case RCC_SWS_PLL: uBaseClk = uCalcPllBaseClk(uConfigVal); break;
    default:
      /* Invalid configuration                            */
      uBaseClk = HSI_VALUE;
  }

  /* Apply HCLK prescaler                                 */
  SystemCoreClock = uBaseClk >> aucPrescShf[(uConfigVal & RCC_HPRE) >> 4];
}


/*- Restore previous compiler options ----------------------------------------*/
#pragma GCC pop_options
