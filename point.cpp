#include <point.h>

 point::point():x(0),y(0){};

 point::point(uint8_t px, uint8_t py):x(px),y(py){};
 
 
 point point::Set(uint8_t px, uint8_t py)
 {
	 this->x = px;
	 this->y = py;
	 return (*this);
 }
 
 point Point;