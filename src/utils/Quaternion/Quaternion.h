#ifndef QUAT_H
#define QUAT_H

#include "../Mat/Mat.h"

class Quat
{	
	public : 
	
	float x;
	float y;
	float z;
	float w;
				
	Quat(float x_, float y_, float z_, float w_) : x(x_),y(y_),z(z_),w(w_) {}
	Quat() : x(0.0f),y(0.0f),z(0.0f),w(1.0f) {}
	
	~Quat()	{}
	
};


typedef float HMatrix[4][4];
//#define X 0
//#define Y 1
//#define Z 2
//#define W 3



#endif


