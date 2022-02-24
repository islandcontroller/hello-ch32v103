/********************************** (C) COPYRIGHT  *******************************
 * File Name          : core_riscv.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2020/04/30
 * Description        : RISC-V Core Peripheral Access Layer Header File
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/
#ifndef __CORE_RISCV_H__
#define __CORE_RISCV_H__

/* IO definitions */
#ifdef __cplusplus
  #define     __I     volatile                /*!< defines 'read only' permissions      */
#else
  #define     __I     volatile const          /*!< defines 'read only' permissions     */
#endif
#define     __O     volatile                  /*!< defines 'write only' permissions     */
#define     __IO    volatile                  /*!< defines 'read / write' permissions   */

/* Standard Peripheral Library old types (maintained for legacy purpose) */
typedef __I uint32_t vuc32;  /* Read Only */
typedef __I uint16_t vuc16;  /* Read Only */
typedef __I uint8_t vuc8;   /* Read Only */

typedef const uint32_t uc32;  /* Read Only */
typedef const uint16_t uc16;  /* Read Only */
typedef const uint8_t uc8;   /* Read Only */

typedef __I int32_t vsc32;  /* Read Only */
typedef __I int16_t vsc16;  /* Read Only */
typedef __I int8_t vsc8;   /* Read Only */

typedef const int32_t sc32;  /* Read Only */
typedef const int16_t sc16;  /* Read Only */
typedef const int8_t sc8;   /* Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

#define   RV_STATIC_INLINE  static  inline
#define   RV_STATIC_FORCE_INLINE RV_STATIC_INLINE __attribute__((always_inline))

/* memory mapped structure for Program Fast Interrupt Controller (PFIC) */
typedef struct{
  __I  uint32_t ISR[8];
  __I  uint32_t IPR[8];
  __IO uint32_t ITHRESDR;
  __IO uint32_t FIBADDRR;
  __IO uint32_t CFGR;
  __I  uint32_t GISR;
  uint8_t RESERVED0[0x10];
  __IO uint32_t FIOFADDRR[4];
  uint8_t RESERVED1[0x90];
  __O  uint32_t IENR[8];
  uint8_t RESERVED2[0x60];
  __O  uint32_t IRER[8];
  uint8_t RESERVED3[0x60];
  __O  uint32_t IPSR[8];
  uint8_t RESERVED4[0x60];
  __O  uint32_t IPRR[8];
  uint8_t RESERVED5[0x60];
  __IO uint32_t IACTR[8];
  uint8_t RESERVED6[0xE0];
  __IO uint8_t IPRIOR[256];
  uint8_t RESERVED7[0x810];
  __IO uint32_t SCTLR;
}PFIC_Type;

/* memory mapped structure for SysTick */
typedef struct
{
  __IO uint32_t CTLR;
  union {
    struct __attribute__((packed)) {
      __IO uint8_t CNTL0;
      __IO uint8_t CNTL1;
      __IO uint8_t CNTL2;
      __IO uint8_t CNTL3;
    };
    __I uint32_t CNTL;
  };
  union {
    struct __attribute__((packed)) {
      __IO uint8_t CNTH0;
      __IO uint8_t CNTH1;
      __IO uint8_t CNTH2;
      __IO uint8_t CNTH3;
    };
    __I uint32_t CNTH;
  };
  union {
    struct __attribute__((packed)) {
      __IO uint8_t CMPLR0;
      __IO uint8_t CMPLR1;
      __IO uint8_t CMPLR2;
      __IO uint8_t CMPLR3;
    };
    __I uint32_t CMPLR;
  };
  union {
    struct __attribute__((packed)) {
      __IO uint8_t CMPHR0;
      __IO uint8_t CMPHR1;
      __IO uint8_t CMPHR2;
      __IO uint8_t CMPHR3;
    };
    __I uint32_t CMPHR;
  };
}SysTick_Type;

#define PFIC            ((PFIC_Type *) 0xE000E000)
#define PFIC_KEY1       ((uint32_t)0xFA050000)
#define PFIC_KEY2       ((uint32_t)0xBCAF0000)
#define PFIC_KEY3       ((uint32_t)0xBEEF0000)

#define SysTick         ((SysTick_Type *) 0xE000F000)


