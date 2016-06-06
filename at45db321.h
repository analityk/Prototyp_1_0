#ifndef at45db321_h__
#define at45db321_h__

#include <avr/io.h>
#include <spi.h>

#define MAIN_MEMORY_PAGE_READ	0xd2
#define CONTINOUS_ARRAY_READ	0x03	// low freq
#define BUFFER_1_READ			0xd1	// low freq
#define BUFFER_2_READ			0xd3	// low freq
#define BUFFER_1_WRITE			0x84	
#define BUFFER_2_WRITE			0x87
#define BUFFER_1_SAVE			0x83
#define BUFFER_2_SAVE			0x86
#define PAGE_ERASE				0x81
#define BLOCK_ERASE				0x50
#define SECTOR_ERASE			0x7c
#define STATUS_REG_READ			0xd7
#define ID_DEVICE_READ			0x9f

uint8_t at45_status(void);

class AT45{
private:
	uint8_t status_byte;
	
public:
	
	uint8_t page_buffer[528];

	AT45():
	status_byte(0)
	{};
	
	void WriteText(uint16_t offset, const char* data);
	
	// copy one specified page from flash into uC flash buffer
	void readPage(uint16_t page);
	
	void readPage(uint16_t page, uint16_t bytes_cnt, uint16_t offset);
	
	// copy page_buffer into flash on specified page number
	void programPage(uint16_t page_cnt);
		
	uint8_t Status(void);
	
	bool Busy(void);
	
};

extern AT45 at45; 
#endif // at45db321_h__
