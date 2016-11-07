#ifndef program_states_h__
#define program_states_h__

#include <avr/io.h>
#include <avr/pgmspace.h>

typedef volatile uint8_t Program_state;

Program_state ps_actual = 0;
Program_state ps_previous;
Program_state ps_viev = 0;

#define PS_MAIN_VIEV	1	// set on start user program
#define PS_INC_LINE		2	// touch inc line arrow
#define PS_DEC_LINE		3	// touch dec line arrow
#define PS_INC_COL		4	// touch inc column arrow
#define PS_DEC_COL		5	// touch del column arrow
#define PS_GET_FIELD	6	// touch adresable field (np A1)
#define PS_SET_TEXT		7	// touch adresable filed on main viev
#define PS_GET_REF		8	// show main viev to get adres a new cells
#define PS_BACKSPACE	9	// push backspace
#define PS_EDIT_OK		10	// push ok (finished editing cells)
#define PS_GET_CHAR		11	// inset char into text buffer
#define PS_CHARS_SIZE	12	// push arrow to set big or small characters on keyboard in text editor
#define PS_SET_COURSOR	13	// insert coursor (virtual pointer in edit buffer) 
#define PS_TEXT_EDIT	14

PROGMEM const uint16_t look_up_keys[] = {
	// x1, x2, y1, y2, key_code
	233, 254, 212, 254, 0x88,	// arrow up
	233, 254, 6, 45, 0x89,		// arrow down
	205, 225, 217, 254, 0x81,	// arrow left
	13, 30, 195, 233, 0x80,		// arrow right
	120, 227, 137, 208, 200,	// a1
	120, 227, 52, 118, 201,		// a2
	10, 108, 131, 195, 202,		// b1
	8, 109, 47, 117, 203,		// b2
	
	//viev textbox digits (big and small chars)
	223, 241, 101, 126, 49,		// '1'
	204, 222, 101, 128, 50,		// '2'
	186, 203, 101, 127, 51,		// '3'
	169, 187, 99, 128, 52,		// '4'
	151, 168, 101, 128, 53,		// '5'
	133, 150, 100, 128, 54,		// '6'
	115, 132, 99, 127, 55,		// '7'
	98, 114, 98, 126, 56,		// '8'
	81, 97, 97, 125, 57,		// '9'
	64, 80, 97, 122, 48,		// '0'
	48, 63, 94, 121, 40,		// '('
	32, 47, 93, 118, 41,		// ')'
	14, 31, 92, 117, 61,		// '='
	55, 71, 65, 90, 43,			// +
	39, 54, 65, 90, 45,			// -
	23, 38, 65, 90, 47,			// /
	5, 22, 64, 90, 42,			// *
	64, 80, 38, 63, 46,			// .
	48, 63, 41, 64, 58,			// :
	31, 47, 41, 64, 39,			// '
	14, 30, 40, 63, 34,			// "
	88, 105, 11, 37, 60,		// <
	72, 87, 11, 38, 62,			// >
	54, 71, 11, 37, 63,			// ?
	38, 53, 11, 37, 91,			// [
	24, 37, 11, 36, 93,			// ]
	6, 23, 11, 35,	124,		// |
	
	// viev textbox big chars
	230, 248, 71, 100, 81,	// q
	212, 229, 71, 99, 87,	// w
	195, 211, 72, 98, 69,	// e
	176, 194, 74, 97, 82,	// r
	158, 175, 71, 97, 84,	// t
	142, 157, 71, 97, 89,	// y
	123, 141, 71, 95, 85,	// u
	106, 122, 71, 95, 73,	// i
	89, 105, 70, 92, 79,	// o
	72, 88, 68, 91, 80,		// p
	
};


uint8_t touched(uint8_t x, uint8_t y, Program_state viev){
	uint8_t n;
	if( viev == PS_MAIN_VIEV ){
		n = 0;
	};
	
	//if( viev == PS_TEXT_EDIT ){
		//n = 9;
	//};
	
	for( uint8_t i = n; i < 255; i++ ){
		uint8_t min_x = pgm_read_byte( look_up_keys + (i*5) );
		uint8_t min_y = pgm_read_byte( look_up_keys + (i*5) + 2);
		uint8_t max_x = pgm_read_byte( look_up_keys + (i*5) + 1);
		uint8_t max_y = pgm_read_byte( look_up_keys + (i*5) + 3);
		
		if(  (x > min_x) && (x < max_x) && (y > min_y) && (y < max_y) ){
			return (pgm_read_byte( look_up_keys + (i*5) + 4));
		};
	};
return 255;
};


#endif // program_states_h__
