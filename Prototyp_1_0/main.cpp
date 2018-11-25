/*
 * Prototyp_1_0.cpp
 *
 * Created: 2016-02-27 12:07:23
 * Author : Szymon
 */ 

#include <main_loop.h>
#include <exmem.h>
#include <avr/power.h>

void dummy_function(void){
	return;
};

uint8_t gcord[5];

void gconvert(uint16_t t){
	uint16_t a = t;
	uint16_t div = 1000;
	for(uint8_t i=4; i>0; i--){
		gcord[i-1] = a / div;
		a -= (gcord[i-1] * div);
		gcord[i-1] += 48;
		div /= 10;
	};
	
	uint8_t tmp = gcord[0];
	gcord[0] = gcord[3];
	gcord[3] = tmp;
	
	tmp = gcord[2];
	gcord[2] = gcord[1];
	gcord[1] = tmp;
	
	
	gcord[4] = 0;
};

void supply_wait(void)__attribute__((naked)) __attribute__((section(".init0")));
void supply_wait(){
	DDRF |= (1<<PINF7);
	PORTF |= (1<<PINF7);
	CLKPR = 0x80;
	CLKPR = 0;
};

int main(void)
{	
	sei();
	
	DDRB |=(1<<PINB6);
	
	_ps_act = PS_MAIN_VIEV;
	_ps_prev = PS_MAIN_VIEV;
	
	clr_txt_box();
	clr_inp_str();
	clr_ram_buf();
	clr_ref_addr();
	
	while(1){
	
		// after start and its normal view
		if( (_ps_act == PS_MAIN_VIEV) && (_ps_prev == PS_MAIN_VIEV) ){
			
			Timer.RegisterCallback( print_time, 10 );
			
			MainViev.Draw( cells );
			
			main_viev_action( Touch.ReadKey( MainViev.keycode ) );
			
			Timer.UnRegisterCallback();
			
		}; 
		
		// after push cell
		if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_MAIN_VIEV) ){
			
			Text.ClrScr();
			Text.SetSpaces(1);
			
			Text.GoToAbs(0, 5);
			Lcd_KS0108.WriteData(0x00);
			Text.GoToAbs(0, 7);
			Lcd_KS0108.WriteData(0x00);
			TextBoxViev.Draw();
			
			// print adres of editing cell
			print_edit_addr(12, 0);
			print_ref_addr(8, 0);
			
			clr_ref_addr();
			
			loadCellString();
			
			Text.GoToAbs(12,2);
			Text.SetSpaces(1);
			
			TextBoxViev.SmalChars();
			
			Timer.RegisterCallback( Text.CoursorBlinkEnable , 50 );
			
			_ps_act = PS_TEXT_EDIT;
			_ps_prev = PS_TEXT_REFRESH;
			
		};
		
		// after push up case 
		if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_TEXT_SMALL) ){
			
			up_case_action( Touch.ReadKey( TextBoxViev.keycode ) );
			
		};
		
		// after push OK
		if( (_ps_act == PS_TEXT_EDIT_END) && (_ps_prev == PS_TEXT_EDIT) ){
			
			calculate();
			
			storeCellString();
			
			clr_inp_str();
			
			clr_scr();
			
			MainViev.Draw( cells );
			
			Touch.delay_keypressed();
			
			Timer.Disable();
			
			_ps_act = PS_MAIN_VIEV;
			_ps_prev = PS_MAIN_VIEV;
		};
		
		// after push addr
		if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_TEXT_EDIT_ADDR) ){
			// pokazaæ main view
			Timer.RegisterCallback( print_time, 10 );
			
			clr_scr();
			
			uint8_t r = 0;
			
			store_act_addr();
			
			do{
				
				MainViev.Draw( cells );
			
				r = Touch.ReadKey( MainViev.keycode );
				
				main_viev_action( r );
				
				print_edit_addr(8, 0);
				
			}while(r <= 199);
			
			add_ref_addr();
			
			load_act_addr();
			
			storeCellString();
			
			Timer.UnRegisterCallback();
			_ps_act = PS_TEXT_EDIT;
			_ps_prev = PS_MAIN_VIEV;
		};
		
		if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_TEXT_REFRESH) ){
			
			Timer.Disable();
			
			show_input_cnts();
			
			write_input_str();
			
			Timer.Enable();
			_ps_act = PS_TEXT_EDIT;
			_ps_prev = PS_TEXT_SMALL;
		};
		
		// after push coursor move
		if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_TEXT_EDIT_COURSOR_MOVE) ){
			_ps_act = PS_TEXT_EDIT;
			_ps_prev = PS_TEXT_SMALL;
		};
		
	};
}; 
