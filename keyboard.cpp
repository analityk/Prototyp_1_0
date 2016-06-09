#include <keyboard.h>

char empty_tab[] = {0,0,0,0};

#define BYTES_PER_CTRL 12

// klawiatura :
// w pamięci uC - adres początku tego, co ma zostać wyświetlone np Keyboard = 0x0000;
// w pamięci ram - kod klawisza, wspołrzędne tekstu, adres tekstu w pamięci ram
//					uint8_t		uint8 uint8				uint16
// obsługa - odczyt współrzędnych
// przejście przez wszystkie elementy klawiatury i złapanie wciśniętego przycisku
// wrzucenie do kolejki io kodu wybranego klawisza chyba, że to klawisz specjalny, np shift

PROGMEM	const uint8_t keymem[] = {
	/*	q	*/	113,3,	113,	32,		0,		0,		4,		4,	0,0,0,0,
	/*	w	*/	119,3,  119,	32,		0,		0,		14,		4,	0,0,0,0,
	/*	e	*/	101,3,	101,	32,		0,		0,		24,		4,	0,0,0,0,
	/*	r	*/	114,3,	114,	32,		0,		0,		34,		4,	0,0,0,0,
	/*	t	*/	116,3,	116,	32,		0,		0,		44,		4,	0,0,0,0,
	/*	y	*/	121,3,	121,	32,		0,		0,		54,		4,	0,0,0,0,
	/*	u	*/	117,3,	117,	32,		0,		0,		64,		4,	0,0,0,0,
	/*	i	*/	105,3,	105,	32,		0,		0,		74,		4,	0,0,0,0,
	/*	o	*/	111,3,	111,	32,		0,		0,		84,	4,	0,0,0,0,
	/*	p	*/	112,3,	112,	32,		0,		0,		94,	4,	0,0,0,0,
	
	/*	a	*/	97, 3,	97,		32,		0,		0,		10,		5,	0,0,0,0,
	/*	s	*/	115,3,	115,	32,		0,		0,		20,		5,	0,0,0,0,
	/*	d	*/	100,3,	100,	32,		0,		0,		30,		5,	0,0,0,0,
	/*	f	*/	102,3,	102,	32,		0,		0,		40,		5,	0,0,0,0,
	/*	g	*/	103,3,	103,	32,		0,		0,		50,		5,	0,0,0,0,
	/*	h	*/	104,3,	104,	32,		0,		0,		60,		5,	0,0,0,0,
	/*	j	*/	106,3,	106,	32,		0,		0,		70,		5,	0,0,0,0,
	/*	k	*/	107,3,	107,	32,		0,		0,		80,		5,	0,0,0,0,
	/*	l	*/	108,3,	108,	32,		0,		0,		90,	5,	0,0,0,0,
	
	/*	z	*/	122,3,	122,	32,		0,		0,		14,		6,	0,0,0,0,
	/*	x	*/	120,3,	120,	32,		0,		0,		24,		6,	0,0,0,0,
	/*	c	*/	99,	3,	99,		32,		0,		0,		34,		6,	0,0,0,0,
	/*	v	*/	118,3,	118,	32,		0,		0,		44,		6,	0,0,0,0,
	/*	b	*/	98,	3,	98,		32,		0,		0,		54,		6,	0,0,0,0,
	/*	n	*/	110,3,	110,	32,		0,		0,		64,		6,	0,0,0,0,
	/*	m	*/	109,3,	109,	32,		0,		0,		74,		6,	0,0,0,0,

	/*	!	*/	33,	2,	0x21,	0,		0,		0,		4,		0,	0,0,0,0,
	/*	%	*/	37,	2,	0x25,	0,		0,		0,		16,		0,	0,0,0,0,
	/*	&	*/	38,	2,	0x26,	0,		0,		0,		28,		0,	0,0,0,0,
	/*	(	*/	40,	2,	0x28,	0,		0,		0,		64,		0,	120, 144, 10, 35,
	/*	)	*/	41,	2,	0x29,	0,		0,		0,		78,		0,	145, 172, 10, 35,
	/*	*	*/	42,	2,	0x2A,	0,		0,		0,		28,		0,	50, 72, 10, 35,
	/*	+	*/	43,	2,	0x2B,	0,		0,		0,		4,		0,	6,  28, 10, 35,
	/*	-	*/	45,	2,	0x2D,	0,		0,		0,		16,		0,	29, 49, 10, 35,
	/*	.	*/	46,	2,	0x2E,	0,		0,		0,		52,		0,	96, 119, 10, 35,
	/*	/	*/	47, 2,	0x2F,	0,		0,		0,		40,		0,	73, 95, 10, 35,
	/*	0	*/	48, 2,	0x30,	0,		0,		0,		10,		7,	6, 28, 210, 240,
	/*	1	*/	49, 2,	0x31,	0,		0,		0,		20,		7,	29, 50, 210, 240,
	/*	2	*/	50, 2,	0x32,	0,		0,		0,		30,		7,	51, 72, 213, 240,
	/*	3	*/	51, 2,	0x33,	0,		0,		0,		40,		7,	72, 96, 212, 239,
	/*	4	*/	52, 2,	0x34,	0,		0,		0,		50,		7,	96, 118, 214, 240,
	/*	5	*/	53, 2,	0x35,	0,		0,		0,		60,		7,	119, 142, 213, 240,
	/*	6	*/	54, 2,	0x36,	0,		0,		0,		70,		7,	143, 166, 215, 241,
	/*	7	*/	55, 2,	0x37,	0,		0,		0,		80,		7,	167, 191, 214, 241,
	/*	8	*/	56, 2,	0x38,	0,		0,		0,		90,		7,	192, 215, 213, 242,
	/*	9	*/	57, 2,	0x39,	0,		0,		0,		100,		7,	216, 241, 215, 243,
	/*	=	*/	61, 2,	0x3D,	0,		0,		0,		116,	0,	222, 251, 10, 40,
	/*	>>	*/	130, 2,	0x80,	0,		0,		0,		0,		2,	0, 18, 66, 90,
	/*	>>	*/	131, 2,	0x80,	0,		0,		0,		0,		3,	0, 18, 91, 123,
	/*	>>	*/	132, 2,	0x80,	0,		0,		0,		0,		4,	0, 18, 124, 154,
	/*	>>	*/	133, 2,	0x80,	0,		0,		0,		0,		5,	0, 18, 154, 184,
	/*	<<	*/	134, 2,	0x81,	0,		0,		0,		116,	2,	228, 245, 66, 93,
	/*	<<	*/	135, 2,	0x81,	0,		0,		0,		116,	3,	228, 245, 94, 123,
	/*	<<	*/	136, 2,	0x81,	0,		0,		0,		116,	4,	228, 245, 124, 154,
	/*	<<	*/	137, 2,	0x81,	0,		0,		0,		116,	5,	228, 245, 155, 185,
	/*	ln	*/	160, 3,	0x6C,	0x6E,	0,		0,		0,		0, 0,0,0,0,
	/*	log	*/	161, 4,	0x6C,	0x6F,	0x67,	0,		64,		0, 0,0,0,0,
	/*  exp */  166, 4, 0x65,	0x78,	0x70,	0,		0,		0, 0,0,0,0,
	/*	sin	*/	162, 4,	0x73,	0x69,	0x6E,	0,		0,		0, 0,0,0,0,
	/*	cos	*/	163, 4,	0x63,	0x6F,	0x73,	0,		30,		0, 0,0,0,0,
	/*	<-	*/  250, 2,	0x7F,	0,		0,		0,	  100,		0, 194, 218, 10, 40, 35,
	/* tg  */	164, 4,  0x74,	0x67,	0,		0,		0,		0, 0,0,0,0,
	/* ^-1  */	165, 4,  0x5E,	0x2D,	0x31,	0,		76,		0, 0,0,0,0
};

