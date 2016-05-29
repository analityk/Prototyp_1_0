#include <touch.h>

touch Touch;

uint16_t touch::rescale_x(uint16_t x)
{
	if( x > 1024 || x < 20 )return 0;
	uint16_t tmp = x * 32;
	if( tmp > 16384 ){
		// dodawac
		uint16_t dx = tmp - 16384;
		uint16_t div_p2 = 6*(dx / 10);
		uint16_t div_p3 = 6*(dx / 100);
		tmp += (div_p2 + div_p3);
		}else{
		// odejmowac
		uint16_t dx = 16384 - tmp;
		uint16_t div_p2 = 6*(dx / 10);
		uint16_t div_p3 = 1*(dx / 100);
		tmp -= (div_p2 + div_p3);
	};

	return (tmp / 32); 
};

uint16_t touch::rescale_y(uint16_t y)
{
	if( y > 1024 || y < 20 )return 0;
	uint16_t tmp = y * 32;
	if( tmp > 16384 ){
		// dodawac
		uint16_t dy = tmp - 16384;
		uint16_t div_p2 = 2*(dy / 10);
		uint16_t div_p3 = 1*(dy / 100);
		tmp += (div_p2 + div_p3);
		}else{
		// odejmowac
		uint16_t dy = 16384 - tmp;
		uint16_t div_p2 = 1*(dy / 10);
		uint16_t div_p3 = 9*(dy / 100);
		tmp -= (div_p2 + div_p3);
	};
	
	return (tmp / 32); 
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
		tx[i] = Adc.voltage_X();
		ty[i] = Adc.voltage_Y();
	};
	
	uint16_t avx = 0; 
	uint16_t avy = 0;
	
	for(uint8_t i=0; i<5; i++){
		avx += tx[i];
		avy += ty[i];
	};
	
	avx /= 5;
	avy /= 5;
	
	
	this->y = rescale_x(avx);
	this->x = rescale_y(avy);
	
}
