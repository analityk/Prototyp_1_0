#include <touch.h>

uint16_t touch::rescale_x(uint16_t x)
{
	float dx;

	uint16_t medi = 445;
	float scale = 1.92;

	if( x > medi ){
		dx = medi + scale*(x-medi);
	}else{
		dx = medi - scale*(medi-x);
	};
	return (uint16_t)(dx);
};

uint16_t touch::rescale_y(uint16_t y)
{
	float dy;

	uint16_t medi = 540;
	float scale = 2.65;

	if( y > medi ){
		dy = medi + scale*(y-medi);
	}else{
		dy = medi - scale*(medi-y);
	};
	return (uint16_t)(dy);
}

void touch::wait_release_key(void)
{
	Touch.delay_keypressed();
	delay(0x10000);
};

void touch::KeyPressed(void)
{
	while( ( Adc.current_X() + Adc.current_Y() ) < 0x0520 ){};
}

bool touch::Press(void)
{
	if( Adc.current_X() + Adc.current_Y() < 0x0520 ){
		return true;
	};
return false;
}

void touch::ReadCoordinates(void)
{
	uint16_t tx[5];
	uint16_t ty[5];
	
	for(uint32_t t=0; t<40; t++){
		Press();//KeyPressed();
	};
	
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

void touch::delay_keypressed(void)
{
	while( !Touch.Press() ){};
};

uint8_t touch::ReadKey(uint8_t from)
{
	delay(0x2fFFFF);
	
	Touch.ReadCoordinates();
	Timer.IdleTime = 0;
	uint8_t r = touched( Touch.x/4, Touch.y/4, from );
	wait_release_key();
	return r;
};

touch Touch;