#include "IConstraint.h"

IConstraint::IConstraint(RigidBody& rbA_, RigidBody& rbB_) : rbA(rbA_), rbB(rbB_), bodiesCanCollide(true)
{
	
	AnchorAL = rbA.getPointInLocal( extract(rbA->exp(), 1,4, 3,4) );
	AnchorBL = rbB.getPointInLocal( extract(rbB->exp(), 1,4, 3,4) );
	
	
	//axises of A in W:
	Mat<float> R(4,4);
	Qt_ToMatrix(rbA.getOrientation(), &R);
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
	
	
	//axises of B in W:
	Qt_ToMatrix(rbB.getOrientation(), &R);
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
ContactConstraint::ContactConstraint(RigidBody& rbA_, RigidBody& rbB_, float penetrationDepth_) : penetrationDepth(penetrationDepth_), IConstraint( rbA_, rbB_)
{

}

void ContactConstraint::addPenaltySpring(float dt)
{

}

void ContactConstraint::applyConstraintImpulse(float dt)
{

}


void ContactConstraint::applyPositionCorrection(float dt)
{

}

void ContactConstraint::computeJacobians()
{
	//A :
	
	
	//---------------------
	
	//B : 
	
}








//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
//---------------------------------------------






//Penetration depth is initialized to 0, by default.	
//Anchors can be iniatilized to the center of mass, by default.
//Axises can be initialized to the local coordinate frame axises.
BallAndSocketJoint::BallAndSocketJoint(RigidBody& rbA_, RigidBody& rbB_, const Mat<float>& AnchorAL_, const Mat<float>& AnchorBL_) : IConstraint( rbA_, rbB_)
{
	AnchorAL = AnchorAL_;
	AnchorBL = AnchorBL_;
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

void BallAndSocketJoint::computeJacobians()
{
	//A :
	JacobianA = operatorL( Identity, (-1.0f)*crossProduct(AnchorAL) );
	
	//---------------------
	
	//B : 
	JacobianB = operatorL( (-1.0f)*Identity, crossProduct(AnchorBL) );
}




//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
//---------------------------------------------




//Penetration depth is initialized to 0, by default.	
//Anchors can be iniatilized to the center of mass, by default.
//Axises can be initialized to the local coordinate frame axises.
HingeJoint::HingeJoint( RigidBody& rbA_, RigidBody& rbB_, const Mat<float>& HJAxisW_, const Mat<float> AnchorW_) : HJAxis(HJAxisW_), AnchorW(AnchorW_), IConstraint( rbA_, rbB_)
{
	BASJoint = BallAndSocketJoint(rbA_,rbB_, rbA.getPointInLocal(AnchorW), rbB.getPointInLocal(AnchorW) );
	
	//--------------------
	
	HJAxisW1 = crossproductV( HJAxisW, extract(AxisA,1,1,3,1)+extract(AxisA,1,2,3,2)+extract(AxisA,1,3,3,3) );
	HJAxisW2 = crossproductV( HJAxisW, HJAxisW1);
	
	//TODO : verify the correctness of this use of the axises in World Reference Frame ?...!!
	
	//--------------------
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

void HingeJoint::computeJacobians()
{
	BASJoint.computeJacobian();
	
	//---------------------------
	
	
	//A :
	Mat<float> zero(0.0f,1,3);
	
	JacobianA = operatorC( 
	
		operatorC( BASJoint.getJacobianA(),
					operatorL( zero, transpose(HJAxisW1)) ),
					
		operatorL( zero, transpose(HJAxisW2))
							 );
	
	//-----------------------
	
	//B :
	JacobianB = operatorC( 
	
		operatorC( BASJoint.getJacobianB(),
					operatorL( zero, -transpose(HJAxisW1)) ),
					
		operatorL( zero, -transpose(HJAxisW2))
							 );
	
}


//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
//---------------------------------------------





//Penetration depth is initialized to 0, by default.	
//Anchors can be iniatilized to the center of mass, by default.
//Axises can be initialized to the local coordinate frame axises.
ILimitConstraint::ILimitConstraint(RigidBody& rbA_, RigidBody& rbB_, float Min_, float Max_) : Min(Min_), Max(Max_), IConstraint( rbA_, rbB_)
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

void ILimitConstraint::computeJacobians()
{

}




//---------------------------------------------
//---------------------------------------------

//---------------------------------------------
//---------------------------------------------





