#include <timer.h>

ISR(TIMER3_OVF_vect){
	if(Timer.slowclk == 1){
		Timer.FastClk();
	};
	
	TCNT3 = 0xFFFF - 1800;
	
	Timer.DeciSeconds++;
	Timer.IdleTime++;
	
	if(Timer.IdleTime > 2000){
		OCR1B = Timer.ocr1b_min;
		
		if(Timer.IdleTime > 3000 ){
			PORTF &=~(1<<PINF7);
			Timer.sleep = 1;
		};
		
		if(Timer.IdleTime > 500 ){
			Timer.IdleTime = 0;
		};
			
	}else{
		OCR1B = Timer.ocr1b_full;
		PORTF |= (1<<PINF7);
		Timer.FastClk();
		Timer.slowclk = 0;
		Timer.sleep = 0;
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
	
	if(Timer.slowclk == 1 || Timer.sleep == 1){
		Timer.SlowClk();
	};
	
};

ISR(TIMER1_OVF_vect){
	if(Timer.slowclk == 1){
		Timer.FastClk();
	};
		
	Timer.periods--;
	if( Timer.periods == 0 ){
		Timer.periods = Timer.cnts_period;
		Timer.callbacks();
	};
	
	if(Timer.slowclk == 1){
		Timer.SlowClk();
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
	cli();
	
	TCCR1B &=~(1<<CS12);
	TCCR1B &=~(1<<CS10);
	TCCR1B |=(1<<CS11);
	
	TCCR3B &=~(1<<CS32);
	TCCR3B &=~(1<<CS30);
	TCCR3B |=(1<<CS31);
	
	Timer.slowclk = 1;
	CLKPR = 0x80;
	CLKPR = 0x07;
	
	sei();
};

void timer::FastClk(void)
{
	cli();
	
	TCCR1B |=(1<<CS12);
	TCCR1B |=(1<<CS10);
	TCCR1B &=~(1<<CS11);
	
	TCCR3B |=(1<<CS32);
	TCCR3B |=(1<<CS30);
	TCCR3B &=~(1<<CS31);
	
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	sei();
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