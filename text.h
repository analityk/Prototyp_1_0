#ifndef text_h__
#define text_h__

#include <avr/io.h>
#include <lcd.h>

#define CHAR_CNT		22
#define LINE_CNT		8
#define MAX_CHAR_CNT	168

class text{
	private:
	
	void Zero(void);
	
	char inLine[8][22];
	uint8_t coursor_x, coursor_y;
	
	void Draw(const char c);
	
	public:
	text():coursor_x(0),coursor_y(0){
		Zero();
	};
	
	void Refresh(void);
	void ClrScr();
	void GoTo(uint8_t x, uint8_t y);
	void WriteChar(const char c);
	void WriteString(const char* c);
	void WriteLine(const char* c);
	
	void WriteBuffer(uint8_t* data);
	
	void NextLine(void);
};

extern text Text;

#endif // text_h__