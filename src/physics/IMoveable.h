#pragma once

#include "../utils/math.h"

class IMoveable
{
	protected :
	
	se3 Pose;
	
	Mat<float> LinearVelocity;
	Mat<float> AngularVelocity;
	
//-----------------------------------------------------
//-----------------------------------------------------
	public :
	
	
	IMoveable();
	IMoveable( const se3& Pose_);													//Orientation has to be initialized from Pose.
	IMoveable( const se3& Pose_, const Mat<float>& Lvel, const Mat<float>& Avel);	//Orientation has to be initialized from Pose.
	
	~IMoveable()
	{
	
	}
	
	
	//---------------------------------------------------------
	//---------------------------------------------------------
	
		
	const se3 getPose() const	{ 	return Pose;	}
	const Mat<float> getPosition()
	{
		return Pose.getT();
	}
		
	const Quat getOrientation()		{	return Qt_FromMat(Pose.exp());	}
	const Mat<float> getTransformation() 	{	return Pose.exp();	}
	
	const Mat<float> getLinearVelocity()	{	return LinearVelocity;	}
	const Mat<float> getAngularVelocity()	{	return AngularVelocity;	}

	
	//---------------------------------------------------------
	//---------------------------------------------------------
	
	void setPose( const se3& pose_)	{	Pose = pose_;	}
	void setPosition( const Mat<float>& t_)	{	Pose.setT(t_);	}
	void setOrientation( const Quat& q)	{	Pose.setOrientation(q);	}
	
	void setLinearVelocity( const Mat<float>& lvel)	{	LinearVelocity = lvel;	}
	void setAngularVelocity( const Mat<float>& avel)	{	AngularVelocity = avel;	}
};


