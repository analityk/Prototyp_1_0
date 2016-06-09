#include <usart.h>

serial::serial(void){
	this->Enable();
	this->SetUbrr(29);	// 38.4kbps
};
	
void serial::SetUbrr(uint16_t ubrr){
	UBRR0H = (uint8_t)(ubrr >> 8);
	UBRR0L = (uint8_t)(ubrr);
};

void serial::Enable(void)
{
	PRR0 &=~(1<<PRUSART0);
	UCSR0B = (1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);
}

void serial::Disable(void)
{
	UCSR0B = (0<<TXEN0)|(0<<RXEN0);	
}

void serial::write(uint8_t* buff, uint8_t size){
	for(uint8_t i=0; i<size; i++){
		while(!(UCSR0A & (1<<UDRE0))){};
		UDR0 =buff[i];
	};
};

void serial::write(const char* s, uint8_t size){
	write( (uint8_t*)s, size);
};

void serial::write(char* t){
	while(*t){
		while(!(UCSR0A & (1<<UDRE0))){};
		UDR0 = *t++;
	};
};

void serial::write(const char* s){
	while(*s){
		while(!(UCSR0A & (1<<UDRE0))){};
		UDR0 = *s++;
	};
};

void serial::write(uint8_t byte)
{
	while(!(UCSR0A & (1<<UDRE0))){};
	UDR0 = byte;
}

uint8_t serial::readByte(){
	while(!(UCSR0A & (1<<RXC0))){};
	return(UDR0);
};

serial Serial;