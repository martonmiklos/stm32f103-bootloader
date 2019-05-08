#ifndef FLASHINFO___H__ 
#define FLASHINFO___H__

#define FLASH_START_ADDRESS    	((uint32_t)0x08000000)

#define FLASH_SECTOR_SIZE            	1024
#define FLASH_SECTOR_COUNT           	32

#define FLASH_SIZE             	((uint32_t)(FLASH_SECTOR_COUNT * FLASH_SECTOR_SIZE)) // STM32F103C6T8 has 32 KB flash memory
#define FLASH_END              	(FLASH_START_ADDRESS + FLASH_SIZE - 1)

#define BOOTLOADER_SECTOR_COUNT 8
#define BOOTLOADER_SIZE		BOOTLOADER_SECTOR_COUNT * FLASH_SECTOR_SIZE

// user app starts at 8KB
// at 0x8002000
#define USER_APP_START         	(FLASH_START_ADDRESS + BOOTLOADER_SIZE)

// EEPROM emulation start address: after 30KByte of used Flash memory
// at 0x8007800
#define EEPROM_START_ADDRESS   	FLASH_START_ADDRESS + (FLASH_SIZE - (2 * FLASH_SECTOR_SIZE))

#endif
