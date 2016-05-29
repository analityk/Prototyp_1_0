#ifndef small_graph_h__
#define small_graph_h__

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <point.h>
#include <lcd.h>


class small_graph{
public:
	small_graph(){};
	~small_graph(){};
	
	void Draw(point left_up, uint8_t witdh, uint8_t* data);
};

#endif // small_graph_h__
