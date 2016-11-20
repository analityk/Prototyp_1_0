#ifndef onp_h__
#define onp_h__

#include <avr/io.h>
#include <array.h>
#include <math.h>

#define ONP_CT_DOUBLE	1
#define ONP_CT_BRACKET	2
#define ONP_CT_OPERATOR	3
#define ONP_CT_AZ		4
#define ONP_CT_az		5
#define ONP_CT_FUNCT	6

#define ONP_ST_DIGIT	1
#define ONP_ST_DOT		2
#define ONP_ST_OPER		3
#define ONP_ST_AZ		4
#define ONP_ST_az		5
#define ONP_ST_BRACKET	6

typedef struct {
	union {
		uint8_t data_tab[4];
		double data_double;
	}Udata;
	uint8_t content_type;
}Token;


uint8_t onp_tokenizer(array<uint8_t>& input);
void token_get(Token* t, uint8_t i);

#endif // onp_h__
