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
#include <keys.h>
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

array< ram_grip > cells_mem(100);

uint8_t cells_col_offset = 0;
uint8_t cells_line_offset = 0;

void print_time(void){
	FormatTime( Timer.Hours, Timer.Minutes, Timer.Seconds );
	Text.GoTo(13,7);
	Text.SetSpaces(1);
	Text.Write(time_str);
};

int main(void)
{	
	sei();
	
	MainViev.Draw();
	
	_ps_act = PS_MAIN_VIEV;
	_ps_prev = PS_MAIN_VIEV;
	
	Timer.RegisterCallback( print_time, 10 );
	Timer.Enable();
	
	while(1){
	
		if( (_ps_act == PS_MAIN_VIEV) && (_ps_prev == PS_MAIN_VIEV) ){
			Timer.Enable();
			Touch.ReadCoordinates();
			uint8_t r = touched( Touch.x/4, Touch.y/4, MainViev.keycode );

			if( r == 0x88 ){ MainViev.DecLine(); MainViev.Draw(); delay(0x22000); };
			if( r == 0x89 ){ MainViev.IncLine(); MainViev.Draw(); delay(0x22000); };
			if( r == 0x81 ){ MainViev.DecCol(); MainViev.Draw(); delay(0x22000); };
			if( r == 0x80 ){ MainViev.IncCol(); MainViev.Draw(); delay(0x22000); };
			
			if( r == 200 ){
				_ps_act = PS_TEXT_EDIT;
				_ps_prev = PS_MAIN_VIEV;
				cells_col_offset = MainViev.ActColumn();
				cells_line_offset = MainViev.ActLine();
			};
			
			if( r == 201 ){
				_ps_act = PS_TEXT_EDIT;
				_ps_prev = PS_MAIN_VIEV;
				cells_col_offset = MainViev.ActColumn();
				cells_line_offset = MainViev.ActLine()+1;
			};
			
			if( r == 202 ){
				_ps_act = PS_TEXT_EDIT;
				_ps_prev = PS_MAIN_VIEV;
				cells_col_offset = MainViev.ActColumn()+1;
				cells_line_offset = MainViev.ActLine();
			};
			
			if( r == 203 ){
				_ps_act = PS_TEXT_EDIT;
				_ps_prev = PS_MAIN_VIEV;
				cells_col_offset = MainViev.ActColumn()+1;
				cells_line_offset = MainViev.ActLine()+1;
			};
			Timer.Disable();
		};
		
		if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_MAIN_VIEV) ){
			Text.SetSpaces(1);
			Text.ClrScr();
			Text.GoToAbs(0, 5);
			Lcd_KS0108.WriteData(0x00);
			Text.GoToAbs(0, 7);
			Lcd_KS0108.WriteData(0x00);
			TextBoxViev.Draw();
			
			Text.GoTo(12,0);
			Text.Write('#');
			Text.GoTo(13, 0);
			Text.Write(cells_col_offset+65);
			Text.GoTo(14, 0);
			Text.Write(cells_line_offset+49);
			
			TextBoxViev.SmalChars();
			_ps_act = PS_TEXT_EDIT;
			_ps_prev = PS_TEXT_SMALL;
		};
		
		if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_TEXT_SMALL) ){
			Touch.ReadCoordinates();
			uint8_t r = touched( Touch.x/4, Touch.y/4, TextBoxViev.keycode );
			if( r == 231 ){
				_ps_act = PS_TEXT_EDIT;
				_ps_prev = PS_TEXT_BIG;
				TextBoxViev.BigChars();
				delay(0x1FFFF);
			};
			if( r == 228 ){
				_ps_act = PS_TEXT_EDIT_END;
				_ps_prev = PS_TEXT_EDIT;
			};
		};
		
		if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_TEXT_BIG) ){
			Touch.ReadCoordinates();
			uint8_t r = touched( Touch.x/4, Touch.y/4, TextBoxViev.keycode );
			if( r == 231 ){
				_ps_act = PS_TEXT_EDIT;
				_ps_prev = PS_TEXT_SMALL;
				TextBoxViev.SmalChars();
				delay(0x1FFFF);
			};
			if( r == 228 ){
				_ps_act = PS_TEXT_EDIT_END;
				_ps_prev = PS_TEXT_EDIT;
			};
		};
		
		// after push OK
		if( (_ps_act == PS_TEXT_EDIT_END) && (_ps_prev == PS_TEXT_EDIT) ){
			Text.ClrScr();
			MainViev.Draw();
			_ps_act = PS_MAIN_VIEV;
			_ps_prev = PS_MAIN_VIEV;
			delay(0x88200);
		};
	};
}; 
