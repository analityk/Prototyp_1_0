#include <text.h>

void text::SetSpaces(uint8_t s)
{
	spaces = s;
};

void text::ClrScr()
{
	Lcd_KS0108.ClrScr();
	coursor_x = 0;
	coursor_y = 0;
};

void text::GoTo(uint8_t x, uint8_t y)
{
	if( y > 8 )return;
	if(( (char_width+spaces)*x) > 123)return;
	Lcd_KS0108.GoToXYText(x,y);
	coursor_x = (char_width+spaces)*x;
	coursor_y = y;
};

void text::GoToAbs(uint8_t x, uint8_t y)
{
	if( y > 8 )return;
	if( x > 123 )return;
	Lcd_KS0108.GoToXY(x,y);
	coursor_x = x;
	coursor_y = y;
};

void text::Draw(const char c)
{
	uint8_t z = c-32;
	for( uint8_t i=0; i<5; i++ ){
		Lcd_KS0108.WriteData( pgm_read_byte( font5x7 + (z*5) + i ) );
		coursor_x++;
	};
	for( uint8_t i=0; i<spaces; i++ ){
		Lcd_KS0108.WriteData(0x00);
		coursor_x++;
	};
};

void text::Write(const char c)
{
	if( (coursor_x + char_width + spaces ) > 127 )return;
	Draw(c);
};

void text::Write(const char* c)
{
	for( uint8_t i=0; i < 30; i++ ){
		if( c[i] == 0 )return;
		if( c[i] == '\n' )return;
		Draw(c[i]);
	};
};


void text::Write(uint8_t* data)
{
	for( uint8_t i = 0; i < 30; i++ ){
		if(*data == 0)break;
		Write( (char)*data++);
	};
};

void text::CoursorBlinkEnable(void)
{
	Text.GoToAbs(Text.coursor_x, Text.coursor_y);
	Lcd_KS0108.coursor ^=(0x7F);
	Lcd_KS0108.WriteData(Lcd_KS0108.coursor);
};

text Text;