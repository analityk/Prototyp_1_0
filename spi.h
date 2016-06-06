#ifndef spi_h__
#define spi_h__

#include <avr/io.h>

#define	PROG_PORT	PORTB
#define PROG_PIN	PINB
#define PROG_DDR	DDRB

#define NOP		asm volatile ("nop")
#define MOSI	PB2
#define MISO	PB3
#define SPCK	PB1
#define CS		PB0

#define SET_MOSI		PROG_PORT |= (1<<MOSI)
#define CLR_MOSI		PROG_PORT &=~(1<<MOSI)
#define SET_SPCK		PROG_PORT |= (1<<SPCK)
#define CLR_SPCK		PROG_PORT &=~(1<<SPCK)

#define DISABLE_SPCK	{ PROG_DDR &=~(1<<SPCK); CLR_SPCK; }
#define ENABLE_SPCK		{ PROG_DDR |=(1<<SPCK); CLR_SPCK; }

#define CS_H	{ DISABLE_SPCK;		PROG_PORT |= (1 << CS); }
#define CS_L	{ ENABLE_SPCK;		PROG_PORT &=~(1 << CS); }

#define GET_MISO	(PROG_PIN & (1<<MISO))

class SPI{
public:
	SPI();
	
	uint8_t transfer(uint8_t znak);
	
};

extern SPI spi;

#endif // spi_h__
