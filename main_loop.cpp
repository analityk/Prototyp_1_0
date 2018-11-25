#include <main_loop.h>

Program_state _ps_act = 0;
Program_state _ps_prev = 0;
Program_state _ps_next = 0;

array< uint8_t > inp_str(CELL_SIZE_EXPR);

typedef union {
	uint8_t t[4];
	double d;
}Ucast;

Ucast ucast;	

uint8_t cord[5];
char time_str[9];

uint8_t TextBoxWindow[20];
uint8_t ram_buffer[CELL_SIZE_EXPR];

uint8_t cell_number;

uint8_t mem_ref_col;
uint8_t mem_ref_lin;

uint8_t ref_adr_col = 255;
uint8_t ref_adr_lin = 255;

uint8_t ram_cells_addr;

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

void clr_ref_addr(void)
{
	ref_adr_col = 255;
	ref_adr_lin = 255;
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
	
	cell_number = ( MainViev.ActColumn() + a )*10 + ( MainViev.ActLine() + b );
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
			set_active_cells(r);
			_ps_act = PS_TEXT_EDIT;
			_ps_prev = PS_MAIN_VIEV;
			break;
		};
		
		default:break;
	};
	
	MainViev.Draw( cells );
	Touch.delay_keypressed();
};

void print_edit_addr(uint8_t x, uint8_t y)
{
	Text.GoTo(x++, y);
	Text.Write('#');
	Text.GoTo(x++, y);
	uint8_t a = (cell_number/10);
	uint8_t b = cell_number - (10*a);
	Text.Write(  a + 65 );
	Text.GoTo(x++, y);
	Text.Write( b + 49 );
};

void print_ref_addr(uint8_t x, uint8_t y)
{
	if( !(ref_adr_col == 255) ){
		Text.GoTo(x++, y);
		Text.Write('#');
		Text.GoTo(x++, y);
		Text.Write(ref_adr_col+65);
		Text.GoTo(x++, y);
		Text.Write(ref_adr_lin+49);
	};
};

void clr_scr(void)
{
	Text.ClrScr();
	Text.SetSpaces(1);
};

void loadCellString(void)
{
	cells[cell_number].GetExpression( inp_str );

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
	cells[cell_number].SaveExpression(inp_str);
	cells[cell_number].SaveResult(&ucast.d);
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

void store_act_addr(void)
{
	mem_ref_col = cell_number / 10;
	mem_ref_lin = cell_number - mem_ref_col;
};

void load_act_addr(void)
{
	cell_number = mem_ref_col*10 + mem_ref_lin;
};

void add_ref_addr(void)
{
	store_act_addr();
	inp_str.insert('#');
	inp_str.insert(cell_number);
};

void show_gctor(uint16_t t)
{
	Text.GoTo(5,0);
	char c[11];
	itoa(t,c,10);
	Text.Write(c);
};

