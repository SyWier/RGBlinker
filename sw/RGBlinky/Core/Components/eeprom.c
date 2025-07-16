/*
 * eeprom.c
 *
 *  Created on: Jul 16, 2025
 *      Author: Daniel
 */

#include "eeprom.h"
#include "log.h"
#include "eeprom_emul.h"
#include "led.h"
#include "animation.h"

// Read out variables from memory when device starts
void eeprom_start(void) {
	// Initialize EEPROM_Emul
	log_message(LOG_INF, "EEPROM", "Init EEPROM");
	HAL_FLASH_Unlock();
	if(EE_OK != EE_Init(EE_CONDITIONAL_ERASE)) {
		log_message(LOG_ERR, "EEPROM", "Failed to init EEPROM");
	}

	// Read out RgbBrigntnessScale
	if(EE_OK != EE_ReadVariable8bits(1, &RgbBrigntnessScale)) {
		log_message(LOG_ERR, "EEPROM", "Failed to read RgbBrigntnessScale");
		log_message(LOG_ERR, "EEPROM", "Trying to write default brightness value ...");
		RgbBrigntnessScale = 2;
		if(EE_OK != EE_WriteVariable8bits(1, RgbBrigntnessScale)) {
			log_message(LOG_ERR, "EEPROM", "Failed to write RgbBrigntnessScale value");
		}
	}
	else {
		log_message(LOG_DBG, "EEPROM", "RgbBrigntnessScale value: %d", RgbBrigntnessScale);
		_RgbBrigntnessScale_old = RgbBrigntnessScale;
	}

	// Read out animationIndex
	if(EE_OK != EE_ReadVariable8bits(2, &Animator.animationIndex)) {
		log_message(LOG_ERR, "EEPROM", "Failed to read animationIndex");
		log_message(LOG_ERR, "EEPROM", "Trying to write default animationIndex value ...");
		Animator.animationIndex = 0;
		if(EE_OK != EE_WriteVariable8bits(2, Animator.animationIndex)) {
			log_message(LOG_ERR, "EEPROM", "Failed to write animationIndex value");
		}
	}
	else {
		log_message(LOG_DBG, "EEPROM", "animationIndex value: %d", Animator.animationIndex);
		_animationIndex_old = Animator.animationIndex;
	}
}

// Write out changed variables to memory when device shuts down
void eeprom_end(void) {
	if(_RgbBrigntnessScale_old != RgbBrigntnessScale) {
		log_message(LOG_INF, "EEPROM", "New RgbBrigntnessScale value");
		if(EE_OK != EE_WriteVariable8bits(1, RgbBrigntnessScale)) {
			log_message(LOG_ERR, "EEPROM", "Failed to write RgbBrigntnessScale");
		}
	}

	if(_animationIndex_old != Animator.animationIndex) {
		log_message(LOG_INF, "EEPROM", "New animationIndex value");
		if(EE_OK != EE_WriteVariable8bits(2, Animator.animationIndex)) {
			log_message(LOG_ERR, "EEPROM", "Failed to write animationIndex");
		}
	}
}


