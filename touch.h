#ifndef touch_h__
#define touch_h__

#include <avr/io.h>
#include <adc.h>
#include <delay.h>
#include <keys.h>
#include <timer.h>

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
	
	void delay_keypressed(void);

	uint8_t ReadKey(uint8_t from);
	void wait_release_key(void);
	
	//fuckup - kolizja linii spck z jend¹ z linii adc
};

extern touch Touch;

#endif // touch_h__
