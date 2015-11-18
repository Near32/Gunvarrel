#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "ISimulationObject.h"
#include "IMoveable.h"
#include "IShape.h"


class RigidBody : public ISimulationObject, public IMoveable
{
	public :
	
	Mat<float> userForce;
	Mat<float> userTorque;
	
	bool isFixed;
	bool canCollide;
	
	float mass;
	float imass;
	
	Mat<float> Inertia;
	Mat<float> iInertia;
	
	Mat<float> iInertiaWorld;
	
	
	std::unique_ptr<IShape> ptrShape;
	
//--------------------------------------------------------
//--------------------------------------------------------
	
	RigidBody();
	RigidBody(const std::string& name_, int id_, bool isActive_);
	RigidBody(const se3& Pose_, const Mat<float>& Lvel, const Mat<float>& Avel);
	RigidBody(const std::string& name_, int id_, bool isActive_, const se3& Pose_);
	RigidBody(const std::string& name_, int id_, bool isActive_, const se3& Pose_, const Mat<float>& Lvel, const Mat<float>& Avel);
	
	RigidBody(const std::string& name_, int id_, bool isActive_, const se3& Pose_, std::unique_ptr<IShape> ptrShape_);
	RigidBody(const std::string& name_, int id_, bool isActive_, const se3& Pose_, const Mat<float>& Lvel, const Mat<float>& Avel, std::unique_ptr<IShape> ptrShape_);
	
	~RigidBody();
	
	//TODO : implantation
	Mat<float> getFirstOrderDerivatives(float dt = 0.001f);
	
	//TODO : implantation
	void Render(const se3& WorldTransformation) override;
	
	Mat<float> transformInertiaTensorL2W();
	
	void addForce(const Mat<float>& force);
	void addForceAtWorldPoint(const Mat<float>& force, const Mat<float>& pointW);
	void addForceAtBodyPoint(const Mat<float>& force, const Mat<float>& pointL);
	
	void addTorque(const Mat<float>& torque);
	
	void clearUser();
	void calculateDerivedData();
		
		
	//------------------------------------------------------
	//------------------------------------------------------
	
	const Mat<float> getForceAccumulator() const
	{
		return userForce;
	}
	
	const Mat<float> getTorqueAccumulator() const
	{ 	
		return userTorque;
	}
	
	const float getMass() const
	{
	 	return mass;	
	}
	const float getInverseMass() const
	{
		return imass;
	}
	
	const Mat<float> getInertialLocal() const
	{
		return Inertia;
	}
	
	const Mat<float> getInverseInertialLocal() const
	{
		return iInertia;
	}
		
	const Mat<float> getInverseInertialWorld() const
	{
		return iInertiaWorld;
	}
			
	const bool getCollisionStatus()	const	
	{	
		return canCollide;
	}
	
	const IShape& getShapeReference() const
	{ 
		return *ptrShape;
	}
	
	const ShapeType getShapeType() const	
	{	
		return ptrShape->getShapeType();
	}
	
		
	
	Mat<float> getPointInWorld( const Mat<float>& pointL);
	Mat<float> getPointInLocal( const Mat<float>& pointW);
		
	
};
#endif
