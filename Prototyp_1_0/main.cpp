/*
 * Prototyp_1_0.cpp
 *
 * Created: 2016-02-27 12:07:23
 * Author : Szymon
 */ 
// here is what more should be done
#include <to_do.h>

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
#include <rpn.h>


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

void delay_keypressed(void){
	while( !Touch.Press() ){};
};

void wait_release_key(void){
	delay_keypressed();
	delay(0x10000);
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

void print_time(void){
	FormatTime( Timer.Hours, Timer.Minutes, Timer.Seconds );
	Text.GoTo(13,7);
	Text.SetSpaces(1);
	Text.SetSpaces(1);
	Text.Write(time_str);
	Text.GoTo(13,7);
};

void dummy_function(void){
	return;
};

array< ram_grip > cells_mem(90);
array< uint8_t > inp_str(50);

uint8_t TextBoxWindow[20];
uint8_t ram_buffer[50];

uint8_t cells_col_offset = 0;
uint8_t cells_line_offset = 0;
uint8_t ram_cells_addr;


int main(void)
{	
	
	Text.ClrScr();
	Text.GoTo(0,0);
	Text.SetSpaces(1);
	
	array<uint8_t> d(50);
	
	for( uint8_t i=0; i<d.cnts(); i++ ){
		d[i] = 0;
	};
	
	/* test test test */
	char expr[] = "3^2+4*4";

	
	for( uint8_t i=0; i<strlen(expr); i++ ){
		d.insert(expr[i]);
	};
	
	uint8_t dts[20];
	
	dtostrf(rpn.infix_to_postfix(d), 5, 5, (char*)(dts) );
	
	Text.GoTo(0,0);
	Text.SetSpaces(1);
	Text.Write(dts);
	
	while(1){
		
	};
	
	//sei();
	//
	//MainViev.Draw();
	//
	//_ps_act = PS_MAIN_VIEV;
	//_ps_prev = PS_MAIN_VIEV;
	//
	//for( uint8_t i=0; i<19; i++ ){
		//TextBoxWindow[i] = ' ';
	//};
	//
	//for( uint8_t i=0; i<90; i++ ){
		//cells_mem[i] = ram.get_mem(50);
		//ram.write_block( cells_mem[i], 0, 50, inp_str.data );
		//if( cells_mem[i] == 0 ){
			//Text.GoTo(0,0);
			//Text.SetSpaces(1);
			//Text.Write(" ramm alloc error ");
			//Touch.ReadCoordinates();
			//wait_release_key();
		//};
	//};
	//
	//for( uint8_t i=0; i<50; i++  ){
		//ram_buffer[i] = 0;
	//};
	//
	//while(1){
	//
		//// after start and its normal view
		//if( (_ps_act == PS_MAIN_VIEV) && (_ps_prev == PS_MAIN_VIEV) ){
			//
			//Timer.RegisterCallback( print_time, 10 );
			//Timer.Enable();
			//
			//Touch.ReadCoordinates();
			//uint8_t r = touched( Touch.x/4, Touch.y/4, MainViev.keycode );
//
			//if( r == 0x88 ){ 
				//MainViev.DecLine(); 
				//MainViev.Draw(); 
				//wait_release_key();
			//};
			//
			//if( r == 0x89 ){ 
				//MainViev.IncLine(); 
				//MainViev.Draw(); 
				//wait_release_key();
			//};
			//
			//if( r == 0x81 ){ 
				//MainViev.DecCol(); 
				//MainViev.Draw();
				//wait_release_key();
			//};
			//
			//if( r == 0x80 ){
				//MainViev.IncCol(); 
				//MainViev.Draw(); 
				//wait_release_key();
			//};
			//
			//if( r == 200 ){
				//_ps_act = PS_TEXT_EDIT;
				//_ps_prev = PS_MAIN_VIEV;
				//cells_col_offset = MainViev.ActColumn();
				//cells_line_offset = MainViev.ActLine();
			//};
			//
			//if( r == 201 ){
				//_ps_act = PS_TEXT_EDIT;
				//_ps_prev = PS_MAIN_VIEV;
				//cells_col_offset = MainViev.ActColumn();
				//cells_line_offset = MainViev.ActLine()+1;
			//};
			//
			//if( r == 202 ){
				//_ps_act = PS_TEXT_EDIT;
				//_ps_prev = PS_MAIN_VIEV;
				//cells_col_offset = MainViev.ActColumn()+1;
				//cells_line_offset = MainViev.ActLine();
			//};
			//
			//if( r == 203 ){
				//_ps_act = PS_TEXT_EDIT;
				//_ps_prev = PS_MAIN_VIEV;
				//cells_col_offset = MainViev.ActColumn()+1;
				//cells_line_offset = MainViev.ActLine()+1;
			//};
			//
			//Timer.Disable();
			//
			//wait_release_key();
		//};
		//
		//// after push cell
		//if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_MAIN_VIEV) ){
			//
			//Text.ClrScr();
			//Text.SetSpaces(1);
			//
			//Text.GoToAbs(0, 5);
			//Lcd_KS0108.WriteData(0x00);
			//Text.GoToAbs(0, 7);
			//Lcd_KS0108.WriteData(0x00);
			//TextBoxViev.Draw();
			//
			//
			//// print adres of editing cell
			//Text.GoTo(12,0);
			//Text.Write('#');
			//Text.GoTo(13, 0);
			//Text.Write(cells_col_offset+65);
			//Text.GoTo(14, 0);
			//Text.Write(cells_line_offset+49);
			//
			//TextBoxViev.SmalChars();
						//
			//inp_str.erase();
			//ram_cells_addr = cells_col_offset*10 + cells_line_offset;
			//ram.read_block( cells_mem[ram_cells_addr], 0, 50, ram_buffer );
			//
			//for( uint8_t i=0; i<50; i++ ){
				//if( ram_buffer[i] == 0 )break;					
				//inp_str.insert(ram_buffer[i]);
			//};
			//
			//Text.GoToAbs(12,2);
			//Text.SetSpaces(1);
			//
			//Timer.RegisterCallback( Text.CoursorBlinkEnable , 50 );
			//Timer.Enable();
			//
			//_ps_act = PS_TEXT_EDIT;
			//_ps_prev = PS_TEXT_REFRESH;
			//
		//};
		//
		//// after push up case 
		//if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_TEXT_SMALL) ){
			//Touch.ReadCoordinates();
			//wait_release_key();
			//
			//uint8_t r = touched( Touch.x/4, Touch.y/4, TextBoxViev.keycode );
			//
			//if( r == 231 ){
				//Timer.Disable();
				//
				//if( TextBoxViev.keycode == 2 ){
					//TextBoxViev.SmalChars();
				//}else{
					//TextBoxViev.BigChars();
				//};
				//
				//Text.GoTo(inp_str.cnts()+2,2);
				//Text.SetSpaces(1);
				//_ps_act = PS_TEXT_EDIT;
				//_ps_prev = PS_TEXT_SMALL;
				//Timer.Enable();
			//};
			//
			//if( r == 228 ){
				//_ps_act = PS_TEXT_EDIT_END;
				//_ps_prev = PS_TEXT_EDIT;
			//};
			//
			//if( r == 227 ){
				//_ps_act = PS_TEXT_EDIT;
				//_ps_prev = PS_TEXT_EDIT_COURSOR_MOVE;
			//};
			//
			//if( r == 229 ){
				//inp_str.remove_last();
				//_ps_act = PS_TEXT_EDIT;
				//_ps_prev = PS_TEXT_REFRESH;
			//};
			//
			//if( r == 230 ){
				//_ps_act = PS_TEXT_EDIT;
				//_ps_prev = PS_TEXT_EDIT_ADDR;
			//};
			//
			//if( r < 200 ){
				//inp_str.insert(r);
				//_ps_act = PS_TEXT_EDIT;
				//_ps_prev = PS_TEXT_REFRESH;
			//};
		//};
		//
		//// after push OK
		//if( (_ps_act == PS_TEXT_EDIT_END) && (_ps_prev == PS_TEXT_EDIT) ){
			//
			//// save content in sram
			//ram_cells_addr = cells_col_offset*10 + cells_line_offset;
			//
			//ram.write_block( cells_mem[ram_cells_addr], 0, inp_str.cnts(), inp_str.data );
//
			//inp_str.erase();
			//
			//Text.ClrScr();
			//MainViev.Draw();
			//_ps_act = PS_MAIN_VIEV;
			//_ps_prev = PS_MAIN_VIEV;
			//wait_release_key();
			//Timer.Disable();
		//};
		//
		//// after push addr
		//if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_TEXT_EDIT_ADDR) ){
			//_ps_act = PS_TEXT_EDIT;
			//_ps_prev = PS_TEXT_SMALL;
		//};
		//
		//if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_TEXT_REFRESH) ){
			//
			//Timer.Disable();
			//Text.SetSpaces(1);
			//Text.GoTo(3,0);
			//convert(inp_str.cnts());
			//Text.Write(cord);
			//Timer.Enable();
			//
			//Text.GoTo(2,2);
			//Text.SetSpaces(1);
			//
			//uint8_t n = 0;
			//
			//if( inp_str.cnts() > 18 ){
				//n = inp_str.cnts() - 18;
			//};
			//
			//for( uint8_t i=0; i < 18; i++ ){
				//TextBoxWindow[i] = inp_str[i+n];
			//};
		//
			//TextBoxWindow[18] = 0;
			//
			//for( uint8_t i=0; i<18; i++ ){
				//Text.Write(' ');
			//};
			//
			//Text.GoTo(20, 2);
			//Text.Write('\x87');
			//
			//Text.GoTo(2,2);
			//Text.SetSpaces(1);
			//
			//Text.Write(TextBoxWindow);
			//
			//_ps_act = PS_TEXT_EDIT;
			//_ps_prev = PS_TEXT_SMALL;
		//};
		//
		//// after push coursor move
		//if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_TEXT_EDIT_COURSOR_MOVE) ){
						//
			//_ps_act = PS_TEXT_EDIT;
			//_ps_prev = PS_TEXT_SMALL;
		//};
		//
	//};
}; 
