#include "RigidBody.h"

RigidBody::RigidBody() : userForce(Mat<float>((float)0,3,1), userTorque(Mat<float>((float)0,3,1), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3), ptrShape(new SphereShape())
{
	ISimulationObject();
	IMoveable();
	type = TSORigidBody;
}


RigidBody::RigidBody(const std::String& name_, int id_, bool isActive_) : userForce( Mat<float>((float)0,3,1) ), userTorque(Mat<float>((float)0,3,1), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3), ptrShape(new SphereShape())
{	
	ISimulationObject(name_,id_,isactive);
	IMoveable();
	type = TSORigidBody;
}


RigidBody::RigidBody(const se3& Pose_, const Mat<float>& Lvel, const Mat<float>& Avel) : userForce(Mat<float>((float)0,3,1), userTorque(Mat<float>((float)0,3,1), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3), ptrShape(new SphereShape())
{
	ISimulationObject();
	IMoveable(Pose_,Lvel,Avel);
	type = TSORigidBody;
}


RigidBody::RigidBody(const std::String& name_, int id_, bool isActive_, const se3& Pose_) : userForce(Mat<float>((float)0,3,1), userTorque(Mat<float>((float)0,3,1), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3), ptrShape(new SphereShape())
{
	ISimulationObject(name_,id_,isActive_);
	IMoveable(Pose_);
	type = TSORigidBody;
}

RigidBody::RigidBody(const std::String& name_, int id_, bool isActive_, const se3& Pose_, const Mat<float>& Lvel, const Mat<float>& Avel) : userForce(Mat<float>((float)0,3,1), userTorque(Mat<float>((float)0,3,1), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3), ptrShape(new SphereShape())
{
	ISimulationObject(name_,id_,isActive_);
	IMoveable(Pose_,Lvel,Avel);
	type = TSORigidBody;
}


	
RigidBody::RigidBody(const std::String& name_, int id_, bool isActive_, const se3& Pose_, std::unique_ptr<IShape> ptrShape_) : userForce(Mat<float>((float)0,3,1), userTorque(Mat<float>((float)0,3,1), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3), ptrShape(ptrShape_)
{
	ISimulationObject(name_,id_,isActive_);
	IMoveable(Pose_);
	type = TSORigidBody;
}

RigidBody::RigidBody(const std::String& name_, int id_, bool isActive_, const se3& Pose_, const Mat<float>& Lvel, const Mat<float>& Avel, std::unique_ptr<IShape> ptrShape_) : userForce(Mat<float>((float)0,3,1), userTorque(Mat<float>((float)0,3,1), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3), ptrShape(ptrShape_)
{
	ISimulationObject(name_,id_,isActive_);
	IMoveable(Pose_,Lvel,Avel);
	type = TSORigidBody;
}

RigidBody::~RigidBody()
{

}


//TODO : implantation
Mat<float> RigidBody::getFirstOrderDerivatives(float dt)
{

}

//TODO : implantation
void RigidBody::Render(const se3& WorldTransformation)
{

}



Mat<float> RigidBody::transformInertiaTensorL2W()
{
	iInertiaWorld =  extract( pose.exp(), 1,1, 3,3);
	iInertiaWorld = transpose(iInertiaWorld)*(iInertia*iInertiaWorld);
	return iInertiaWorld;
}


void RigidBody::addForce(const Mat<float>& force)
{
	userForce += force;
}

void RigidBody::addForceAtWorldPoint(const Mat<float>& force, const Mat<float>& pointW)
{
	addForce(force);
	addTorque( crossproductV( pointW-extract(&pose, 1,4, 3,4), force) );	
}

void RigidBody::addForceAtBodyPoint(const Mat<float>& force, const Mat<float>& pointL)
{
	addForceAtWorlPoint( force, getPointInWorld(pointL) );
}


void RigidBody::addTorque(const Mat<float>& torque)
{
	userTorque += torque;
}

void RigidBody::clearUser()
{
	userForce *= 0.0f;
	userTorque *= 0.0f;
}


void RigidBody::calculateDerivedData()
{
	//Derived the Inertial Tensor in the world coordinate frame :
	transformInertiaTensorL2W();

}

Mat<float> RigidBody::getPointInWorld( const Mat<float>& pointL)
{
	return transpose( extract(pose.exp(), 1,1, 3,3))* (pointL-extract(pose.exp(), 1,4, 3,4) );	
}


