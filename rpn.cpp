#include <rpn.h>

uint8_t tab2f[20];

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

bool is_bracket(uint8_t d){
	if( (d=='(') || (d==')') ){
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
	
	if( is_bracket(r) ){
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

void clr_mem_cnt(uint8_t* t, uint8_t cnt){
	for( uint8_t i=0; i<cnt; i++ ){
		t[i] = 0;
	};
};

void RPN::set_priority(Token* t)
{
	if( t->content_type == ONP_CT_BRACKET ){
		t->prior_lvl = ONP_PL_BRACKET;
	};
	
	if( t->content_type == ONP_CT_OPERATOR ){
		uint8_t symbol = t->Udata.data_tab[0];
		switch(symbol){
			case '+':{
				t->prior_lvl = ONP_PL_ADD;
				break;
			};
			case '-':{
				t->prior_lvl = ONP_PL_SUB;
				break;
			};
			case '*':{
				t->prior_lvl = ONP_PL_MUL;
				break;
			};
			case '/':{
				t->prior_lvl = ONP_PL_DIV;
				break;
			};
			case '^':{
				t->prior_lvl = ONP_PL_POW;
				break;
			};
			default:break;
		};
	};
	
	if( t->content_type == ONP_CT_FUNCT ){
		t->prior_lvl = ONP_PL_FUNC;
	};
};

uint8_t RPN::infix_to_postfix(void)
{
	Token t;
	array<Token> out(30);
	array<Token> stack(20);
	
	for( uint8_t i=0; i<tkcnt; i++ ){
		get_token(&t, i);
		set_priority(&t);
		
		switch(t.content_type){
			
			case ONP_CT_DOUBLE : {
				out.insert(t);
				break;
			};
			
			case ONP_CT_BRACKET : {
				if( t.Udata.data_tab[0] == '(' ){
					stack.insert(t);
				}else{
					for( uint8_t i=0; i<30; i++ ){
						if( stack.read_last().Udata.data_tab[0] == '(' ){
							stack.remove_last();
							break;
						};
						out.insert( stack.read_last() );
						stack.remove_last();
					};
				};
				break;
			};
			
			case ONP_CT_FUNCT:{
				stack.insert(t);
				break;
			};
			
			case ONP_CT_OPERATOR : {
				if( (stack.cnts() == 0)||(stack.read_last().Udata.data_tab[0] == '(' ) ){
					stack.insert(t);
				}else{
					if( t.prior_lvl > stack.read_last().prior_lvl ){
						stack.insert(t);
					}else{
						for( uint8_t i=0; i<30; i++ ){
							out.insert( stack.read_last() );
							stack.remove_last();
							if( stack.cnts() == 0 )break;
							if( t.prior_lvl == stack.read_last().prior_lvl )break;
							if( stack.read_last().Udata.data_tab[0] == '(' )break;
						};
						stack.insert(t);
					};
				};
				break;
			};
			default:break;
		};
	};
	
	while( stack.cnts() > 0 ){
		out.insert( stack.read_last() );
		stack.remove_last();
	};
	
	
	for( uint8_t i=0; i<tkcnt; i++ ){
		tokens[i] = out[i];
	};
	
	return 0;
};

uint8_t RPN::tokenizer(array<uint8_t>& r)
{
	if( r.cnts() <= 1 ){
		// only one symbol 
		if( is_digit(r[0])){
			double d = 0.0;
			uint8_t t[5];
			dtostrf(d, 5, 5, (char*)(t) );
			tokens[0].content_type = ONP_CT_DOUBLE;
			tokens[0].Udata.data_double = d;
			return 1;
		};
	};
	
	clr_mem_cnt(tab2f, 10);
	
	uint8_t tokcnt = 0;
	uint8_t tcnt = 0;
	uint8_t fcnt = 0;
		
	uint8_t a = 0;
	uint8_t b = sym_type(r[0]);
		
	for( uint8_t i = 0; i < r.cnts(); i++ ){
		a = b;
		b = sym_type(r[i+1]);
			
		if( (a == ONP_ST_DIGIT) || (a == ONP_ST_DOT) ){
			tab2f[tcnt] = r[i];
			tcnt++;
				
			// następny nie jest cyfrą i nie jest kropką
			// next is not digit and is not dot
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
	this->tkcnt = tokcnt;
	return tokcnt;
};

void RPN::get_token(Token* t, uint8_t i)
{
	*t = tokens[i];
};

RPN rpn;