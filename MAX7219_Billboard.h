#ifndef MAX7219_H_
	#define MAX7219_H_
	
	#include <avr/io.h>
	#include "SPI.h"
	
	/*** <--- MAX7219 Address Space ---> ***/
	#define NO_OP				0x00
	#define DIGIT_0				0x01
	#define DIGIT_1				0x02
	#define DIGIT_2				0x03
	#define DIGIT_3				0x04
	#define DIGIT_4				0x05
	#define DIGIT_5				0x06
	#define DIGIT_6				0x07
	#define DIGIT_7				0x08
	#define DECODE_MODE			0x09
	#define INTENSITY			0x0A
	#define SCAN_LIMIT			0x0B
	#define SHUT_DOWN			0x0C
	#define DISPLAY_TEST		0x0F
	
	/*** <--- MAX7219 Settings ---> ***/
	// Power Options
	#define DISPLAY_OFF			0x00
	#define DISPLAY_ON			0x01
	// Decoder Options:
	//		Set a bit 7..0 to enable BCD decoding for that particular digit.
	//		Only used for 7-Segment displays.
	//		Set to 0x00 when using a matrix display.
	#define DECODE_MODE_SET		0x00
	// Set Default Brightness:
	//		There are 15 brightness levels, 0x00->0x0F
	#define BRIGHTNESS			0x08
	// Scan Limit Setting:
	//		0x00: Display only digit 0
	//		0x01: Display only digits 0 & 1
	//		...
	//		0x06: Display only digits 0, 1, 2, 3, 4, 5 & 6
	//		0x07: Display all digits
	#define SCAN_LIMIT_SET		0x07
	// Display Test:
	//		0x00: Normal Mode
	//		0x01: Test Mode -> All LEDs on
	#define DISPLAY_TEST_SET	0x00
	
	/*** <--- Display Size ---> ***/
	// Number of displays per row
	#define WIDTH			4
	// Number of rows
	#define HEIGHT			2
	
	/*** <--- Function Prototypes ---> ***/
	// MAX7219 Settings
	void MAX7219_init(void);
	void MAX7219_set_decode_mode(uint8_t val);
	void MAX7219_set_brightness(uint8_t val);
	void MAX7219_set_scan_limit(uint8_t val);
	void MAX7219_set_shutdown(uint8_t val);
	void MAX7219_set_display_test(uint8_t val);
	
	// MAX7219 Screen Editing
	void MAX7219_clear(void);
	void MAX7219_update(void);
	void MAX7219_set_point(uint8_t x, uint8_t y, uint8_t val);
	void MAX7219_shift_right(uint8_t wrap);
	void MAX7219_shift_left(uint8_t wrap);
	void MAX7219_shift_up(uint8_t wrap);
	void MAX7219_shift_down(uint8_t wrap);
	
#endif