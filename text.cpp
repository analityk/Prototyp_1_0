#include <text.h>

void text::Zero(void)
{
	for(uint8_t i=0;  i < 8; i++){
		Lcd_KS0108.GoToXY(0,i);
		for(uint8_t j=0; j < 21; j++){
			inLine[i][j] = ' ';
		};
	};
};

void text::Refresh(void)
{
	for(uint8_t i=0; i < 8; i++){
		Lcd_KS0108.GoToXY(0,i);
		for(uint8_t j=0; j < 21; j++){
			Draw(inLine[i][j]);
		};
	};
	Lcd_KS0108.GoToXY(0,0);
};

void text::ClrScr()
{
	for(uint8_t i=0; i < 8; i++){
		Lcd_KS0108.GoToXY(126,i);
		Lcd_KS0108.WriteData(0x00);
		Lcd_KS0108.WriteData(0x00);
	};
	Zero();
	Refresh();
	coursor_x = 0;
	coursor_y = 0;
}

void text::GoTo(uint8_t x, uint8_t y)
{
	if( x > 21 )return;
	if( y > 8 )return;
	Lcd_KS0108.GoToXYText(x,y);
	coursor_x = x;
	coursor_y = y;
}

void text::Draw(const char c)
{
	uint8_t z = c-32;
	for(uint8_t i=0; i<5; i++){
		Lcd_KS0108.WriteData( pgm_read_byte( font5x7 + (z*5) + i ));
	};
	Lcd_KS0108.WriteData(0x00);
};

void text::WriteChar(const char c)
{
	inLine[coursor_y][coursor_x] = c;
};

void text::WriteString(const char* c)
{
	for(uint8_t i=0; i < 168; i++){
		if( c[i] == 0 )break;
		inLine[coursor_y][coursor_x] = c[i];
		coursor_x++;
		if(coursor_x > 20){
			if( c[i+1] > ' ' && c[i]!=' ' ){
				Lcd_KS0108.GoToXY(126,coursor_y);
				Lcd_KS0108.WriteData(0x10);
				Lcd_KS0108.WriteData(0x10);
			};
			
			coursor_x = 0;
			
			coursor_y++;
			if(coursor_y > 8){
				coursor_y = 7;
			};
		};
	};
};

void text::NextLine(void)
{
	coursor_x = 0;
	coursor_y++;
	if(coursor_y > LINE_CNT){
		coursor_y = LINE_CNT - 1;
	};
	Lcd_KS0108.GoToXY(coursor_x,coursor_y);
}

text Text;