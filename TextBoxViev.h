#ifndef TextBoxViev_h__
#define TextBoxViev_h__

#include <avr/io.h>
#include <string.h>
#include <lcd.h>
#include <text.h>

class textBoxViev{
public:
	uint8_t keycode;
	textBoxViev():keycode(3){};
		
	void Draw(void);
	void BigChars(void);
	void SmalChars(void);
	
};

extern textBoxViev TextBoxViev;

#endif // TextBoxViev_h__
