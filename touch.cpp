#include <touch.h>

uint16_t touch::rescale_x(uint16_t x)
{
	float dx;
	
	if( x > 505 ){
		dx = 505 + 1.2*(x-505);
		}else{
		dx = 505 - 1.18*(505-x);
	};
	return (uint16_t)(dx);
};

uint16_t touch::rescale_y(uint16_t y)
{
	float dy;
		
	if( y > 505 ){
		dy = 500 + 1.67*(y-505);
		}else{
		dy = 500 - 1.67*(505-y);
	};
	return (uint16_t)(dy);
}

void touch::KeyPressed(void)
{
	while( ( Adc.current_X() + Adc.current_Y() ) < 0x0500 ){};
}

bool touch::Press(void)
{
	if( Adc.current_X() + Adc.current_Y() < 0x0500 ){
		return true;
	};
return false;
}

void touch::ReadCoordinates(void)
{
	uint16_t tx[5];
	uint16_t ty[5];
	
	KeyPressed();
	
	for(uint8_t i=0; i<5; i++){
		tx[i] = Adc.voltage_Y();
		ty[i] = Adc.voltage_X();
	};
	
	uint16_t avx = 0; 
	uint16_t avy = 0;
	
	for(uint8_t i=0; i<5; i++){
		avx += tx[i];
		avy += ty[i];
	};
	
	avx /= 5;
	avy /= 5;
	
	
	y = rescale_y(avy);
	x = rescale_x(avx);
	
};

touch Touch;