#ifndef array_h__
#define array_h__

#include <avr/io.h>
#include <stdlib.h>

template < class T > class array{
public:
	T* data;
	uint8_t rozm;
	uint8_t poz;	
	
	array(uint8_t tsize){
		data = (T*)malloc(sizeof(T)*tsize);
		for(uint8_t i=0; i<tsize; i++){
			data[i] = T();
		}
		rozm = tsize;
		poz = 0;
	};
	
	~array(){
		poz = 0;
		rozm = 0;
		free(data);
	};
	
	uint8_t insert(T t){
		if( poz == rozm ){ return 1; };
		data[poz] = t;
		poz++;
		return 0;
	};
	
	T& read_last(void){
		return data[poz-1];
	};
	
	uint8_t remove_last(void){
		if( poz == 0 )return 1;
		poz--;
		data[poz] = 0;
		return 0;
	};
	
	uint8_t free_space(void){
		return (rozm - poz);
	}
	
	uint8_t size(void){
		return (rozm);
	};
	
	T& operator[] (uint8_t i)
	{
		if( (i >= 0) && (i <= rozm) ){
			return data[i];
		};
	return data[rozm-1];
	};
};


#endif // array_h__
