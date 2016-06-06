#include <adc.h>

uint16_t adc::current_X(void)
{
	DDRF |=( (1<<PINF4)|(1<<PINF3) );
	PORTF &=~(1<<PINF4);
	PORTF |=(1<<PINF3);
	
	ADMUX =	1;
	ADCSRA |= (1<<ADSC);
	while( ADCSRA & (1 << ADSC )){};
	uint8_t lsb = ADCL;
	uint8_t msb = ADCH;
	uint16_t t = lsb + (msb<<8);
	
	DDRF &=~( (1<<PINF4)|(1<<PINF3) );
	PORTF&=~( (1<<PINF4)|(1<<PINF3) );
	return t;
}

uint16_t adc::current_Y(void)
{
	DDRF |=( (1<<PINF5)|(1<<PINF0) );
	PORTF &=~(1<<PINF5);
	PORTF |=(1<<PINF0);
	
	ADMUX = 3;
	ADCSRA |= (1<<ADSC);
	while( ADCSRA & (1 << ADSC )){};
	uint8_t lsb = ADCL;
	uint8_t msb = ADCH;
	uint16_t t = lsb + (msb<<8);
	
	DDRF &=~( (1<<PINF5)|(1<<PINF0) );
	PORTF&=~( (1<<PINF5)|(1<<PINF0) );
	
	return t;
}

uint16_t adc::voltage_X(void)
{
	DDRF |= ( (1<<PINF1)|(1<<PINF3) );
	
	PORTF &=~(1<<PINF1);
	PORTF |=(1<<PINF3);
	
	ADMUX = 0;
	ADCSRA |= (1<<ADSC);
	while( ADCSRA & (1 << ADSC )){};
	uint8_t lsb = ADCL;
	uint8_t msb = ADCH;
	uint16_t t = lsb + (msb<<8);
	
	DDRF  &=~ ( (1<<PINF1)|(1<<PINF3) );
	PORTF &=~ ( (1<<PINF1)|(1<<PINF3) );
	return t;
}

uint16_t adc::voltage_Y(void)
{
	DDRF |= ( (1<<PINF2) | (1<<PINF0) );
	
	PORTF &=~(1<<PINF2);
	PORTF |=(1<<PINF0);
	
	ADMUX = 1;
	ADCSRA |= (1<<ADSC);
	while( ADCSRA & (1 << ADSC )){};
	uint8_t lsb = ADCL;
	uint8_t msb = ADCH;
	uint16_t t = lsb + (msb<<8);
	
	DDRF  &=~ ( (1<<PINF2)|(1<<PINF0) );
	PORTF &=~ ( (1<<PINF2)|(1<<PINF0) );
	
	return t;
}

uint16_t adc::adc_read_10b(uint8_t pin)
{
	ADMUX = pin;
	ADCSRA |= (1<<ADSC);
	while( ADCSRA & (1 << ADSC )){};
	uint8_t lsb = ADCL;
	uint8_t msb = ADCH;
	uint16_t t = lsb + (msb<<8);
	return t;
}

adc Adc;