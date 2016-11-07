/*
 * Prototyp_1_0.cpp
 *
 * Created: 2016-02-27 12:07:23
 * Author : Szymon
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <timer.h>
#include <string.h>
#include <sram.h>
#include <array.h>
#include <lcd.h>
#include <text.h>
#include <adc.h>
#include <touch.h>
#include <spi.h>
#include <at45db321.h>
#include <MainViev.h>
#include <TextBoxViev.h>

#include <program_states.h>

void supply_wait(void)__attribute__((naked)) __attribute__((section(".init0")));
void supply_wait(){
	PORTG =1;
	DDRG  =1;
};

void retclk(void) __attribute__((naked)) __attribute__((section(".fini0")));
void retclk(void){
	CLKPR = 0x80;
	CLKPR = 0x88;
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
	
	
	cord[4] = 0;
};

void delay(uint32_t volatile t){
	while(t--){};
};

//23.07

char time_str[9];

void FormatTime( uint8_t h, uint8_t m, uint8_t s ){
	convert( (uint8_t)(h) );
	time_str[0] = cord[2];
	time_str[1] = cord[3];
	time_str[2] = ':';
	convert( (uint8_t)(m) );
	time_str[3] = cord[2];
	time_str[4] = cord[3];
	time_str[5] = ':';
	convert( (uint8_t)(s) );
	time_str[6] = cord[2];
	time_str[7] = cord[3];
	time_str[8] = 0;
};

int main(void)
{
	CLKPR = 0x80;
	CLKPR = 0x80;
	
	sei();
	
	TextBoxViev.BigChars();
	TextBoxViev.Draw();
	
	//
	//Timer.Enable();
	//
	//Timer.RegisterCallback( Text.CoursorBlinkEnable, 50 );
	
	while(1){
		Touch.ReadCoordinates();
		TextBoxViev.BigChars();
		TextBoxViev.Draw();
		
		
		//uint8_t read_key = touched( Touch.x/4, Touch.y/4, 0 );
				
		//Timer.Disable();
		
		Text.SetSpaces(1);
		convert(Touch.x / 4);
		Text.GoTo(2,2);
		Text.Write((char*)(cord) );
			
		convert(Touch.y / 4);
		Text.GoTo(12,2);
		Text.Write( (char*)(cord) );
		
		delay(0xFFFF);
		//Timer.Enable();
	};
	
	
}; 
