#include "IConstraint.h"

IConstraint::IConstraint(std::shared_ptr<RigidBody> rbA_, std::shared_ptr<RigidBody> rbB_) : rbA(rbA_), rbB(rbB_), bodiesCanCollide(true)
{
	
	AnchorA = extract(rbA->exp(), 1,4, 3,4);
	AnchorB = extract(rbB->exp(), 1,4, 3,4);
	
	
	//axises of A :
	Mat<float> R(4,4);
	Qt_ToMatrix(rbA->getOrientation(), &R);
	R = extract( R, 1,1, 3,3);
	
	
	Mat<float> tu((float)0,3,1);
	tu.set((float)1,1,1);		//tx
	AxisA = R*tu;
	
	tu *= (float)0;
	tu.set((float)1,2,1);		//ty
	AxisA = operatorL( AxisA, R*tu);
	
	tu *= (float)0;
	tu.set((float)1,3,1);		//tz
	AxisA = operatorL( AxisA, R*tu);
	
	//---------------------------------
	
	
	//axises of B :
	Qt_ToMatrix(rbB->getOrientation(), &R);
	R = extract( R, 1,1, 3,3);
	
	
	tu *= (float)0;
	tu.set((float)1,1,1);		//tx
	AxisB = R*tu;
	
	tu *= (float)0;
	tu.set((float)1,2,1);		//ty
	AxisB = operatorL( AxisB, R*tu);
	
	tu *= (float)0;
	tu.set((float)1,3,1);		//tz
	AxisB = operatorL( AxisB, R*tu);
	
	
	//---------------------------------
	
}
	


//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
//---------------------------------------------






//Penetration depth is initialized to 0, by default.	
//Anchors can be iniatilized to the center of mass, by default.
//Axises can be initialized to the local coordinate frame axises.
ContactConstraint::ContactConstraint(std::shared_ptr<RigidBody> rbA_, std::shared_ptr<RigidBody> rbB_, float penetrationDepth_) : penetrationDepth(penetrationDepth_), IConstraint( rbA_, rbB_)
{

}

void ContactConstraint::addPenaltySpring(float dt)
{

}

void ContactConstraint::applyConstraintImpulse(float dt)
{

}


void ConstactConstraint::applyPositionCorrection(float dt)
{

}

Mat<float> ContactConstraint::getJacobianA()
{

}


Mat<float> ContactConstraint::getJacobianB()
{

}

	
Mat<float> ContactConstraint::getLowLimit()
{

}

Mat<float> ContactConstraint::getHighLimit()
{

}








//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
//---------------------------------------------






//Penetration depth is initialized to 0, by default.	
//Anchors can be iniatilized to the center of mass, by default.
//Axises can be initialized to the local coordinate frame axises.
BallAndSocketJoint::BallAndSocketJoint(std::shared_ptr<RigidBody> rbA_, std::shared_ptr<RigidBody> rbB_) : IConstraint( rbA_, rbB_)
{

}

	
void BallAndSocketJoint::addPenaltySpring(float dt)
{

}

void BallAndSocketJoint::applyConstraintImpulse(float dt)
{

}


void BallAndSocketJoint::applyPositionCorrection(float dt)
{

}

Mat<float> BallAndSocketJoint::getJacobianA()
{

}


Mat<float> BallAndSocketJoint::getJacobianB()
{

}

	
Mat<float> BallAndSocketJoint::getLowLimit()
{

}

Mat<float> BallAndSocketJoint::getHighLimit()
{

}





//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
//---------------------------------------------




//Penetration depth is initialized to 0, by default.	
//Anchors can be iniatilized to the center of mass, by default.
//Axises can be initialized to the local coordinate frame axises.
HingeJoint::HingeJoint(std::shared_ptr<RigidBody> rbA_, std::shared_ptr<RigidBody> rbB_, const Mat<float>& HingeAxis = XAxis) : HingeAxis(HingeAxis_), IConstraint( rbA_, rbB_)
{

}


	
void HingeJoint::addPenaltySpring(float dt)
{

}

void HingeJoint::applyConstraintImpulse(float dt)
{

}


void HingeJoint::applyPositionCorrection(float dt)
{

}

Mat<float> HingeJoint::getJacobianA()
{

}


Mat<float> HingeJoint::getJacobianB()
{

}

	
Mat<float> HingeJoint::getLowLimit()
{

}

Mat<float> HingeJoint::getHighLimit()
{

}






//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
//---------------------------------------------





//Penetration depth is initialized to 0, by default.	
//Anchors can be iniatilized to the center of mass, by default.
//Axises can be initialized to the local coordinate frame axises.
ILimitConstraint::ILimitConstraint(std::shared_ptr<RigidBody> rbA_, std::shared_ptr<RigidBody> rbB_, float Min_, float Max_) : Min(Min_), Max(Max_), IConstraint( rbA_, rbB_)
{

}

	
void ILimitConstraint::addPenaltySpring(float dt)
{

}

void ILimitConstraint::applyConstraintImpulse(float dt)
{

}


void ILimitConstraint::applyPositionCorrection(float dt)
{

}

Mat<float> ILimitConstraint::getJacobianA()
{

}


Mat<float> ILimitConstraint::getJacobianB()
{

}

	
Mat<float> ILimitConstraint::getLowLimit()
{

}

Mat<float> ILimitConstraint::getHighLimit()
{

}





//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
//---------------------------------------------





