/*
 * Prototyp_1_0.cpp
 *
 * Created: 2016-02-27 12:07:23
 * Author : Szymon
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <sram.h>
#include <array.h>
#include <ram_alloc.h>
#include <usart.h>
#include <lcd.h>
#include <text.h>
#include <delay.h>
#include <adc.h>
#include <keyboard.h>
#include <touch.h>
#include <spi.h>
#include <at45db321.h>

void delay_milis(uint8_t milis){
	startMillis();
	delay_ms(milis);
	stopMillis();
};

void delay_s(uint8_t s){
	for(uint8_t i=0; i<s; i++){
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
	};
};

uint8_t cord[5];

void convert(uint16_t t){
	uint16_t a = t;
	uint16_t div = 1000;
	for(uint8_t i=4; i>0; i--){
		cord[i-1] = a / div;
		a -= (cord[i-1] * div);
		cord[i-1] += 48;
		div /= 10;
	};
	uint8_t tmp = cord[0];
	cord[0] = cord[3];
	cord[3] = tmp;
	
	tmp = cord[2];
	cord[2] = cord[1];
	cord[1] = tmp;
	
	
	cord[4] = '\n';
};


void print(uint8_t* t, uint8_t x, uint8_t y){
	for(uint8_t i=4; i>0; i--){
		Text.GoTo(x++, y);
		Text.WriteChar(cord[i-1]);
	};
};

typedef struct{
	uint8_t ut;
	char uc[10];
}Ttest;

void delay(uint32_t volatile t){
	while(t--){};
};

//ISR(USART0_RX_vect){};

int main(void)
{
	cli();
	
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	//sei();
	
	Serial.Enable();
	
	//at45.WriteText( 0, "q w e r t y u i o p\n" );
	//at45.WriteText( 32, "a s d f g h j k l\n" );
	//at45.WriteText( 64, "z x c v b n m\n" );
	//at45.WriteText( 96, "1 2 3 4 5 6 7 8 9 0\n" );
	//at45.WriteText( 128, "Q W E R T Y U I O P\n" );
	//at45.WriteText( 160, "A S D F G H J K L\n" );
	//at45.WriteText( 192, "Z X C V B N M\n" );
	//at45.WriteText( 224, "1 2 3 4 5 6 7 8 9 0\n" );
	//at45.WriteText( 256,"14254617\n");
	//
	//at45.programPage(0);
	
	Ttest test;
	test.uc[0] = 'a';
	test.uc[1] = 'b';
	test.uc[2] = 'c';
	test.uc[3] = 'd';
	test.uc[4] = 'e';
	test.uc[5] = 'f';
	test.ut = 12;
	
	ram_alloc< Ttest >lista(10);
	lista.push(test);
	
	//at45.readPage(0);
	//
	//ram_grip at45_grip = ram.get_mem(1056);
	//
	//ram.write_block(at45_grip, 0, 528, at45.page_buffer);
	//
	//Text.ClrScr();
//
	//for( uint8_t i=0; i<4; i++){
		//uint8_t x = (uint8_t) ram.readByte(at45_grip, 256 + (i*2)) - 48;
		//uint8_t y = (uint8_t) ram.readByte(at45_grip, 256 + (i*2) + 1) - 48;
		//Text.GoTo(x,y);
		//
		//uint8_t t[32];
		//
		//ram.read_block(at45_grip, i*32, 32 , t);
		//
		//Text.WriteString( ( char* ) t );
	//};
	
	//Text.Refresh();
	//delay(0xFFFFF);
	
	Text.ClrScr();
	if( lista.at(0).ut > 0 ){
		Text.WriteString( "cos tu jest" );
	}else{
		Text.WriteString( "cos tu bylo" );
	};
	
	Text.GoTo(0,1);
	Text.WriteString( (char*) lista.at(0).uc );
	Text.GoTo(0,2);
	
	
	convert(lista.at(0).ut);
	Text.WriteBuffer( cord );
	
	Text.Refresh();
	
	
	while(1){
		
	};
}; 