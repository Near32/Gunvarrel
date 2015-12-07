#ifndef IMOVEABLE_H
#define IMOVEABLE_H

#include "../utils/math.h"
#include <exception>

class IMoveable
{
	protected :
	
	se3* Pose;
	
	Mat<float>* LinearVelocity;
	Mat<float>* AngularVelocity;
	
//-----------------------------------------------------
//-----------------------------------------------------
	public :
	
	
	IMoveable();
	IMoveable( const se3& Pose_);													//Orientation has to be initialized from Pose.
	IMoveable( const se3& Pose_, const Mat<float>& Lvel, const Mat<float>& Avel);	//Orientation has to be initialized from Pose.
	
	virtual ~IMoveable();
	
	
	//---------------------------------------------------------
	//---------------------------------------------------------
	
		
	se3 getPose() const	
	{ 
		return *Pose;
	}
	
	Mat<float> getPosition()
	{
		return Pose->getT();
	}
		
	Quat getOrientation()		
	{	
		return Qt_FromMat( this->Pose->exp());
	}
	
	Mat<float> getMatOrientation()
	{
		return Qt2Mat<float>( this->getOrientation() );
	}
	
	Mat<float> getTransformation()	const
	{
		return this->Pose->exp();	
	}
	
	Mat<float> getLinearVelocity()	const
	{
		return *(this->LinearVelocity);
	}
	
	Mat<float> getAngularVelocity()	const
	{
		return *(this->AngularVelocity);
	}

	
	//---------------------------------------------------------
	//---------------------------------------------------------
	
	void setPose( const se3& pose_)	{	*Pose = pose_;	}
	void setPosition( const Mat<float>& t_)	{	Pose->setT(t_);	}
	void setOrientation( const Quat& q)	{	Pose->setOrientation(q);	}
	void setMatOrientation( const Mat<float>& q)	{	setOrientation( Mat2Qt<float>(q) );	}
	void setLinearVelocity( const Mat<float>& lvel)	{	*LinearVelocity = lvel;	}
	void setAngularVelocity( const Mat<float>& avel)	{	*AngularVelocity = avel;	}
	
};

#endif
