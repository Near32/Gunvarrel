#ifndef ICONSTRAINT_H
#define ICONSTRAINT_H

#include "RigidBody.h"

class IConstraint
{
	public :
	
	std::shared_ptr<RigidBody> rbA;
	std::shared_ptr<RigidBody> rbB;
	
	bool bodiesCanCollide;
	
	Mat<float> AnchorA;
	Mat<float> AnchorB;
	
	Mat<float> AxisA;
	Mat<float> AxisB;
	
	
	//-----------------------------------
	//-----------------------------------
	
	//Anchors can be iniatilized to the center of mass, by default.
	//Axises can be initialized to the local coordinate frame axises.
	IConstraint(std::shared_ptr<RigidBody> rbA_, std::shared_ptr<RigidBody> rbB_);
	
	~IConstraint()	{}
	
	virtual void addPenaltySpring(float dt = 0.001f) = 0;
	virtual void applyConstraintImpulse(float dt = 0.001f) = 0;
	virtual void applyPositionCorrection(float dt = 0.001f) = 0;
	
	virtual Mat<float> getJacobianA() = 0;
	virtual Mat<float> getJacobianB() = 0;
	
	virtual Mat<float> getLowLimit() = 0;
	virtual Mat<float> getHighLimit() = 0;
	
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
	ContactConstraint(std::shared_ptr<RigidBody> rbA_, std::shared_ptr<RigidBody> rbB_, float penetrationDepth_ = 0.0f);
	
	
	~ContactConstraint()	{}	
		
	virtual void addPenaltySpring(float dt = 0.001f) override;
	virtual void applyConstraintImpulse(float dt = 0.001f) override;
	virtual void applyPositionCorrection(float dt = 0.001f) override;
	
	virtual Mat<float> getJacobianA() override;
	virtual Mat<float> getJacobianB() override;
	
	virtual Mat<float> getLowLimit() override;
	virtual Mat<float> getHighLimit() override;
	
	
	
};



class BallAndSocketJoint : public IConstraint
{
	public :
	
	
	
	//-----------------------------------------
	//-----------------------------------------
	
	//Penetration depth is initialized to 0, by default.	
	//Anchors can be iniatilized to the center of mass, by default.
	//Axises can be initialized to the local coordinate frame axises.
	BallAndSocketJoint(std::shared_ptr<RigidBody> rbA_, std::shared_ptr<RigidBody> rbB_);
	
	
	~BallAndSocketJoint()	{}
	
	virtual void addPenaltySpring(float dt = 0.001f) override;
	virtual void applyConstraintImpulse(float dt = 0.001f) override;
	virtual void applyPositionCorrection(float dt = 0.001f) override;
	
	virtual Mat<float> getJacobianA() override;
	virtual Mat<float> getJacobianB() override;
	
	virtual Mat<float> getLowLimit() override;
	virtual Mat<float> getHighLimit() override;
	
	
	
};




class HingeJoint : public IConstraint
{
	public :
	
	Mat<float> HingeAxis;
	
	//-----------------------------------------
	//-----------------------------------------
	
	//Penetration depth is initialized to 0, by default.	
	//Anchors can be iniatilized to the center of mass, by default.
	//Axises can be initialized to the local coordinate frame axises.
	HingeJoint(std::shared_ptr<RigidBody> rbA_, std::shared_ptr<RigidBody> rbB_, const Mat<float>& HingeAxis = XAxis);
	
	
	~HingeJoint()	{}
	
	virtual void addPenaltySpring(float dt = 0.001f) override;
	virtual void applyConstraintImpulse(float dt = 0.001f) override;
	virtual void applyPositionCorrection(float dt = 0.001f) override;
	
	virtual Mat<float> getJacobianA() override;
	virtual Mat<float> getJacobianB() override;
	
	virtual Mat<float> getLowLimit() override;
	virtual Mat<float> getHighLimit() override;
	
	
	
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
	ILimitConstraint(std::shared_ptr<RigidBody> rbA_, std::shared_ptr<RigidBody> rbB_, float Min_ = -INF, float Max_ = INF);
	
	
	~ILimitConstraint()	{}
	
	virtual void addPenaltySpring(float dt = 0.001f) override;
	virtual void applyConstraintImpulse(float dt = 0.001f) override;
	virtual void applyPositionCorrection(float dt = 0.001f) override;
	
	virtual Mat<float> getJacobianA() override;
	virtual Mat<float> getJacobianB() override;
	
	virtual Mat<float> getLowLimit() override;
	virtual Mat<float> getHighLimit() override;
	
	
	
};


//TODO : declare and implant [Linear-Angular]LimitConstraint



#endif
