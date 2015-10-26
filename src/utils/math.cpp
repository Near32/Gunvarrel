#include "math.h"

//------------------------------------
//		Identity Matrix 3x3 :
//------------------------------------
float id3_val_array[9] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
Mat<float> Identity3( id3_val_array,3,3);

float xaxis_val_array[3] = {1.0f, 0.0f, 0.0f};
Mat<float> XAxis( xaxis_val_array,3,1);

float yaxis_val_array[3] = {0.0f, 1.0f, 0.0f};
Mat<float> YAxis( yaxis_val_array,3,1);

float zaxis_val_array[3] = {0.0f, 0.0f, 1.0f};
Mat<float> ZAxis( zaxis_val_array,3,1);
//------------------------------------
//------------------------------------
float EPSILON = (float)1e-100;
float INF = (float)1e100;

//------------------------------------
//------------------------------------


//------------------------------------
//------------------------------------

se3::se3() : t( Mat<float>((float)0,3,1) ), w( Mat<float>((float)0,3,1) ), SE3( Mat<float>((float)0, 4,4) ), hasChanged(true)	//so that SE3 would be computed on the first access...
{

}

se3::se3(const Mat<float>& w_, const Mat<float>& t_) : t(t_), w(w_), SE3(Mat<float>((float)0, 4,4) ), hasChanged(true)	//so that SE3 would be computed on the first access...
{

}

se3::se3(const float* w_t_array) : SE3( Mat<float>((float)0,4,4) ), hasChanged(true)
{
	this->w.set( w_t_array[0], 1,1);
	this->w.set( w_t_array[1], 2,1);
	this->w.set( w_t_array[2], 3,1);
	
	this->t.set( w_t_array[3], 1,1);
	this->t.set( w_t_array[4], 2,1);
	this->t.set( w_t_array[5], 3,1);
}
	
se3::~se3()
{

}
	
Mat<float> se3::exp()					//compute the (R | t) matrix.
{
	if(hasChanged)
	{
		SE3 = expM( operatorC( w, t) );
	}
	
	return SE3;
}

void se3::setT(const Mat<float>& t_)
{
	if(t_.getLine() == 3 && t_.getColumn() == 1)
	{
		this->t = t_;
		hasChanged = true;
	}
}

void se3::setW(const Mat<float>& w_)
{
	if(w_.getLine() == 3 && w_.getColumn() == 1)
	{
		this->w = w_;
		hasChanged = true;
	}
}
