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
*	\brief		Timer control system and user interface function headers.
*	\author		Pat Satyshur
*	\version	1.0
*	\date		1/20/2013
*	\copyright	Copyright 2013, Pat Satyshur
*	\ingroup 	timer_main
*
*	@{
*/

#ifndef _TIMER_SCHEDULER_H_
#define _TIMER_SCHEDULER_H_

#include "stdint.h"

#define TIMER_MAX_NUMBER_OF_EVENTS		5

void TimerInit( void );

void TimerLoadEvents(uint8_t *RAM_EventList[TIMER_MAX_NUMBER_OF_EVENTS][4]);

void SortEvents(uint8_t *EventMatrix[TIMER_MAX_NUMBER_OF_EVENTS][4]);
void ParseEvents(uint8_t *EventMatrix[TIMER_MAX_NUMBER_OF_EVENTS][4]);
void EventSwitch(uint8_t *EventMatrix[TIMER_MAX_NUMBER_OF_EVENTS][4], uint8_t EventNumber1, uint8_t EventNumber2);






#endif
/** @} */
