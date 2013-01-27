/*   This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/** \file
*	\brief		Main code header file for the standalone timer module
*	\author		Pat Satyshur
*	\version	1.1
*	\date		1/20/2013
*	\copyright	Copyright 2013, Pat Satyshur
*	\ingroup 	timer_main
*
*	@{
*/

#ifndef _MAIN_H_
#define _MAIN_H_

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include <avr/interrupt.h>
#include <avr/pgmspace.h> 
#include <stdio.h>
#include <string.h>

//Common file includes
//#include "config.h"
#include "UART.h"
#include "commands.h"
#include "twi.h"
#include "ds3232m.h"
#include "timer.h"

/** Control the LEDs
*
* \param[in]	LEDNumber The number of the LED to control. Valid inputs are 1-3.
* \param[in]	State The new state for the LED. 1 is on, 0 is off.
*/
void LED (uint8_t LEDNumber, uint8_t State);

void Buzzer (uint16_t TimeToBuzzMS);

#endif

/** @} */