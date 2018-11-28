#include <cell.h>

array<Cell> cells(100);

void Cell::SaveExpression(array< uint8_t >& expr)
{
	ram.write_block(grip, CELL_START_EXPR, CELL_SIZE_EXPR, expr.data);
};

void Cell::SaveResult(double* result)
{
	union{
		double d;
		uint8_t t[4];
	}cast;
	
	cast.d = *result;
		
	ram.write_block(grip, CELL_START_RESULT, CELL_SIZE_RESULT, cast.t);
	
};

void Cell::SaveReference(array<uint8_t>& ref)
{
	ram.write_block(grip, CELL_START_REF, CELL_SIZE_REF, ref.data );
};

void Cell::GetExpression(array< uint8_t >& expr)
{
	expr.erase();
	if( expr.size() < CELL_SIZE_EXPR )return;
	
	ram.read_block(grip, CELL_START_EXPR, CELL_SIZE_EXPR, expr.data);
	
	for( uint8_t i=0; i<CELL_SIZE_EXPR; i++ ){
		if( !(expr[i] == 0) ){
			expr.poz++;
		};
	};
};

void Cell::GetResult(double* result)
{
	union{
		double d;
		uint8_t t[4];
	}cast;
	
	ram.read_block(grip, CELL_START_RESULT, CELL_SIZE_RESULT, cast.t);
	
	*result =  cast.d;
};

void Cell::GetReference(array<uint8_t>& ref)
{
	ref.erase();
	ram.read_block(grip, CELL_START_REF, CELL_SIZE_REF, ref.data );
};

void Cell::GetViev(char* r)
{
	ram.read_block( grip, 0, 8, (uint8_t*)(r) );
};
