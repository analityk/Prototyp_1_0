/*
 * Prototyp_1_0.cpp
 *
 * Created: 2016-02-27 12:07:23
 * Author : Szymon
 */ 

#include <avr/io.h>
#include <array.h>
#include <lcd.h>
#include <text.h>
#include <delay.h>
#include <adc.h>
#include <keyboard.h>
#include <touch.h>

void delay_milis(uint8_t milis){
	startMillis();
	delay_ms(milis);
	stopMillis();
};

uint8_t cord[4];

void convert(uint16_t t){
	uint16_t a = t;
	uint16_t div = 1000;
	for(uint8_t i=4; i>0; i--){
		cord[i-1] = a / div;
		a -= (cord[i-1] * div);
		cord[i-1] += 48;
		div /= 10;
	};
};

void print(uint8_t* t, uint8_t x, uint8_t y){
	for(uint8_t i=4; i>0; i--){
		Text.GoTo(x++, y);
		Text.WriteChar(cord[i-1]);
	};
};


int main(void)
{

	Keyboard kb;
	
	kb.Init();
	
	Text.GoTo(7,0);
	Text.WriteString("Siema");
	Text.GoTo(6,7);
	Text.WriteString("zapraszam");
	
	Text.Refresh();
	kb.DrawVisible();
	
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	delay_ms(250);
	
	Text.ClrScr();
	
	kb.LoadGroup(kb.KeysNavi);
	kb.LoadGroup(kb.KeysNum);
	kb.LoadGroup(kb.KeysBasic);
	
	kb.Draw();
	
	while(1){
		
		Touch.ReadCoordinates();
		
		convert(Touch.x / 4);
		Text.GoTo(7,3);
		Text.WriteChar('x');
		print(cord, 9, 3);
		
		convert(Touch.y / 4);
		Text.GoTo(7,4);
		Text.WriteChar('y');
		print(cord, 9, 4);
		
		uint8_t t = kb.Readkey();
		
		convert(t);
		Text.GoTo(7,5);
		Text.WriteChar('k');
		print(cord, 9, 5);
		
		Text.Refresh();
		kb.Draw();
		
		delay_ms(75);
	}
}