#ifndef keyboard_h__
#define keyboard_h__

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <font5x7.h>
#include <text.h>
#include <array.h>
#include <touch.h>

const uint8_t known_keys_cnt = 63;

PROGMEM const uint8_t qwerty[] = {'q','w','e','r','t','y','u','i','o','p','a','s','d','f','g','h','j','k','l','z','x','c','v','b','n','m','0','1','2','3','4','5','6','7','8','9', 5};
const uint8_t qwertyKeysGroupID = 5;
const uint8_t qwertyKeys_cnt = 37;

PROGMEM const uint8_t basicKeys[]	= { 43, 45, 42, 47, 46, 40, 41, 250, 61, 0 };
const uint8_t basicKeysGroupID = 0;
const uint8_t basicKeys_cnt = 10;

PROGMEM const uint8_t sinKeys[]		= { 162, 163, 164, 250, 61, 1 };
const uint8_t sinKeysGroupID = 1;
const uint8_t sinKeys_cnt = 6;

PROGMEM const uint8_t logKeys[]		= { 160, 161, 166, 250, 61, 2 };
const uint8_t logKeysGroupID = 2;
const uint8_t logKeys_cnt = 6;

PROGMEM const uint8_t numKeys[]		= { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 3 };
const uint8_t numKeysGroupID = 3;
const uint8_t numKeys_cnt = 11;

PROGMEM const uint8_t naviKeys[]	= { 130, 131, 132, 133, 134, 135, 136, 137, 4 };
const uint8_t naviKeysGroupID = 4;
const uint8_t naviKeys_cnt = 9;

class CKey{
	public:
	char* funcName;
	uint8_t fn_size;
	uint8_t buttonID;
	uint8_t x_point;
	uint8_t y_point;
	uint8_t low_x;
	uint8_t low_y;
	uint8_t high_x;
	uint8_t high_y;
	bool visible;
	
	CKey()
	{
		funcName = (char*)malloc(4);
		for(uint8_t i=0; i<4; i++){
			funcName[i] = 0;
		};
	};
	
	~CKey(){};
	
};

class Keyboard{
	private:
	uint8_t loaded;
	public:

	array< CKey > Keys;
	array< uint8_t > KeysBasic;
	array< uint8_t > KeysSin;
	array< uint8_t > KeysLog;
	array< uint8_t > KeysNum;
	array< uint8_t > KeysNavi;
	array< uint8_t > KeysQwerty;

	Keyboard():
	loaded(0),
	Keys(known_keys_cnt),
	KeysBasic(basicKeys_cnt),
	KeysSin(sinKeys_cnt),
	KeysLog(logKeys_cnt),
	KeysNum(numKeys_cnt),
	KeysNavi(naviKeys_cnt),
	KeysQwerty(qwertyKeys_cnt)
	{};
	
	void Init();
	void Draw();
	void DrawVisible();
	void LoadGroup( array< uint8_t >& group );
	void UnloadGroup( array< uint8_t >& group );
	uint8_t Readkey(void);
};

#endif // keyboard_h__