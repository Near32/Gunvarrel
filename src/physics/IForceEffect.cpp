#include "IForceEffect.h"

IForceEffect::IForceEffect() : endTime(INF)
{

}

IForceEffect::~IForceEffect()
{

}

//------------------------------------
//------------------------------------


//------------------------------------
//------------------------------------

//gravityVector is initialized to -z by default :
GravityForceEffect::GravityForceEffect(const Mat<float>& g) : gravityVector(g)
{
	IForceEffect();
}



GravityForceEffect::~GravityForceEffect()
{

}
	
void GravityForceEffect::Apply(float dt, RigidBody& RB)
{
	// this force is applied to the center of mass of the rigid body so it does not involve the creation of any torque.
	RB.addForce( RB.getMass()*gravityVector);
	 
}


//--------------------------------------------
//--------------------------------------------

//--------------------------------------------
//--------------------------------------------


SpringForceEffect::SpringForceEffect( const Mat<float>& p1in1, const Mat<float>& p2in2, RigidBody* body_, RigidBody* other_, float restLength_, float springConstant_) : IForceEffect(), connectionPoint(p1in1), otherConnectionPoint(p2in2), body(body_), other(other_), restLength(restLength_), springConstant( springConstant_)
{

}
	
SpringForceEffect::~SpringForceEffect()
{

}

	
void SpringForceEffect::Apply(float dt, RigidBody& RB)
{
	Mat<float> p1W( body->getPointInWorld(connectionPointL) );
	Mat<float> p2W( other->getPointInWorld(otherConnectionPointL));
	
	Mat<float> force(p1W-p2W);
	
	float magnitude = norme2(force);
	force *= 1.0f/magnitude;
	
	magnitude = spingConstant*fabs_(magnitude-restLength);
	
	body->addForceAtWorldPoint( (float)(-magnitude)*force, p1W);			
	other->addForceAtWorldPoint( (float)(magnitude)*force, p2W);
	
}
