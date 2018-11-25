#include <keys.h>

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
		
		case 2:{
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