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
#define CELL_SIZE_ALLOC		(CELL_SIZE_EXPR + CELL_SIZE_REF + CELL_SIZE_RESULT)

class Cell{
public:
	ram_grip grip;
	Cell(){
		grip = ram.get_mem(CELL_SIZE_ALLOC);
		
		if( grip == 0 ){
			while(1){};
		};
		
		ram.clr_block(grip, CELL_SIZE_ALLOC);
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
