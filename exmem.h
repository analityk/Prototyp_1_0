#ifndef exmem_h__
#define exmem_h__

#include <avr/io.h>
#include <exmem_io.h>

// address space is from 0x0000 0000 to 0x0001 ffff
// max allocable semi-spaces is 0xffff (assumptions)


// allocators record occupies highest address space and rise down
// allocate memory occupies lowest address space and rise up

typedef uint16_t grip16_t;

typedef struct {
public:
	uint32_t start_address;		// msb is always 0x00
	uint16_t byte_cnt;
	uint32_t internal_ptr;		// msb is always 0x00
}AllocRecord;

class ExRam{
private:
	uint8_t allocate_record_size;
	uint16_t allocate_records;
	
	uint32_t firs_free_allocator_address(void){
		return (uint32_t)(0x0001FFFF-(allocate_record_size*(allocate_records+1)));
	};
	
	uint32_t latest_allocator_address(void){
		return (uint32_t)(0x0001FFFF - (allocate_record_size*allocate_records));
	};
	
	void write_byte_addr_long(uint32_t direct_address, uint8_t data){
		DDRD = 0xFF;
		CLR_CE_2;
		LO_ADDR_PORT = direct_address;
		HI_ADDR_PORT = direct_address>>8;
		uint8_t t = 0x01 & (direct_address>>16);
		if(t==1){
			PORTF |=(1<<PINF6);
		}else{
			PORTF &=~(1<<PINF6);
		};
		CLR_RW;
		CLR_CE_1;
		SET_CE_2;
		
		asm volatile("nop");
		DATA_PORT = data;
		RAM_STANDBY;
	};
	
	uint8_t read_byte_addr_long(uint32_t direct_address){
		SET_RW;
		CLR_OE;
		CLR_CE_1;
		SET_CE_2;
		
		PORTA = direct_address;
		PORTC = (direct_address >> 8);
		uint8_t b = 0x01 & (direct_address>>16);
		if(b==1){
			PORTF |=(1<<PINF6);
		}else{
			PORTF &=~(1<<PINF6);
		};
		
		DDRD = 0;
		
		asm volatile("nop");
		
		uint8_t t = PIND;
		
		RAM_STANDBY;
		return t;
	}
	
	
	AllocRecord read_latest_alloc_record(void){
		uint32_t direct_address = latest_allocator_address();
		AllocRecord a;
		union {
			AllocRecord ar;
			uint8_t ab[sizeof(AllocRecord)];
		}cast;
		for(uint8_t i=0; i<sizeof(AllocRecord); i++){
			cast.ab[i] = read_byte_addr_long(direct_address-i);
		};
		return cast.ar;
	};
	
	AllocRecord read_record(grip16_t record){
		uint32_t addr = (uint32_t)(0x0001FFFF - (allocate_record_size*record));
		
		AllocRecord a;
		union {
			AllocRecord ar;
			uint8_t ab[sizeof(AllocRecord)];
		}cast;
		
		for(uint8_t i=0; i<sizeof(AllocRecord); i++){
			cast.ab[i] = read_byte_addr_long(addr-i);
		};
		return cast.ar;
	};
	
	void store_record(uint32_t addr, AllocRecord a){
		union {
			AllocRecord ar;
			uint8_t ab[sizeof(AllocRecord)];
		}cast;
		cast.ar = a;
		for(uint8_t i=0; i<sizeof(AllocRecord); i++){
			write_byte_addr_long(addr-i, cast.ab[i]);
		};
	};
	
	void store_newest_alloc_addres(AllocRecord a){
		union {
			AllocRecord ar;
			uint8_t ab[sizeof(AllocRecord)];
		}cast;
		cast.ar = a;
		uint32_t direct_address = firs_free_allocator_address();
		for(uint8_t i=0; i<sizeof(AllocRecord); i++){
			write_byte_addr_long(direct_address-i, cast.ab[i]);
		};
		allocate_records++;
	};
	
public:

	ExRam(){
		allocate_record_size = sizeof(AllocRecord);
		allocate_records = 0;
	};
	
	grip16_t alloc(uint16_t byte_cnt){
		AllocRecord latest_alloc_record = read_latest_alloc_record();
		uint32_t lowest_free_address = latest_alloc_record.start_address + latest_alloc_record.byte_cnt;
		uint32_t highest_needed_address = lowest_free_address + byte_cnt;
		if(highest_needed_address > firs_free_allocator_address())return 0;
		AllocRecord new_record;
		new_record.start_address = lowest_free_address;
		new_record.byte_cnt = byte_cnt;
		new_record.internal_ptr = 0;
		store_newest_alloc_addres(new_record);
		return allocate_records;
	};
	
	uint8_t read_byte(grip16_t g){
		AllocRecord ar = read_record(g);
		uint32_t direct_address = ar.start_address+ar.internal_ptr;
		uint8_t data = read_byte_addr_long(direct_address);
		ar.internal_ptr++;
		if(ar.internal_ptr > ar.byte_cnt){
			ar.internal_ptr = 0;
		};
		uint32_t addr = (uint32_t)(0x0001FFFF - (allocate_record_size*g));
		store_record(addr, ar);
		return data;
	};
	
	void write_byte(grip16_t g, uint8_t data){
		AllocRecord ar = read_record(g);
		uint32_t direct_address = ar.start_address+ar.internal_ptr;
		write_byte_addr_long(direct_address, data);
		ar.internal_ptr++;
		if(ar.internal_ptr > ar.byte_cnt){
			ar.internal_ptr = 0;
		};
		uint32_t addr = (uint32_t)(0x0001FFFF - (allocate_record_size*g));
		store_record(addr, ar);
	};
	
	void write_block(grip16_t g, uint8_t* data, uint16_t cnt){
		//AllocRecord ar = read_record(g);
		
	}
};

extern ExRam exram;

#endif // exmem_h__
