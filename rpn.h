#ifndef rpn_h__
#define rpn_h__

#include <avr/io.h>
#include <stdlib.h>
#include <array.h>

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

#define ONP_PL_BRACKET	0
#define ONP_PL_ADD	1
#define ONP_PL_SUB	1
#define ONP_PL_FUNC	5
#define ONP_PL_MUL	2
#define ONP_PL_DIV	2
#define ONP_PL_POW	4
#define ONP_PL_NEG	3

class Token{
public:
	union {
		uint8_t data_tab[4];
		double data_double;
	}Udata;
	uint8_t content_type;
	uint8_t prior_lvl;
	
	Token(){
		for( uint8_t i = 0; i<4; i++ ){
			Udata.data_tab[i] = 0;
		};
		content_type = 0;
		prior_lvl = 0;
	};
	
	~Token(){
		for( uint8_t i = 0; i<4; i++ ){
			Udata.data_tab[i] = 0;
		};
		content_type = 0;
		prior_lvl = 0;
	};
};

class RPN{
	private:
		void set_priority(Token* t);
		uint8_t tkcnt;
public:
	
	Token tokens[40];

	RPN():tkcnt(0)
	{};
	
	uint8_t infix_to_postfix(void);
	uint8_t tokenizer(array<uint8_t>& r);
	void get_token(Token* t, uint8_t i);	
};

extern RPN rpn;

#endif // rpn_h__
