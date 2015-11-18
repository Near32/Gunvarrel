#include "IMoveable.h"

IMoveable::IMoveable()
{
	Pose = se3();
	//Orientation = Qt_FromMat( extract(Pose.exp(), 1,1,3,3) );	
	
	LinearVelocity = Mat<float>((float)0,3,1);
	AngularVelocity = Mat<float>((float)0,3,1);
}
	
//Orientation has to be initialized from Pose.
IMoveable::IMoveable( const se3& Pose_) : Pose(Pose_)
{
	//Orientation = Qt_FromMat( extract(Pose.exp(), 1,1,3,3) );	
	
	LinearVelocity = Mat<float>((float)0,3,1);
	AngularVelocity = Mat<float>((float)0,3,1);
}

//Orientation has to be initialized from Pose.
IMoveable::IMoveable( const se3& Pose_, const Mat<float>& Lvel, const Mat<float>& Avel): Pose(Pose_), LinearVelocity(Lvel), AngularVelocity(Avel)
{
	//Orientation = Qt_FromMat( extract(Pose.exp(), 1,1,3,3) );	
}	
