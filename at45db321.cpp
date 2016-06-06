#include <at45db321.h>

void AT45::WriteText(uint16_t offset, const char* data)
{
	
	uint8_t t = 0;
	
	for(uint16_t i=offset; i<528; i++){
		if(data[t] == '\n')break;
		page_buffer[i] = data[t];
		t++;
	};
}

void AT45::readPage(uint16_t page)
{
	Status();
	
	CS_L;
	
	uint16_t p = page << 2;
	
	uint8_t lo = p;
	uint8_t hi = p >> 8;
	
	spi.transfer(CONTINOUS_ARRAY_READ);
	spi.transfer(hi);
	spi.transfer(lo);	
	spi.transfer(0x00);
	
	for(uint16_t i=0; i<528; i++){
		page_buffer[i] = spi.transfer(0x00);
	};
	
	
	CS_H;
}

void AT45::readPage(uint16_t page, uint16_t bytes_cnt, uint16_t offset)
{
	
	Status();
	
	CS_L;
		
	uint16_t p = (page << 2);
	
	uint8_t hi_cnt = (offset >> 8);
	uint8_t lo_cnt = offset;
	uint8_t lo = (p | hi_cnt);
	uint8_t hi = (p >> 8);
	
	spi.transfer(CONTINOUS_ARRAY_READ);
	spi.transfer(hi);
	spi.transfer(lo);
	spi.transfer(lo_cnt);
	
	for(uint16_t i=0; i<bytes_cnt; i++){
		page_buffer[i] = spi.transfer(0x00);
	};

	CS_H;
}

void AT45::programPage(uint16_t page_cnt)
{
	
	Status();
	
	CS_L;
	
	uint16_t p = page_cnt << 2;
	
	uint8_t lo = p;
	uint8_t hi = p >> 8;
	
	spi.transfer(0x82);
	spi.transfer(hi);
	spi.transfer(lo);
	spi.transfer(0x00);
	
	for(uint16_t i=0; i<528; i++){
		spi.transfer(page_buffer[i]);
	};
	
	
	CS_H;
	
}

uint8_t AT45::Status(void)
{
	CS_L;
	
	uint8_t ret = spi.transfer(0xd7);
	
	do{
		ret = spi.transfer(0xd7);
	}while(!( ret & 0x80 ) );
	
	CS_H;
	return ret;
}

bool AT45::Busy(void)
{
	return (status_byte & 0x80);
}

AT45 at45;