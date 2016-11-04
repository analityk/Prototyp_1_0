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


void print(uint8_t* t, uint8_t x, uint8_t y){
	Text.Write( (char*)cord);
};

void delay(uint32_t volatile t){
	while(t--){};
};


int main(void)
{
	CLKPR = 0x80;
	CLKPR = 0x80;
	
	MainViev.Draw();
	Text.GoToAbs(12, 2);
	
	Timer.Enable();
	
	Timer.RegisterCallback( Text.CoursorBlinkEnable, 240 );
	Timer.RegisterCallback( Lcd_KS0108.LightOFF, 2);
	Timer.RegisterCallback( Lcd_KS0108.LightON, 3);
	
	while(1){
		
	};
	
	
}; 