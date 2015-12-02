#ifndef ICONSTRAINTSOLVERSTRATEGY
#define ICONSTRAINTSOLVERSTRATEGY


#include "../RigidBody.h"
#include "../IConstraint.h"
#include "../../utils/math.h"


class Simulation;

class IConstraintSolverStrategy
{

	protected :
	
	Mat<float> constraintsJacobian;
	Mat<float> constraintsImpulse;
	Mat<float> lambda;
	Mat<float> M;
	
	Simulation* sim;
	
	public :
	
	IConstraintSolverStrategy(Simulation* sim);
	
	~IConstraintSolverStrategy();
	
	
	virtual void Solve(float dt, std::vector<std::unique_ptr<IConstraint> >& c, Mat<float>& q, Mat<float>& qdot, SparseMat<float>& invM, SparseMat<float>& S, const Mat<float>& Fext) = 0;

	//------------------------------------
	
	Mat<float> getConstraintsJacobian() const;
	
	Mat<float> getLambda()	const;
	
	
		
	
};




//-----------------------------------------
//-----------------------------------------
//-----------------------------------------
//-----------------------------------------



class SimultaneousImpulseBasedConstraintSolverStrategy : public IConstraintSolverStrategy
{
	private :
	
	//std::vector<std::unique_ptr<IConstraint> > c;
	
	
	//-----------------------------
	
	public :
	
	SimultaneousImpulseBasedConstraintSolverStrategy(Simulation* sim);
	
	~SimultaneousImpulseBasedConstraintSolverStrategy();
	
	
	void computeConstraintsJacobian(std::vector<std::unique_ptr<IConstraint> >& c);
		
	virtual void Solve(float dt, std::vector<std::unique_ptr<IConstraint> >& c, Mat<float>& q, Mat<float>& qdot, SparseMat<float>& invM, SparseMat<float>& S, const Mat<float>& Fext ) override;

};


#endif
