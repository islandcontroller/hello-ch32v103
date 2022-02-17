/******************************************************************************
 * _start
 * Reset handler
 ******************************************************************************/
.section  .text.startup
.balign 4
.globl  _start
_start:
  /* Clear registers                                                          */
  lui   x0,     0                     /* Dummy write to 0 with zeros          */
regfile_init:
  li    x1,     0
  li    x2,     0
  li    x3,     0
  li    x4,     0
  li    x5,     0
  li    x6,     0
  li    x7,     0
  li    x8,     0
  li    x9,     0
  li    x10,    0
  li    x11,    0
  li    x12,    0
  li    x13,    0
  li    x14,    0
  li    x15,    0
  li    x16,    0
  li    x17,    0
  li    x18,    0
  li    x19,    0
  li    x20,    0
  li    x21,    0
  li    x22,    0
  li    x23,    0
  li    x24,    0
  li    x25,    0
  li    x26,    0
  li    x27,    0
  li    x28,    0
  li    x29,    0
  li    x30,    0
  li    x31,    0

/* Disable interrupts during setup                                            */
_csr_preinit:
  csrw  mstatus,  zero                /* Disable all interrupts               */

/* Initialise C-Runtime Library                                               */
_crt0_init:
  .option push
  .option norelax
  la    sp,   _estack                 /* Initialise stack pointer             */
  la    gp,   __global_pointer$       /* Initialise global pointer            */
  .option pop

__crt0_clear_bss:
  la    a0,   _sbss                   /* Start of .bss section                */
  la    a1,   _ebss                   /* End of .bss section                  */
__crt0_clear_bss_loop:
  bgeu  a0,   a1,   __crt0_copy_data  /* Done, or skip if no .bss present     */
  sw    zero, 0(a0)
  addi  a0,   a0,   4                 /* Word-wise increment of write addr    */
  j     __crt0_clear_bss_loop

__crt0_copy_data:
  la    a0,   _sidata                 /* Start LMA of .data section ("src")   */
  la    a1,   _sdata                  /* Start VMA of .data section ("dest")  */
  la    a2,   _edata                  /* End VMA of .data section             */
__crt0_copy_data_loop:
  bgeu  a1,   a2,   _csr_init         /* Done, or skip if no .data present    */
  lw    t0,   0(a0)                   /* Load word from LMA                   */
  sw    t0,   0(a1)                   /* Store word to VMA                    */
  addi  a0,   a0,   4                 /* Word-wise increment of LMA and VMA   */
  addi  a1,   a1,   4
  j     __crt0_copy_data_loop

/* Initialise CSRs for interrupt handling                                     */
_csr_init:
  la    a0,   _vector_base            /* Set trap vector address              */
  ori   a0,   a0,   1                 /* Vectored mode                        */
  csrw  mtvec,  a0
  li    t0,   0x88                    /* Enable interrupts                    */
  csrw  mstatus,  t0

/* Jump to main() function                                                    */
_app_start:
  jal   SystemInit                    /* Core system init                     */
__app_main_enter:
  li    a0,   0                       /* argc = 0                             */
  li    a1,   0                       /* argv = NULL                          */
  jal   ra,   main                    /* Call main() with return address      */
__app_main_exit:
  j     __app_main_exit               /* Endless loop                         */


/******************************************************************************
 * _vector_base
 * Interrupt Vector Table
 ******************************************************************************/
