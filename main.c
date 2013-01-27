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
*	\brief		Main code for the standalone timer module
*	\author		Pat Satyshur
*	\version	1.1
*	\date		1/20/2013
*	\copyright	Copyright 2013, Pat Satyshur
*	\ingroup 	timer_main
*
*	\defgroup	timer_main Standalone Timer
*
*	@{
*/
#include "main.h"

#define sbi(var, mask)   ((var) |= (uint8_t)(1 << mask))
#define cbi(var, mask)   ((var) &= (uint8_t)~(1 << mask))

#define STATUS_LED 0

/** Initalizes the hardware of the ATMega328p and the RTC*/
void hwinit(void);

/** Makes short delays. This function uses for loops, so the delays are probably not exactly 1ms */
void delay_ms(uint16_t x); 		// general purpose delay

int main (void)
{
    hwinit(); 				//Setup IO pins and defaults
	InitTWI();
	DS3232M_Init();
	UARTinit();				//Setup UART
	UARTRXINTON();			//Enable UART receive interrupt
	sei();
	
	LED(3,1);
	printf_P(PSTR("Waiting for command:\n"));
	
    while(1)
    {
		RunCommand();
		//asm volatile ("nop");
    }
   
    return(0);
}

void hwinit (void)
{
	//Disable watchdog if enabled by bootloader/fuses
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	
	//Disable JTAG (this command must be sent twice)
	MCUCR = 0x80;
	MCUCR = 0x80;

	//Disable clock division
	clock_prescale_set(clock_div_1);

	//---------------------------------------------------------------
	//Setup ports
	//---------------------------------------------------------------
    //DDRx Function:	1 = output, 0 = input
	//PORTx Function:	1 = pull up, 0 = high impedance (as input)
	//					1 = high, 0 = low 				(as output)
	//---------------------------------------------------------------
	
	//Port B: 
	//  2: AVR CS (input, pullup)  This pin is the AVR's CS pin. It is not used, but must be a high input or an output for proper functioning of the SPI system
	DDRB = 0x00;
	PORTB = (1<<2);
	
	//Port C:
	//	0: LED1 (output, low)
	//	1: LED2 (output, low)
	//	2: LED3 (output, low)
	//	3: Buzzer (output, low)
	DDRC = (1<<0)|(1<<1)|(1<<2)|(1<<3);
	PORTC = 0x00;
	
	//Port D:
	//  5: RTC Interrupt (input, pullup)
	DDRD = 0x00;
	PORTD = (1<<5);
	//---------------------------------------------------------------
	
	//Set up pin change interrupts
	//	PCINT21 - interrupt from RTC
	//	PCINT23 - interrupt from external I/O
	PCIFR = 0x07;	//Clear interrupt flags
	PCICR = 0x04;	//Enable PCI2
	PCMSK2 = (1<<5); // | (1<<7);	//Enable PCINT pins
	
	//Setup timer 1 for driving the buzzer at 4kHz
	//CTC mode
	//Clock will be FCPU, but it is not turned on here
	TCCR1A = 0x00;
	TCCR1B = 0x08;
	OCR1AH = 0x03;
	OCR1AL = 0xE8;
	TIMSK1 = 0x02;
	TCNT1H = 0x00;
	TCNT1L = 0x00;
}

//General short delays
void delay_ms(uint16_t x)
{
  uint8_t y, z;
  for ( ; x > 0 ; x--){
    for ( y = 0 ; y < 80 ; y++){
      for ( z = 0 ; z < 40 ; z++){
        asm volatile ("nop");
      }
    }
  }
}

void LED (uint8_t LEDNumber, uint8_t state)
{
	if(LEDNumber == 1)
	{
		if(state == 0)
		{
			PORTC &= ~(1<<0);
		}
		else
		{
			PORTC |= (1<<0);
		}
	}
	
	else if(LEDNumber == 2)
	{
		if(state == 0)
		{
			PORTC &= ~(1<<1);
		}
		else
		{
			PORTC |= (1<<1);
		}
	}

	else if(LEDNumber == 3)
	{
		if(state == 0)
		{
			PORTC &= ~(1<<2);
		}
		else
		{
			PORTC |= (1<<2);
		}
	}
	return;
}

/**Pin change 2 interrupt handler*/
ISR(PCINT2_vect)
{
	uint8_t PortStatus;
	TimeAndDate CurrentTime;
	
	PortStatus = PIND;
	//printf("PD: 0x%02X\n", PortStatus);
	
	if((PortStatus & (1<<5)) == 0)
	{
		DS3232M_DisableAlarm(1);
		DS3232M_GetTime(&CurrentTime);
		printf("Alarm at %02u/%02u/20%02u %02u:%02u:%02u\n", CurrentTime.month, CurrentTime.day, CurrentTime.year, CurrentTime.hour, CurrentTime.min, CurrentTime.sec);
	}

	//LED(1,1);
}

void Buzzer (uint16_t TimeToBuzzMS)
{
	TCCR1B |= 0x01;				//Buzzer on
	delay_ms(TimeToBuzzMS);
	TCCR1B &= 0xF8;				//Buzzer off
	return;
}

/** The timer1 OCR1A compare match interrupt. This will toggle the buzzer pin when the timer is enabled*/
ISR(TIMER1_COMPA_vect)
{
	PORTC ^= (1<<3);
}

/** @} */