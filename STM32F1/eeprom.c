/**
  ******************************************************************************
  * @file    EEPROM_Emulation/src/eeprom.c 
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    07/27/2009
  * @brief   This file provides all the EEPROM emulation firmware functions.
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
/** @addtogroup EEPROM_Emulation
  * @{
  */ 

/* Includes ------------------------------------------------------------------*/
#include "eeprom.h"
#include "flashinfo.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Global variable used to store variable value in read sequence */
uint16_t DataVar = 0;

/* Virtual address defined by the user: 0xFFFF value is prohibited */
/* Virtual address defined by the user: 0xFFFF value is prohibited */
uint16_t VirtAddVarTab[EEPROMFieldInvalid] = {
    EEPROMFieldAutoPower,
    EEPROMFieldSN_H,
    EEPROMFieldSN_L,
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static PageIndex_t EE_FindValidPage(EEOperation_t Operation);

/**
  * @brief  Returns the last stored variable data, if found, which correspond to
  *   the passed virtual address
  * @param  VirtAddress: Variable virtual address
  * @param  Data: Global variable contains the read variable value
  * @retval Success or error status:
  *           - 0: if variable was found
  *           - 1: if the variable was not found
  *           - NO_VALID_PAGE: if no valid page was found.
  */
EEOperationResult EE_ReadVariable(EEPROMFieldAddresses_t VirtAddress, uint16_t* Data)
{
  PageIndex_t ValidPage = PAGE0;
  uint16_t AddressValue;
  uint32_t Address, PageStartAddress;

  /* Get active Page for read operation */
  ValidPage = EE_FindValidPage(READ_FROM_VALID_PAGE);

  /* Check if there is no valid page */
  if (ValidPage == PAGE_INVALID) {
    return EEOperationNoPages;
  }

  /* Get the valid Page start Address */
  PageStartAddress = (uint32_t)(EEPROM_START_ADDRESS + (uint32_t)(ValidPage * PAGE_SIZE));

  /* Get the valid Page end Address */
  Address = (uint32_t)((EEPROM_START_ADDRESS - 2) + (uint32_t)((1 + ValidPage) * PAGE_SIZE));

  /* Check each active page address starting from end */
  while (Address > (PageStartAddress + 2)) {
    /* Get the current location content to be compared with virtual address */
    AddressValue = (*(__IO uint16_t*)Address);

    /* Compare the read address with the virtual address */
    if (AddressValue == VirtAddress) {
      /* Get content of Address-2 which is variable value */
      *Data = (*(__IO uint16_t*)(Address - 2));

      /* In case variable value is read, reset ReadStatus flag */
      return EEOperationOK;
    } else {
      /* Next address location */
      Address = Address - 4;
    }
  }
  return EEOperationVariableNotFound;
}

/**
  * @brief  Find valid Page for write or read operation
  * @param  Operation: operation to achieve on the valid page.
  *   This parameter can be one of the following values:
  *     @arg READ_FROM_VALID_PAGE: read operation from valid page
  *     @arg WRITE_IN_VALID_PAGE: write operation from valid page
  * @retval Valid page number (PAGE0 or PAGE1) or NO_VALID_PAGE in case
  *   of no valid page was found
  */
static PageIndex_t EE_FindValidPage(EEOperation_t Operation)
{
  PageStatus_t PageStatus0, PageStatus1;

  /* Get Page0 actual status */
  PageStatus0 = (*(__IO uint16_t*)PAGE0_BASE_ADDRESS);

  /* Get Page1 actual status */
  PageStatus1 = (*(__IO uint16_t*)PAGE1_BASE_ADDRESS);

  /* Write or read operation */
  switch (Operation) {
    case WRITE_IN_VALID_PAGE:   /* ---- Write operation ---- */
      if (PageStatus1 == PageStatusValidPage) {
        /* Page0 receiving data */
        if (PageStatus0 == PageStatusReceiveData) {
          return PAGE0;         /* Page0 valid */
        } else {
          return PAGE1;         /* Page1 valid */
        }
      } else if (PageStatus0 == PageStatusValidPage) {
        /* Page1 receiving data */
        if (PageStatus1 == PageStatusReceiveData) {
          return PAGE1;         /* Page1 valid */
        } else {
          return PAGE0;         /* Page0 valid */
        }
      } else {
        return PAGE_INVALID;   /* No valid Page */
      }
    case READ_FROM_VALID_PAGE:  /* ---- Read operation ---- */
      if (PageStatus0 == PageStatusValidPage) {
        return PAGE0;           /* Page0 valid */
      } else if (PageStatus1 == PageStatusValidPage) {
        return PAGE1;           /* Page1 valid */
      } else {
        return PAGE_INVALID;  /* No valid Page */
      }
    default:
      return PAGE0;             /* Page0 valid */
  }
}