void Keyboard::Init(void)
{
	array< uint8_t > r(BYTES_PER_CTRL);
	
	for( uint8_t i=0; i<Keys.size(); i++ ){
		
		for( uint8_t j=0; j<r.size(); j++ ){
			r[j] = pgm_read_byte(keymem + j + ( BYTES_PER_CTRL * i ) );
		};
		
		Keys[i].buttonID = r[0];
		Keys[i].fn_size = r[1];
		
		for( uint8_t t=0; t<r[1]; t++ ){
			Keys[i].funcName[t] = r[t+2];
		};
		
		Keys[i].x_point = r[6];
		Keys[i].y_point = r[7];
		Keys[i].visible = false;
		Keys[i].low_x = r[8];
		Keys[i].high_x = r[9];
		Keys[i].low_y = r[10];
		Keys[i].high_y = r[11];
		
	};

	for( uint8_t i=0; i<KeysBasic.size(); i++ ){
		KeysBasic[i] = pgm_read_byte(basicKeys + i);
	};
	
	for( uint8_t i=0; i<KeysSin.size(); i++ ){
		KeysSin[i] = pgm_read_byte(sinKeys + i);
	};
	
	for( uint8_t i=0; i<KeysLog.size(); i++ ){
		KeysLog[i] = pgm_read_byte(logKeys + i);
	}
	
	for(uint8_t i=0; i<KeysNum.size(); i++ ){
		KeysNum[i] = pgm_read_byte(numKeys + i);
	};
	
	for( uint8_t i=0; i<KeysNavi.size(); i++ ){
		KeysNavi[i] = pgm_read_byte(naviKeys + i);
	};
	
	for( uint8_t i=0; i<KeysQwerty.size(); i++ ){
		KeysQwerty[i] = pgm_read_byte(qwerty + i);
	};
	

};

void Keyboard::Draw()
{	
	for( uint8_t i=0; i<Keys.size(); i++ ){
		uint8_t x = 0;
		uint8_t y = 0;
		bool vis = false;
		
		x = Keys[i].x_point;
		y = Keys[i].y_point;
		vis = Keys[i].visible;
		
		if( vis ){
			Lcd_KS0108.GoToXY(x,y);
			
			uint8_t z = Keys[i].funcName[0] - 32;
			for(uint8_t i=0; i<5; i++){
				Lcd_KS0108.WriteData( pgm_read_byte( font5x7 + (z*5) + i ));
			};
			
			Lcd_KS0108.WriteData(0x00);
		};
		
	};


};

