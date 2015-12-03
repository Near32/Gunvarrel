#include "RigidBody.h"

RigidBody::RigidBody() : ISimulationObject(), IMoveable(), userForce( Mat<float>((float)0,3,1) ), userTorque(Mat<float>((float)0,3,1)), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3), ptrShape(new SphereShape(this))
{
	type = TSORigidBody;
}


RigidBody::RigidBody(const std::string& name_, int id_, bool isActive_) : ISimulationObject(name_,id_,isActive_), IMoveable(), userForce( Mat<float>((float)0,3,1) ), userTorque(Mat<float>((float)0,3,1)), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3), ptrShape(new SphereShape(this))
{	
	type = TSORigidBody;
}

RigidBody::RigidBody(const se3& Pose_, const std::string& name_, int id_, bool isActive_) : ISimulationObject(name_,id_,isActive_), IMoveable(Pose_), userForce( Mat<float>((float)0,3,1) ), userTorque(Mat<float>((float)0,3,1)), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3), ptrShape(new SphereShape(this))
{	
	type = TSORigidBody;
}

RigidBody::RigidBody(const se3& Pose_, const std::string& name_, int id_, ShapeType shtype, bool isActive_) : ISimulationObject(name_,id_,isActive_), IMoveable(Pose_), userForce( Mat<float>((float)0,3,1) ), userTorque(Mat<float>((float)0,3,1)), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3)
{	

	if(shtype == BOX)
	{
		//ptrShape = std::unique_ptr<IShape>(new BoxShape(this));
		ptrShape = new BoxShape(this);
	}
	else
	{
		//ptrShape = std::unique_ptr<IShape>(new SphereShape(this));
		ptrShape = new SphereShape(this);
	}
	
	type = TSORigidBody;
}

RigidBody::RigidBody(const se3& Pose_, const Mat<float>& Lvel, const Mat<float>& Avel) : ISimulationObject(), IMoveable(Pose_,Lvel,Avel), userForce(Mat<float>((float)0,3,1) ), userTorque(Mat<float>((float)0,3,1) ), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3), ptrShape(new SphereShape(this))
{
	type = TSORigidBody;
}


RigidBody::RigidBody(const std::string& name_, int id_, bool isActive_, const se3& Pose_) : ISimulationObject(name_,id_,isActive_), IMoveable(Pose_),userForce(Mat<float>((float)0,3,1) ), userTorque(Mat<float>((float)0,3,1) ), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3), ptrShape(new SphereShape(this))
{
	type = TSORigidBody;
}

RigidBody::RigidBody(const std::string& name_, int id_, bool isActive_, const se3& Pose_, const Mat<float>& Lvel, const Mat<float>& Avel) : ISimulationObject(name_,id_,isActive_), IMoveable(Pose_,Lvel,Avel),userForce(Mat<float>((float)0,3,1) ), userTorque(Mat<float>((float)0,3,1) ), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3), ptrShape(new SphereShape(this))
{
	type = TSORigidBody;
}


/*	
RigidBody::RigidBody(const std::string& name_, int id_, bool isActive_, const se3& Pose_, std::unique_ptr<IShape> ptrShape_) : ISimulationObject(name_,id_,isActive_), IMoveable(Pose_), userForce(Mat<float>((float)0,3,1) ), userTorque(Mat<float>((float)0,3,1) ), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3), ptrShape(std::move(ptrShape_))
{
	type = TSORigidBody;
}

RigidBody::RigidBody(const std::string& name_, int id_, bool isActive_, const se3& Pose_, const Mat<float>& Lvel, const Mat<float>& Avel, std::unique_ptr<IShape> ptrShape_) : ISimulationObject(name_,id_,isActive_), IMoveable(Pose_,Lvel,Avel),userForce(Mat<float>((float)0,3,1) ), userTorque(Mat<float>((float)0,3,1) ), isFixed(false), canCollide(true), mass(1.0f), imass(1.0f), Inertia(Identity3), iInertia(Identity3), ptrShape( std::move(ptrShape_))
{
	type = TSORigidBody;
}
*/

RigidBody::~RigidBody()
{

}



//TODO : implantation
void RigidBody::Render(const se3& WorldTransformation)
{

}


void RigidBody::computeInertia()
{
	//it is assumed that the ptrShape is initialized.
	switch(ptrShape->getShapeType())
	{
		case BOX :
		{
		float lx = ((BoxShape&)(*ptrShape)).getHeight();
		float ly = ((BoxShape&)(*ptrShape)).getWidth();
		float lz = ((BoxShape&)(*ptrShape)).getDepth();
		
		Inertia = Mat<float>(0.0f,3,3);
		Inertia.set( (mass/12)*(ly*ly+lz*lz), 1,1); 
		Inertia.set( (mass/12)*(lx*lx+lz*lz), 2,2);
		Inertia.set( (mass/12)*(ly*ly+lx*lx), 3,3);
		
		iInertia = Inertia;
		iInertia.set( 1.0f/iInertia.get(1,1), 1,1);
		iInertia.set( 1.0f/iInertia.get(2,2), 2,2);
		iInertia.set( 1.0f/iInertia.get(3,3), 3,3);
		}
		break;
		
		case SPHERE :
		{
		//TODO....
		}
		break;
		
		default :
		{
		//TODO....
		}
		break;
	}
}

Mat<float> RigidBody::transformInertiaTensorL2W()
{
	iInertiaWorld =  extract( Pose->exp(), 1,1, 3,3);
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
	addTorque( crossproductV( pointW-extract(Pose->exp(), 1,4, 3,4), force) );	
}

void RigidBody::addForceAtBodyPoint(const Mat<float>& force, const Mat<float>& pointL)
{
	addForceAtWorldPoint( force, getPointInWorld(pointL) );
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
	return transpose( extract(Pose->exp(), 1,1, 3,3))* (pointL-extract(Pose->exp(), 1,4, 3,4) );	
}

Mat<float> RigidBody::getPointInLocal( const Mat<float>& pointW)
{
	return extract(Pose->exp(), 1,1, 3,3) * pointW + extract(Pose->exp(), 1,4, 3,4) ;	
}

Mat<float> RigidBody::getAxisInWorld( const Mat<float>& aL)
{
	return transpose( extract(Pose->exp(), 1,1, 3,3)) * aL;	
}

Mat<float> RigidBody::getAxisInLocal( const Mat<float>& aW)
{
	return extract(Pose->exp(), 1,1, 3,3) * aW;	
}


