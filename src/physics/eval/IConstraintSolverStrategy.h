#ifndef ICONSTRAINTSOLVERSTRATEGY
#define ICONSTRAINTSOLVERSTRATEGY

#include "../Simulation.h"
#include "../RigidBody.h"

class IConstraintSolverStrategy
{

	protected :
	
	Mat<float> constraintsJacobian;
	Mat<float> lambda;
	Mat<float> M;
	
	public :
	
	IConstraintSolverStrategy()
	{
	
	}
	
	~IConstraintSolverStrategy()
	{
	
	}
	
	
	virtual void Solve(float dt, std::vector<std::unique_ptr<IConstraint> >& constraints, Mat<float>& q, Mat<float>& qdot, SparseMat<float>& invM, SparseMat& S, const Mat<float>& Fext) = 0;

	//------------------------------------
	
	Mat<float> getConstraintsJacobian() const
	{
		return constraintsJacobian;
	}
	
	Mat<float> getLambda()	const
	{
		return lambda;
	}
	
	
		
	
};







//-----------------------------------------



class SimultaneousImpulseBasedConstraintSolverStrategy : public IConstraintSolverStrategy
{
	private :
	
	std::vector<std::unique_ptr<IConstraint> >& c;
	
	
	//-----------------------------
	
	public :
	
	SimultaneousImpulseBasedConstraintSolverStrategy()
	{
	
	}
	
	~SimultaneousImpulseBasedConstraintSolverStrategy()
	{
	
	}
	
	
	void computeConstraintsJacobian()
	{
		//TODO
	}
		
	void Solve(float dt, std::vector<std::unique_ptr<IConstraint> >& constraints, Mat<float>& q, Mat<float>& qdot, SparseMat<float>& invM, SparseMat<float>& S, const Mat<float>& Fext ) override
	{
		c = constrains;
		computeConsraintsJacobian();
		
		Mat<float> tConstraintsJacobian( transpose(constraintsJacobian) );
		Mat<float> temp( invGJ( constraintsJacobian * SM2Mat<float>( invM*tConstraintsJacobian ) ) * constraintsJacobian );
		
		Mat<float> tempInvMFext( dt*SM2Mat<float>( invM * Fext ) );
		//lambda = (-1.0f) * ( temp * tempInvMFext + (1.0f/dt) * ( temp * qdot) ) ;
		lambda = (-1.0f) * ( (temp * tempInvMFext) + ( temp * qdot) ) ;
		
		
		constraintsImpulse =  tConstraintsJacobian * lambda;
		
		//qdot += dt * tempInvMFext + dt*SM2Mat<float>( invM * constraintsImpulse );
		qdot += tempInvMFext + SM2Mat<float>( invM * constraintsImpulse );
		
		q += dt*( S*qdot );
		
	}

};


#endif
