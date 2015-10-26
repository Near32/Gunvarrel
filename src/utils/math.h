#ifndef MATH_H
#define MATH_H

#include "Mat/Mat.h"
#include "MVG/MVG.h"
#include "Quaternion/Quaternion.h"

extern Mat<float> Identity3;

class se3
{
	private :
	
	Mat<float> w;		//rotational tangent values.
	Mat<float> t;		//translational values.
	Mat<float> SE3;		// (R | t) matrix.
	bool hasChanged;
	
	public :
	
	se3();
	se3(const Mat<float>& w_, const Mat<float>& t_);
	se3(const float* w_t_array);
	
	~se3();
	
	Mat<float> exp();					//compute the (R | t) matrix.
	void setT(const Mat<float>& t_);
	void setW(const Mat<float>& w_);
	

};


#endif
