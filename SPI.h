#ifndef SPI_H_
	#define SPI_H_

	#include <avr/io.h>

	/*** <--- SPI Settings ---> ***/
	#define SPI_PORT	PORTB
	#define SPI_DDR		DDRB
	#define SCK			(1 << 5)
	#define MOSI		(1 << 3)
	#define SS0			(1 << 2)
	#define SS1			(1 << 1)
	
	// SS Controls
	#define SS0_LOW		(SPI_PORT &= ~SS0)
	#define SS0_HIGH	(SPI_PORT |= SS0)
	
	#define SS1_LOW		(SPI_PORT &= ~SS1)
	#define SS1_HIGH	(SPI_PORT |= SS1)


	/*** <--- Function Prototypes ---> ***/
	void SPI_init(void);
	void SPI_write_data(uint8_t *data, uint8_t length);
	
#endif /* SPI_H_ */