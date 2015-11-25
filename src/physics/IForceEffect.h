#ifndef IFORCEEFFECT_H
#define IFORCEEFFECT_H

class IForceEffect
{
	protected :
	
	float endTime;
	
	
	//--------------------------------------------------------
	
	public :
	
	IForceEffect();
	
	virtual ~IForceEffect();
	
	virtual void Apply(float dt = 0.001f, std::shared_ptr<RigidBody> ptrRB = NULL) = 0;
	
	
	float getEndTime() const	
	{	
		return endTime;	
	}
	
	void setEndTime(const float et)	
	{
		endTime = et;	
	}
};



class GravityForceEffect : public IForceEffect
{
	public :
	
	Mat<float> gravityVector;
	
	
	//--------------------------------------------------------
	
	//gravityVector is initialized to -z by default :
	GravityForceEffect();
	GravityForceEffect(const Mat<float>& g = ZAxis);
	
	~GravityForceEffect();
	
	virtual void Apply(float dt = 0.001f, std::shared_ptr<RigidBody> ptrRB = NULL) override;
	
};


class SpringForceEffect : public IForceEffect
{
	public :
	
	Mat<float> connectionPointL;	// has to be specified in the Local frame -> easier to deal we the changes of the pose of that rigid body...
	Mat<float> otherConnectionPointL;	//idem, but in the one of the other RigidBody :
	
	std::shared_ptr<RigidBody> body;
	std::shared_ptr<RigidBody> other;
	
	float springConstant;
	float restLength;
	
	
	//----------------------------------------
	//----------------------------------------
	SpringForceEffect();
	SpringForceEffect( const Mat<float>& p1in1, const Mat<float>& p2in2, RigidBody* body_, RigidBody* other_, float restLength_ = 1.0f, float springConstant_ = 1.0f);
	
	~SpringForceEffect();
	
	virtual void Apply(float dt = 0.001f, std::shared_ptr<RigidBody> ptrRB = NULL) override;
};


 
#endif