void Keyboard::LoadGroup( array< uint8_t >& group )
{
	for(uint8_t i=0; i<group.size()-1; i++){
		
		for(uint8_t k=0; k<Keys.size(); k++){
			
			if( group[i] == Keys[k].buttonID ){
				Keys[k].visible = true;
			};
			
		};
		
	};
	uint8_t t = group.size()-1;
	loaded |= (1 << group[ t ]);
};

void Keyboard::UnloadGroup( array< uint8_t >& group )
{
	for(uint8_t i=0; i<group.size()-1; i++){
		
		for(uint8_t k=0; k<Keys.size(); k++){
			
			if( group[i] == Keys[k].buttonID ){
				Keys[k].visible = false;
			};
			
		};
		
	};
	uint8_t t = group.size()-1;
	loaded &=~(1 << group[ t ]);
};

uint8_t Keyboard::Readkey(void)
{
	Touch.ReadCoordinates();
	
	uint8_t x = Touch.x / 4;
	uint8_t y = Touch.y / 4;
	
	if( loaded & (1 << basicKeysGroupID) ){	// basicKeys
		for( uint8_t i=0; i<KeysBasic.size() - 1; i++ ){
			
			uint8_t lx = Keys[ KeysBasic[i] ].low_x;
			uint8_t ly = Keys[ KeysBasic[i] ].low_y;
			uint8_t hx = Keys[ KeysBasic[i] ].high_x;
			uint8_t hy = Keys[ KeysBasic[i] ].high_y;
			
			if( (x >= lx) && (x < hx) && (y>=ly) && (y<hy) ){
				return KeysBasic[i];
			};
		};
	};
	
	if( loaded & (1 << sinKeysGroupID) ){	// sinKeys
		for( uint8_t i=0; i<KeysSin.size() - 1; i++ ){
			uint8_t lx = Keys[ KeysSin[i] ].low_x;
			uint8_t ly = Keys[ KeysSin[i] ].low_y;
			uint8_t hx = Keys[ KeysSin[i] ].high_x;
			uint8_t hy = Keys[ KeysSin[i] ].high_y;
			
			if( (x >= lx) && (x < hx) && (y>=ly) && (y<hy) ){
				return KeysSin[i];
			};
		};
	};
	
	if( loaded & (1 << logKeysGroupID) ){	// logKeys
		for( uint8_t i=0; i<KeysLog.size() - 1; i++ ){
			uint8_t lx = Keys[ KeysLog[i] ].low_x;
			uint8_t ly = Keys[ KeysLog[i] ].low_y;
			uint8_t hx = Keys[ KeysLog[i] ].high_x;
			uint8_t hy = Keys[ KeysLog[i] ].high_y;
			
			if( (x >= lx) && (x < hx) && (y>=ly) && (y<hy) ){
				return KeysLog[i];
			};
		};
	};
	
	if( loaded & (1 << numKeysGroupID) ){	// numKeys
		for( uint8_t i=0; i<KeysNum.size() - 1; i++ ){
			uint8_t lx = Keys[ KeysNum[i] ].low_x;
			uint8_t ly = Keys[ KeysNum[i] ].low_y;
			uint8_t hx = Keys[ KeysNum[i] ].high_x;
			uint8_t hy = Keys[ KeysNum[i] ].high_y;
			
			if( (x >= lx) && (x < hx) && (y>=ly) && (y<hy) ){
				return KeysNum[i];
			};
		};
	};
	
	if( loaded & (1 << naviKeysGroupID) ){	// naviKeys
		for( uint8_t i=0; i<KeysNavi.size() - 1; i++ ){
			uint8_t lx = Keys[ KeysNavi[i] ].low_x;
			uint8_t ly = Keys[ KeysNavi[i] ].low_y;
			uint8_t hx = Keys[ KeysNavi[i] ].high_x;
			uint8_t hy = Keys[ KeysNavi[i] ].high_y;
			
			if( (x >= lx) && (x < hx) && (y>=ly) && (y<hy) ){
				return KeysNavi[i];
			};
		};
	};
	
	if( loaded & (1 << qwertyKeysGroupID) ){	// Qwerty
		for( uint8_t i=0; i<KeysQwerty.size() - 1; i++ ){
			uint8_t lx = Keys[ KeysQwerty[i] ].low_x;
			uint8_t ly = Keys[ KeysQwerty[i] ].low_y;
			uint8_t hx = Keys[ KeysQwerty[i] ].high_x;
			uint8_t hy = Keys[ KeysQwerty[i] ].high_y;
			
			if( (x >= lx) && (x < hx) && (y>=ly) && (y<hy) ){
				return KeysQwerty[i];
			};
		};
	};
	
	return 255;
};