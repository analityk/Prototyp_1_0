/*
 * Prototyp_1_0.cpp
 *
 * Created: 2016-02-27 12:07:23
 * Author : Szymon
 */ 

#include <main_loop.h>

void dummy_function(void){
	return;
};

int main(void)
{	
	sei();

	_ps_act = PS_MAIN_VIEV;
	_ps_prev = PS_MAIN_VIEV;
	
	clr_txt_box();
	clr_inp_str();	
	clr_ram_mem();
	clr_ram_buf();
	
	while(1){
	
		// after start and its normal view
		if( (_ps_act == PS_MAIN_VIEV) && (_ps_prev == PS_MAIN_VIEV) ){
			
			Timer.RegisterCallback( print_time, 10 );
			
			MainViev.Draw();
			
			fill_cells();
			
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
			print_edit_addr();
			
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
			
			MainViev.Draw();
			
			_ps_act = PS_MAIN_VIEV;
			_ps_prev = PS_MAIN_VIEV;
			
			Touch.delay_keypressed();
			
			Timer.Disable();
		};
		
		// after push addr
		if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_TEXT_EDIT_ADDR) ){
			_ps_act = PS_TEXT_EDIT;
			_ps_prev = PS_TEXT_SMALL;
		};
		
		if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_TEXT_REFRESH) ){
			
			Timer.Disable();
			
			show_input_cnts();
			
			write_input_str();
			
			_ps_act = PS_TEXT_EDIT;
			_ps_prev = PS_TEXT_SMALL;
			
			Timer.Enable();
		};
		//
		// after push coursor move
		if( (_ps_act == PS_TEXT_EDIT) && (_ps_prev == PS_TEXT_EDIT_COURSOR_MOVE) ){
			_ps_act = PS_TEXT_EDIT;
			_ps_prev = PS_TEXT_SMALL;
		};
		
	};
}; 
