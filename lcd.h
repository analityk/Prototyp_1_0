#ifndef lcd_h__
#define lcd_h__

// http://radzio.dxp.pl/
// http://sunge.awardspace.com/glcd-sd/node4.html

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <font5x7.h>
#include <delay.h>

#define LCD_SET_X		0xB8
#define	LCD_SET_Y		0x40
#define LCD_START_LINE	0xC0
#define LCD_ON			0x3F
#define LCD_OFF			0x3E
#define LCD_BUSY		0x80

#define LCD_CS0_DDR		DDRE
#define LCD_CS0_PORT	PORTE
#define LCS_CS0			PE4

#define LCD_CS1_DDR		DDRE
#define LCD_CS1_PORT	PORTE
#define LCS_CS1			PE3


#define SET_LCD_CS0	{	LCD_CS1_DDR |=(1<<LCS_CS1);		\
						LCD_CS1_PORT |=(1<<LCS_CS1);	\
						LCD_CS0_DDR |=(1<<LCS_CS0);		\
						LCD_CS0_PORT &=~(1<<LCS_CS0);	\
}

#define SET_LCD_CS1 {	LCD_CS0_DDR |=(1<<LCS_CS0);		\
						LCD_CS0_PORT |=(1<<LCS_CS0);	\
						LCD_CS1_DDR |=(1<<LCS_CS1);		\
						LCD_CS1_PORT &=~(1<<LCS_CS1);	\
}

#define CLR_LCD_CS {	LCD_CS0_DDR |=(1<<LCS_CS0);		\
						LCD_CS0_PORT |=(1<<LCS_CS0);	\
						LCD_CS1_DDR |=(1<<LCS_CS1);		\
						LCD_CS1_PORT |=(1<<LCS_CS1);	\
}

#define LCD_RESET_DDR	DDRE
#define LCD_RESET_PORT	PORTE
#define LCD_RESET		PE2
#define SET_LCD_RESET	{ LCD_RESET_DDR |=(1<<LCD_RESET); LCD_RESET_PORT |=(1<<LCD_RESET); }
#define CLR_LCD_RESET	{ LCD_RESET_DDR |=(1<<LCD_RESET); LCD_RESET_PORT &=~(1<<LCD_RESET); }

#define LCD_RS_DDR	DDRE
#define LCD_RS_PORT	PORTE
#define LCD_RS		PE7
#define SET_LCD_RS	{ LCD_RS_DDR |=(1<<LCD_RS); LCD_RS_PORT |=(1<<LCD_RS); }
#define CLR_LCD_RS	{ LCD_RS_DDR |=(1<<LCD_RS); LCD_RS_PORT &=~(1<<LCD_RS); }

#define LCD_RW_DDR	DDRE
#define LCD_RW_PORT	PORTE
#define LCD_RW		PE6
#define SET_LCD_RW	{ LCD_RW_DDR |=(1<<LCD_RW); LCD_RW_PORT |=(1<<LCD_RW); }
#define CLR_LCD_RW	{ LCD_RW_DDR |=(1<<LCD_RW); LCD_RW_PORT &=~(1<<LCD_RW); }

#define LCD_EN_DDR	DDRE
#define LCD_EN_PORT	PORTE
#define LCD_EN		PE5
#define SET_LCD_EN	{ LCD_EN_DDR |=(1<<LCD_EN); LCD_EN_PORT |=(1<<LCD_EN); }
#define CLR_LCD_EN	{ LCD_EN_DDR |=(1<<LCD_EN); LCD_EN_PORT &=~(1<<LCD_EN); }

#define LCD_DATA_PORT	PORTD
#define LCD_DATA_DDR	DDRD
#define LCD_DATA_PIN	PIND

#define LCD_WAIT { asm volatile("nop"); asm volatile("nop"); }

class lcd_KS0108{
	private:
	uint8_t axis_x,axis_y;
	uint8_t controller;
	public:
	lcd_KS0108():axis_x(0), axis_y(0), controller(0){
		LCD_DATA_DDR = 0xFF;
		LCD_DATA_PORT = 0x00;
		CLR_LCD_RS;
		CLR_LCD_RW;
		CLR_LCD_EN;
		CLR_LCD_CS;
		Init();
	};
	uint8_t Status(uint8_t controller);
	void Init(void);
	void WriteCommand(uint8_t Command, uint8_t controller);
	void WriteData(uint8_t data);
	uint8_t ReadData(void);
	
	void GoToXY(uint8_t x, uint8_t y);
	void GoToXYText(uint8_t x, uint8_t y);
	void ClrScr(void);
	void SetPixel(uint8_t x, uint8_t y);
	void ClrPixel(uint8_t x, uint8_t y);
};

extern lcd_KS0108 Lcd_KS0108;

#endif // lcd_h__
