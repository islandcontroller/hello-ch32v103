/*!****************************************************************************
 * @file
 * hw_tim3.c
 *
 * @brief
 * Low-level configuration of TIM3
 *
 * @date  17.02.2022
 ******************************************************************************/

/*- Header files -------------------------------------------------------------*/
#include "ch32v10x.h"
#include "hw_tim3.h"


/*!****************************************************************************
 * @brief
 * Activate peripheral clocks and configure Timer 3
 *
 * @date  17.02.2022
 ******************************************************************************/
void vInitHW_TIM3(void)
{
  /* Enable peripheral clock supply                       */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Configure base timer for 1kHz PWM with pulse width
   * range of [0 .. 100]                                  */
  TIM_TimeBaseInitTypeDef sInitBase = {
    .TIM_Prescaler = 80,
    .TIM_CounterMode = TIM_CounterMode_Up,
    .TIM_Period = 100,
  };
  TIM_TimeBaseInit(TIM3, &sInitBase);

  /* Configure Channel 1 for PWM output on PA6            */
  TIM_OCInitTypeDef sInitCh1 = {
    .TIM_OCMode = TIM_OCMode_PWM1,
    .TIM_OutputState = TIM_OutputState_Enable,
    .TIM_OCPolarity = TIM_OCPolarity_Low,
    .TIM_Pulse = 0
  };
  TIM_OC1Init(TIM3, &sInitCh1);

  /* Enable timer main outputs                            */
  TIM_CtrlPWMOutputs(TIM3, ENABLE);

  /* Apply Capture register writes immediately            */
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);

  /* Enable automatic preload                             */
  TIM_ARRPreloadConfig(TIM3, ENABLE);

  /* Start timer module                                   */
  TIM_Cmd(TIM3, ENABLE);
}
