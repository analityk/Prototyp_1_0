#ifndef touch_h__
#define touch_h__

#include <avr/io.h>
#include <adc.h>

class touch{
private:
uint16_t rescale_x(uint16_t x);
uint16_t rescale_y(uint16_t y);

public:
	uint16_t x;
	uint16_t y;
	
	touch(){};
	void KeyPressed(void);
	bool Press(void);
	void ReadCoordinates(void);
	
};

extern touch Touch;

#endif // touch_h__
