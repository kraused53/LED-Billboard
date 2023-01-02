#include "ADC.h"


/*
 * Name:
 *        ADC_init
 * Inputs:
 *        None
 *            - void
 * Return:
 *        None
 *            - void
 * Description:
 *            Initializes the ATMega328ps ADC for 8-Bit mode.
 *
 */
void ADC_init(void) {
    // REFS0 : Select Avcc as reference
    // ADLAR : Left Adjust for 8-Bit mode
    // Set the rest to 0s to select ADC0 as input
    ADMUX = (1 << REFS0) | (1 << ADLAR);
    
    // ADEN  : Enable the ADC module
    // ADPS2 : Use with ADSP0 to select a clock divider of 32 (Makes 500 kHz ADC clk)
    // ADPS0 : Use with ADSP2 to select a clock divider of 32 (Makes 500 kHz ADC clk)
    ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);
}


/*
 * Name:
 *        ADC_select_channel
 * Inputs:
 *        uint8_t chan
 *            - The channel to select. Can be 0x00 -> 0x08. If out of bounds, channel is not changed
 * Return:
 *        None
 *            - void
 * Description:
 *            Selects the given ADC channel.
 *
 */
void ADC_select_channel(uint8_t chan) {
    // If given channel is out of bounds, do nothing
    if(chan > 0x08) {
        return;
    }
    
    // Clear old channel values without changing other settings
    ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));
    
    // Set new channel
    ADMUX |= chan;
}


/*
 * Name:
 *        ADC_read_byte
 * Inputs:
 *        None
 *            - void
 * Return:
 *        uint8_t
 *            - Returns the 8-Bit ADC value
 * Description:
 *            Stars n ADC read, waits for it to finish are returns the value of ADCH
 *
 */
uint8_t ADC_read_byte(void) {
    // Start a new conversion
    ADCSRA |= (1 << ADSC);
    
    // Wait for conversion to complete (ADSC will be pulled low)
    while(1 << ADSC);
    
    // Return 8-Bit conversion result
    return ADCH;
}