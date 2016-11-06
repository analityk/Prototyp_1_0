#include <timer.h>

ISR(TIMER3_OVF_vect){
TCNT3 = 0xFFFF - GLOB_TIMER_RT;
	Timer.DeciSeconds++;
	
	if( Timer.DeciSeconds == 10 ){
		Timer.DeciSeconds = 0;
		Timer.Seconds++;
	};
	
	if( Timer.Seconds == 60 ){
		Timer.Seconds = 0;
		Timer.Minutes++;
	};
	
	if( Timer.Minutes == 60 ){
		Timer.Minutes = 0;
		Timer.Hours++;
	};
	
	if( Timer.Hours == 24 ){
		Timer.Hours = 0;
	};
	
};

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
	
	Timer.tcnt1 = 0xFFFF - TCNT1;
};


void timer::Enable(void)
{
	TIMSK1 |=(1<<TOIE1);
};

void timer::Disable(void)
{
	TIMSK1 &=~(1<<TOIE1);
};

void timer::Reset(void)
{
	TCNT1 = 0xFFFF-GLOB_TIMER_TCNT;
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