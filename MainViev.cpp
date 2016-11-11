#include <MainViev.h>

void mainViev::Draw(void)
{
	// znaki, litery, cyfry
	
	Text.SetSpaces(1);
	Text.GoTo(0,0);
	
	
	Text.Write("\x88 \x81                \x80 ");
	
	Text.GoToAbs(34, 0);
	Text.Write( (char)(column+65) );
	Text.GoToAbs(91, 0);
	Text.Write( (char)(column+66) );
	
	Text.GoTo(0,2);
	Text.Write( (char)(line+49) );
	
	Text.GoTo(0,5);
	Text.Write( (char)(line+50) );

	Text.GoTo(0,7);
	Text.Write("\x89");
	
	// wykropkowanie poziome
	for( uint8_t k=0; k<8; k+=3	){
		for( uint8_t i=6; i<126; i+=6 ){
			Lcd_KS0108.SetPixel( i, 7+(8*k));
		};
	};
		
	// wykropkowanie pionowe
		
	for( uint8_t k=0; k<2; k++ ){
		for( uint8_t i=0; i<60; i+=4 ){
			Lcd_KS0108.SetPixel(6+(60*k), 3+i);
		};
	};
		
	for( uint8_t i=0; i<60; i+=4 ){
		Lcd_KS0108.SetPixel(126, 3+i);
	};
	Text.GoToAbs(0,0);
};

void mainViev::DecLine(void)
{
	if( line > 0 )line--;
};

void mainViev::IncLine(void)
{
	if( line < LINE_CNT_MAX )line++;
};

void mainViev::DecCol(void)
{
	if( column > 0 )column--;
};

void mainViev::IncCol(void)
{
	if( column < COLUMN_CNT_MAX )column++;
};

uint8_t mainViev::ActLine(void)
{
	return line;
};

uint8_t mainViev::ActColumn(void)
{
	return column;
};

mainViev MainViev;