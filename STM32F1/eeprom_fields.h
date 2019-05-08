/*
 * eeprom_fields.h
 *
 *  Created on: 2017. nov. 29.
 *      Author: mm
 */

#ifndef EEPROM_FIELDS_H_
#define EEPROM_FIELDS_H_

#include <stdint.h>

typedef enum {
  EEPROMFieldAutoPower = 0,
  EEPROMFieldSN_H = 1,
  EEPROMFieldSN_L = 2,
  EEPROMField_StayInBoot = 3,
  // Invalid field
  EEPROMFieldInvalid
} EEPROMFieldAddresses_t;

#endif /* EEPROM_FIELDS_H_ */
