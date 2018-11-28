#ifndef adc_h__
#define adc_h__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <timer.h>

#define ZERO_PORTF_ADC	{ PORTF &=~(1<<PINF0); PORTF &=~(1<<PINF1); PORTF &=~(1<<PINF2); PORTF &=~(1<<PINF3); PORTF &=~(1<<PINF4); PORTF &=~(1<<PINF5); }
#define ZERO_DDRF_ADC	{ DDRF &=~(1<<PINF0);  DDRF &=~(1<<PINF1);  DDRF &=~(1<<PINF2);  DDRF &=~(1<<PINF3);  DDRF &=~(1<<PINF4);  DDRF &=~(1<<PINF5); }

class adc{
public:
	adc(){
		ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
		ADMUX = 0;
		ZERO_DDRF_ADC;
		ZERO_PORTF_ADC;
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
