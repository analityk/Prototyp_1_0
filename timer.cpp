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
	
	Timer.periods--;
	if( Timer.periods == 0 ){
		Timer.periods = Timer.cnts_period;
		Timer.callbacks();
	};
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
	callbacks = callback;
	periods = period;
	cnts_period = period;
};

timer Timer;