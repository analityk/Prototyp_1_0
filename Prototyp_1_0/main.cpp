/*
 * Prototyp_1_0.cpp
 *
 * Created: 2016-02-27 12:07:23
 * Author : Szymon
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <sram.h>
#include <array.h>
#include <usart.h>
#include <lcd.h>
#include <text.h>
#include <delay.h>
#include <adc.h>
#include <keyboard.h>
#include <touch.h>
#include <spi.h>
#include <at45db321.h>

void delay_milis(uint8_t milis){
	startMillis();
	delay_ms(milis);
	stopMillis();
};

void delay_s(uint8_t s){
	for(uint8_t i=0; i<s; i++){
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
	};
};

uint8_t cord[4];

void convert(uint16_t t){
	uint16_t a = t;
	uint16_t div = 1000;
	for(uint8_t i=4; i>0; i--){
		cord[i-1] = a / div;
		a -= (cord[i-1] * div);
		cord[i-1] += 48;
		div /= 10;
	};
};

void print(uint8_t* t, uint8_t x, uint8_t y){
	for(uint8_t i=4; i>0; i--){
		Text.GoTo(x++, y);
		Text.WriteChar(cord[i-1]);
	};
};

//ISR(USART0_RX_vect){};

int main(void)
{
	cli();
	
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	//sei();
	
	uint16_t ram_alloc[3];
	
	uint8_t tab[10];
	
	Serial.Enable();
	
	ram_alloc[0] = ram.get_mem(12);
	ram_alloc[1] = ram.get_mem(12);
	ram_alloc[2] = ram.get_mem(12);
	
	for(uint8_t i=0; i<10; i++){
		tab[i] = i;
	};
	ram.write_block(ram_alloc[0], 0, 10, tab);
	
	for(uint8_t i=0; i<10; i++){
		tab[i] = 2*i;
	};
	ram.write_block(ram_alloc[1], 0, 10, tab);
	
	for(uint8_t i=0; i<10; i++){
		tab[i] = 3*i;
	};
	ram.write_block(ram_alloc[2], 0, 10, tab);
	

	while(1){
		for(uint8_t j=0; j<3; j++){
			
			for(uint8_t i=0; i<10; i++){
				tab[i] = ram.read_mem(ram_alloc[j], i);
				Serial.write(tab[i]);
			};
			delay_s(2);
		};
		delay_s(10);
	};
};