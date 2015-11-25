#ifndef ICONSTRAINT_H
#define ICONSTRAINT_H

#include "RigidBody.h"

class IConstraint
{
	public :
	
	RigidBody& rbA;
	RigidBody& rbB;
	
	bool bodiesCanCollide;
	
	Mat<float> AnchorAL;
	Mat<float> AnchorBL;
	
	Mat<float> AxisA;	//in World
	Mat<float> AxisB;	//in World
	
	//TODO : warning about their use in the HingJoint constraint and the definition in Worl of Local of the HJAxises...
	
	Mat<float> constraintsImpules;
	Mat<float> constraintForce;
	
	Mat<float> JacobianA;
	Mat<float> JacobianB;
	
	//-----------------------------------
	//-----------------------------------
	
	//Anchors can be iniatilized to the center of mass, by default.
	//Axises can be initialized to the local coordinate frame axises.
	IConstraint(RigidBody& rbA_, RigidBody& rbB_);
	
	~IConstraint()	{}
	
	virtual void addPenaltySpring(float dt = 0.001f) = 0;
	virtual void applyConstraintImpulse(float dt = 0.001f) = 0;
	virtual void applyPositionCorrection(float dt = 0.001f) = 0;
	virtual void computeJacobians() = 0;
	
	
	Mat<float> getJacobianA() const
	{
		return JacobianA;
	}
	
	Mat<float> getJacobianB() const
	{
		return JacobianB;
	}
	
	
	
};



class ContactConstraint : public IConstraint
{
	public :
	
	float penetrationDepth;
	
	//-----------------------------------------
	//-----------------------------------------
	
	//Penetration depth is initialized to 0, by default.	
	//Anchors can be iniatilized to the center of mass, by default.
	//Axises can be initialized to the local coordinate frame axises.
	ContactConstraint(RigidBody& rbA_, RigidBody& rbB_, float penetrationDepth_ = 0.0f);
	
	
	~ContactConstraint()	
	{
	
	}	
		
	virtual void addPenaltySpring(float dt = 0.001f) override;
	virtual void applyConstraintImpulse(float dt = 0.001f) override;
	virtual void applyPositionCorrection(float dt = 0.001f) override;
	virtual void computeJacobians() override;
	
	
};



class BallAndSocketJoint : public IConstraint
{
	public :
	
	
	
	//-----------------------------------------
	//-----------------------------------------
	
	//Anchors can be iniatilized to the center of mass, by default.
	//Axises can be initialized to the local coordinate frame axises.
	BallAndSocketJoint(RigidBody& rbA_, RigidBody& rbB_, const Mat<float>& AnchorAL, const Mat<float>& AnchorBL);
	
	
	~BallAndSocketJoint()	
	{
	
	}
	
	virtual void addPenaltySpring(float dt = 0.001f) override;
	virtual void applyConstraintImpulse(float dt = 0.001f) override;
	virtual void applyPositionCorrection(float dt = 0.001f) override;
	
	virtual void computeJacobians() override;
	
	
	
};




class HingeJoint : public IConstraint
{
	public :
	
	Mat<float> HJAxisW;
	Mat<float> HJAxisW1;
	Mat<float> HJAxisW2;
	
	BallAndSocketJoint BASJoint;
	Mat<float> AnchorW;
	
	//-----------------------------------------
	//-----------------------------------------
	
	//Anchors can be iniatilized to the center of mass, by default.
	//Axises can be initialized to the local coordinate frame axises.
	HingeJoint( RigidBody& rbA_, RigidBody& rbB_, const Mat<float>& HJAxis, const Mat<float> AnchorW);
	
	
	~HingeJoint()
	{
	
	}
	
	virtual void addPenaltySpring(float dt = 0.001f) override;
	virtual void applyConstraintImpulse(float dt = 0.001f) override;
	virtual void applyPositionCorrection(float dt = 0.001f) override;
	
	virtual void computeJacobians() override;
	
	
};




class ILimitConstraint : public IConstraint
{
	public :
	
	float Min;
	float Max;
	
	//-----------------------------------------
	//-----------------------------------------
	
	//Penetration depth is initialized to 0, by default.	
	//Anchors can be iniatilized to the center of mass, by default.
	//Axises can be initialized to the local coordinate frame axises.
	ILimitConstraint(RigidBody& rbA_, RigidBody& rbB_, float Min_ = -INF, float Max_ = INF);
	
	
	~ILimitConstraint()	
	{
	
	}
	
	virtual void addPenaltySpring(float dt = 0.001f) override;
	virtual void applyConstraintImpulse(float dt = 0.001f) override;
	virtual void applyPositionCorrection(float dt = 0.001f) override;
	
	virtual void computeJacobians() override;
	
	virtual Mat<float> getLowLimit();
	virtual Mat<float> getHighLimit();
	
	
	
};


//TODO : declare and implant [Linear-Angular]LimitConstraint



#endif
