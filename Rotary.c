#include "Rotary.h"

#define R_START 0x0

// Use the full-step state table (emits a code at 00 only)
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

const unsigned char ttable[7][4] = {
	// R_START
	{R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
	// R_CW_FINAL
	{R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
	// R_CW_BEGIN
	{R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
	// R_CW_NEXT
	{R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
	// R_CCW_BEGIN
	{R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
	// R_CCW_FINAL
	{R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
	// R_CCW_NEXT
	{R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};

void rotary_init(uint pin1, uint pin2) {
	//Enable internal pull-up
	PORTD |= (1 << PORTD2) | (1 << PORTD3);

	// Initialize state.
	state = R_START;

	// Enable interrupts for encoder
	PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);

}

unsigned char rotary_process()
{
	unsigned char i, j;

	if((PIND&(1<<PIND3)))
	{
		i = 1;
	}
	else{
		i = 0;
	}

	if((PIND&(1<<PIND2)))
	{
		j = 1;
	}
	else{
		j = 0;
	}

	// Get state of input pins
	unsigned char pinstate = (j << 1) | (i);

	// Find state
	state = ttable[state & 0xf][pinstate];

	return (state & 0x30);
}

// Handle encoder's button
void init_button()
{
	// PD4 as input
	DDRD &= (0 << DDD4);

	// Enable Pull-up on PD4
	PORTD |= (1 << PORTD4);
}