#include <spi.h>

SPI::SPI()
{
	PROG_DDR |= ( (1<<SPCK)|(1<<MOSI)|(1<<CS) );
	PROG_DDR &=~(0<<MISO);
	PROG_PORT |= ( (1<<SPCK)|(1<<MOSI)|(1<<CS) );
	PROG_PORT &=~(0<<MISO);
	
	SPCR = (0<<SPIE)|(1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<CPHA);
	SPSR |=(1<<SPI2X);
	
	DISABLE_SPCK;
	
}

uint8_t SPI::transfer(uint8_t znak)
{
	uint8_t volatile ret;
	SPDR = znak;
	while(!(SPSR &(1<<SPIF))){};
	ret = SPDR;
	return ret;
};

SPI spi;