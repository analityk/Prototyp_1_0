﻿#include <sram.h>

void RAM::setBank(uint8_t bank)
{
	switch(bank){
		
		case 0:{
			GET_BANK_A;
			break;
		};
		
		case 1:{
			GET_BANK_B;
		break;
		};
		
		default:break;
	};
	
	_bank = bank;
};

 RAM::RAM()
{
	_bank = BANK_A;
	tsrs.cnt = 0;
	tsrs.start = 0;
	tsrs.type_size = 0;
	max_tsrs_cnt[BANK_A] = 0;
	max_tsrs_cnt[BANK_B] = 0;
	all_alloc_mem[BANK_A] = 0;
	all_alloc_mem[BANK_B] = 0;
	offset_error = false;
	addr_error = false;
	DDRB = 0xF0;
	RAM_STANDBY;
	LO_ADDR_DDR = 0xff;
	HI_ADDR_DDR = 0xff;
	GET_BANK_A;
};

void RAM::clr_err(void)
{
	offset_error = false;
	addr_error = false;
};

ram_grip RAM::get_mem(uint16_t cnt_bytes)
{

	{
		uint8_t t = 0;
		for(uint8_t i=0; i<BANKS_CNT; i++){
			setBank(i);
			
			if( (mem_size - (size_of_trst * ( (max_tsrs_cnt[_bank] - (GRIP_FACTOR * i) + 1 ))) - all_alloc_mem[_bank]) < cnt_bytes ){
				t++;
				continue;
			}else{
				break;
			};
			
		};
		
		if( t == BANKS_CNT ){
			return 0;
		};
		
	};
			
	if( max_tsrs_cnt[_bank] == 0 ){
		tsrs.cnt = cnt_bytes;
		tsrs.start = 0x0000;
		tsrs.type_size = 1;
		all_alloc_mem[_bank] = cnt_bytes;
		
		uint16_t addr = mem_size - size_of_trst;
		
		WriteByte(addr + 0, (uint8_t)(tsrs.cnt) );
		WriteByte(addr + 1, (uint8_t)(tsrs.cnt >> 8) );
		WriteByte(addr + 2, (uint8_t)(tsrs.start) );
		WriteByte(addr + 3, (uint8_t)(tsrs.start >> 8) );
		WriteByte(addr + 4, (uint8_t)(tsrs.type_size) );
		
		max_tsrs_cnt[_bank] = (0x4000 * _bank) + 1;
		
	};
	
	if( max_tsrs_cnt[_bank] > 0 ){
		
		uint16_t addr = mem_size - ( size_of_trst *  ( max_tsrs_cnt[_bank] - ( GRIP_FACTOR * _bank ) ) );

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

		if( (mem_size - (size_of_trst * (max_tsrs_cnt[_bank]- (GRIP_FACTOR * _bank) + 1)) - (tmp.start + tmp.cnt) ) < cnt_bytes ){
			return 0;
		};
					
		tmp.start = tmp.start + tmp.cnt;
		tmp.cnt = cnt_bytes;
		tmp.type_size = 1;
		
		all_alloc_mem[_bank] += cnt_bytes;
		
		addr = mem_size - (size_of_trst *  ( (max_tsrs_cnt[_bank] - (GRIP_FACTOR * _bank) )+ 1) );
		
		WriteByte( addr + 0, (uint8_t)(tmp.cnt) );
		WriteByte( addr + 1, (uint8_t)(tmp.cnt >> 8) );
		WriteByte( addr + 2, (uint8_t)(tmp.start) );
		WriteByte( addr + 3, (uint8_t)(tmp.start >> 8) );
		WriteByte( addr + 4, (uint8_t)(tmp.type_size) );
		
		max_tsrs_cnt[_bank]++;
		return max_tsrs_cnt[_bank];
	};
	
	return 0;
}

uint8_t RAM::readByte(ram_grip adr, uint16_t offset)
{
	setBank( adr / GRIP_FACTOR );
	
	uint16_t addr = mem_size - ( size_of_trst * ( adr - ( GRIP_FACTOR * _bank ) ) );
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
	
	if( offset > tmp.cnt ){
		offset_error = true;
		return 0;
	};
	
	addr = tmp.start + offset;
	
	uint8_t r = ReadByte(addr);
	return r;
	
};

void RAM::read_block(ram_grip adr, uint16_t offset, uint16_t cnt_to_copy, uint8_t* where)
{
	setBank( adr / GRIP_FACTOR );
	
	uint16_t addr = mem_size - ( size_of_trst * ( adr - ( GRIP_FACTOR * _bank ) ) );
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
	
	if( offset > tmp.cnt ){
		offset_error = true;
		return;
	};
	
	addr = tmp.start + offset;
	
	CLR_OE;
	CLR_CE_1;
	SET_CE_2;
	SET_RW;
	
	for(uint16_t i = 0; i<cnt_to_copy; i++){
			
		PORTA = (addr + i);
		PORTC = ( (addr + i) >> 8);
		DDRD = 0;
		asm volatile("nop");
		where[i] = PIND;
	};
	RAM_STANDBY;
}

void RAM::write_block(ram_grip adr, uint16_t offset, uint16_t cnt_to_copy, uint8_t* from)
{
	setBank( adr / GRIP_FACTOR );
	
	uint16_t addr = mem_size - ( size_of_trst * ( adr - ( GRIP_FACTOR * _bank ) ) );
	
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
	
	if( tmp.cnt < (offset + cnt_to_copy) ){
		offset_error = true;
		return;
	};
	
	DDRD = 0xFF;
	for(uint16_t i = 0; i<cnt_to_copy; i++){
		
		uint16_t adres = tmp.start + offset + i;
			
			CLR_CE_2;
			LO_ADDR_PORT = adres;
			HI_ADDR_PORT = adres>>8;
			CLR_RW;
			CLR_CE_1;
			SET_CE_2;
			
			asm volatile("nop");
			DATA_PORT = from[i];			
	};
	RAM_STANDBY;
	
};

void RAM::WriteByte(ram_grip adr, uint8_t data)
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

uint8_t RAM::ReadByte(ram_grip adr)
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
};

RAM ram;