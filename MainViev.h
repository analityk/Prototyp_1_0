#ifndef MainViev_h__
#define MainViev_h__

#include <avr/io.h>
#include <lcd.h>
#include <text.h>

#define LINE_CNT_MAX	7
#define COLUMN_CNT_MAX	8

class mainViev{
private:
	uint8_t column;
	uint8_t line;
	
public:
	uint8_t keycode;
	
	mainViev():column(0), line(0),keycode(1){};
	
	void Draw(void);
	
	void DecLine(void);
	void IncLine(void);
	void DecCol(void);
	void IncCol(void);
	
	uint8_t ActLine(void);
	uint8_t ActColumn(void);
	
};

extern mainViev MainViev;

#endif // MainViev_h__
