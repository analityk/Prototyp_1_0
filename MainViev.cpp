#include <MainViev.h>

void format_onp_results(char* d){
	if( d[7] == '+' ){
		d[6] = 'E';
		d[7] = d[9];
		d[8] = d[10];
		d[9] = 0;
		d[10] = 0;
	}else{
		d[6] = 'e';
		d[7] = d[9];
		d[8] = d[10];
		d[9] = 0;
		d[10] = 0;
	};
};

void mainViev::Draw( array<Cell>& r )
{
	
	for( uint8_t i=0; i<2; i++ ){
		for( uint8_t j=0; j<2; j++ ){
			
			uint8_t addr = (column+j)*10 + (line+i);
			
			double d;
			
			uint8_t dts[20];
			
			cells[addr].GetViev((char*)(dts));
			
			if( dts[0] == '\'' ){
				dts[8] = 0;
				dts[0] = ' ';
				
				for( uint8_t i=strlen((char*)(dts)); i<8; i++ ){
					dts[i] = ' ';
				};
				
			}else{
				if( !(dts[0] == 0) ){
					cells[addr].GetResult(&d);
				}else{
					d = 0;
				};
				
				dtostre(d, (char*)(dts), 3, 1 );
				format_onp_results( (char*)(dts) );
				
			};
			
			Text.GoTo( 2+(10*j), 2+(i*3) );
			Text.SetSpaces(1);
			Text.Write(dts);
		};
	};
	
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