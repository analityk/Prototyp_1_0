#ifndef text_h__
#define text_h__

#include <avr/io.h>
#include <lcd.h>
#include <array.h>
#include <timer.h>

class text{
	private:
	
	void Zero(void);
	
	uint8_t coursor_x, coursor_y;
	uint8_t spaces;
	uint8_t char_width;
	
	void Draw(const char c);
	
	public:
	text(): coursor_x(0), coursor_y(0), spaces(1), char_width(5){
		ClrScr();
	};
	
	uint8_t Get_Coursor_X(void);
	uint8_t Get_Coursor_Y(void);
	
	void SetSpaces(uint8_t s);
	
	void ClrScr();
	
	void GoTo(uint8_t x, uint8_t y);
	void GoToAbs(uint8_t x, uint8_t y);
	
	void Write(const char c);
	void Write(const char* c);
	void Write(uint8_t* data);
	void Write(array<uint8_t>& data);
	
	static void CoursorBlinkEnable(void);
	
};

extern text Text;

#endif // text_h__

/*
	NIE MA BUFORA EKRANU i huk z nim. piszemy na nim ró¿ne rzeczy ale zmiennym tekstem jest tylko jedna
	linia - textbox. Tam mamy bufor uint8_t i tyle. go odœwie¿amy za ka¿dym razem. 
*/