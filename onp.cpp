#include <onp.h>

//typedef struct {
	//union {
		//uint8_t data_tab[4];
		//double data_double;
	//}Udata;
	//uint8_t content_type;
//}Token;

//#define ONP_CT_DOUBLE	1
//#define ONP_CT_BRACKET	2
//#define ONP_CT_OPERATOR	3
//#define ONP_CT_AZ		4 
//#define ONP_CT_az		5
//#define ONP_CT_FUNCT	6
//
//#define ONP_ST_DIGIT	1
//#define ONP_ST_DOT		2
//#define ONP_ST_OPER		3
//#define ONP_ST_AZ		4
//#define ONP_ST_az		5
//#define ONP_ST_BRACKET	6

Token tokens[24];

uint8_t tab2f[10];

uint8_t check_syntax(uint8_t* token_digit){
	return 0; // 0 if ok, otherwise return position with not expected char
};

bool is_operator(uint8_t d){
	if( (d=='+')||(d=='-')||(d=='*')||(d=='/')||(d=='^')||(d=='%')||(d=='~')||(d==':')||(d=='!')||(d=='|') ){
		return true;
	};
	return false;
};

bool is_digit(uint8_t d){
	if( (d>47)&&(d<58) ){
		return true;
	};
	return false;
};

bool is_any_bracket(uint8_t d){
	if( (d == '[') || (d == ']') || (d=='(') || (d==')') ){
		return true;
	};
	return false;
};

bool is_dot(uint8_t d)
{
	if( d == 46 ){
		return true;
	};
	return false;
};

bool is_AZ(uint8_t d){
	if( (d>=65)&&(d<=90) ){
		return true;
	};
	return false;
};

bool is_az(uint8_t d){
	if( (d>=97)&&(d<=122) ){
		return true;
	};
	return false;
};

//		sin(1.23)+cos(45.67)
//		123+sin(40)
//		(123+(sin(30))+2)*3

uint8_t sym_type(uint8_t r){
	if( is_digit(r) ){
		return ONP_ST_DIGIT;
	};
	
	if( is_dot(r) ){
		return ONP_ST_DOT;
	};
	
	if( is_operator(r) ){
		return ONP_ST_OPER;
	};
	
	if( is_any_bracket(r) ){
		return ONP_ST_BRACKET;
	};
	
	if( is_AZ(r) ){
		return ONP_ST_AZ;
	};
	
	if( is_az(r) ){
		return ONP_ST_az;
	};
	return 0;
};

void clr_mem(uint8_t* t){
	for( uint8_t i=0; i<255; i++ ){
		if( t[i] == 0 )break;
		t[i]=0;
	};
};

void clr_mem_cnt(uint8_t* t, uint8_t cnt){
	for( uint8_t i=0; i<cnt; i++ ){
		t[i] = 0;
	};
};

uint8_t onp_tokenizer(array<uint8_t>& r){
	if( r.cnts() <= 1 ){
		// tylko jeden znak
	};
	
	uint8_t tokcnt = 0;
	uint8_t tcnt = 0;
	uint8_t fcnt = 0;
	
	uint8_t a = 0;
	uint8_t b = sym_type(r[1]);
	
	// 12.3+(4*3.0)
	for( uint8_t i = 0; i < r.cnts(); i++ ){
		a = b;
		b = sym_type(r[i+1]);
		
		if( (a == ONP_ST_DIGIT) || (a == ONP_ST_DOT) ){
			tab2f[tcnt] = r[i];
			tcnt++;
			
			// następny nie jest cyfrą i nie jest kropką
			if( !((b == ONP_ST_DIGIT)||(b == ONP_ST_DOT)) ){
				if( !(tcnt == 0) ){
					tab2f[tcnt] = 0;
					tokens[tokcnt].Udata.data_double = atof((char*)(tab2f));
					tokens[tokcnt].content_type = ONP_CT_DOUBLE;
					clr_mem_cnt(tab2f, 10);
					tcnt = 0;
					tokcnt++;
				};
			};
		};
		
		if( a == ONP_ST_OPER ){
			tokens[tokcnt].Udata.data_tab[0] = r[i];
			tokens[tokcnt].Udata.data_tab[1] = 0;
			tokens[tokcnt].content_type = ONP_CT_OPERATOR;
			tokcnt++;
		};
		
		if( a == ONP_ST_BRACKET){
			tokens[tokcnt].Udata.data_tab[0] = r[i];
			tokens[tokcnt].Udata.data_tab[1] = 0;
			tokens[tokcnt].content_type = ONP_CT_BRACKET;
			tokcnt++;
		};
		
		if( (a==ONP_ST_AZ) || (a==ONP_ST_az) ){
			tokens[tokcnt].Udata.data_tab[fcnt] = r[i];
			fcnt++;
			
			if(  !(	 (b==ONP_ST_AZ) || (b==ONP_ST_az)  ) ){
				for( uint8_t l = fcnt; l<4; l++ ){
					tokens[tokcnt].Udata.data_tab[l] = 0;
				};
				fcnt = 0;
				tokens[tokcnt].content_type = ONP_CT_FUNCT;
				tokcnt++;
			};
		};
		
	};
	return tokcnt;
};

void token_get(Token* t, uint8_t i)
{
	*t = tokens[i];
};
