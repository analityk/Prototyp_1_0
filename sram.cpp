#include <sram.h>

 RAM::RAM()
{
	tsrs.cnt = 0;
	tsrs.start = 0;
	tsrs.type_size = 0;
	max_tsrs_cnt = 0;
	offset_error = false;
	addr_error = false;
	DDRB = 0xF0;
	RAM_STANDBY;
	LO_ADDR_DDR = 0xff;
	HI_ADDR_DDR = 0xff;
	GET_BANK_A;
};

uint16_t RAM::get_mem(uint16_t cnt_bytes)
{
	if( (mem_size - (size_of_trst * (max_tsrs_cnt+1)) - sum_of_all_allocate_memory) < cnt_bytes ){
		return 0; // nie ma tyle pamięci
	};
	// jeśli można to zrealizować to trzeba pobrac ostatni zaalokowany obiekt, żeby móc określić adres początkowy
	// nowej ramki 
	// ale jeśli mamy pierwszą ramkę, to max_trst_cnt = 0 i trzeba skoczyć pod adres 0xFFFF - 5 i tam zapisać co natępuje
	
	if( max_tsrs_cnt == 0 ){
		tsrs.cnt = cnt_bytes;
		tsrs.start = 0x0000;
		tsrs.type_size = 1;
		sum_of_all_allocate_memory = cnt_bytes;
		
		uint16_t addr = mem_size - size_of_trst;
		
		WriteByte(addr + 0, (uint8_t)(tsrs.cnt) );
		WriteByte(addr + 1, (uint8_t)(tsrs.cnt >> 8) );
		WriteByte(addr + 2, (uint8_t)(tsrs.start) );
		WriteByte(addr + 3, (uint8_t)(tsrs.start >> 8) );
		WriteByte(addr + 4, (uint8_t)(tsrs.type_size) );
		
		max_tsrs_cnt = 1;
		return max_tsrs_cnt;
	};
	
	
	// jeśli jest coś już zaalokowane a trzeba zaalokować coś nowego to trzeba pobrać dane na temat poprzedniej alokacji
	// ustalić czy mamy dość pamięci, ustalić gdzie zaczyna się nowy blok, gdzie się kończy
	
	if( max_tsrs_cnt > 0 ){
		
		// sprawa adresu
		// 0xFFFF - (max_tsrs_cnt * size_of_trst)
		uint16_t addr = mem_size - ( size_of_trst *  max_tsrs_cnt );
		_tsRS tmp;
/*	*/		
		uint8_t t[5];
		t[0] = ReadByte( addr + 0 );
		t[1] = ReadByte( addr + 1 );
		t[2] = ReadByte( addr + 2 );
		t[3] = ReadByte( addr + 3 );
		t[4] = ReadByte( addr + 4 );
		
		tmp.cnt		= (t[1] << 8) | t[0];
		tmp.start	= (t[3] << 8) | t[2];
		tmp.type_size = t[4];
		
		// teraz wiadomo gdzie kończy się uprzednio zaalokowana pamięć, jest to adres tmp.start + tmp.cnt
		// do tej wartości dodajemy cnt_bytes by sprawdzić dokąd sięga
		// z kolei potrzebujemy zapisać w ram dodatkowy obiekt _tsrs więc 
		
		if( (mem_size - (size_of_trst * (max_tsrs_cnt + 1)) - (tmp.start + tmp.cnt) ) < cnt_bytes ){
			return 0;
		};
		
		// skoro wiadomo, że pamięć się zmieści i wiemy, jak wysoko zaalokować nową pamięć, możemy ustalić cnt i start 
		// dla nowej ramki
		
		tmp.start = tmp.start + tmp.cnt;
		tmp.cnt = cnt_bytes;
		tmp.type_size = 1;
		
		sum_of_all_allocate_memory += cnt_bytes;
		
		// ustalamy gdzie zapisać dane o alokacji
		// +1 bo zapisujemy 5 bajtów niżej niż ostatni zapisany obiekt
		addr = mem_size - (size_of_trst *  (max_tsrs_cnt + 1) );
		
		WriteByte( addr + 0, (uint8_t)(tmp.cnt) );
		WriteByte( addr + 1, (uint8_t)(tmp.cnt >> 8) );
		WriteByte( addr + 2, (uint8_t)(tmp.start) );
		WriteByte( addr + 3, (uint8_t)(tmp.start >> 8) );
		WriteByte( addr + 4, (uint8_t)(tmp.type_size) );
		
		max_tsrs_cnt++;
		return max_tsrs_cnt;
	};
	
	return 0;
}