/* ################ Programmable Fast Interrupt Controller ################## */
/*********************************************************************
 * @fn      PFIC_EnableIRQ
 *
 * @brief   Enable Interrupt
 *
 * @param   IRQn: Interrupt Numbers
 *
 * @return  none
 */
RV_STATIC_INLINE void PFIC_EnableIRQ(IRQn_Type IRQn){
  PFIC->IENR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/*********************************************************************
 * @fn      PFIC_DisableIRQ
 *
 * @brief   Disable Interrupt
 *
 * @param   IRQn: Interrupt Numbers
 *
 * @return  none
 */
RV_STATIC_INLINE void PFIC_DisableIRQ(IRQn_Type IRQn)
{
  uint32_t t;

  t = PFIC->ITHRESDR;
  PFIC->ITHRESDR = 0x10;
  PFIC->IRER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
  PFIC->ITHRESDR = t;
}

/*********************************************************************
 * @fn      PFIC_GetStatusIRQ
 *
 * @brief   Get Interrupt Enable State
 *
 * @param   IRQn: Interrupt Numbers
 *
 * @return  1 - Interrupt Enable
 *          0 - Interrupt Disable
 */
RV_STATIC_INLINE uint32_t PFIC_GetStatusIRQ(IRQn_Type IRQn)
{
  return((uint32_t) ((PFIC->ISR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}

/*********************************************************************
 * @fn      PFIC_GetPendingIRQ
 *
 * @brief   Get Interrupt Pending State
 *
 * @param   IRQn: Interrupt Numbers
 *
 * @return  1 - Interrupt Pending Enable
 *          0 - Interrupt Pending Disable
 */
RV_STATIC_INLINE uint32_t PFIC_GetPendingIRQ(IRQn_Type IRQn)
{
  return((uint32_t) ((PFIC->IPR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}

/*********************************************************************
 * @fn      PFIC_SetPendingIRQ
 *
 * @brief   Set Interrupt Pending
 *
 * @param   IRQn: Interrupt Numbers
 *
 * @return  None
 */
RV_STATIC_INLINE void PFIC_SetPendingIRQ(IRQn_Type IRQn)
{
  PFIC->IPSR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/*********************************************************************
 * @fn      PFIC_ClearPendingIRQ
 *
 * @brief   Clear Interrupt Pending
 *
 * @param   IRQn: Interrupt Numbers
 *
 * @return  None
 */
RV_STATIC_INLINE void PFIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  PFIC->IPRR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/*********************************************************************
 * @fn      PFIC_GetActive
 *
 * @brief   Get Interrupt Active State
 *
 * @param   IRQn: Interrupt Numbers
 *
 * @return  1 - Interrupt Active
 *          0 - Interrupt No Active
 */
RV_STATIC_INLINE uint32_t PFIC_GetActive(IRQn_Type IRQn)
{
  return((uint32_t)((PFIC->IACTR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}

/*********************************************************************
 * @fn      PFIC_SetPriority
 *
 * @brief   Set Interrupt Priority
 *
 * @param   IRQn - Interrupt Numbers
 *          priority -
 *              bit7 - pre-emption priority
 *              bit6~bit4 - subpriority
 * @return  None
 */
RV_STATIC_INLINE void PFIC_SetPriority(IRQn_Type IRQn, uint8_t priority)
{
  PFIC->IPRIOR[(uint32_t)(IRQn)] = priority;
}

/*********************************************************************
 * @fn      PFIC_SetFastIRQ
 *
 * @brief   Set VTF Interrupt
 *
 * @param   add - VTF interrupt service function base address.
 *          IRQn -Interrupt Numbers
 *          num - VTF Interrupt Numbers
 * @return  None
 */
RV_STATIC_INLINE void PFIC_SetFastIRQ(uint32_t addr, IRQn_Type IRQn, uint8_t num)
{
  if(num > 3)  return ;
  PFIC->FIBADDRR = addr;
  PFIC->FIOFADDRR[num] = ((uint32_t)IRQn<<24)|(addr&0xfffff);
}

/*********************************************************************
 * @fn      PFIC_SystemReset
 *
 * @brief   Initiate a system reset request
 *
 * @return  None
 */
RV_STATIC_INLINE void PFIC_SystemReset(void)
{
  PFIC->CFGR = PFIC_KEY3|(1<<7);
}

/*********************************************************************
 * @fn      PFIC_HaltPushCfg
 *
 * @brief   Enable Hardware Stack
 *
 * @param   NewState - DISABLE or ENABLE

 * @return  None
 */
RV_STATIC_INLINE void PFIC_HaltPushCfg(FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    PFIC->CFGR = PFIC_KEY1;
  }
  else{
    PFIC->CFGR = PFIC_KEY1|(1<<0);
  }
}

/*********************************************************************
 * @fn      PFIC_INTNestCfg
 *
 * @brief   Enable Interrupt Nesting
 *
 * @param   NewState - DISABLE or ENABLE

 * @return  None
 */
RV_STATIC_INLINE void PFIC_INTNestCfg(FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    PFIC->CFGR = PFIC_KEY1;
  }
  else
  {
    PFIC->CFGR = PFIC_KEY1|(1<<1);
  }
}

/* ########################### SysTick Timer ################################ */
/* Bit definitions for SysTick register                                       */
#define SYSTICK_CTLR_STE              0x00000001UL  /* Counter Enable         */

/* Access functions for 8-bit write registers                                 */
#define SYSTICK_REG_WRITE8(reg,val32) {     \
  SysTick->reg##3 = (uint8_t)(value >> 24); \
  SysTick->reg##2 = (uint8_t)(value >> 16); \
  SysTick->reg##1 = (uint8_t)(value >>  8); \
  SysTick->reg##0 = (uint8_t)(value >>  0); \
}

/*!****************************************************************************
 * @brief
 * Set SysTick counter state
 *
 * @param[in] state       Enable or disable counter
 * @date  18.02.2022
 * @date  24.02.2022  Changed naming convention
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void SysTick_Cmd(FunctionalState state)
{
  if (state != DISABLE)
  {
    SysTick->CTLR |= SYSTICK_CTLR_STE;
  }
  else
  {
    SysTick->CTLR &= ~SYSTICK_CTLR_STE;
  }
}

/*!****************************************************************************
 * @brief
 * Set SysTick counter low word
 *
 * Byte-wise copy into SysTick CNTL register
 *
 * @param[in] value       Counter value, low word
 * @date  18.02.2022
 * @date  24.02.2022  Changed naming convention
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void SysTick_SetValueLow(uint32_t value)
{
  SYSTICK_REG_WRITE8(CNTL, value);
}

/*!****************************************************************************
 * @brief
 * Set SysTick counter high word
 *
 * Byte-wise copy into SysTick CNTH register
 *
 * @param[in] value       Counter value, high word
 * @date  18.02.2022
 * @date  24.02.2022  Changed naming convention
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void SysTick_SetValueHigh(uint32_t value)
{
  SYSTICK_REG_WRITE8(CNTH, value);
}

/*!****************************************************************************
 * @brief
 * Set SysTick counter value (doubleword)
 *
 * @param[in] value       64-bit counter value
 * @date  18.02.2022
 * @date  24.02.2022  Changed naming convention
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void SysTick_SetValue(uint64_t value)
{
  SysTick_SetValueHigh((uint32_t)(value >> 32));
  SysTick_SetValueLow((uint32_t)value);
}

/*!****************************************************************************
 * @brief
 * Get SysTick counter value, low word
 *
 * @return  (uint32_t)  Counter value, low word
 * @date  18.02.2022
 * @date  24.02.2022  Changed naming convention
 ******************************************************************************/
RV_STATIC_FORCE_INLINE uint32_t SysTick_GetValueLow(void)
{
  return SysTick->CNTL;
}

/*!****************************************************************************
 * @brief
 * Get SysTick counter value, high word
 *
 * @return  (uint32_t)  Counter value, low word
 * @date  24.02.2022
 ******************************************************************************/
RV_STATIC_FORCE_INLINE uint32_t SysTick_GetValueHigh(void)
{
  return SysTick->CNTH;
}

/*!****************************************************************************
 * @brief
 * Get 64 bit SysTick counter value
 *
 * Low and high words may be out of sync
 *
 * @return  (uint64_t)  Counter value
 * @date  18.02.2022
 ******************************************************************************/
RV_STATIC_FORCE_INLINE uint64_t SysTick_GetValue64(void)
{
  return ((uint64_t)SysTick->CNTH << 32) | (uint64_t)SysTick->CNTL;
}

/*!****************************************************************************
 * @brief
 * Set SysTick compare value low word
 *
 * @param[in] value       Compare value, low word
 * @date  18.02.2022
 * @date  24.02.2022  Changed naming convention
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void SysTick_SetCompareLow(uint32_t value)
{
  SYSTICK_REG_WRITE8(CMPLR, value);
}

/*!****************************************************************************
 * @brief
 * Set SysTick compare value high word
 *
 * @param[in] value       Compare value, high word
 * @date  18.02.2022
 * @date  24.02.2022  Changed naming convention
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void SysTick_SetCompareHigh(uint32_t value)
{
  SYSTICK_REG_WRITE8(CMPHR, value);
}

/*!****************************************************************************
 * @brief
 * Set SysTick compare value (doubleword)
 *
 * @param[in] value       64-bit compare value
 * @date  18.02.2022
 * @date  24.02.2022  Changed naming convention
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void SysTick_SetCompare(uint64_t value)
{
  SysTick_SetCompareHigh((uint32_t)(value >> 32));
  SysTick_SetCompareLow((uint32_t)value);
}

/* ########################## Core functions ################################ */
/*********************************************************************
 * @fn      __NOP
 *
 * @brief   nop
 *
 * @return  none
 */
RV_STATIC_INLINE void __NOP()
{
  __asm volatile ("nop");
}

/*********************************************************************
 * @fn      __WFI
 *
 * @brief   Wait for Interrupt
 *
 * @return  None
 */
RV_STATIC_FORCE_INLINE void __WFI(void)
{
  PFIC->SCTLR &= ~(1<<3); // wfi
  asm volatile ("wfi");
}

/*********************************************************************
 * @fn      __WFE
 *
 * @brief   Wait for Events
 *
 * @return  None
 */
RV_STATIC_FORCE_INLINE void __WFE(void)
{
  PFIC->SCTLR |= (1<<3)|(1<<5);   // (wfi->wfe)+(__sev)
  asm volatile ("wfi");
  PFIC->SCTLR |= (1<<3);
  asm volatile ("wfi");
}

/* ###################### Machine Register Access ########################### */
/*********************************************************************
 * @fn      __get_FFLAGS
 *
 * @brief   Return the Floating-Point Accrued Exceptions
 *
 * @return  fflags value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_FFLAGS(void)
{
    uint32_t result;

    __asm volatile("csrr %0, ""fflags": "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_FFLAGS
 *
 * @brief   Set the Floating-Point Accrued Exceptions
 *
 * @param   value  - set FFLAGS value
 *
 * @return  none
 */
RV_STATIC_FORCE_INLINE void __set_FFLAGS(uint32_t value)
{
    __asm volatile("csrw fflags, %0":: "r"(value));
}

/*********************************************************************
 * @fn      __get_FRM
 *
 * @brief   Return the Floating-Point Dynamic Rounding Mode
 *
 * @return  frm value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_FRM(void)
{
    uint32_t result;

    __asm volatile("csrr %0,""frm": "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_FRM
 *
 * @brief   Set the Floating-Point Dynamic Rounding Mode
 *
 * @param   value  - set frm value
 *
 * @return  none
 */
RV_STATIC_FORCE_INLINE void __set_FRM(uint32_t value)
{
    __asm volatile("csrw frm, %0" :: "r"(value));
}

/*********************************************************************
 * @fn      __get_FCSR
 *
 * @brief   Return the Floating-Point Control and Status Register
 *
 * @return  fcsr value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_FCSR(void)
{
    uint32_t result;

    __asm volatile("csrr %0," "fcsr" : "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_FCSR
 *
 * @brief   Set the Floating-Point Dynamic Rounding Mode
 *
 * @param   value  - set fcsr value
 *
 * @return  none
 */
RV_STATIC_FORCE_INLINE void __set_FCSR(uint32_t value)
{
    __asm volatile("csrw fcsr, %0" : : "r"(value));
}

/*********************************************************************
 * @fn      __get_MSTATUS
 *
 * @brief   Return the Machine Status Register
 *
 * @return  mstatus value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MSTATUS(void)
{
    uint32_t result;

    __asm volatile("csrr %0," "mstatus": "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_MSTATUS
 *
 * @brief   Set the Machine Status Register
 *
 * @param   value  - set mstatus value
 *
 * @return  none
 */
RV_STATIC_FORCE_INLINE void __set_MSTATUS(uint32_t value)
{
    __asm volatile("csrw mstatus, %0" : : "r"(value));
}

/*********************************************************************
 * @fn      __get_MISA
 *
 * @brief   Return the Machine ISA Register
 *
 * @return  misa value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MISA(void)
{
    uint32_t result;

    __asm volatile("csrr %0,""misa" : "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_MISA
 *
 * @brief   Set the Machine ISA Register
 *
 * @param   value  - set misa value
 *
 * @return  none
 */
RV_STATIC_FORCE_INLINE void __set_MISA(uint32_t value)
{
    __asm volatile("csrw misa, %0" : : "r"(value));
}

/*********************************************************************
 * @fn      __get_MIE
 *
 * @brief   Return the Machine Interrupt Enable Register
 *
 * @return  mie value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MIE(void)
{
    uint32_t result;

    __asm volatile("csrr %0," "mie": "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_MISA
 *
 * @brief   Set the Machine ISA Register
 *
 * @param   value  - set mie value
 *
 * @return  none
 */
RV_STATIC_FORCE_INLINE void __set_MIE(uint32_t value)
{
    __asm volatile("csrw mie, %0": : "r"(value));
}

/*********************************************************************
 * @fn      __get_MTVEC
 *
 * @brief   Return the Machine Trap-Vector Base-Address Register
 *
 * @return  mtvec value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MTVEC(void)
{
    uint32_t result;

    __asm volatile("csrr %0," "mtvec": "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_MTVEC
 *
 * @brief   Set the Machine Trap-Vector Base-Address Register
 *
 * @param   value  - set mtvec value
 *
 * @return  none
 */
RV_STATIC_FORCE_INLINE void __set_MTVEC(uint32_t value)
{
    __asm volatile("csrw mtvec, %0":: "r"(value));
}

/*********************************************************************
 * @fn      __get_MTVEC
 *
 * @brief   Return the Machine Seratch Register
 *
 * @return  mscratch value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MSCRATCH(void)
{
    uint32_t result;

    __asm volatile("csrr %0," "mscratch" : "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_MTVEC
 *
 * @brief   Set the Machine Seratch Register
 *
 * @param   value  - set mscratch value
 *
 * @return  none
 */
RV_STATIC_FORCE_INLINE void __set_MSCRATCH(uint32_t value)
{
    __asm volatile("csrw mscratch, %0" : : "r"(value));
}

/*********************************************************************
 * @fn      __get_MEPC
 *
 * @brief   Return the Machine Exception Program Register
 *
 * @return  mepc value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MEPC(void)
{
    uint32_t result;

    __asm volatile("csrr %0," "mepc" : "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_MEPC
 *
 * @brief   Set the Machine Exception Program Register
 *
 * @return  mepc value
 */
RV_STATIC_FORCE_INLINE void __set_MEPC(uint32_t value)
{
    __asm volatile("csrw mepc, %0" : : "r"(value));
}

/*********************************************************************
 * @fn      __get_MCAUSE
 *
 * @brief   Return the Machine Cause Register
 *
 * @return  mcause value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MCAUSE(void)
{
    uint32_t result;

    __asm volatile("csrr %0," "mcause": "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_MEPC
 *
 * @brief   Set the Machine Cause Register
 *
 * @return  mcause value
 */
RV_STATIC_FORCE_INLINE void __set_MCAUSE(uint32_t value)
{
    __asm volatile("csrw mcause, %0":: "r"(value));
}

/*********************************************************************
 * @fn      __get_MTVAL
 *
 * @brief   Return the Machine Trap Value Register
 *
 * @return  mtval value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MTVAL(void)
{
    uint32_t result;

    __asm volatile("csrr %0," "mtval" : "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_MTVAL
 *
 * @brief   Set the Machine Trap Value Register
 *
 * @return  mtval value
 */
RV_STATIC_FORCE_INLINE void __set_MTVAL(uint32_t value)
{
    __asm volatile("csrw mtval, %0":: "r"(value));
}

/*********************************************************************
 * @fn      __get_MIP
 *
 * @brief   Return the Machine Interrupt Pending Register
 *
 * @return  mip value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MIP(void)
{
    uint32_t result;

    __asm volatile("csrr %0," "mip": "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_MIP
 *
 * @brief   Set the Machine Interrupt Pending Register
 *
 * @return  mip value
 */
RV_STATIC_FORCE_INLINE void __set_MIP(uint32_t value)
{
    __asm volatile("csrw mip, %0":: "r"(value));
}

/*********************************************************************
 * @fn      __get_MCYCLE
 *
 * @brief   Return Lower 32 bits of Cycle counter
 *
 * @return  mcycle value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MCYCLE(void)
{
    uint32_t result;

    __asm volatile("csrr %0," "mcycle": "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_MCYCLE
 *
 * @brief   Set Lower 32 bits of Cycle counter
 *
 * @return  mcycle value
 */
RV_STATIC_FORCE_INLINE void __set_MCYCLE(uint32_t value)
{
    __asm volatile("csrw mcycle, %0" : : "r"(value));
}

/*********************************************************************
 * @fn      __get_MCYCLEH
 *
 * @brief   Return Upper 32 bits of Cycle counter
 *
 * @return  mcycleh value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MCYCLEH(void)
{
    uint32_t result;

    __asm volatile("csrr %0,""mcycleh" : "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_MCYCLEH
 *
 * @brief   Set Upper 32 bits of Cycle counter
 *
 * @return  mcycleh value
 */
RV_STATIC_FORCE_INLINE void __set_MCYCLEH(uint32_t value)
{
    __asm volatile("csrw mcycleh, %0":: "r"(value));
}

/*********************************************************************
 * @fn      __get_MINSTRET
 *
 * @brief   Return Lower 32 bits of Instructions-retired counter
 *
 * @return  mcause value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MINSTRET(void)
{
    uint32_t result;

    __asm volatile("csrr %0,""minstret": "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_MINSTRET
 *
 * @brief   Set Lower 32 bits of Instructions-retired counter
 *
 * @return  minstret value
 */
RV_STATIC_FORCE_INLINE void __set_MINSTRET(uint32_t value)
{
    __asm volatile("csrw minstret, %0":: "r"(value));
}

/*********************************************************************
 * @fn      __get_MINSTRETH
 *
 * @brief   Return Upper 32 bits of Instructions-retired counter
 *
 * @return  minstreth value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MINSTRETH(void)
{
    uint32_t result;

    __asm volatile("csrr %0,""minstreth": "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __set_MINSTRETH
 *
 * @brief   Set Upper 32 bits of Instructions-retired counter
 *
 * @return  minstreth value
 */
RV_STATIC_FORCE_INLINE void __set_MINSTRETH(uint32_t value)
{
    __asm volatile("csrw minstreth, %0":: "r"(value));
}

/*********************************************************************
 * @fn      __get_MVENDORID
 *
 * @brief   Return Vendor ID Register
 *
 * @return  mvendorid value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MVENDORID(void)
{
    uint32_t result;

    __asm volatile("csrr %0,""mvendorid": "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __get_MARCHID
 *
 * @brief   Return Machine Architecture ID Register
 *
 * @return  marchid value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MARCHID(void)
{
    uint32_t result;

    __asm volatile("csrr %0,""marchid": "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __get_MIMPID
 *
 * @brief   Return Machine Implementation ID Register
 *
 * @return  mimpid value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MIMPID(void)
{
    uint32_t result;

    __asm volatile("csrr %0,""mimpid": "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __get_MHARTID
 *
 * @brief   Return Hart ID Register
 *
 * @return  mhartid value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MHARTID(void)
{
    uint32_t result;

    __asm volatile("csrr %0,""mhartid": "=r"(result));
    return (result);
}

/*********************************************************************
 * @fn      __get_SP
 *
 * @brief   Return SP Register
 *
 * @return  SP value
 */
RV_STATIC_FORCE_INLINE uint32_t __get_SP(void)
{
    uint32_t result;

    asm volatile("mv %0,""sp": "=r"(result):);
    return (result);
}

#endif/* __CORE_RISCV_H__ */





