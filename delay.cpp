#include <delay.h>

uint16_t volatile time_now;

uint8_t nested = 0;

void delay(uint32_t volatile t){
	while(t--){};
};

void delay_ms(uint8_t ms)
{
	TCCR0B = (1<<CS02)|(0<<CS01)|(1<<CS00);
	for(uint8_t i=0; i<ms; i++){
		TCNT0 = 237;
		while(!(TIFR0 & (1<<TOV0))){};
		TIFR0 |= (1<<TOV0);
	};
};

void delay_us(uint8_t us)
{
	TCCR0B = 1;
	for(uint8_t i=0; i<us; i++){
		TCNT0 = 239;
		while(!(TIFR0 & (1<<TOV0))){};
		TIFR0 |= (1<<TOV0);
	};
};

//ISR(TIMER0_OVF_vect){
//TCNT0 = 5;
//time_now++;
//};

uint16_t Millis(void)
{
	return time_now;
};


uint8_t startMillis(void)
{
	if(nested > 0) return 1;
	nested = 1;
	TCCR0B = (1<<CS01)|(1<<CS00);	// 64
	time_now = 0;
	TIMSK0 |= (1<<TOIE0);
	
	return 0;
};



uint8_t stopMillis(void)
{
	if(nested == 1){
		nested = 0;
		}else{
		return 2;
	};
	
	TIMSK0 &=~(1<<TOIE0);
	time_now = 0;
	return 0;
};