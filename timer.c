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
*	\brief		Timer control system and user interface functions.
*	\author		Pat Satyshur
*	\version	1.0
*	\date		1/20/2013
*	\copyright	Copyright 2013, Pat Satyshur
*	\ingroup 	timer_main
*
*	@{
*/

#include "main.h"
#include <avr/eeprom.h>

/**The inital values for the timer events
* This will be loaded into EEPROM
* Format: <hour>, <minute>, <day of week>, <control>
*
*	-hour: 			the hour of the event (24-hour format)
*	-minute: 		the minute of the event
*	-day of week: 	the day(s) of the week to trigger the output. 
*					A one in a bit position corresponds to an active on that DoW, starting with the LSB as day 1.
*					(ex: 0x7F is all 7 days)
*	-control:		the four LSBs correspond to the outputs states to be set by the event, with the LSB being output 0. 
*					A '1' in the corresponding position indicates that the output is turned on by the event.
*/
const uint8_t EEMEM EventList[TIMER_MAX_NUMBER_OF_EVENTS][4] = {
	{6,		 0,		0x7F,	0x0F},		//all on a 6am
	{20,	 0,		0x7F,	0x00},		//all off at 8pm
	{0,		 0,		0x00,	0x00},
	{0,		 0,		0x00,	0x00} };



void TimerInit( void )
{
	


	return;
}

//TODO: Make this return somthing to indicate whether events were found in EEPROM
void TimerLoadEvents(uint8_t *RAM_EventList[TIMER_MAX_NUMBER_OF_EVENTS][4])
{
	uint8_t i;
	uint8_t j;
	//uint8_t RAM_EventList[TIMER_MAX_NUMBER_OF_EVENTS][4];
	uint8_t TempEvent[4];
	uint16_t TempTime;
	uint16_t TempTime2;
	
	printf("Reading events from EEPROM...\n");
	for(i=0; i<TIMER_MAX_NUMBER_OF_EVENTS; i++)
	{
		*RAM_EventList[i][0] = eeprom_read_byte(&EventList[i][0]);
		*RAM_EventList[i][1] = eeprom_read_byte(&EventList[i][1]);
		*RAM_EventList[i][2] = eeprom_read_byte(&EventList[i][2]);
		*RAM_EventList[i][3] = eeprom_read_byte(&EventList[i][3]);
		if( (i == 0) && (*RAM_EventList[i][0] == 0xFF) )		//EEPROM reads as 0xFF when blank
		{
			printf_P(PSTR("Events not found in EEPROM\n"));
		}
		else if(*RAM_EventList[i][0] != 0xFF)
		{
			printf("Event %u: %02u:%02u, 0x%02X, 0x%02X\n", i, *RAM_EventList[i][0], *RAM_EventList[i][1], *RAM_EventList[i][2], *RAM_EventList[i][3]);
		}
	}
	
	return;
}

//events should be sorted before calling this...
//Each event represents a change of state of the inputs.
void ParseEvents(uint8_t *EventMatrix[TIMER_MAX_NUMBER_OF_EVENTS][4])
{
	uint8_t CurrentTimerState;
	uint8_t OldTimerState;

	return;
}

void SortEvents(uint8_t *EventMatrix[TIMER_MAX_NUMBER_OF_EVENTS][4])
{
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t k = 0;
	uint16_t TempTime1 = 0;
	uint16_t TempTime2 = 0;
	
	TempTime1 = ((*EventMatrix[0][1]) + (*EventMatrix[0][0]*60));
	for(i=0;i<TIMER_MAX_NUMBER_OF_EVENTS; i++)
	{
		for(j=i; j<TIMER_MAX_NUMBER_OF_EVENTS; j++)
		{
			TempTime2 = ((*EventMatrix[i][1]) + (*EventMatrix[i][0]*60));
			if( (TempTime2 < TempTime1) && (TempTime2 > 0) )
			{
				TempTime1 = TempTime2;
				k = j;
			}
		}
		EventSwitch(EventMatrix, i, k);
	}
	
	return;
}


void EventSwitch(uint8_t *EventMatrix[TIMER_MAX_NUMBER_OF_EVENTS][4], uint8_t EventNumber1, uint8_t EventNumber2)
{
	uint8_t TempEvent[4];
	if((EventNumber1 != EventNumber2) && (EventNumber1 < TIMER_MAX_NUMBER_OF_EVENTS) && (EventNumber2 < TIMER_MAX_NUMBER_OF_EVENTS))
	{
		TempEvent[0] = *EventMatrix[EventNumber1][0];
		TempEvent[1] = *EventMatrix[EventNumber1][1];
		TempEvent[2] = *EventMatrix[EventNumber1][2];
		TempEvent[3] = *EventMatrix[EventNumber1][3];
		
		EventMatrix[EventNumber1][0] = EventMatrix[EventNumber2][0];
		EventMatrix[EventNumber1][1] = EventMatrix[EventNumber2][1];
		EventMatrix[EventNumber1][2] = EventMatrix[EventNumber2][2];
		EventMatrix[EventNumber1][3] = EventMatrix[EventNumber2][3];
		
		*EventMatrix[EventNumber2][0] = TempEvent[0];
		*EventMatrix[EventNumber2][1] = TempEvent[1];
		*EventMatrix[EventNumber2][2] = TempEvent[2];
		*EventMatrix[EventNumber2][3] = TempEvent[3];
	}
	return;
}



/** @} */