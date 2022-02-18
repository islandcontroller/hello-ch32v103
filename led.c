/*!****************************************************************************
 * @file
 * led.c
 *
 * @brief
 * Breathing animation using PWM on TIM3 Channel 1
 *
 * @date  17.02.2022
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include <stdbool.h>
#include "ch32v10x.h"
#include "hw_stk.h"
#include "led.h"


/*- Macros -------------------------------------------------------------------*/
/*! @brief SysTick timer counter interval for brightness changes              */
#define LED_TIME_INTER                10000UL


/*- Private variables --------------------------------------------------------*/
/*! Count direction                                                           */
static bool bCountDirUp;

/*! Brightness value                                                          */
static uint16_t uiBrightness;

/*! Last change timestamp                                                     */
static uint32_t ulLastChange;


/*!****************************************************************************
 * @brief
 * Initialise LED animation module
 *
 * @date  17.02.2022
 ******************************************************************************/
void vInitLed(void)
{
  bCountDirUp = true;
  uiBrightness = 0;
  ulLastChange = STK_GetValueLow();
}

/*!****************************************************************************
 * @brief
 * Handle animation in main() polling
 *
 * @date  17.02.2022
 ******************************************************************************/
void vPollLed(void)
{
  /* Early exit until minimum interval is reached         */
  uint32_t ulNow = STK_GetValueLow();
  if (ulNow - ulLastChange < LED_TIME_INTER)
  {
    return;
  }
  else
  {
    ulLastChange = ulNow;
  }

  /* Ramp-Up / Ramp-down                                  */
  if (bCountDirUp)
  {
    if (uiBrightness < 100)
    {
      ++uiBrightness;
    }
    else
    {
      bCountDirUp = false;
    }
  }
  else
  {
    if (uiBrightness > 0)
    {
      --uiBrightness;
    }
    else
    {
      bCountDirUp = true;
    }
  }

  /* Apply new brightness value                           */
  TIM_SetCompare1(TIM3, uiBrightness);
}
