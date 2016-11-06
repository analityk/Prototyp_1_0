#ifndef timer_h__
#define timer_h__

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef NULL
#define NULL 0
#endif



#define GLOB_TIMER_TCNT	180		// 180 - 10 ms
#define GLOB_TIMER_RT	1800	// 1800 - 0,1s

typedef void (*TPcallback)(void);

class timer{
public:
	TPcallback callbacks[4];
	uint16_t periods[4];
	uint16_t cnts_period[4];
	
	uint16_t tcnt1;
	
	uint8_t volatile Hours;
	uint8_t volatile Minutes;
	uint8_t volatile Seconds;
	uint8_t volatile DeciSeconds;
	
	timer():
	Hours(0),Minutes(0),Seconds(0),DeciSeconds(0)
	{	
		// timer 1 and callbacks config
		TCCR1A = 0;
		TCCR1B = (1<<CS02)|(1<<CS00);	// fcpu / 1024
		TCNT1 = 0xFFFF-GLOB_TIMER_TCNT;
		TIMSK1 = (0<<TOIE1);
		for( uint8_t i=0; i<4; i++ ){
			callbacks[i] = NULL;
			periods[i] = 0;
			cnts_period[i] = 0;
		};
		//timer 2 for global time
		TCCR3A = 0;
		TCCR3B = (1<<CS02)|(1<<CS00);
		TCNT3 = 0xFFFF-GLOB_TIMER_RT;
		TIMSK3 = (1<<TOIE3);
	};
	
	void Enable(void);
	void Disable(void);
	void Reset(void);
	
	void RegisterCallback(TPcallback callback, uint16_t period);
	
};

extern timer Timer;
//
//extern uint8_t volatile Hours;
//extern uint8_t volatile Minutes;
//extern uint8_t volatile Seconds;
//extern uint8_t volatile DeciSeconds;

#endif // timer_h__
