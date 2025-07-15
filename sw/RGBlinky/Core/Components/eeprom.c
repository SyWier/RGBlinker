/*
 * eeprom.c
 *
 *  Created on: Jul 16, 2025
 *      Author: Daniel
 */

#include "eeprom.h"ű
#include "log.h"
#include "eeprom_emul.h"
#include "led.h"

void eeprom_init(void) {
	// Initialize EEPROM_Emul
	log_message(LOG_INF, "EEPROM", "Init EEPROM");
	HAL_FLASH_Unlock();
	if(EE_OK != EE_Init(EE_CONDITIONAL_ERASE)) {
		log_message(LOG_ERR, "EEPROM", "Failed to init EEPROM");
	}

	// Read out RgbBrigntnessScale
	if(EE_OK != EE_ReadVariable8bits(1, &RgbBrigntnessScale)) {
			log_message(LOG_ERR, "EEPROM", "Failed to read EEPROM");
			log_message(LOG_ERR, "EEPROM", "Trying to write default brightness value ...");
			RgbBrigntnessScale = 2;
			if(EE_OK != EE_WriteVariable8bits(1, RgbBrigntnessScale)) {
				log_message(LOG_ERR, "EEPROM", "Failed to write RgbBrigntnessScale value");
			}
		}
		else {
			log_message(LOG_INF, "EEPROM", "RgbBrigntnessScale value: %d", RgbBrigntnessScale);
			_RgbBrigntnessScale_old = RgbBrigntnessScale;
		}
}

// Write out RgbBrigntnessScale
void eeprom_write(void) {
	if(_RgbBrigntnessScale_old == RgbBrigntnessScale) {
		return;
	}
	log_message(LOG_INF, "EEPROM", "New RgbBrigntnessScale value");

	if(EE_OK != EE_WriteVariable8bits(1, RgbBrigntnessScale)) {
			log_message(LOG_ERR, "EEPROM", "Failed to write EEPROM");
	}
	log_message(LOG_DBG, "EEPROM", "Data written successfully");
}


