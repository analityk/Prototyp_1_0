#ifndef program_states_h__
#define program_states_h__

#include <avr/io.h>
#include <avr/pgmspace.h>

typedef volatile uint8_t Program_state;

Program_state _ps_act = 0;
Program_state _ps_prev = 0;
Program_state _next = 0;

#define PS_MAIN_VIEV	1	// set on start user program
#define PS_TEXT_EDIT	2	//
#define PS_TEXT_SMALL	3
#define PS_TEXT_BIG		4
#define PS_TEXT_EDIT_END	5

#endif // program_states_h__
