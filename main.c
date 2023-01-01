#define F_CPU 16000000UL
#define SMALL_DELAY 1
#define LARGE_DELAY 80

#include <avr/io.h>
#include <util/delay.h>
#include "MAX7219_Billboard.h"

void test_vertical(void);
void test_horizontal(void);
void test_scroll_right(void);
void test_scroll_left(void);
void test_scroll_up(void);
void test_scroll_down(void);
void test_fill_column(uint8_t col, uint8_t lvl);
void load_arrows(void);

int main(void) {
    
    MAX7219_init();
    
    test_fill_column(0, 0);
    test_fill_column(1, 1);
    test_fill_column(2, 2);
    test_fill_column(3, 3);
    test_fill_column(4, 3);
    test_fill_column(5, 0);
    test_fill_column(6, 0);
    test_fill_column(7, 5);
    test_fill_column(8, 16);
    test_fill_column(9, 15);
    test_fill_column(10, 14);
    test_fill_column(11, 13);
    test_fill_column(12, 12);
    test_fill_column(13, 11);
    test_fill_column(14, 10);
    test_fill_column(15, 9);
    test_fill_column(16, 8);
    MAX7219_update();
    // Main loop
    while (1) {
//        test_vertical();
//        test_horizontal();
        
//        load_arrows();
//        test_scroll_right();
//        test_scroll_left();
//        test_scroll_up();
//        test_scroll_down();
//        _delay_ms(LARGE_DELAY);
    }
}

void test_vertical(void) {
    // Set Pixels
    // For each Column
    for(uint8_t i = 0; i < (8*WIDTH); i++) {
        // For each row
        for(uint8_t j = 0; j < (8*HEIGHT); j++) {
            MAX7219_set_point(i, j, 1);
            MAX7219_update();
            _delay_ms(SMALL_DELAY);
        }
    }
    
    // Clear Pixels
    // For each Column
    for(uint8_t i = 0; i < (8*WIDTH); i++) {
        // For each row
        for(uint8_t j = 0; j < (8*HEIGHT); j++) {
            MAX7219_set_point(i, j, 0);
            MAX7219_update();
            _delay_ms(SMALL_DELAY);
        }
    }
}

void test_horizontal(void) {
    // Set Pixels
    // For each row
    for(uint8_t i = 0; i < (8*HEIGHT); i++) {
        // For each column
        for(uint8_t j = 0; j < (8*WIDTH); j++) {
            MAX7219_set_point(j, i, 1);
            MAX7219_update();
            _delay_ms(SMALL_DELAY);
        }
    }
    
    // Clear Pixels
    // For each Column
    for(uint8_t i = 0; i < (8*HEIGHT); i++) {
        // For each row
        for(uint8_t j = 0; j < (8*WIDTH); j++) {
            MAX7219_set_point(j, i, 0);
            MAX7219_update();
            _delay_ms(SMALL_DELAY);
        }
    }
}

void test_scroll_right(void) {
    uint8_t i;
    
    for(i = 0; i < 32; i++) {
        MAX7219_shift_right(1);
        MAX7219_update();
        _delay_ms(LARGE_DELAY);
    }

}

void test_scroll_left(void) {
    uint8_t i;
    
    for(i = 0; i < 32; i++) {
        MAX7219_shift_left(1);
        MAX7219_update();
        _delay_ms(LARGE_DELAY);
    }

}

void test_scroll_up(void) {
    uint8_t i;
    
    for(i = 0; i < 32; i++) {
        MAX7219_shift_up(1);
        MAX7219_update();
        _delay_ms(LARGE_DELAY);
    }

}

void test_scroll_down(void) {
    uint8_t i;
    
    for(i = 0; i < 32; i++) {
        MAX7219_shift_down(1);
        MAX7219_update();
        _delay_ms(LARGE_DELAY);
    }

}

void test_fill_column(uint8_t col, uint8_t lvl) {
    MAX7219_fill_column(col, lvl);
}

void load_arrows(void) {
    uint8_t i, j;
    
    for(j = 0; j < 5; j++) {
        for(i = 0; i < 8; i++) {
            MAX7219_set_point((8*j)+i, i, 1);
            MAX7219_set_point((8*j)+7-i, i+8, 1);
            
            MAX7219_set_point((8*j)+i-1, i, 1);
            MAX7219_set_point((8*j)+7-i-1, i+8, 1);
            
            MAX7219_set_point((8*j)+i-4, i, 1);
            MAX7219_set_point((8*j)+7-i-4, i+8, 1);
            
            MAX7219_set_point((8*j)+i-5, i, 1);
            MAX7219_set_point((8*j)+7-i-5, i+8, 1);
        }
    }
}