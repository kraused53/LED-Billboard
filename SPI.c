#include "SPI.h"


/*
 * Name:
 *        SPI_init
 * Inputs:
 *        None
 *            - void
 * Return:
 *        None
 *            - void
 * Description:
 *            Initializes the ATMega328p's internal SPI module to send commands to 
 *        the MAX7219s.
 *
 */
void SPI_init(void) {
    // Setup all SPI control lines as outputs
    SPI_DDR |= SS1 | SS0 | MOSI | SCK;
    
    // Enable the SPI module
    // SPE:  Enable the SPI MOdule
    // MSTR: Tell the module to run as a master device
    // SPR0: Set the clock-divider to 16 (Creates a 1 MHz SPI Clock)
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}


/*
 * Name:
 *        SPI_write_data
 * Inputs:
 *        uint8_t *data
 *            - An array of bytes to be sent over SPI
 *        uint8_t length
 *            - The number of bytes to send over SPI
 * Return:
 *        None
 *            - void
 * Description:
 *            Sends the given array of bytes through the SPI module. Bytes are 
 *        sent in ascending order [index 0 -> index (lenght-1)], one at a time.
 * 
 */
void SPI_write_data(uint8_t *data, uint8_t length) {
    // Step through the array one byte at a time
    for(uint8_t ind = 0; ind < length; ind++) {
        // Load the current byte into the SPI data register.
        SPDR = data[ind];
        
        // SPI transmission begins as soon as data is loaded.
        // Wait for the SPI module to signal that it has finished.
        while(!(SPSR & (1 << SPIF)));
    }
}