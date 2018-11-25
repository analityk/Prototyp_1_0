#include <timer.h>

ISR(TIMER3_OVF_vect){

	if(Timer.sloclk == 0){
		TCNT3 = 0xFFFF - 1800;
	};
	
	Timer.DeciSeconds++;
	Timer.IdleTime++;
	
	if(Timer.IdleTime > 200){
		if(Timer.sloclk == 0){
			OCR1B += 1;
			if(OCR1B > Timer.ocr1b_min){
				OCR1B = Timer.ocr1b_min;
			};
			if(Timer.IdleTime > 600){
				PORTF &=~(1<<PINF7);
			};
		};
	}else{
		OCR1B = Timer.ocr1b_full;
		PORTF |= (1<<PINF7);
	};
	
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
	Timer.periods--;
	if( Timer.periods == 0 ){
		Timer.periods = Timer.cnts_period;
		Timer.callbacks();
	};
};


void timer::HaltTimer(void)
{
	TCNT1 = 0;
	TCNT3 = 0xFFFF - 500;
};	

void timer::ResumeTimer(void)
{
	TCNT1 = 0;
	TCNT3 = 0;
};

void timer::SlowClk(void)
{
	Timer.sloclk = 1;
}

void timer::FastClk(void)
{
	Timer.sloclk = 0;
};

void timer::Enable(void)
{
	TIMSK1 |=(1<<TOIE1);
};

void timer::Disable(void)
{
	TIMSK1 &=~(1<<TOIE1);
};

void timer::RegisterCallback(TPcallback callback, uint16_t period)
{
	callbacks = callback;
	periods = period;
	cnts_period = period;
	Timer.Enable();
};

void timer::UnRegisterCallback(void)
{
	callbacks = NULL;
	periods = 0xFFFF;
	cnts_period = 0xFFFF;
	Timer.Disable();
};

timer Timer;