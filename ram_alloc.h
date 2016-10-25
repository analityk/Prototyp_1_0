#ifndef ram_alloc_h__
#define ram_alloc_h__

#include <avr/io.h>
#include <sram.h>

template< class T > 
class ram_alloc {
	
private:
	ram_grip grip;
	uint16_t _size_T;
	uint16_t _size;
	uint16_t _cnt;
	
public:
	ram_alloc( uint16_t  cnt ){
		_size_T = sizeof( T );
		_size = _size_T * cnt;
		grip = ram.get_mem( _size );
		ram.write_block( grip, 0, _size, (uint8_t*) 0 );
	};
	
	T operator[] (uint16_t i){
		T t;
		
	}
	
};

#endif // ram_alloc_h__