uint8_t RAM::read_mem(uint16_t adr, uint16_t offset)
{
	// najpierw trzeba pobrać info o alokacji bloku adr
	// czyli ustalić adres skąd je wziąć
	
	if( adr > max_tsrs_cnt ){
		addr_error = true;
		return 0;
	};
	
	uint16_t addr = mem_size - ( size_of_trst * adr );
/*	*/
	uint8_t t[5];
	t[0] = ReadByte( addr + 0 );
	t[1] = ReadByte( addr + 1 );
	t[2] = ReadByte( addr + 2 );
	t[3] = ReadByte( addr + 3 );
	t[4] = ReadByte( addr + 4 );
	
	_tsRS tmp;
	
	tmp.cnt		= (t[1] << 8) | t[0];
	tmp.start	= (t[3] << 8) | t[2];
	tmp.type_size = t[4];
	
	// można sprawdzić, czy nie mieszamy bloków czyli czy offset nie przekracza cnt
	
	if( offset > tmp.cnt ){
		offset_error = true;
		return 0;
	};
	
	// teraz trzeba ustalić adres w pamięci ram, z którego należy odczytać dane
	addr = tmp.start + offset;
	
	uint8_t r = ReadByte(addr);
	return r;
	
};

void RAM::write_block(uint16_t adr, uint16_t offset, uint16_t cnt_to_copy, uint8_t* from)
{
	// nie przydzielono bloku o tak dużym numerze
	if( adr > max_tsrs_cnt ){
		addr_error = true;
		return;
	};
	
	uint16_t addr = mem_size - ( size_of_trst * adr );
/*	*/
	uint8_t t[5];
	t[0] = ReadByte( addr + 0 );
	t[1] = ReadByte( addr + 1 );
	t[2] = ReadByte( addr + 2 );
	t[3] = ReadByte( addr + 3 );
	t[4] = ReadByte( addr + 4 );
		
	_tsRS tmp;
		
	tmp.cnt		= (t[1] << 8) | t[0];
	tmp.start	= (t[3] << 8) | t[2];
	tmp.type_size = t[4];
	// czy zapisywany obiek zmieści się do wybranego bloku pamięci? 
	
	if( tmp.cnt < (offset + cnt_to_copy) ){
		offset_error = true;
		return;
	};
	
	// skoro wszystko jest ok to można zapisywać dane. 
	for(uint16_t i = 0; i<cnt_to_copy; i++){
		WriteByte( tmp.start + offset + i, from[i]);
	};
	
};

void RAM::WriteByte(uint16_t adr, uint8_t data)
{
	DDRD = 0xFF;
	CLR_CE_2;
	LO_ADDR_PORT = adr;
	HI_ADDR_PORT = adr>>8;
	CLR_RW;
	CLR_CE_1;
	SET_CE_2;
	
	asm volatile("nop");
	DATA_PORT = data;
	RAM_STANDBY;
};

uint8_t RAM::ReadByte(uint16_t adr)
{
	SET_RW;
	CLR_OE;
	CLR_CE_1;
	SET_CE_2;
	
	PORTA = adr;
	PORTC = (adr >> 8);
	DDRD = 0;
	
	asm volatile("nop");
	
	uint8_t t = PIND;
	
	RAM_STANDBY;
	return t;
}

RAM ram;