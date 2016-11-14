#ifndef array_h__
#define array_h__

#include <avr/io.h>
#include <stdlib.h>

template < class T > class array{
private:
	bool _full;
	bool _destroy;
public:
	T* data;
	uint8_t rozm;
	uint8_t poz;
	
	array(uint8_t tsize){
		data = (T*)malloc(sizeof(T)*tsize);
		for(uint8_t i=0; i<tsize; i++){
			data[i] = T();
		};
		rozm = tsize;
		poz = 0;
		_full = false;
		_destroy = false;
	};
	
	~array(){
		poz = 0;
		rozm = 0;
		free(data);
	};
	
	void destroy(void){
		if( _destroy )return;
		poz = 0;
		rozm = 0;
		free(data);
		_destroy = true;
	};
	
	uint8_t insert(T t){
		if( _destroy )return 2;
		if( poz == rozm ){ return 1; };
		data[poz] = t;
		poz++;
		return 0;
	};
	
	void erase(void){
		if( _destroy )return;
		for(uint8_t i=0; i<rozm; i++){
			data[i] = 0;
		};
		poz = 0;
		_full = false;
	};
	
	T& read_last(void){
		if( !(_destroy) ){
			return data[poz-1];
		};
	};
	
	bool full(void){
		if( _destroy )return;
		if( free_space() == 0 ){
			return true;
		};
	return false;
	};
	
	uint8_t remove_last(void){
		if( _destroy )return 2;
		if( poz == 0 )return 1;
		poz--;
		data[poz] = 0;
		return 0;
	};
	
	uint8_t free_space(void){
		if( _destroy )return;
		return (rozm - poz);
	}
	
	uint8_t size(void){
		if( _destroy )return 0;
		return (rozm);
	};
	
	uint8_t cnts(void){
		return poz - 1;
	};
	
	T& operator[] (uint8_t i)
	{
		if( _destroy )return (data[0]);
		if( (i >= 0) && (i <= rozm) ){
			return data[i];
		};
	return data[rozm-1];
	};
};


#endif // array_h__
