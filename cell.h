#ifndef cell_h__
#define cell_h__

#include <avr/io.h>
#include <array.h>
#include <sram.h>
#include <text.h>
#include <delay.h>

#define CELL_START_EXPR		0
#define CELL_SIZE_EXPR		50

#define CELL_START_RESULT	51
#define CELL_SIZE_RESULT	4

#define CELL_START_REF		55
#define CELL_SIZE_REF		30
#define CELL_SIZE_ALLOC		(CELL_SIZE_EXPR+CELL_SIZE_REF+CELL_SIZE_RESULT)

class Cell{
private:
	ram_grip grip;
	
public:
	
	Cell(){
		array<uint8_t> tmp(CELL_SIZE_ALLOC);
		tmp.erase();
		grip = ram.get_mem(CELL_SIZE_ALLOC);
		ram.write_block(grip, 0, CELL_SIZE_ALLOC, tmp.data);
				
		if( grip == 0 ){
			Text.GoTo(5, 5);
			char c[5];
			itoa(grip, c, 10);
			Text.Write(c);
			Text.GoTo(0,0);
			Text.Write("External ram error with grip n.");
			Text.GoTo(0,1);
			Text.Write(grip);
			while(1){};
		};
	};
	
	~Cell(){};

	void SaveExpression(array< uint8_t >& expr);
	void SaveResult(double* result);
	void SaveReference(array<uint8_t>& ref);
	
	void GetExpression(array< uint8_t >& expr);
	void GetResult(double* result);
	void GetReference(array<uint8_t>& ref);
	void GetViev(char* r);
	
};

extern array<Cell> cells;

#endif // cell_h__
