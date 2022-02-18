/********************************** (C) COPYRIGHT  *******************************
* File Name          : core_riscv.h
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : RISC-V Core Peripheral Access Layer Header File
*******************************************************************************/
#ifndef __CORE_RISCV_H__
#define __CORE_RISCV_H__

/* default headers */
#include <stdint.h>

/* define compiler specific symbols */
#if defined ( __CC_ARM   )
  #define __ASM            __asm              /*!< asm keyword for ARM Compiler          */
  #define __INLINE         __inline           /*!< inline keyword for ARM Compiler       */

#elif defined ( __ICCARM__ )
  #define __ASM           __asm               /*!< asm keyword for IAR Compiler          */
  #define __INLINE        inline              /*!< inline keyword for IAR Compiler. Only avaiable in High optimization mode! */

#elif defined   (  __GNUC__  )
  #define __ASM            __asm              /*!< asm keyword for GNU Compiler          */
  #define __INLINE         inline             /*!< inline keyword for GNU Compiler       */

#elif defined   (  __TASKING__  )
  #define __ASM            __asm              /*!< asm keyword for TASKING Compiler      */
  #define __INLINE         inline             /*!< inline keyword for TASKING Compiler   */
#endif

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


#define PFIC            ((PFIC_Type *) 0xE000E000 )
#define NVIC            PFIC
#define NVIC_KEY1       ((uint32_t)0xFA050000)
#define	NVIC_KEY2				((uint32_t)0xBCAF0000)
#define	NVIC_KEY3				((uint32_t)0xBEEF0000)

#define SysTick         ((SysTick_Type *) 0xE000F000)
#define STK             SysTick


/* ##########################   NVIC functions  #################################### */

