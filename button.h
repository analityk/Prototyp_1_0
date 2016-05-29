#ifndef button_h__
#define button_h__

#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <point.h>
#include <font5x7.h>
#include <small_graph.h>
#include <array.h>

class button {
public:
	
	button(){};
	void Draw(const char* txt, point P);
	
	~button(){};
};

#endif // button_h__
