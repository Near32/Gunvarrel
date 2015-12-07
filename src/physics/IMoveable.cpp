#include "IMoveable.h"

IMoveable::IMoveable()
{	
	try
	{
		Pose = new se3();
		//Orientation = Qt_FromMat( extract(Pose.exp(), 1,1,3,3) );	
		LinearVelocity = new Mat<float>((float)0,3,1);
		AngularVelocity = new Mat<float>((float)0,3,1);
	}
	catch( std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		throw e;
	}
}
	
//Orientation has to be initialized from Pose.
IMoveable::IMoveable( const se3& Pose_)
{
	try
	{
		Pose = new se3();
		LinearVelocity = new Mat<float>((float)0,3,1);
		AngularVelocity = new Mat<float>((float)0,3,1);
	}
	catch(std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		throw e;
	}
	
	*Pose = Pose_;
	//Orientation = Qt_FromMat( extract(Pose.exp(), 1,1,3,3) );		
}

//Orientation has to be initialized from Pose.
IMoveable::IMoveable( const se3& Pose_, const Mat<float>& Lvel, const Mat<float>& Avel): Pose( new se3() ), LinearVelocity( new Mat<float>(Lvel)), AngularVelocity( new Mat<float>(Avel))
{
	*Pose = Pose_;
	//Orientation = Qt_FromMat( extract(Pose.exp(), 1,1,3,3) );	
}	

IMoveable::~IMoveable()
{
		delete Pose;
		delete LinearVelocity;
		delete AngularVelocity;
}