.section  .isr_vector,"ax",@progbits
.balign 4
.globl  _vector_base
_vector_base:
  /* Internal interrupts                                                      */
  j     _start
  .word 0
  j     NMI_Handler                   /* NMI Handler                          */
  j     HardFault_Handler             /* Hard Fault Handler                   */
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  .word 0
  j     SysTick_Handler               /* SysTick Handler                      */
  .word 0
  j     SW_handler                    /* SW Handler                           */
  .word 0
  /* External Interrupts                                                      */
  j     WWDG_IRQHandler               /* Window Watchdog                      */
  j     PVD_IRQHandler                /* PVD through EXTI Line detect         */
  j     TAMPER_IRQHandler             /* TAMPER                               */
  j     RTC_IRQHandler                /* RTC                                  */
  j     FLASH_IRQHandler              /* Flash                                */
  j     RCC_IRQHandler                /* RCC                                  */
  j     EXTI0_IRQHandler              /* EXTI Line 0                          */
  j     EXTI1_IRQHandler              /* EXTI Line 1                          */
  j     EXTI2_IRQHandler              /* EXTI Line 2                          */
  j     EXTI3_IRQHandler              /* EXTI Line 3                          */
  j     EXTI4_IRQHandler              /* EXTI Line 4                          */
  j     DMA1_Channel1_IRQHandler      /* DMA1 Channel 1                       */
  j     DMA1_Channel2_IRQHandler      /* DMA1 Channel 2                       */
  j     DMA1_Channel3_IRQHandler      /* DMA1 Channel 3                       */
  j     DMA1_Channel4_IRQHandler      /* DMA1 Channel 4                       */
  j     DMA1_Channel5_IRQHandler      /* DMA1 Channel 5                       */
  j     DMA1_Channel6_IRQHandler      /* DMA1 Channel 6                       */
  j     DMA1_Channel7_IRQHandler      /* DMA1 Channel 7                       */
  j     ADC1_2_IRQHandler             /* ADC1_2                               */
  .word 0
  .word 0
  .word 0
  .word 0
  j     EXTI9_5_IRQHandler            /* EXTI Line 9..5                       */
  j     TIM1_BRK_IRQHandler           /* TIM1 Break                           */
  j     TIM1_UP_IRQHandler            /* TIM1 Update                          */
  j     TIM1_TRG_COM_IRQHandler       /* TIM1 Trigger and Commutation         */
  j     TIM1_CC_IRQHandler            /* TIM1 Capture Compare                 */
  j     TIM2_IRQHandler               /* TIM2                                 */
  j     TIM3_IRQHandler               /* TIM3                                 */
  j     TIM4_IRQHandler               /* TIM4                                 */
  j     I2C1_EV_IRQHandler            /* I2C1 Event                           */
  j     I2C1_ER_IRQHandler            /* I2C1 Error                           */
  j     I2C2_EV_IRQHandler            /* I2C2 Event                           */
  j     I2C2_ER_IRQHandler            /* I2C2 Error                           */
  j     SPI1_IRQHandler               /* SPI1                                 */
  j     SPI2_IRQHandler               /* SPI2                                 */
  j     USART1_IRQHandler             /* USART1                               */
  j     USART2_IRQHandler             /* USART2                               */
  j     USART3_IRQHandler             /* USART3                               */
  j     EXTI15_10_IRQHandler          /* EXTI Line 15..10                     */
  j     RTCAlarm_IRQHandler           /* RTC Alarm through EXTI Line          */
  j     USBWakeUp_IRQHandler          /* USB Wakeup from suspend              */
  j     USBHD_IRQHandler              /* USBHD                                */

/* Weak definitions of interrupt handlers in .text                            */
.section  .text.vector_handler, "ax", @progbits
.weak NMI_Handler
.weak HardFault_Handler
.weak SysTick_Handler
.weak SW_handler
.weak WWDG_IRQHandler
.weak PVD_IRQHandler
.weak TAMPER_IRQHandler
.weak RTC_IRQHandler
.weak FLASH_IRQHandler
.weak RCC_IRQHandler
.weak EXTI0_IRQHandler
.weak EXTI1_IRQHandler
.weak EXTI2_IRQHandler
.weak EXTI3_IRQHandler
.weak EXTI4_IRQHandler
.weak DMA1_Channel1_IRQHandler
.weak DMA1_Channel2_IRQHandler
.weak DMA1_Channel3_IRQHandler
.weak DMA1_Channel4_IRQHandler
.weak DMA1_Channel5_IRQHandler
.weak DMA1_Channel6_IRQHandler
.weak DMA1_Channel7_IRQHandler
.weak ADC1_2_IRQHandler
.weak EXTI9_5_IRQHandler
.weak TIM1_BRK_IRQHandler
.weak TIM1_UP_IRQHandler
.weak TIM1_TRG_COM_IRQHandler
.weak TIM1_CC_IRQHandler
.weak TIM2_IRQHandler
.weak TIM3_IRQHandler
.weak TIM4_IRQHandler
.weak I2C1_EV_IRQHandler
.weak I2C1_ER_IRQHandler
.weak I2C2_EV_IRQHandler
.weak I2C2_ER_IRQHandler
.weak SPI1_IRQHandler
.weak SPI2_IRQHandler
.weak USART1_IRQHandler
.weak USART2_IRQHandler
.weak USART3_IRQHandler
.weak EXTI15_10_IRQHandler
.weak RTCAlarm_IRQHandler
.weak USBWakeUp_IRQHandler
.weak USBHD_IRQHandler
