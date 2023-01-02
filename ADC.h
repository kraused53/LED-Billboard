#ifndef ADC_H_
    #define ADC_H_
    
    #include <avr/io.h>
    
    /*** <--- ADC Settings ---> ***/
    
    
    /*** <--- Function Prototypes ---> ***/
    void ADC_init(void);
    void ADC_select_channel(uint8_t chan);
    uint8_t ADC_read_byte(void);

#endif /* ADC_H_ */