/*******************************************************************************
* Function Name  : NVIC_EnableIRQ
* Description    : Enable Interrupt
* Input          : IRQn: Interrupt Numbers
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void NVIC_EnableIRQ(IRQn_Type IRQn){
  NVIC->IENR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/*******************************************************************************
* Function Name  : NVIC_DisableIRQ
* Description    : Disable Interrupt
* Input          : IRQn: Interrupt Numbers
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void NVIC_DisableIRQ(IRQn_Type IRQn)
{
  NVIC->IRER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/*******************************************************************************
* Function Name  : NVIC_GetStatusIRQ
* Description    : Get Interrupt Enable State
* Input          : IRQn: Interrupt Numbers
* Return         : 1: Interrupt Enable
*                  0: Interrupt Disable
*******************************************************************************/
RV_STATIC_INLINE uint32_t NVIC_GetStatusIRQ(IRQn_Type IRQn)
{
  return((uint32_t) ((NVIC->ISR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}

/*******************************************************************************
* Function Name  : NVIC_GetPendingIRQ
* Description    : Get Interrupt Pending State
* Input          : IRQn: Interrupt Numbers
* Return         : 1: Interrupt Pending Enable
*                  0: Interrupt Pending Disable
*******************************************************************************/
RV_STATIC_INLINE uint32_t NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
  return((uint32_t) ((NVIC->IPR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}

/*******************************************************************************
* Function Name  : NVIC_SetPendingIRQ
* Description    : Set Interrupt Pending
* Input          : IRQn: Interrupt Numbers
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  NVIC->IPSR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/*******************************************************************************
* Function Name  : NVIC_ClearPendingIRQ
* Description    : Clear Interrupt Pending
* Input          : IRQn: Interrupt Numbers
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  NVIC->IPRR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
}

/*******************************************************************************
* Function Name  : NVIC_GetActive
* Description    : Get Interrupt Active State
* Input          : IRQn: Interrupt Numbers
* Return         : 1: Interrupt Active
*                  0: Interrupt No Active
*******************************************************************************/
RV_STATIC_INLINE uint32_t NVIC_GetActive(IRQn_Type IRQn)
{
  return((uint32_t)((NVIC->IACTR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
}

/*******************************************************************************
* Function Name  : NVIC_SetPriority
* Description    : Set Interrupt Priority
* Input          : IRQn: Interrupt Numbers
*                  priority: bit7:pre-emption priority
*                            bit6-bit4: subpriority
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void NVIC_SetPriority(IRQn_Type IRQn, uint8_t priority)
{
  NVIC->IPRIOR[(uint32_t)(IRQn)] = priority;
}

/*******************************************************************************
* Function Name  : __WFI
* Description    : Wait for Interrupt
* Input          : None
* Return         : None
*******************************************************************************/
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void __WFI(void)
{
  NVIC->SCTLR &= ~(1<<3);	// wfi
  asm volatile ("wfi");
}

/*******************************************************************************
* Function Name  : __WFE
* Description    : Wait for Events
* Input          : None
* Return         : None
*******************************************************************************/
__attribute__( ( always_inline ) ) RV_STATIC_INLINE void __WFE(void)
{
  NVIC->SCTLR |= (1<<3)|(1<<5);		// (wfi->wfe)+(__sev)
  asm volatile ("wfi");
  NVIC->SCTLR |= (1<<3);
  asm volatile ("wfi");
}

/*******************************************************************************
* Function Name  : NVIC_SetFastIRQ
* Description    : Set Fast Interrupt
* Input          : addr£ºFast interrupt service function base address.
*                  IRQn£ºInterrupt Numbers
*                  num£ºFast Interrupt Numbers
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void NVIC_SetFastIRQ(uint32_t addr, IRQn_Type IRQn, uint8_t num){
  if(num > 3)  return ;
  NVIC->FIBADDRR = addr;
  NVIC->FIOFADDRR[num] = ((uint32_t)IRQn<<24)|(addr&0xfffff);
}

/*******************************************************************************
* Function Name  : NVIC_SystemReset
* Description    : Initiate a system reset request
* Input          : None
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void NVIC_SystemReset(void)
{
  NVIC->CFGR = NVIC_KEY3|(1<<7);
}

/*******************************************************************************
* Function Name  : NVIC_HaltPushCfg
* Description    : Enable Hardware Stack
* Input          : NewState: DISABLE or ENABLE
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void NVIC_HaltPushCfg(FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
  	NVIC->CFGR = NVIC_KEY1;
  }
  else{
  	NVIC->CFGR = NVIC_KEY1|(1<<0);
  }
}

/*******************************************************************************
* Function Name  : NVIC_INTNestCfg
* Description    : Enable Interrupt Nesting
* Input          : NewState: DISABLE or ENABLE
* Return         : None
*******************************************************************************/
RV_STATIC_INLINE void NVIC_INTNestCfg(FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
  	NVIC->CFGR = NVIC_KEY1;
  }
  else
  {
  	NVIC->CFGR = NVIC_KEY1|(1<<1);
  }
}

/* ########################### SysTick Timer ################################ */
/* Bit definitions for STK register                                           */
#define STK_CTLR_STE                  0x00000001UL  /* Counter Enable         */

/* Access functions for 8-bit write registers                                 */
#define STK_REG_WRITE8(reg,val32) {     \
  STK->reg##3 = (uint8_t)(value >> 24); \
  STK->reg##2 = (uint8_t)(value >> 16); \
  STK->reg##1 = (uint8_t)(value >>  8); \
  STK->reg##0 = (uint8_t)(value >>  0); \
}

/*!****************************************************************************
 * @brief
 * Set SysTick counter state
 *
 * @param[in] state       Enable or disable counter
 * @date  18.02.2022
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void STK_Cmd(FunctionalState state)
{
  if (state != DISABLE)
  {
    STK->CTLR |= STK_CTLR_STE;
  }
  else
  {
    STK->CTLR &= ~STK_CTLR_STE;
  }
}

/*!****************************************************************************
 * @brief
 * Set SysTick counter low word
 *
 * Byte-wise copy into STK_CNTL register
 *
 * @param[in] value       Counter value, low word
 * @date  18.02.2022
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void STK_SetValueLow(uint32_t value)
{
  STK_REG_WRITE8(CNTL, value);
}

/*!****************************************************************************
 * @brief
 * Set SysTick counter high word
 *
 * Byte-wise copy into STK_CNTH register
 *
 * @param[in] value       Counter value, high word
 * @date  18.02.2022
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void STK_SetValueHigh(uint32_t value)
{
  STK_REG_WRITE8(CNTH, value);
}

/*!****************************************************************************
 * @brief
 * Set SysTick counter value (doubleword)
 *
 * @param[in] value       64-bit counter value
 * @date  18.02.2022
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void STK_SetValue(uint64_t value)
{
  STK_SetValueHigh((uint32_t)(value >> 32));
  STK_SetValueLow((uint32_t)value);
}

/*!****************************************************************************
 * @brief
 * Get SysTick counter value, low word
 *
 * @return  (uint32_t)  Counter value, low word
 * @date  18.02.2022
 ******************************************************************************/
RV_STATIC_FORCE_INLINE uint32_t STK_GetValueLow(void)
{
  return STK->CNTL;
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
RV_STATIC_FORCE_INLINE uint64_t STK_GetValue(void)
{
  return ((uint64_t)STK->CNTH << 32) | (uint64_t)STK->CNTL;
}

/*!****************************************************************************
 * @brief
 * Set SysTick compare value low word
 *
 * @param[in] value       Compare value, low word
 * @date  18.02.2022
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void STK_SetCompareLow(uint32_t value)
{
  STK_REG_WRITE8(CMPLR, value);
}

/*!****************************************************************************
 * @brief
 * Set SysTick compare value high word
 *
 * @param[in] value       Compare value, high word
 * @date  18.02.2022
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void STK_SetCompareHigh(uint32_t value)
{
  STK_REG_WRITE8(CMPHR, value);
}

/*!****************************************************************************
 * @brief
 * Set SysTick compare value (doubleword)
 *
 * @param[in] value       64-bit compare value
 * @date  18.02.2022
 ******************************************************************************/
RV_STATIC_FORCE_INLINE void STK_SetCompare(uint64_t value)
{
  STK_SetCompareHigh((uint32_t)(value >> 32));
  STK_SetCompareLow((uint32_t)value);
}

/* ###################### Machine Register Access ########################### */
/**
 * @brief  Return the Floating-Point Accrued Exceptions
 */
RV_STATIC_FORCE_INLINE uint32_t __get_FFLAGS(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "fflags" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set the Floating-Point Accrued Exceptions
 */
RV_STATIC_FORCE_INLINE void __set_FFLAGS(uint32_t value)
{
  __ASM volatile ("csrw fflags, %0" : : "r" (value) );
}

/**
 * @brief  Return the Floating-Point Dynamic Rounding Mode
 */
RV_STATIC_FORCE_INLINE uint32_t __get_FRM(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "frm" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set the Floating-Point Dynamic Rounding Mode
 */
RV_STATIC_FORCE_INLINE void __set_FRM(uint32_t value)
{
  __ASM volatile ("csrw frm, %0" : : "r" (value) );
}

/**
 * @brief  Return the Floating-Point Control and Status Register
 */
RV_STATIC_FORCE_INLINE uint32_t __get_FCSR(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "fcsr" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set the Floating-Point Control and Status Register
 */
RV_STATIC_FORCE_INLINE void __set_FCSR(uint32_t value)
{
  __ASM volatile ("csrw fcsr, %0" : : "r" (value) );
}

/**
 * @brief  Return the Machine Status Register
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MSTATUS(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "mstatus" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set the Machine Status Register
 */
RV_STATIC_FORCE_INLINE void __set_MSTATUS(uint32_t value)
{
  __ASM volatile ("csrw mstatus, %0" : : "r" (value) );
}

/**
 * @brief  Return the Machine ISA Register
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MISA(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "misa" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set the Machine ISA Register
 */
RV_STATIC_FORCE_INLINE void __set_MISA(uint32_t value)
{
  __ASM volatile ("csrw misa, %0" : : "r" (value) );
}

/**
 * @brief  Return the Machine Interrupt Enable Register
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MIE(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "mie" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set the Machine ISA Register
 */
RV_STATIC_FORCE_INLINE void __set_MIE(uint32_t value)
{
  __ASM volatile ("csrw mie, %0" : : "r" (value) );
}


/**
 * @brief  Return the Machine Trap-Vector Base-Address Register
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MTVEC(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "mtvec" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set the Machine Trap-Vector Base-Address Register
 */
RV_STATIC_FORCE_INLINE void __set_MTVEC(uint32_t value)
{
  __ASM volatile ("csrw mtvec, %0" : : "r" (value) );
}

/**
 * @brief  Return the Machine Seratch Register
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MSCRATCH(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "mscratch" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set the Machine Seratch Register
 */
RV_STATIC_FORCE_INLINE void __set_MSCRATCH(uint32_t value)
{
  __ASM volatile ("csrw mscratch, %0" : : "r" (value) );
}

/**
 * @brief  Return the Machine Exception Program Register
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MEPC(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "mepc" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set the Machine Exception Program Register
 */
RV_STATIC_FORCE_INLINE void __set_MEPC(uint32_t value)
{
  __ASM volatile ("csrw mepc, %0" : : "r" (value) );
}

/**
 * @brief  Return the Machine Cause Register
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MCAUSE(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "mcause" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set the Machine Cause Register
 */
RV_STATIC_FORCE_INLINE void __set_MCAUSE(uint32_t value)
{
  __ASM volatile ("csrw mcause, %0" : : "r" (value) );
}

/**
 * @brief  Return the Machine Trap Value Register
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MTVAL(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "mtval" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set the Machine Trap Value Register
 */
RV_STATIC_FORCE_INLINE void __set_MTVAL(uint32_t value)
{
  __ASM volatile ("csrw mtval, %0" : : "r" (value) );
}

/**
 * @brief  Return the Machine Interrupt Pending Register
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MIP(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "mip" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set the Machine Interrupt Pending Register
 */
RV_STATIC_FORCE_INLINE void __set_MIP(uint32_t value)
{
  __ASM volatile ("csrw mip, %0" : : "r" (value) );
}

/**
 * @brief  Return Lower 32 bits of Cycle counter
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MCYCLE(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "mcycle" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set Lower 32 bits of Cycle counter
 */
RV_STATIC_FORCE_INLINE void __set_MCYCLE(uint32_t value)
{
  __ASM volatile ("csrw mcycle, %0" : : "r" (value) );
}

/**
 * @brief  Return Upper 32 bits of Cycle counter
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MCYCLEH(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "mcycleh" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set Upper 32 bits of Cycle counter
 */
RV_STATIC_FORCE_INLINE void __set_MCYCLEH(uint32_t value)
{
  __ASM volatile ("csrw mcycleh, %0" : : "r" (value) );
}

/**
 * @brief  Return Lower 32 bits of Instructions-retired counter
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MINSTRET(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "minstret" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set Lower 32 bits of Instructions-retired counter
 */
RV_STATIC_FORCE_INLINE void __set_MINSTRET(uint32_t value)
{
  __ASM volatile ("csrw minstret, %0" : : "r" (value) );
}

/**
 * @brief  Return Upper 32 bits of Instructions-retired counter
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MINSTRETH(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "minstreth" : "=r" (result) );
  return (result);
}

/**
 * @brief  Set Upper 32 bits of Instructions-retired counter
 */
RV_STATIC_FORCE_INLINE void __set_MINSTRETH(uint32_t value)
{
  __ASM volatile ("csrw minstreth, %0" : : "r" (value) );
}

/**
 * @brief  Return Vendor ID Register
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MVENDORID(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "mvendorid" : "=r" (result) );
  return (result);
}

/**
 * @brief  Return Machine Architecture ID Register
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MARCHID(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "marchid" : "=r" (result) );
  return (result);
}

/**
 * @brief  Return Machine Implementation ID Register
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MIMPID(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "mimpid" : "=r" (result) );
  return (result);
}

/**
 * @brief  Return Hart ID Register
 */
RV_STATIC_FORCE_INLINE uint32_t __get_MHARTID(void)
{
  uint32_t result;

  __ASM volatile ( "csrr %0," "mhartid" : "=r" (result) );
  return (result);
}

#endif/* __CORE_RISCV_H__ */





