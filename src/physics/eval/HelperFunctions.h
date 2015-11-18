#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include "../RigidBody.h"

Mat<float> closestPointLOfBOXGivenPointL(const RigidBody& rb, const Mat<float>& pointL)
{
	BoxShape& box = rb.getShapeReference();
	Mat<float> min((float)0,3,1);
	Mat<float> max((float)0,3,1);
	
	min.set( -box.getHeight()/2.0f, 1,1);
	min.set( -box.getWidth()/2.0f, 2,1);
	min.set( -box.getDepth()/2.0f, 3,1);
	max.set( -min.get(1,1), 1,1);
	max.set( -min.get(2,1), 2,1);
	max.set( -min.get(3,1), 3,1);
	
	
	Mat<float> clpL(3,1);
	//compute the closest point by projecting the point to the closest face using Voronoi regions approach.
	
	for(int i=1;i<=3;i++)
	{
		float v = pointL.get(i,1);
		if( v < min.get(i,1))	v = min.get(i,1);
		if( v > max.get(i,1))	v = max.get(i,1);
		clpL.set( v, i,1);
	}
	
	return clpL;
}  

Mat<float> closestPointWOfBOXgivenPointL(const RigidBody& rb, const Mat<float>& pointL)
{
	Mat<float> clpL( closestPointLOfBOXGivenPointL( rb, pointL) );
	return rb.getPointInWorld( clpL);
}

Mat<float> closestPointWOfBOXgivenPointW(const RigidBody& rb, const Mat<float>& pointW)
{
	return closestPointWOfBOXGivenPointL( rb, rb.getPointInLocal(pointW) ) ;
}


bool testOBBPlane( const RigidBody& plane, const RigidBody& box)
{
	BoxShape boxs = box.getShapeReference();
	float d = plane.getShapeReference().getDistance();
	Mat<float> n(plane.getShapeReference().getNormal());
	
	Mat<float> e((float)0,3,3);
	for(int i=1;i<=3;i++)	e.set(1.0f, i,i);
	//identity matrix to be rotated in order to have the basis related to the coordinate frame of the box :
	e = extract( box.getTransformation(), 1,1, 3,3) * e;
	
	float r = boxs.getHeight()*abs_f( dotProduct( Cola(e,1), n) ) + boxs.getWidth()*abs_f( dotProduct( Cola(e,2), n) ) + boxs.getDepth()*abs_f( dotProduct( Cola(e,3), n) );
	
	float s = dotProduct( n, box.getPosition()) - d;
	
	return abs_f(s) <= r;
	
}

bool testOBBOBB( const RigidBody& b1, const RigidBody& b2)
{
	Mat<float> pointsB1(3,8);
	//register the position of the 8 points composing b1:
	Mat<float> min1((float)0,3,1);
	Mat<float> max1((float)0,3,1);
	
	min1.set( -box.getHeight()/2.0f, 1,1);
	min1.set( -box.getWidth()/2.0f, 2,1);
	min1.set( -box.getDepth()/2.0f, 3,1);
	max1.set( -min1.get(1,1), 1,1);
	max1.set( -min1.get(2,1), 2,1);
	max1.set( -min1.get(3,1), 3,1);
	
	int col = 1;
	
	for(int pm1=1;pm1--;)
	{
		for(int pm2=1;pm2--;)
		{
			for(int pm3=1;pm3--;)
			{
				pointsB1.set( pm1*( min1.get(1,1) ) + (1-pm1)*( max1.get(1,1) ), 1,col);
				pointsB1.set( pm2*( min1.get(2,1) ) + (1-pm2)*( max1.get(2,1) ), 2,col);
				pointsB1.set( pm3*( min1.get(3,1) ) + (1-pm3)*( max1.get(3,1) ), 3,col);
				
				col++;
			}
		}
	}
	
	
}



#endif
