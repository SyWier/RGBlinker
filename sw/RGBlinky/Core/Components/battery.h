/*
 * battery.h
 *
 *  Created on: Jun 11, 2025
 *      Author: Daniel
 */

#ifndef COMPONENTS_BATTERY_H_
#define COMPONENTS_BATTERY_H_

#include "main.h"

void Battery_Init();
uint16_t Battery_Get();
uint8_t Battery_Percent(uint16_t millivolts);
void Battery_Print();
void Battery_Gauge();
void PowerOff();

#endif /* COMPONENTS_BATTERY_H_ */
