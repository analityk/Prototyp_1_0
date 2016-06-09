#ifndef usart_h__
#define usart_h__

#include <avr/io.h>
#include <avr/interrupt.h>

class serial{
public:
	serial(void);
	
	void SetUbrr(uint16_t ubrr);
	void Enable(void);
	void Disable(void);
	void write(uint8_t* buff, uint8_t size);
	void write(const char* s, uint8_t size);
	void write(char* t);
	void write(const char* s);
	void write(uint8_t byte);
	uint8_t readByte(void);
};

extern serial Serial;

#endif // usart_h__
