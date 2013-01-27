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
*	\brief		Command interpreter application specific functions
*	\author		Pat Satyshur
*	\version	1.1
*	\date		1/13/2013
*	\copyright	Copyright 2013, Pat Satyshur
*	\ingroup 	timer_main
*
*	@{
*/

#include "main.h"
//#include "commands.h"


//The number of commands
const uint8_t NumCommands = 9;

//Handler function declerations

//LED control function
static int _F1_Handler (void);
const char _F1_NAME[] PROGMEM 			= "led";
const char _F1_DESCRIPTION[] PROGMEM 	= "Turn LED on or off";
const char _F1_HELPTEXT[] PROGMEM 		= "led <number>";

//Get time from RTC
static int _F2_Handler (void);
const char _F2_NAME[] PROGMEM 			= "gettime";
const char _F2_DESCRIPTION[] PROGMEM 	= "Show the current date and time";
const char _F2_HELPTEXT[] PROGMEM 		= "'gettime' has no parameters";

//Set time on RTC
static int _F3_Handler (void);
const char _F3_NAME[] PROGMEM 			= "settime";
const char _F3_DESCRIPTION[] PROGMEM 	= "Set the time";
const char _F3_HELPTEXT[] PROGMEM 		= "settime <month> <day> <year> <hr> <min> <sec>";

//Read temperature from the RTC
static int _F4_Handler (void);
const char _F4_NAME[] PROGMEM 			= "temp";
const char _F4_DESCRIPTION[] PROGMEM 	= "Get the temperature from the RTC";
const char _F4_HELPTEXT[] PROGMEM 		= "'temp' has no parameters";

//Buzzer
static int _F5_Handler (void);
const char _F5_NAME[] PROGMEM 			= "beep";
const char _F5_DESCRIPTION[] PROGMEM 	= "Test the buzzer";
const char _F5_HELPTEXT[] PROGMEM 		= "beep <time (ms)>";

//Scan the TWI bus for devices
static int _F6_Handler (void);
const char _F6_NAME[] PROGMEM 			= "twiscan";
const char _F6_DESCRIPTION[] PROGMEM 	= "Scan for TWI devices";
const char _F6_HELPTEXT[] PROGMEM 		= "'twiscan' has no parameters";

//Set an alarm on the rtc
static int _F7_Handler (void);
const char _F7_NAME[] PROGMEM 			= "alarm";
const char _F7_DESCRIPTION[] PROGMEM 	= "Set the alarm";
const char _F7_HELPTEXT[] PROGMEM 		= "alarm <hr> <min>";

//Get the status of the RTC
static int _F8_Handler (void);
const char _F8_NAME[] PROGMEM 			= "rstat";
const char _F8_DESCRIPTION[] PROGMEM 	= "Show the status of the RTC";
const char _F8_HELPTEXT[] PROGMEM 		= "'rstat' has no parameters";

//Get the timer events
static int _F9_Handler (void);
const char _F9_NAME[] PROGMEM 			= "events";
const char _F9_DESCRIPTION[] PROGMEM 	= "Get the timer events";
const char _F9_HELPTEXT[] PROGMEM 		= "'events' has no parameters";

//Command list
const CommandListItem AppCommandList[] PROGMEM =
{
	{ _F1_NAME, 1,  2, _F1_Handler, _F1_DESCRIPTION, _F1_HELPTEXT },
	{ _F2_NAME, 0,  0, _F2_Handler, _F2_DESCRIPTION, _F2_HELPTEXT },
	{ _F3_NAME, 6,  6, _F3_Handler, _F3_DESCRIPTION, _F3_HELPTEXT },
	{ _F4_NAME, 0,  0, _F4_Handler, _F4_DESCRIPTION, _F4_HELPTEXT },
	{ _F5_NAME, 1,  1, _F5_Handler, _F5_DESCRIPTION, _F5_HELPTEXT },
	{ _F6_NAME, 0,  0, _F6_Handler, _F6_DESCRIPTION, _F6_HELPTEXT },
	{ _F7_NAME, 2,  2, _F7_Handler, _F7_DESCRIPTION, _F7_HELPTEXT },
	{ _F8_NAME, 0,  0, _F8_Handler, _F8_DESCRIPTION, _F8_HELPTEXT },
	{ _F9_NAME, 0,  0, _F9_Handler, _F9_DESCRIPTION, _F9_HELPTEXT },
};

//Command functions

//Change LED state
static int _F1_Handler (void)
{
	if( NumberOfArguments() == 1 )
	{
		if(argAsInt(1) == 1)
		{
			LED(1,1);
			LED(2,1);
			LED(3,1);
		}
		else
		{
			LED(1,0);
			LED(2,0);
			LED(3,0);
		}
	}
	else
	{
		LED( (uint8_t)argAsInt(1), (uint8_t)argAsInt(2) );
	}
	return 0;
}

//Get date and time from RTC
static int _F2_Handler (void)
{

	TimeAndDate CurrentTime;
	DS3232M_GetTime(&CurrentTime);
	printf("%02u/%02u/20%02u %02u:%02u:%02u\n", CurrentTime.month, CurrentTime.day, CurrentTime.year, CurrentTime.hour, CurrentTime.min, CurrentTime.sec);
	return 0;
}

//Set time on RTC
static int _F3_Handler (void)
{
	TimeAndDate CurrentTime;
	
	CurrentTime.month	= argAsInt(1);
	CurrentTime.day		= argAsInt(2);
	CurrentTime.year	= argAsInt(3);
	CurrentTime.hour	= argAsInt(4);
	CurrentTime.min		= argAsInt(5);
	CurrentTime.sec		= argAsInt(6);
	
	DS3232M_SetTime(&CurrentTime);
	printf_P(PSTR("Done\n"));
	return 0;
}

//Read temperature from the RTC
static int _F4_Handler (void)
{
	int8_t TempLHS;
	uint8_t TempRHS;

	if(DS3232M_GetTemp(&TempLHS, &TempRHS) == 0)
	{
		printf("Temp: %d.%d C\n", TempLHS, TempRHS);
	}
	return 0;
}

//Buzzer
static int _F5_Handler (void)
{
	uint16_t TimeToBeep = argAsInt(1);
	Buzzer (TimeToBeep);
	return 0;
}

//Scan the TWI bus for devices
static int _F6_Handler (void)
{
	InitTWI();
	TWIScan();
	return  0;
}

static int _F7_Handler (void)
{
	TimeAndDate CurrentTime;
	CurrentTime.hour	= argAsInt(1);
	CurrentTime.min		= argAsInt(2);
	CurrentTime.sec		= 0;

	DS3232M_SetAlarm(1, 0x08,  &CurrentTime);
	DS3232M_EnableAlarm(1);

	return 0;
}

static int _F8_Handler (void)
{
	DS3232M_GetStatus();
	return 0;
}

static int _F9_Handler (void)
{
	char subcommand[6];
	uint8_t RAM_EventList[TIMER_MAX_NUMBER_OF_EVENTS][4];
	
	TimerLoadEvents(&RAM_EventList);
	printf_P(PSTR("Enter an command: "));
	GetNewCommand();
	
	//get the entered command
	argAsChar(0, subcommand);
	if(strcmp(subcommand, "add") == 0)
	{
		if(NumberOfArguments() == 4)
		{
			argAsInt(1);
			argAsInt(2);
			argAsInt(3);
			argAsInt(5);
		}
		else
		{
			printf_P(PSTR("Expected four arguments\n"));
		}
	}
	
	
	
	
	/*printf("numcommands: %u\n", NumberOfArguments());
	argAsChar(0, tempstring);
	printf("0: %s\n", tempstring);
	argAsChar(1, tempstring);
	printf("1: %s\n", tempstring);
	argAsChar(2, tempstring);
	printf("2: %s\n", tempstring);*/

	return 0;
}

ISR(USART_RX_vect)
{
	uint8_t c;
	c = UDR0;				//Get char from UART recieve buffer
	CommandGetInputChar(c);
	//UDR0 = c;				//Send char out on UART transmit buffer
}

/** @} */
