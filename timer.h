#ifndef timer_h__
#define timer_h__

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef NULL
#define NULL 0
#endif

#define GLOB_TIMER_TCNT	45	// 180 - 10 ms
typedef void (*TPcallback)(void);

// okres = 10 ms
class timer{
public:
	TPcallback callbacks[4];
	uint16_t periods[4];
	uint16_t cnts_period[4];
	
	timer(){	
		TCCR1A = 0;
		TCCR1B = (1<<CS02)|(1<<CS00);	// fcpu / 1024 =
		TCNT1 = 0xFFFF-GLOB_TIMER_TCNT;
		TIMSK1 = (1<<TOV1);
		for( uint8_t i=0; i<4; i++ ){
			callbacks[i] = NULL;
			periods[i] = 0;
			cnts_period[i] = 0;
		};
	};
	
	void Enable(void);
	void Disable(void);
	void Reset(void);
	
	void RegisterCallback(TPcallback callback, uint16_t period);
	
};

extern timer Timer;

#endif // timer_h__
