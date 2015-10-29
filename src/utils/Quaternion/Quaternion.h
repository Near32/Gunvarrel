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


Quat Qt_Mul(Quat qL, Quat qR);
Quat Euler2Qt(float roll, float pitch, float yaw);
void Qt2Euler( const Quat& q, float* roll, float* pitch, float* yaw);
Mat<float> Euler2Rot( const float& roll, const float& pitch, const float& yaw);
void Rot2Euler( const Mat<float>& R, Mat<float>& angles);
void Qt_ToMatrix(Quat q, HMatrix mat);
template<typename T>
void Qt_ToMatrix(Quat q,Mat<T>* mat);
Quat Qt_FromMatrix(const HMatrix& mat);
template<typename T>
Quat Qt_FromMat(const Mat<T>& mat);
template<typename T>
Quat Qt_FromNVect(Mat<T> vect);
template<typename T>
Quat Mat2Qt(Mat<T> vect);
template<typename T>
Mat<T> Qt2Mat(Quat q);





#endif


