#ifndef rpn_h__
#define rpn_h__

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <array.h>


#define ONP_CT_UNDEF	0
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

#define ONP_F_UNDEF	0
#define ONP_F_SIN	1
#define ONP_F_COS	2
#define ONP_F_TAN	3
#define ONP_F_CTG	4
#define ONP_F_ASIN	5
#define ONP_F_ACOS	6
#define ONP_F_ATAN	7
#define ONP_F_ACTG	8
#define ONP_F_LN	9
#define ONP_F_LOG	10
#define ONP_F_EXP	11
#define ONP_F_LNX	12
#define ONP_F_ADD	13
#define ONP_F_SUB	14
#define ONP_F_MUL	15
#define ONP_F_DIV	16
#define ONP_F_POW	17
#define ONP_F_INT	18	// integral
#define ONP_F_DER	19  // derivative

#define ONP_PCNT_1	1
#define ONP_PCNT_2	2
#define ONP_PCNT_3	3

class Token{
public:
	union {
		uint8_t data_tab[4];
		double data_double;
	}Udata;
	uint8_t content_type;
	uint8_t prior_lvl;
	uint8_t func_type;
	uint8_t pcnt;
	
	Token(){
		for( uint8_t i = 0; i<4; i++ ){
			Udata.data_tab[i] = 0;
		};
		content_type = 0;
		prior_lvl = 0;
		func_type = 0;
		pcnt = 0;
	};
	
	~Token(){
		for( uint8_t i = 0; i<4; i++ ){
			Udata.data_tab[i] = 0;
		};
		content_type = 0;
		prior_lvl = 0;
		func_type = 0;
		pcnt = 0;
	};
};

class RPN{
	private:
		void set_priority(Token* t);
		uint8_t tkcnt;
public:
	double results;
	Token tokens[40];

	RPN():tkcnt(0),results(0.0)
	{};
	
	double infix_to_postfix(array<uint8_t>& r);
	
	uint8_t tokenizer(array<uint8_t>& r);
	
	void get_token(Token* t, uint8_t i); 
};

extern RPN rpn;

#endif // rpn_h__
