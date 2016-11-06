#include <lcd.h>

uint8_t lcd_KS0108::Status(uint8_t controller)
{
	uint8_t volatile t = 0;
	
	if(controller == 0){
		SET_LCD_CS0;
		CLR_LCD_RS;
		SET_LCD_RW;
		
		CLR_LCD_EN;
		LCD_WAIT;
		LCD_DATA_DDR = 0;
		t = LCD_DATA_PIN;
		
		SET_LCD_EN;
		
		CLR_LCD_CS;
		return t;
	};
	
	if(controller == 1){
		SET_LCD_CS1;
		CLR_LCD_RS;
		SET_LCD_RW;
		
		CLR_LCD_EN;
		LCD_WAIT;
		LCD_DATA_DDR = 0;
		t = LCD_DATA_PIN;
		
		SET_LCD_EN;
		
		CLR_LCD_CS;
		return t;
	};
	return 0;
};

void lcd_KS0108::Init(void)
{
	
	WriteCommand( LCD_RESET, 0 );
	WriteCommand( LCD_RESET, 1 );
	
	delay_ms(10);
	
	WriteCommand( LCD_ON, 0 );
	WriteCommand( LCD_ON, 1 );
	ClrScr();
	GoToXY(0,0);
	
	DDRG |=(1<<PG0);
	PORTG |=(1<<PG0);
}

void lcd_KS0108::WriteCommand(uint8_t Command, uint8_t controller)
{
	while( Status( controller ) & LCD_BUSY ){};
	LCD_DATA_DDR = 0xFF;
	CLR_LCD_RW;
	CLR_LCD_RS;
	if(controller == 0){
		SET_LCD_CS0;
	}else{
		SET_LCD_CS1;
	};
	LCD_DATA_PORT = Command;
	SET_LCD_EN;
	LCD_WAIT;
	CLR_LCD_EN;
	CLR_LCD_CS;
}

void lcd_KS0108::WriteData(uint8_t data)
{
	if( axis_x == 64 ){
		GoToXY(64,axis_y);
	};
	
	controller = axis_x / 64;
	
	while( Status( controller ) & LCD_BUSY ){};
	if( controller ){
		SET_LCD_CS0;
	}else{
		SET_LCD_CS1;
	};
	LCD_DATA_DDR = 0xFF;
	CLR_LCD_RW;
	SET_LCD_RS;
	LCD_DATA_PORT = data;
	SET_LCD_EN;
	LCD_WAIT;
	CLR_LCD_EN;
	
	CLR_LCD_CS;
	axis_x++;
}

uint8_t lcd_KS0108::ReadData(void)
{
	uint8_t data;

	while( Status( axis_x/64 ) & LCD_BUSY ){};
	if( (axis_x/64) > 0 ){
		SET_LCD_CS0;
	}else{
		SET_LCD_CS1;
	};
	LCD_DATA_DDR = 0x00;
	SET_LCD_RW;
	SET_LCD_RS;
	
	SET_LCD_EN;
	LCD_WAIT;
	data = LCD_DATA_PIN;
	CLR_LCD_EN;
	CLR_LCD_CS;
	axis_x++;
	return data;
};

void lcd_KS0108::GoToXY(uint8_t x, uint8_t y)
{
	uint8_t i;
	axis_x = x;
	axis_y = y;

	for(i = 0; i < 2; i++)
	{
		WriteCommand(LCD_SET_Y | 0, i);
		WriteCommand(LCD_SET_X | y, i);
		WriteCommand(LCD_START_LINE | 0, i);
	}
	WriteCommand(LCD_SET_Y | (x % 64), (x / 64));
	WriteCommand(LCD_SET_X | y, (x / 64));
};

void lcd_KS0108::GoToXYText(uint8_t x, uint8_t y)
{
	GoToXY( 6*x, y );
};

void lcd_KS0108::ClrScr(void)
{
	for(uint8_t j = 0; j < 8; j++)
	{
		GoToXY(0,j);
		for(uint8_t i = 0; i < 128; i++)
		WriteData(0x00);
	};
}

void lcd_KS0108::SetPixel(uint8_t x, uint8_t y)
{
	uint8_t tmp;
	GoToXY(x, y/8);
	tmp = ReadData();
	GoToXY(x, y/8);
	tmp = ReadData();
	GoToXY(x, y/8);
	//if(color)
	tmp |= (1 << (y%8));
	//else
	//tmp &= ~(1 << (y%8));
	WriteData(tmp);
}

void lcd_KS0108::ClrPixel(uint8_t x, uint8_t y)
{
	uint8_t tmp;
	GoToXY(x, y/8);
	tmp = ReadData();
	GoToXY(x, y/8);
	tmp = ReadData();
	GoToXY(x, y/8);
	//if(color)
	//tmp |= (1 << (y%8));
	//else
	tmp &= ~(1 << (y%8));
	WriteData(tmp);
}

void lcd_KS0108::LightON(void)
{
	PORTG |=(1<<0);
	DDRG  |=(1<<0);
};

void lcd_KS0108::LightOFF(void)
{
	DDRG  |=(1<<0);
	PORTG &=~(1<<0);
};
lcd_KS0108 Lcd_KS0108;