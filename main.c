#include "Define.h"
#include "IncFile1.h"

volatile uint DutyCycle = 0;
volatile uint Speed = 50;
volatile uint mode = 0;
static uint counter = 0;
static uint counter_release = 0;

static uint PinState();
static uint Mode_Blink(uint times);

uint power_on = 0;

// Handle the button on encoder
ISR(PCINT2_vect)
{
	if (mode == PWM_Rotary)
	{
		unsigned char result = rotary_process();

		if (result == DIR_NONE) {
			
		}
		else if (result == DIR_CW) {
			// Only increment if under max
			if (DutyCycle < MAX_10)
			/* 0x03ff  1023*/
			{
				DutyCycle += INCREMENT;
			}

			// Set to max if it is already max out
			if (DutyCycle >= MAX_10)
			{
				DutyCycle = MAX_10 - 5;
				// Bit smaller than the MAX
			}
		}
		else if (result == DIR_CCW) {
			// Only decrease if hasn't bottom out
			if (DutyCycle > 1)
			{
				DutyCycle -= INCREMENT;
			}

			// Set to 0 if it is already bottom out
			if (DutyCycle <= 0)
			{
				DutyCycle = 1;
			}
		}
		OCR1A = DutyCycle;
		OCR1B = DutyCycle;
	}
	else if (mode == BLINK)
	{
		unsigned char result = rotary_process();

		if (result == DIR_NONE) {
			
		}
		else if (result == DIR_CW) {
			Speed += 2;
		}
		else if (result == DIR_CCW) {
			if (Speed > 0) Speed -= 2;
		}
	}
}

int main(void)
{
	// Set P-MOSFET to high to turn off LED
	DDRB |= (1 << DDB1);
	DDRB |= (1 << DDB2);
	PORTB |= (1 << PORTB1) | (1 << PORTB2);
	_delay_ms(2000);

	// Press the button to turn on
	//
	init_button();
	// Set pin 5 as output, status LED
	DDRB |= (1 << DDB5);

	// Counting loop for debouncing
	while (1)
	{
		if (!(PIND & (1 << PIND4)))
		{
			++power_on;
			//Mode_Blink(1);
			_delay_ms(1);

		}

		if (power_on > 100)
		{
			break;
		}
	}
	PORTB |= (1 << PORTB5);
	_delay_ms(1000);
	PORTB &= ~(1 << PORTB5);
	//_delay_ms(1000);

	_PWM_16();
	rotary_init();

	// Enable global interrupts
	sei();
	
	// Initialize the duty cycle
	// to lowest, to avoid frying 
	// the red LEDs
	OCR1A = 1;
	OCR1B = 1;

	_delay_ms(1000);

	// Main loop
	while(1)
	{
		if (PinState())
		{
			_delay_ms(250);
			//++mode;
		}
		//else if ((!PinState()))
		//{
			//PORTB &= ~(1 << PORTB5);
			//_delay_ms(30);
		//}

		if (mode > 2) mode = 0;

		// Choose corresponding function
		switch(mode)
		{
			case FULL_ON:

				// Blink LED
				Mode_Blink(FULL_ON+1);

				OCR1A = (0.9 * 0x03ff);
				OCR1B = (0.9 * 0x03ff);

				while(!PinState())
				{
					
				}
				++mode;
				_delay_ms(30);
				break;

			case PWM_Rotary:
				
				// Blink LED
				Mode_Blink(PWM_Rotary+1);
				DutyCycle = 1;
				OCR1A = DutyCycle;
				OCR1B = DutyCycle;

				while (!PinState())
				{
					//_delay_ms(5);
				}
				++mode;
				_delay_ms(30);
				break;

			case BLINK:
			
				// Blink LED
				Mode_Blink(BLINK+1);

				_delay_ms(80);
				while(!PinState())
				{
					DutyCycle += Speed;
					if (DutyCycle > 0x03ff) DutyCycle = 0;
					OCR1A = DutyCycle;
					OCR1B = DutyCycle;
				}
				++mode;
				_delay_ms(30);
				break;
		}
	}

	return 0;
}

static uint PinState()
{
	// Check the current pin state

	// Check for press
	if (!(PIND & (1 << PIND4)))
	{
		++counter;

		if (counter > 250)
		{
			_delay_ms(90);
			// Reset counter
			//
			// Mysterious point here, the thing stops
			// working as soon as reset the counter to zero
			// every time reaches the debounce threshold
			//counter = 0;
			return Button_Pressed;
		}
	}
	else // Check for release
	{
		++counter_release;

		if (counter_release > 250)
		{
			_delay_ms(90);
			// Reset counter
			//counter_release = 0;
			return Button_Released;
		}
	}
	return 0;
}

static uint Mode_Blink(uint times)
{
	for (uint k = 0; k < times; ++k)
	{
		PORTB |= (1 << PORTB5);
		_delay_ms(50);
		PORTB &= ~(1 << PORTB5);
		_delay_ms(70);
	}
	return 0;
}
