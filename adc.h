#ifndef adc_h__
#define adc_h__

#include <avr/io.h>
#include <delay.h>

class adc{
public:
	adc(){
		//PRR = (0<<PRADC);
		ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
		ADMUX = 0;
	};
		
	
	uint16_t current_X(void);
	uint16_t current_Y(void);
	
	uint16_t voltage_X(void);
	uint16_t voltage_Y(void);
	
	uint16_t adc_read_10b(uint8_t pin);
	uint8_t adc_read_8b(uint8_t pin);
	
};

extern adc Adc;

#endif // adc_h__
