#include "IMoveable.h"

IMoveable::IMoveable()
{
	Pose = new se3();
	//Orientation = Qt_FromMat( extract(Pose.exp(), 1,1,3,3) );	
	
	LinearVelocity = new Mat<float>((float)0,3,1);
	AngularVelocity = new Mat<float>((float)0,3,1);
}
	
//Orientation has to be initialized from Pose.
IMoveable::IMoveable( const se3& Pose_) : Pose( new se3() )
{
	*Pose = Pose_;
	//Orientation = Qt_FromMat( extract(Pose.exp(), 1,1,3,3) );	
	
	LinearVelocity = new Mat<float>((float)0,3,1);
	AngularVelocity = new Mat<float>((float)0,3,1);
}

//Orientation has to be initialized from Pose.
IMoveable::IMoveable( const se3& Pose_, const Mat<float>& Lvel, const Mat<float>& Avel): Pose( new se3() ), LinearVelocity( new Mat<float>(Lvel)), AngularVelocity( new Mat<float>(Avel))
{
	*Pose = Pose_;
	//Orientation = Qt_FromMat( extract(Pose.exp(), 1,1,3,3) );	
}	
