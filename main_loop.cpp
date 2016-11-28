#include <main_loop.h>

Program_state _ps_act = 0;
Program_state _ps_prev = 0;
Program_state _ps_next = 0;

array< ram_grip > cells_mem(CELLS_CNT);
array< uint8_t > inp_str(RAM_SIZE_ALLOC);

typedef union {
	uint8_t t[4];
	double d;
}Ucast;

Ucast ucast;	

uint8_t cord[5];
char time_str[9];

uint8_t TextBoxWindow[20];
uint8_t ram_buffer[RAM_SIZE_INPUT];

uint8_t cells_col_offset;
uint8_t cells_line_offset;

uint8_t ram_cells_addr;

void fill_cells(void){
	for( uint8_t i=0; i<2; i++ ){
		for( uint8_t j=0; j<2; j++ ){
			// a1
			union {
				uint8_t t[4];
				double d;
			}Ucast;
			
			ram_cells_addr = (MainViev.ActColumn()+j)*10 + (MainViev.ActLine()+i);
			ram.read_block( cells_mem[ram_cells_addr], 0, 1, Ucast.t );
			
			uint8_t dts[20];
			
			if( Ucast.t[0] == '\'' ){
				ram.read_block( cells_mem[ram_cells_addr], 1, 8, dts );
				dts[8] = 0;
				for( uint8_t i=strlen((char*)(dts)); i<8; i++ ){
					dts[i] = ' ';
				};
				}else{
				if( !(Ucast.t[0] == 0) ){
					ram.read_block( cells_mem[ram_cells_addr], RAM_SIZE_INPUT+1, RAM_SIZE_RESULT, Ucast.t );
					}else{
					Ucast.d = 0.0;
				};
				
				dtostre(Ucast.d, (char*)(dts), 3, 1 );
				format_onp_results( (char*)(dts) );
				
			};
			
			Text.GoTo( 2+(10*j), 2+(i*3) );
			Text.SetSpaces(1);
			Text.Write(dts);
		};
	};
};

void format_onp_results(char* d){
	if( d[7] == '+' ){
		d[6] = 'E';
		d[7] = d[9];
		d[8] = d[10];
		d[9] = 0;
		d[10] = 0;
		}else{
		d[6] = 'e';
		d[7] = d[9];
		d[8] = d[10];
		d[9] = 0;
		d[10] = 0;
	};
};

void clr_ram_mem(void){
	for( uint8_t i=0; i<CELLS_CNT; i++ ){
		
		cells_mem[i] = ram.get_mem(RAM_SIZE_ALLOC);
		
		ram.write_block( cells_mem[i], 0, RAM_SIZE_ALLOC, inp_str.data );
		
		if( cells_mem[i] == 0 ){
			Text.GoTo(0,0);
			Text.SetSpaces(1);
			Text.Write(" ramm alloc error ");
			
			uint8_t asd[20];
			itoa(i, (char*)(asd), 10);
			Text.GoTo(0,1);
			Text.Write(asd);

			Touch.ReadCoordinates();
			Touch.wait_release_key();
		};
	};
};

void clr_inp_str(void)
{
	inp_str.erase();
};

void clr_txt_box(void)
{
	for( uint8_t i=0; i<19; i++ ){
		TextBoxWindow[i] = ' ';
	};
};

void clr_ram_buf(void)
{
	for( uint8_t i=0; i<RAM_SIZE_INPUT; i++  ){
		ram_buffer[i] = 0;
	};
};

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

void set_active_cells(uint8_t button)
{
	uint8_t a = 0;
	uint8_t b = 0;
	switch(button){
		case 200:{ a=0; b=0; break; };
		case 201:{ a=0; b=1; break; };
		case 202:{ a=1; b=0; break; };
		case 203:{ a=1; b=1; break; };
		default:break;
	};
	cells_col_offset = MainViev.ActColumn()+a;
	cells_line_offset = MainViev.ActLine()+b;
};

void main_viev_action(uint8_t r)
{
	switch(r){
		case 0x88:{
			MainViev.DecLine();
			break;
		};
		case 0x89:{
			MainViev.IncLine();
			break;
		};
		case 0x81:{
			MainViev.DecCol();
			break;
		};
		case 0x80:{
			MainViev.IncCol();
			break;
		};
		case 200: case 201: case 202: case 203:
		{
			_ps_act = PS_TEXT_EDIT;
			_ps_prev = PS_MAIN_VIEV;
			set_active_cells(r);
			break;
		};
		
		default:break;
	};
	
	MainViev.Draw();
	Touch.wait_release_key();
};

