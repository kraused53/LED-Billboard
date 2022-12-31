#include "MAX7219_Billboard.h"


// Local Screen Buffer
uint8_t DISPLAY_MEM[8*HEIGHT][WIDTH];


/*** <--- MAX7219 Screen Editing ---> ***/

/*
 * Name:
 *      MAX7219_clear
 * Inputs:
 *      None
 *          - void
 * Return:
 *      None
 *          - void
 * Description:
 *          Clear the local screen buffer. Does NOT update screen
 *
 */
void MAX7219_clear(void) {
    uint8_t row, disp;

    // Only need two bytes of data at a time
    uint8_t data[2];
	
    // The pixel data will always be 0
    data[1] = 0x00;
	
    /*** <--- Line 0 of displays ---> ***/
    // For each row in the display line
    for(row = 1; row <= 8; row++) {
        // The first byte of data is the current row
        data[0] = row;
        // Activate slave select 0
        SS0_LOW;
        // For each display in the line
        for(disp = 0; disp < WIDTH; disp++) {
            SPI_write_data(data, 2);
        }
        // Deactivate slave select 0
        SS0_HIGH;
    }
	
    /*** <--- Line 1 of displays ---> ***/
    // For each row in the display line
    for(row = 1; row <= 8; row++) {
        // The first byte of data is the current row
        data[0] = row;
        // Activate slave select 1
        SS1_LOW;
        // For each display in the line
        for(disp = 0; disp < WIDTH; disp++) {
            SPI_write_data(data, 2);
        }
        // Deactivate slave select 1
        SS1_HIGH;
    }
}


/*
 * Name:
 *		MAX7219_update
 * Inputs:
 *		None
 *			- void
 * Return:
 *		None
 *			- void
 * Description:
 *			Uses SPI to send the local screen buffer to the billboard.
 *
 */
void MAX7219_update(void) {
	uint8_t row, disp;

	// For each row, we need two bytes for each display
	uint8_t data[2*WIDTH];
		
	/*** <--- Line 0 of displays ---> ***/
	// For each row in the display
	for(row = 1; row <= 8; row++) {
		// For each display in the line
		for(disp = 0; disp < WIDTH; disp++) {
			// The two bytes for each display are:
			// The current row
			data[2*disp] = row;
			// And the pixel values for that row
			data[(2*disp)+1] = DISPLAY_MEM[row-1][disp];
		}
		
		// Activate slave select 0
		SS0_LOW;
		// Send the row data over SPI
		SPI_write_data(data, 2*WIDTH);
		// Deactivate slave select 0
		SS0_HIGH;
	}

	/*** <--- Line 1 of displays ---> ***/
	// For each row in the display
	for(row = 1; row <= 8; row++) {
		// For each display in the line
		for(disp = 0; disp < WIDTH; disp++) {
			// The two bytes for each display are:
			// The current row
			data[2*disp] = row;
			// And the pixel values for that row
			data[(2*disp)+1] = DISPLAY_MEM[8+row-1][disp];
		}
		
		// Activate slave select 0
		SS1_LOW;
		// Send the row data over SPI
		SPI_write_data(data, 2*WIDTH);
		// Deactivate slave select 0
		SS1_HIGH;
	}
}


/*
 * Name:
 *		MAX7219_set_point
 * Inputs:
 *		uint8_t x
 *			- Select pixel column
 *		uint8_t y
 *			- Select pixel row
 *		uint8_t val
 *			- If 0 -> pixel is turned off, else pixel is turned on
 * Return:
 *		None
 *			- void
 * Description:
 *			This function changes the value of the given pixel in the local screen buffer. Does NOT update screen.
 *
 */
void MAX7219_set_point(uint8_t x, uint8_t y, uint8_t val) {
	uint8_t in_x;
	uint8_t rem_x;
	
	// Isolate which display the dot is in
	in_x = x/8;
	
	// If x or y are out of bounds, do nothing
	if((in_x >= WIDTH) || (y >= (8*HEIGHT))) {
		return;
	}
	
	// Isolate which pixel on the sub-display is to be set
	rem_x = x-(8*in_x);
	
	// If val is set
	if(val) {
		// Set the selected bit
		DISPLAY_MEM[y][in_x] |= (0x80 >> rem_x);
	}else {
		// Clear the selected bit
		DISPLAY_MEM[y][in_x] &= ~(0x80 >> rem_x);
	}
}


/*
 * Name:
 *		MAX7219_shift_right
 * Inputs:
 *		uint8_t wrap
 *			- If 0 -> pixels shifted onto screen are all off, else pixels are wrapped around when shifted off of screen. 
 * Return:
 *		None
 *			- void
 * Description:
 *			Shift all pixels to the right by one pixel. Does NOT update the screen.
 *
 */
