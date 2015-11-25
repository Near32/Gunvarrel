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
	
	
	virtual void Solve(float dt, std::vector<std::unique_ptr<IConstraint> >& constraints, const Mat<float>& q, const Mat<float>& qdot, const Mat<float>& invM, const Mat<float>& Fext) = 0;

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
		
	}
		
	void Solve(float dt, std::vector<std::unique_ptr<IConstraint> >& constraints, const Mat<float>& q, const Mat<float>& qdot, const Mat<float>& invM, const Mat<float>& Fext ) override
	{
		c = constrains;
		computeConsraintsJacobian();
		
		Mat<float> temp = invGJ( constraintsJacobian*(invM*transpose(constraintsJacobian)) ) * constraintsJacobian;
		
		lambda = (-1.0f) * ( temp * invM * Fext + (1.0f/dt) * ( temp * qdot) ) ;
		 
		
		
		
		
	}

};


#endif