void print_edit_addr(void)
{
	Text.GoTo(12,0);
	Text.Write('#');
	Text.GoTo(13, 0);
	Text.Write(cells_col_offset+65);
	Text.GoTo(14, 0);
	Text.Write(cells_line_offset+49);
};

void clr_scr(void)
{
	Text.ClrScr();
	Text.SetSpaces(1);
};

void loadCellString(void)
{
	inp_str.erase();
	ram_cells_addr = cells_col_offset*10 + cells_line_offset;
	ram.read_block( cells_mem[ram_cells_addr], 0, RAM_SIZE_INPUT, ram_buffer );
	
	for( uint8_t i=0; i<RAM_SIZE_INPUT; i++ ){
		if( ram_buffer[i] == 0 )break;
		inp_str.insert(ram_buffer[i]);
	};
};

void up_case_action(uint8_t r)
{
	uint8_t t = r;
	switch(r){
		case 231:{
				Timer.Disable();
				
				if( TextBoxViev.keycode == 2 ){
					TextBoxViev.SmalChars();
					}else{
					TextBoxViev.BigChars();
				};
				
				Text.GoTo(inp_str.cnts()+2,2);
				Text.SetSpaces(1);
				_ps_act = PS_TEXT_EDIT;
				_ps_prev = PS_TEXT_SMALL;
				Timer.Enable();
			break;
		};
		case 228:{
				_ps_act = PS_TEXT_EDIT_END;
				_ps_prev = PS_TEXT_EDIT;
			break;
		};
		case 227:{
				_ps_act = PS_TEXT_EDIT;
				_ps_prev = PS_TEXT_EDIT_COURSOR_MOVE;
			break;
		};
		
		case 229:{
				inp_str[inp_str.cnts()-1]=0;
				inp_str.remove_last();
				_ps_act = PS_TEXT_EDIT;
				_ps_prev = PS_TEXT_REFRESH;
			break;
		};
		
		case 230:{
				_ps_act = PS_TEXT_EDIT;
				_ps_prev = PS_TEXT_EDIT_ADDR;
			break;
		};
		
		default:{
				if( t < 200 ){
					inp_str.insert(r);
					_ps_act = PS_TEXT_EDIT;
					_ps_prev = PS_TEXT_REFRESH;
				};
			break;
		};
	};
};

void calculate(void)
{
	if( !(inp_str[0]=='"') ){
		ucast.d = rpn.infix_to_postfix( inp_str );
		if ( ucast.d == NAN ){
			_ps_act = PS_TEXT_EDIT;
			_ps_prev = PS_TEXT_SMALL;
			return;
		};
	}else{
		ucast.d = 0.0;
	};
};

void storeCellString(void)
{
	ram_cells_addr = cells_col_offset*10 + cells_line_offset;
	ram.write_block( cells_mem[ram_cells_addr], 0, RAM_SIZE_INPUT, inp_str.data );
	ram.write_block( cells_mem[ram_cells_addr], RAM_SIZE_INPUT+1, RAM_SIZE_RESULT, ucast.t );
};

void show_input_cnts(void)
{
	char t[6];
	Text.SetSpaces(1);
	Text.GoTo(3,0);
	itoa(inp_str.cnts(), t, 10);
	Text.Write(t);
};

void write_input_str(void)
{
	uint8_t n = 0;
	
	if( inp_str.cnts() > 18 ){
		n = inp_str.cnts() - 18;
	};
	
	for( uint8_t i=0; i < 18; i++ ){
		TextBoxWindow[i] = inp_str[i+n];
	};
	
	TextBoxWindow[18] = 0;
	
	Text.GoTo(2,2);
	Text.SetSpaces(1);
	
	for( uint8_t i=0; i<18; i++ ){
		Text.Write(' ');
	};
	
	Text.GoTo(20, 2);
	Text.Write('\x87');
	
	Text.GoTo(2,2);
	Text.SetSpaces(1);
	
	Text.Write(TextBoxWindow);
};

