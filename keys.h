#ifndef keys_h__
#define keys_h__

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <program_states.h>

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
	10, 243, 137, 189, 195,	// 227 znak sterujacy, przeniesienie kursora w konkretne miejsce.
	2, 33, 190, 221, 196,	// 228 znak steruj�cy, OK
	44, 66, 202, 235, 197,	// 229 znak steruj�cy, backspace
	220, 240, 215, 250, 198,// 230 znak steruj�cy, adres kom�rki
	231, 248, 6, 37, 199,	// 231 znak steruj�cy, zmiana znak�w na ma�e/wielkie
	223, 241, 101, 126, 49,	// '1'
	204, 222, 101, 128, 50,	// '2'
	186, 203, 101, 127, 51,	// '3'
	169, 187, 99, 128, 52,	// '4'
	151, 168, 101, 128, 53,	// '5'
	133, 150, 100, 128, 54,	// '6'
	115, 132, 99, 127, 55,	// '7'
	98, 114, 98, 126, 56,	// '8'
	81, 97, 97, 125, 57,	// '9'
	64, 80, 97, 122, 48,	// '0'
	48, 63, 94, 121, 40,	// '('
	32, 47, 93, 118, 41,	// ')'
	14, 31, 92, 117, 61,	// '='
	55, 71, 65, 90, 43,		// +
	39, 54, 65, 90, 45,		// -
	23, 38, 65, 90, 47,		// /
	5, 22, 64, 90, 42,		// *
	64, 80, 38, 63, 46,		// .
	48, 63, 41, 64, 58,		// :
	31, 47, 41, 64, 39,		// '
	14, 30, 40, 63, 34,		// "
	88, 105, 11, 37, 60,	// <
	72, 87, 11, 38, 62,		// >
	54, 71, 11, 37, 63,		// ?
	38, 53, 11, 37, 91,		// [
	24, 37, 11, 36, 93,		// ]
	6, 23, 11, 35,	124,	// |
	
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
	222, 247, 42, 66, 65,	// a
	204, 221, 43, 66, 83,	// s
	187, 203, 43, 66, 68,	// d
	168, 186, 41, 66, 70,	// f
	150, 167, 42, 65, 71,	// g
	132, 149, 41, 65, 72,	// h
	115, 131, 41, 64, 74,	// j
	98, 114, 40, 64, 75,	// k
	82, 97, 41, 65, 76,		// l
	212, 229, 13, 39, 90,	// z
	194, 211, 13, 40, 88,	// x
	176, 193, 13, 40, 67,	// c
	158, 175, 13, 40, 86,	// v
	140, 157, 13, 39, 66,	// b
	123, 139, 13, 39, 78,	// n
	105, 122, 13, 39, 77	// m
};

uint8_t touched(uint8_t x, uint8_t y, uint8_t caller){

	switch(caller){
		case 1:{
			for( uint8_t i=0; i<8; i++){
				uint8_t min_x = pgm_read_byte( look_up_keys + (i*5) );
				uint8_t min_y = pgm_read_byte( look_up_keys + (i*5) + 2);
				uint8_t max_x = pgm_read_byte( look_up_keys + (i*5) + 1);
				uint8_t max_y = pgm_read_byte( look_up_keys + (i*5) + 3);
				if(  (x > min_x) && (x < max_x) && (y > min_y) && (y < max_y) ){
					return (pgm_read_byte( look_up_keys + (i*5) + 4));
				};
				
			};
			return 255;
			break;
		};
		
		case  2:{
			for( uint8_t i=8; i<66; i++){
				uint8_t min_x = pgm_read_byte( look_up_keys + (i*5) );
				uint8_t min_y = pgm_read_byte( look_up_keys + (i*5) + 2);
				uint8_t max_x = pgm_read_byte( look_up_keys + (i*5) + 1);
				uint8_t max_y = pgm_read_byte( look_up_keys + (i*5) + 3);
				
				if(  (x > min_x) && (x < max_x) && (y > min_y) && (y < max_y) ){
					uint8_t r = pgm_read_byte( look_up_keys + (i*5) + 4);
					if( r < 140 )return r;
					if( r > 140 )return r + 32;
				};
				
			};
			return 255;
			break;
		};
		
		case 3:{
				for( uint8_t i=8; i<66; i++){
					uint8_t min_x = pgm_read_byte( look_up_keys + (i*5) );
					uint8_t min_y = pgm_read_byte( look_up_keys + (i*5) + 2);
					uint8_t max_x = pgm_read_byte( look_up_keys + (i*5) + 1);
					uint8_t max_y = pgm_read_byte( look_up_keys + (i*5) + 3);
					
					if(  (x > min_x) && (x < max_x) && (y > min_y) && (y < max_y) ){
						uint8_t r = pgm_read_byte( look_up_keys + (i*5) + 4);
						if( r < 65 )return r;
						if( (r == 124)||(r==91)||(r==93) )return r;
						if( r >= 65 )return r + 32;
					};
				
				};
				
			return 255;
			break;
		};
		
		default:break;
	};
	
	return 255;
};

#endif // keys_h__