/**
  ******************************************************************************
  * @file    EEPROM_Emulation/inc/eeprom.h 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    07/27/2009
  * @brief   This file contains all the functions prototypes for the EEPROM 
  *          emulation firmware library.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPROM_H
#define __EEPROM_H

/* Includes ------------------------------------------------------------------*/
#include "eeprom_fields.h"
#include "flashinfo.h"

/* Exported constants --------------------------------------------------------*/
/* Define the STM32F10Xxx Flash page size depending on the used STM32 device */
#if defined (STM32F10X_LD) || defined (STM32F10X_MD)
  #define PAGE_SIZE  (uint16_t)0x400  /* Page size = 1KByte */
#elif defined (STM32F10X_HD) || defined (STM32F10X_CL)
  #define PAGE_SIZE  (uint16_t)0x800  /* Page size = 2KByte */
#endif

/* Pages 0 and 1 base and end addresses */
#define PAGE0_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + 0x000))
#define PAGE0_END_ADDRESS       ((uint32_t)(EEPROM_START_ADDRESS + (PAGE_SIZE - 1)))

#define PAGE1_BASE_ADDRESS      ((uint32_t)(EEPROM_START_ADDRESS + PAGE_SIZE))
#define PAGE1_END_ADDRESS       ((uint32_t)(EEPROM_START_ADDRESS + (2 * PAGE_SIZE - 1)))

#if !defined(__IO)
#define     __IO    volatile             /*!< Defines 'read / write' permissions*/
#endif

/* Used Flash pages for EEPROM emulation */
typedef enum  {
  PAGE0 = 0,
  PAGE1 = 1,
  PAGE_INVALID,
} PageIndex_t;

/* Page status definitions */
typedef enum {
  PageStatusNoValidPage = 0x00AB,
  PageStatusErased = 0xFFFF,
  PageStatusReceiveData = 0xEEEE,
  PageStatusPageFull = 0x0080,
  PageStatusValidPage = 0x0000,
  PageStatusWriteError = 0x0001,
  PageStatusOk = 0x0002
} PageStatus_t;

/* Valid pages in read and write defines */

typedef enum {
  READ_FROM_VALID_PAGE = 0,
  WRITE_IN_VALID_PAGE = 1
} EEOperation_t;

/* Page full define */
#define PAGE_FULL               ((uint8_t)0x80)

/* Variables' number */
#define NumbOfVar               ((uint8_t)EEPROMFieldInvalid)

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  FLASH_BUSY = 1,
  FLASH_ERROR_WRP,
  FLASH_ERROR_PG,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
} FLASH_Status;

typedef enum {
  EEOperationOK,
  EEOperationVariableNotFound,
  EEOperationNoPages,
} EEOperationResult;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
EEOperationResult EE_ReadVariable(EEPROMFieldAddresses_t VirtAddress, uint16_t* Data);

#endif /* __EEPROM_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
