#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "MAX7219-Billboard.h"


int main(void) {
	
	DDRB |= (1 << 5);	
	
    // Main loop
    while (1) {
		PORTB |= (1 << 5);
		_delay_ms(500);
		PORTB &= ~(1 << 5);
		_delay_ms(500);
    }
}