void MAX7219_shift_right(uint8_t wrap) {
	uint8_t row, disp;
	uint8_t carry_this[8*HEIGHT] = {0};
	uint8_t carry_next[8*HEIGHT] = {0};
	
	// For each display
	for(disp = 0; disp < WIDTH; disp++) {
		// For each row
		for(row = 0; row < (8*HEIGHT); row++) {
			// Record if there is a bit to be shifted outside the display buffer
			if(DISPLAY_MEM[row][disp] & 0x01) {
				carry_next[row] = 0x80;
			}else {
				carry_next[row] = 0x00;
			}
			
			// Left shift the current byte by one
			DISPLAY_MEM[row][disp] >>= 1;
			// Carry over the bit from the previous byte
			DISPLAY_MEM[row][disp] |= carry_next[row];
			
			// Carry next becomes carry this for the next display
			carry_this[row] = carry_next[row];
		}
	}

	// If wrapping is enabled
	if(wrap) {
		// Fill the top row with the contents of the previously held row
		for(row = 0; row < (8*HEIGHT); row++) {
			DISPLAY_MEM[row][0] |= carry_this[row];
		}
	}
}


/*
 * Name:
 *		MAX7219_shift_left
 * Inputs:
 *		uint8_t wrap
 *			- If 0 -> pixels shifted onto screen are all off, else pixels are wrapped around when shifted off of screen.
 * Return:
 *		None
 *			- void
 * Description:
 *			Shift all pixels to the left by one pixel. Does NOT update the screen.
 *
 */
void MAX7219_shift_left(uint8_t wrap) {
	uint8_t row, disp;
	uint8_t carry_this[8*HEIGHT] = {0};
	uint8_t carry_next[8*HEIGHT] = {0};
	
	// For each display
	for(disp = WIDTH; disp > 0; disp--) {
		// For each row
		for(row = 0; row < (8*HEIGHT); row++) {
			// Record if there is a bit to be shifted outside the display buffer
			if(DISPLAY_MEM[row][disp-1] & 0x80) {
				carry_next[row] = 0x01;
			}else {
				carry_next[row] = 0x00;
			}
			
			// Left shift the current byte by one
			DISPLAY_MEM[row][disp-1] <<= 1;
			// Carry over the bit from the previous byte
			DISPLAY_MEM[row][disp-1] |= carry_next[row];
			
			// Carry next becomes carry this for the next display
			carry_this[row] = carry_next[row];
		}
	}

	// If wrapping is enabled
	if(wrap) {
		// Fill the top row with the contents of the previously held row
		for(row = 0; row < (8*HEIGHT); row++) {
			DISPLAY_MEM[row][WIDTH-1] |= carry_this[row];
		}
	}
}


/*
 * Name:
 *		MAX7219_shift_up
 * Inputs:
 *		uint8_t wrap
 *			- If 0 -> pixels shifted onto screen are all off, else pixels are wrapped around when shifted off of screen.
 * Return:
 *		None
 *			- void
 * Description:
 *			Shift all pixels up by one pixel. Does NOT update the screen.
 *
 */
void MAX7219_shift_up(uint8_t wrap) {
	uint8_t hold[WIDTH];
	uint8_t row, disp;
	
	// Save top row of local buffer for future wrapping;
	for(row = 0; row < WIDTH; row++) {
		hold[row] = DISPLAY_MEM[(8*HEIGHT-1)][row];
	}
	
	// For each row except the bottom one
	for(row = 0; row < ((8*HEIGHT)-1); row++) {
		// For each display
		for(disp = 0; disp < WIDTH; disp++) {
			// Shift up
			DISPLAY_MEM[row][disp] = DISPLAY_MEM[row+1][disp];
		}
	}
	
	// If wrapping is enabled
	if(wrap) {
		// Fill the bottom row with the contents of the previously held row
		for(row = 0; row < WIDTH; row++) {
			DISPLAY_MEM[(8*HEIGHT)-1][row] = hold[row];
		}
		}else {
		// Fill the bottom row with 0s
		for(row = 0; row < WIDTH; row++) {
			DISPLAY_MEM[(8*HEIGHT)-1][row] = 0x00;
		}
	}
}


/*
 * Name:
 *		MAX7219_shift_down
 * Inputs:
 *		uint8_t wrap
 *			- If 0 -> pixels shifted onto screen are all off, else pixels are wrapped around when shifted off of screen.
 * Return:
 *		None
 *			- void
 * Description:
 *			Shift all pixels down by one pixel. Does NOT update the screen.
 *
 */
void MAX7219_shift_down(uint8_t wrap) {
	uint8_t hold[WIDTH];
	uint8_t row, disp;
	
	// Save bottom row of local buffer for future wrapping;
	for(row = 0; row < WIDTH; row++) {
		hold[row] = DISPLAY_MEM[(8*HEIGHT-1)][row];
	}
	
	// For each row except the top one
	for(row = ((8*HEIGHT)-1); row > 0; row--) {
		// For each display
		for(disp = 0; disp < WIDTH; disp++) {
			// Shift down
			DISPLAY_MEM[row][disp] = DISPLAY_MEM[row-1][disp];
		}
	}
	
	// If wrapping is enabled
	if(wrap) {
		// Fill the top row with the contents of the previously held row
		for(row = 0; row < WIDTH; row++) {
			DISPLAY_MEM[0][row] = hold[row];
		}
	}else {
		// Fill the top row with 0s
		for(row = 0; row < WIDTH; row++) {
			DISPLAY_MEM[0][row] = 0x00;
		}
	}
}