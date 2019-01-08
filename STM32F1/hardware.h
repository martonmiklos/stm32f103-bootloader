/* *****************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 LeafLabs LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ****************************************************************************/

#ifndef __HARDWARE_H
#define __HARDWARE_H

#include "stm32f10x_type.h"
#include "cortexm3_macro.h"
#include "common.h"

/* macro'd register and peripheral definitions */
#define PERIPH_BASE ((u32)0x40000000)

#define APB1_BASE   PERIPH_BASE
#define APB2_BASE   (PERIPH_BASE + 0x10000)

#define RCC_BASE    (PERIPH_BASE + 0x21000)
#define FLASH_BASE  (PERIPH_BASE + 0x22000)

#define GPIOA       (PERIPH_BASE + 0x10800)
#define GPIOB       (PERIPH_BASE + 0x10C00)
#define GPIOC       (PERIPH_BASE + 0x11000)
#define GPIOD       (PERIPH_BASE + 0x11400)

#define AFIO_BASE   (APB2_BASE + 0x0000)
#define AFIO_MAPR   (AFIO_BASE + 0x04)

#define FLASH_KEY1     0x45670123
#define FLASH_KEY2     0xCDEF89AB
#define FLASH_RDPRT    0x00A5
#define FLASH_SR_BSY   0x01
#define FLASH_CR_PER   0x02
#define FLASH_CR_PG    0x01
#define FLASH_CR_START 0x40

#define GPIO_CRL(port)  port
#define GPIO_CRH(port)  (port+0x04)
#define GPIO_IDR(port)  (port+0x08)
#define GPIO_ODR(port)  (port+0x0c)
#define GPIO_BSRR(port) (port+0x10)
#define GPIO_CR(port,pin) (port + (0x04*(pin>7)))

#define CR_OUTPUT_OD 	0x05
#define CR_OUTPUT_PP	0x01	
#define CR_INPUT 	  	0x04
#define CR_INPUT_PU_PD	0x08

#define SCS_BASE   ((u32)0xE000E000)
#define NVIC_BASE  (SCS_BASE + 0x0100)
#define SCB_BASE   (SCS_BASE + 0x0D00)

#define STK_BASE   (SCS_BASE + 0x0010)
#define STK_CTRL   (STK_BASE + 0x0000)

#define TIM1_APB2_ENB ((u32)0x00000800)
#define TIM1          ((u32)0x40012C00)
#define TIM1_PSC      (TIM1+0x28)
#define TIM1_ARR      (TIM1+0x2C)
#define TIM1_RCR      (TIM1+0x30)
#define TIM1_CR1      (TIM1+0x00)
#define TIM1_CR2      (TIM1+0x04)
#define TIM1_DIER     (TIM1+0x0C)
#define TIM1_UP_IRQ_Channel ((u8)0x19)

#define USB_HP_IRQ  ((u8)0x13)
#define USB_LP_IRQ  ((u8)0x14)
#define TIM2_IRQ    ((u8)0x1C)


/* AIRCR  */
#define AIRCR_RESET         0x05FA0000
#define AIRCR_RESET_REQ     (AIRCR_RESET | (u32)0x04);

/* temporary copyage of example from kiel */
#define __VAL(__TIMCLK, __PERIOD) ((__TIMCLK/1000000UL)*__PERIOD)
#define __PSC(__TIMCLK, __PERIOD)  (((__VAL(__TIMCLK, __PERIOD)+49999UL)/50000UL) - 1)
#define __ARR(__TIMCLK, __PERIOD) ((__VAL(__TIMCLK, __PERIOD)/(__PSC(__TIMCLK, __PERIOD)+1)) - 1)

// SWD and JTAG DEBUGGING
#define AFIO_MAPR_SWJ_CFG                      (0x7 << 24)
#define AFIO_MAPR_SWJ_CFG_FULL_SWJ             (0x0 << 24)
#define AFIO_MAPR_SWJ_CFG_FULL_SWJ_NO_NJRST    (0x1 << 24)
#define AFIO_MAPR_SWJ_CFG_NO_JTAG_SW           (0x2 << 24)
#define AFIO_MAPR_SWJ_CFG_NO_JTAG_NO_SW        (0x4 << 24)

// more bit twiddling to set Control register bits
#define CR_SHIFT(pin) ((pin - 8*(pin>7))<<2)

#define REG_GET(addr)           (*(vu32*)(addr))

#define REG_SET(addr,val)       do { *(vu32*)(addr) = val; } while(0)
#define REG_BOR(addr,val)       do { *(vu32*)(addr) |= val; } while(0)
#define REG_BAND(addr,val)      do { *(vu32*)(addr) &= val; } while(0)



 typedef struct {
    vu32 ISER[2];
    u32  RESERVED0[30];
    vu32 ICER[2];
    u32  RSERVED1[30];
    vu32 ISPR[2];
    u32  RESERVED2[30];
    vu32 ICPR[2];
    u32  RESERVED3[30];
    vu32 IABR[2];
    u32  RESERVED4[62];
    vu32 IPR[15];
} NVIC_TypeDef;

typedef struct {
    u8 NVIC_IRQChannel;
    u8 NVIC_IRQChannelPreemptionPriority;
    u8 NVIC_IRQChannelSubPriority;
    bool NVIC_IRQChannelCmd; /* TRUE for enable */
} NVIC_InitTypeDef;

typedef struct {
    vuc32 CPUID;
    vu32 ICSR;
    vu32 VTOR;
    vu32 AIRCR;
    vu32 SCR;
    vu32 CCR;
    vu32 SHPR[3];
    vu32 SHCSR;
    vu32 CFSR;
    vu32 HFSR;
    vu32 DFSR;
    vu32 MMFAR;
    vu32 BFAR;
    vu32 AFSR;
} SCB_TypeDef;

typedef struct {
    vu32 CR;
    vu32 CFGR;
    vu32 CIR;
    vu32 APB2RSTR;
    vu32 ABP1RSTR;
    vu32 AHBENR;
    vu32 APB2ENR;
    vu32 APB1ENR;
    vu32 RCC_BDCR;
    vu32 CSR;
} RCC_TypeDef;

typedef struct {
    vu32 ACR;
    vu32 KEYR;
    vu32 OPTKEYR;
    vu32 SR;
    vu32 CR;
    vu32 AR;
    vu32 RESERVED0;
    vu32 OBR;
    vu32 WRPR;
} FLASH_TypeDef;


//void setPin(u32 bank, u8 pin);
//void resetPin(u32 bank, u8 pin);
void gpio_write_bit(u32 bank, u8 pin, u8 val);
unsigned int crMask(int pin);

void strobePin(u32 bank, u8 pin, u8 count, u32 rate,u8 onState);

#if defined(BUTTON_BANK) && defined (BUTTON_PIN) && defined (BUTTON_ON_STATE)
bool readButtonState();
#else
#define readButtonState() FALSE
#endif

void systemHardReset(void);
void systemReset(void);
void setupCLK(void);
void setupLEDAndButton(void);
void setupFLASH(void);
bool checkUserCode(u32 usrAddr);
void jumpToUser(u32 usrAddr);

bool flashWriteWord(u32 addr, u32 word);
bool flashErasePage(u32 addr);
bool flashErasePages(u32 addr, u16 n);
void flashLock(void);
void flashUnlock(void);
void nvicInit(NVIC_InitTypeDef *);
void nvicDisableInterrupts(void);

int getFlashEnd(void);
int getFlashPageSize(void);

	
#endif
