#ifndef main_loop_h__
#define main_loop_h__

#include <avr/io.h>
#include <lcd.h>
#include <array.h>
#include <cell.h>
#include <delay.h>
#include <timer.h>
#include <sram.h>
#include <text.h>
#include <touch.h>
#include <at45db321.h>
#include <program_states.h>
#include <keys.h>
#include <MainViev.h>
#include <TextBoxViev.h>
#include <rpn.h>

#define RAM_SIZE_INPUT	50
#define RAM_SIZE_RESULT	4
#define RAM_SIZE_REF	30
#define RAM_SIZE_ALLOC	(RAM_SIZE_INPUT+RAM_SIZE_REF+RAM_SIZE_RESULT)

#define CELLS_CNT	90

extern Program_state _ps_act;
extern Program_state _ps_prev;
extern Program_state _ps_next;

extern uint8_t TextBoxWindow[20];

void clr_ram_mem(void);
void clr_inp_str(void);
void clr_txt_box(void);
void clr_ram_buf(void);
void clr_ref_addr(void);

void clr_scr(void);

void fill_cells(void);

void FormatTime( uint8_t h, uint8_t m, uint8_t s );
void print_time(void);

void set_active_cells(uint8_t button);

void main_viev_action(uint8_t r);
void up_case_action(uint8_t r);

void calculate(void);

void print_edit_addr(uint8_t x, uint8_t y);
void print_ref_addr(uint8_t x, uint8_t y);

void loadCellString(void);
void storeCellString(void);

void show_input_cnts(void);
void show_gctor(uint16_t t);

void write_input_str(void);

void store_act_addr(void);
void load_act_addr(void);
void add_ref_addr(void);

#endif // main_loop_h__
