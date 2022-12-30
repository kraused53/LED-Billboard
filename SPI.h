#ifndef SPI_H_
	#define SPI_H_

	#include <avr/io.h>

	/*** <--- SPI Settings ---> ***/


	/*** <--- Function Prototypes ---> ***/
	void SPI_init(void);
	void SPI_write_data(uint8_t *data, uint8_t length);
	
#endif /* SPI_H_ */