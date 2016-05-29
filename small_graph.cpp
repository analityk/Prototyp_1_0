#include <small_graph.h>

void small_graph::Draw(point left_up, uint8_t witdh, uint8_t* data)
{
	Lcd_KS0108.GoToXY(left_up.x, left_up.y);
	for(uint8_t i=0; i<witdh; i++){
		Lcd_KS0108.WriteData(data[i]);
	};
};
