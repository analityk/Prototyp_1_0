#ifndef exmem_io_h__
#define exmem_io_h__

#include <avr/io.h>

#define RW	PB7
#define SET_RW	{ PORTB |= (1<<RW); }
#define CLR_RW	{ PORTB &=~(1<<RW); }

#define CE_1	PG1
#define SET_CE_1	{ PORTG |= (1<<CE_1); }
#define CLR_CE_1	{ PORTG &=~(1<<CE_1); }

#define CE_2	PG2
#define SET_CE_2	{ PORTG |= (1<<CE_2); }
#define CLR_CE_2	{ PORTG &=~(1<<CE_2); }

#define OE	PB4
#define SET_OE	{ PORTB |= (1<<OE); }
#define CLR_OE	{ PORTB &=~(1<<OE); }

#define DATA_PORT	PORTD
#define DATA_DDR	DDRD
#define DATA_READ	PIND

#define DATA_PORT_DISABLE	{ DATA_DDR = 0; DATA_PORT = 0; }
#define DATA_PORT_INPUT		{ DATA_DDR = 0; DATA_PORT = 0; }
#define DATA_PORT_OUTPUT	{ DATA_DDR = 0xFF; }

#define LO_ADDR_PORT	PORTA
#define HI_ADDR_PORT	PORTC

#define LO_ADDR_DDR		DDRA
#define HI_ADDR_DDR		DDRC

#define GET_BANK_A	{	DDRF |=(1<<PF6); PORTF |= (1<<PF6); }
#define GET_BANK_B	{	DDRF |=(1<<PF6); PORTF &=~(1<<PF6);	}

#define RAM_DISABLE	{ CLR_CE_1; SET_CE_2; SET_OE; SET_RW; }
#define RAM_READ	{ CLR_CE_1; SET_CE_2; CLR_OE; SET_RW; DATA_PORT_INPUT; }
#define RAM_WRITE	{ CLR_CE_1; SET_CE_2; SET_OE; CLR_RW; DATA_PORT_OUTPUT;	}
#define RAM_STANDBY	{ SET_CE_1;	SET_CE_2; SET_OE; SET_RW; DATA_PORT_DISABLE; }

#endif // exmem_io_h__
