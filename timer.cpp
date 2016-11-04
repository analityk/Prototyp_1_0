#include <timer.h>

ISR(TIMER1_OVF_vect){
	TCNT1 = 0xFFFF-GLOB_TIMER_TCNT;
	for( uint8_t i=0; i<4; i++ ){
		if( !(Timer.callbacks[i] == NULL )){
			Timer.periods[i]--;
			if( Timer.periods[i] == 0 ){
				Timer.periods[i] = Timer.cnts_period[i];
				Timer.callbacks[i]();
			};
		};
	};
};


void timer::Enable(void)
{
	sei();
};

void timer::Disable(void)
{
	cli();
};

void timer::Reset(void)
{
	TCNT0 = 75;
};

void timer::RegisterCallback(TPcallback callback, uint16_t period)
{
	for( uint8_t i=0; i<4; i++ ){
		if( callbacks[i] == NULL ){
			callbacks[i] = callback;
			periods[i] = period;
			cnts_period[i] = period;
			break;
		};
	};
};

timer Timer;