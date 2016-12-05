#ifndef sram_h__
#define sram_h__

#include <avr/io.h>
#include <array.h>


#define RW	PB7
#define SET_RW	{ PORTB |= (1<<RW); }
#define CLR_RW	{ PORTB &=~(1<<RW); }
	
#define CE_1	PB6
#define SET_CE_1	{ PORTB |= (1<<CE_1); }
#define CLR_CE_1	{ PORTB &=~(1<<CE_1); }

#define CE_2	PB5	
#define SET_CE_2	{ PORTB |= (1<<CE_2); }
#define CLR_CE_2	{ PORTB &=~(1<<CE_2); }

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

#define BANKS_CNT	2

#define BANK_A	0
#define BANK_B	1

#define GRIP_FACTOR	0x4000
#define GRIP_FAIL	0

typedef uint16_t ram_grip;

typedef struct {
	uint16_t start;	// adres pocz�tku danych dla tej konkretnej alokacji
	uint16_t cnt;	// ilo�� danych przydzielonych tej konkretnej alokacji
	uint8_t type_size;
}_tsRS;

const uint8_t size_of_trst = 5;
const uint16_t mem_size = 0xFFFF;

// zwraca numer struktury _tsRS, kt�ra zwiera dane o zaalokowanej pami�ci
// przez ten numer mo�na odwo�a� si� do przydzielonej pami�ci np:
// const uint16_t str = get_mem(20);
// uint8_t znak = (uint8_t)read_mem(str, 0);


// funkcja read_mem zwraca bajt z przydzielonej pami�ci
// uint8_t read_mem(uint16_t gm_cnt, uint16_t offset)


// max_tsrs_cnt x 5 - ilo�� pami�ci zarezerwowana na potrzeby zapami�tania gdzie i co jest
// ta pami�� leci z g�ry co 5 bajt�w

// all_alloc_mem_a - to jest liczba rosn�ca od zera i oznacza ile bajt�w pami�ci zosta�o przydzielonych
// userowi, zatem

// nie mo�na przydzieli� tak du�o pami�ci, �eby przydzielony obszar m�g� nadpisa� pami�� na potrzeby _tsrs
// avaiable_mem zwraca licz� bajt�w, kt�ra mo�e jeszcze zosta� zaalokowana w jednym bloku i oznacza
// 0xFFFF - (5 x max_tsrs_cnt + 1) - all_alloc_mem_a;


class RAM {	
private:
	uint8_t _bank;
	_tsRS tsrs;
	uint16_t max_tsrs_cnt[2];
	uint16_t all_alloc_mem[2];
	
	uint8_t ReadByte(uint16_t adr);
	
	void setBank(uint8_t bank);
	
	void get_mem_struct(ram_grip grip, _tsRS* p);
	
public:
	RAM();
	
	bool offset_error;
	bool addr_error;

	void clr_err(void);
	
	//uint16_t avaiable_mem(void);
	ram_grip get_mem(uint16_t cnt_bytes);
	
	void release(ram_grip grip);
	
	void free_mem(ram_grip grip);
	
	void WriteByte(uint16_t adr, uint8_t data);
	
	uint8_t readByte(ram_grip adr, uint16_t offset);
	
	void read_block(ram_grip adr, uint16_t offset, uint16_t cnt_to_copy, uint8_t* where);
	
	void write_block(ram_grip adr, uint16_t offset, uint16_t cnt_to_copy, uint8_t* from);
	
	double read_double(ram_grip grip, uint16_t offset);
	
};

extern RAM ram;

#endif // sram_h__
