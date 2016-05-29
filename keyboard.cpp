#include <keyboard.h>

char empty_tab[] = {0,0,0,0};

#define BYTES_PER_CTRL 12

PROGMEM	const uint8_t keymem[] = {

	/*	!	*/	33,	2,	0x21,	0,		0,		0,		4,		0, 0,0,0,0,
	/*	%	*/	37,	2,	0x25,	0,		0,		0,		16,		0, 0,0,0,0,
	/*	&	*/	38,	2,	0x26,	0,		0,		0,		28,		0, 0,0,0,0,
	/*	(	*/	40,	2,	0x28,	0,		0,		0,		64,		0,	120, 144, 10, 35,
	/*	)	*/	41,	2,	0x29,	0,		0,		0,		78,		0,	145, 172, 10, 35,
	/*	*	*/	42,	2,	0x2A,	0,		0,		0,		28,		0,	50, 72, 10, 35,
	/*	+	*/	43,	2,	0x2B,	0,		0,		0,		4,		0,	6,  28, 10, 35,
	/*	-	*/	45,	2,	0x2D,	0,		0,		0,		16,		0,	29, 49, 10, 35,
	/*	.	*/	46,	2,	0x2E,	0,		0,		0,		52,		0,	96, 119, 10, 35,
	/*	/	*/	47,  2,	0x2F,	0,		0,		0,		40,		0,	73, 95, 10, 35,
	/*	0	*/	48, 2,	0x30,	0,		0,		0,		4,		7,	6, 28, 210, 240,
	/*	1	*/	49, 2,	0x31,	0,		0,		0,		16,		7,	29, 50, 210, 240,
	/*	2	*/	50, 2,	0x32,	0,		0,		0,		28,		7,	51, 72, 213, 240,
	/*	3	*/	51, 2,	0x33,	0,		0,		0,		40,		7,	72, 96, 212, 239,
	/*	4	*/	52, 2,	0x34,	0,		0,		0,		52,		7,	96, 118, 214, 240,
	/*	5	*/	53, 2,	0x35,	0,		0,		0,		64,		7,	119, 142, 213, 240,
	/*	6	*/	54, 2,	0x36,	0,		0,		0,		76,		7,	143, 166, 215, 241,
	/*	7	*/	55, 2,	0x37,	0,		0,		0,		88,		7,	167, 191, 214, 241,
	/*	8	*/	56, 2,	0x38,	0,		0,		0,		100,	7,	192, 215, 213, 242,
	/*	9	*/	57, 2,	0x39,	0,		0,		0,		112,	7,	216, 241, 215, 243,
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

};

void Keyboard::Draw()
{
	
	for( uint8_t i=0; i<Keys.size(); i++){
		if( Keys[i].visible != true ){
			b.Draw( empty_tab, Point.Set( Keys[i].x_point, Keys[i].y_point) );
		};
	};
	
	for( uint8_t i=0; i<Keys.size(); i++){
		if( Keys[i].visible == true ){
			b.Draw( Keys[i].funcName, Point.Set( Keys[i].x_point, Keys[i].y_point) );
		};
	};
};

void Keyboard::DrawVisible()
{
	for( uint8_t i=0; i<Keys.size(); i++){
		if( Keys[i].visible == true ){
			b.Draw( Keys[i].funcName, Point.Set( Keys[i].x_point, Keys[i].y_point) );
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
	
	return 255;
};