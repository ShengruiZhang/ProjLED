#define F_CPU 16000000UL

typedef unsigned int uint;
typedef unsigned char uchar;

#define PWM_16 1
#define Rotary_Encoder 1

#define PWM_Rotary 0
#define BLINK 1
#define FULL_ON 2

#define Button_Pressed 1
#define Button_Released 0

#define MAX_10 0x03ff

#define INCREMENT 20

#define DIR_NONE 0x0

// Clockwise step.
#define DIR_CW 0x10

// Counter-clockwise step.
#define DIR_CCW 0x20
/*
(PIND & (1 << PIND4)) = 1 when button is released
(PIND & (1 << PIND4)) = 0 when button is pressed
*/