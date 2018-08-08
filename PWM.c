#include "PWM.h"

void _PWM_16()
{
	// Set PB1, PB2 as output
	DDRB |= (1 << DDB1);
	DDRB |= (1 << DDB2);

	// Set inverting mode for both pins
	TCCR1A |= (1 << COM1A0) | (1 << COM1A1);
	TCCR1A |= (1 << COM1B0) | (1 << COM1B1);

	// Mode 7 Fast PWM, TOP = 1023
	TCCR1A |= (1 << WGM10) | (1 << WGM11);
	TCCR1B |= (1 << WGM12);

	// Initialize the duty cycle to 50%
	//OCR1A = (0x03ff/2);
	//OCR1B = (0x03ff/2);

	// No prescaling | Start the timer
	TCCR1B |= (1 << CS10) | (0 << CS11);

}
