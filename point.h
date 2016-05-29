#ifndef point_h__
#define point_h__

#include <avr/io.h>

class point{
	public:
	uint8_t x, y;
	point();
	point(uint8_t px, uint8_t py);
	point Set(uint8_t px, uint8_t py);
	~point(){};
};

extern point Point;

#endif // point_h__
