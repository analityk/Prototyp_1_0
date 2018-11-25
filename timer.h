#ifndef timer_h__
#define timer_h__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>

#ifndef NULL
#define NULL 0
#endif

typedef void (*TPcallback)(void);

class timer{
public:

	uint8_t sloclk;
	TPcallback callbacks;
	uint16_t periods;
	uint16_t cnts_period;
	
	uint16_t tcnt1;
	
	uint16_t volatile ocr1b_full;
	uint16_t volatile ocr1b_min;	
	uint16_t volatile icr1;
	
	uint8_t volatile Hours;
	uint8_t volatile Minutes;
	uint8_t volatile Seconds;
	uint8_t volatile DeciSeconds;
	uint16_t volatile IdleTime;
	
	timer(): sloclk(0), ocr1b_full(70), ocr1b_min(89), icr1(90),
	Hours(0), Minutes(0), Seconds(0), DeciSeconds(0), IdleTime(0)
	{	
		// timer 1 and callbacks config / 5ms
		TCCR1A = (1<<COM1B1)|(1<<COM1B0)|(1<<WGM11);
		TCCR1B = (1<<WGM13)|(1<<CS12)|(1<<CS10);
		TCNT1 = 0;
		ICR1 = icr1;
		OCR1B = ocr1b_full;
		
		callbacks = NULL;
		periods = 0;
		cnts_period = 0;
		
		//100ms
		TCCR3A = 0;
		TCCR3B = (1<<CS32)|(1<<CS30);
		TCNT3 = 0xFFFF - 1800;
		TIMSK3 = (1<<TOIE3);
	};
	
	void HaltTimer(void);
	void ResumeTimer(void);
	void SlowClk(void);
	void FastClk(void);
	
	void Enable(void);
	void Disable(void);
	
	void RegisterCallback(TPcallback callback, uint16_t period);
	void UnRegisterCallback(void);
	
};

extern timer Timer;

#endif // timer_h__
