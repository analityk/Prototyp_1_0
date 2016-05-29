#include <button.h>

void button::Draw(const char* txt, point P)
{
	small_graph g;
	uint8_t len = 0;
	while(txt[len] != 0){len++;};
	uint8_t* pusz = (uint8_t*)malloc( (6 * len) + 5 );
	pusz[0] = 0x00; // 0xFF;
	pusz[1] = 0x00; // 0x81;
	pusz[2] = 0x00;
	
	for(uint8_t j=0; j<len; j++){
		for(uint8_t i=0; i < 5; i++){
			pusz[ (i+3)+j*6 ] = pgm_read_byte( font5x7 + ((txt[j] - 32)*5) + i );
			pusz[ (i+4)+j*6 ] = 0x00;
		};
	};
	pusz[ (6 * len) + 3 ] = 0x00; // 0x81;
	pusz[ (6 * len) + 4 ] = 0x00; // 0xFF;
	
	g.Draw(P, ((6 * len) + 5), pusz);
	free(pusz);
};

