#ifndef ram_alloc_h__
#define ram_alloc_h__

#include <avr/io.h>
#include <sram.h>

template< class T > 
class ram_alloc {
	
private:

	union {
		T ut;
		uint8_t ub[sizeof(T)];
	}UCastUnion;

	T t;
	
	ram_grip grip;
	uint16_t _size_T;
	uint16_t _size;
	uint16_t _cnt;
	
	uint16_t _poz;
	
	
public:
	ram_alloc( uint16_t  cnt ){
		_size_T = sizeof( T );
		_size = _size_T * cnt;
		grip = ram.get_mem( _size );
		_poz = 0;
	};
	
	void push(T t){
		if( _poz == _cnt ){
			return;
		};
		UCastUnion.ut = t;
		ram.write_block(grip, _poz, _size_T, UCastUnion.ub );
		_poz++;
	};
	
	void pop(void){
		if( _poz == 0 ){
			return;
		};
		_poz--;
		ram.write_block(grip, _poz, _size_T, 0);
	};
	
	bool release(void){
		return false;
	};
	
	T at(uint16_t i) {
		if( i >= _poz ){
			for( uint16_t a=0; a<_size_T; a++ ){
				UCastUnion.ub[a] = 0;
			};
			return UCastUnion.ut;
		};
		ram.read_block(grip, i*_size_T, sizeof( T ), UCastUnion.ub);
		
		t = UCastUnion.ut;
		
		return t;
	}
	
};

#endif // ram_alloc_h__
