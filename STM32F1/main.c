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

/**
 *  @file main.c
 *
 *  @brief main loop and calling any hardware init stuff.
 *  logic to handle bootloader entry and jumping to user code.
 */

#include "common.h"
#include "dfu.h"
#include "hardware.h"
#include "eeprom.h"

#include <stdbool.h>

int main() 
{
    uint16_t stayInBootKey = 0;
    systemReset();  // peripherals but not PC
    setupCLK(); 	// not USB, that is handled by USB portion
    setupLEDAndButton();


    if (EE_ReadVariable(EEPROMField_StayInBoot, &stayInBootKey) != EEOperationOK
                || stayInBootKey != 0x4242
                || !checkUserCode(USER_CODE_FLASH0X8002000)) {
		// do the startup LED quickflash
	 	strobePin(LED_BANK, LED_PIN, STARTUP_BLINKS, BLINK_FAST, LED_ON_STATE);

		// only set up USB and flash if in the bootloader (as we are now)
		setupUSB();
    	setupFLASH();

    	while (1) {
	        // is DFU in progress?
	        if (dfuUploadStarted()) {
	        	// wait until we're done
	        	while (!dfuUploadDone());
	        	// success flash, we also need to wait a little longer for manifest to be sent
	        	// otherwise it will be successful but we'll get "unable to read DFU status" error

	        	// flashing faster once we're done takes time, this serves as our wait
	        	strobePin(LED_BANK, LED_PIN, STARTUP_BLINKS, BLINK_FAST, LED_ON_STATE);

	            // uncomment the following line if you always want to execute code after completion
	            break;
	        } else {
	        	// while in bootloader, flash the LED slowly
	        	strobePin(LED_BANK, LED_PIN, 1, BLINK_SLOW, LED_ON_STATE);
	        }
	    }
	}

	if (checkUserCode(USER_CODE_FLASH0X8002000))  {
		jumpToUser(USER_CODE_FLASH0X8002000);
	}

	return 0;
}
