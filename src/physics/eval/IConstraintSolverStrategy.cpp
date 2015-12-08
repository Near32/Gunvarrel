#include "IConstraintSolverStrategy.h"
#include "../Simulation.h"

IConstraintSolverStrategy::IConstraintSolverStrategy(Simulation* sim_) : sim(sim_)
{

}

IConstraintSolverStrategy::~IConstraintSolverStrategy()
{

}

Mat<float> IConstraintSolverStrategy::getConstraintsJacobian() const
{
	return constraintsJacobian;
}

Mat<float> IConstraintSolverStrategy::getLambda()	const
{
	return lambda;
}



//-----------------------------------------
//-----------------------------------------
//-----------------------------------------

//-----------------------------------------
//-----------------------------------------
//-----------------------------------------


SimultaneousImpulseBasedConstraintSolverStrategy::SimultaneousImpulseBasedConstraintSolverStrategy(Simulation* sim_) : IConstraintSolverStrategy(sim_)
{

}

SimultaneousImpulseBasedConstraintSolverStrategy::~SimultaneousImpulseBasedConstraintSolverStrategy()
{

}


void SimultaneousImpulseBasedConstraintSolverStrategy::computeConstraintsJacobian(std::vector<std::unique_ptr<IConstraint> >& c)
{
	size_t size = c.size();
	int n = sim->simulatedObjects.size();
	
	c[0]->computeJacobians();
		
	Mat<float> tJA(c[0]->getJacobianA());
	Mat<float> tJB(c[0]->getJacobianB());
	size_t sl = tJA.getLine();
	int idA = 6 * ( c[0]->rbA.getID() );
	int idB = 6 * ( c[0]->rbB.getID() );
	
	Mat<float> temp((float)0,sl, 6*n );
	
	for(int i=1;i<=sl;i++)
	{
		for(int j=1;j<=6;j++)
		{
			temp.set( i, idA+j, tJA.get(i,j) );
			temp.set( i, idB+j, tJB.get(i,j) );
		}
	}
	
	constraintsJacobian = temp;
	
	
	for(int i=1;i<size;i++)
	{
		c[i]->computeJacobians();
		
		Mat<float> tJA(c[i]->getJacobianA());
		Mat<float> tJB(c[i]->getJacobianB());
		size_t sl = tJA.getLine();
		int idA = 6 * ( c[i]->rbA.getID() );
		int idB = 6 * ( c[i]->rbB.getID() );
		
		temp = Mat<float>((float)0,sl, 6*n );
		
		for(int i=1;i<=sl;i++)
		{
			for(int j=1;j<=6;j++)
			{
				temp.set( i, idA+j, tJA.get(i,j) );
				temp.set( i, idB+j, tJB.get(i,j) );
			}
		}
		
		constraintsJacobian = operatorC(constraintsJacobian, temp );
	}
}
	
void SimultaneousImpulseBasedConstraintSolverStrategy::Solve(float dt, std::vector<std::unique_ptr<IConstraint> >& c, Mat<float>& q, Mat<float>& qdot, SparseMat<float>& invM, SparseMat<float>& S, const Mat<float>& Fext )
{
	computeConstraintsJacobian(c);
	
	SparseMat<float> tConstraintsJacobian( transpose(constraintsJacobian) );
	Mat<float> temp( invGJ( constraintsJacobian * ( invM*tConstraintsJacobian ).SM2mat() ) * constraintsJacobian );//invM*tConstraintsJacobian ) * constraintsJacobian );
	
	Mat<float> tempInvMFext( invM *(dt * Fext) ) ;//SM2Mat<float>( invM * Fext ) );
	//lambda = (-1.0f) * ( temp * tempInvMFext + (1.0f/dt) * ( temp * qdot) ) ;
	lambda = (-1.0f) * ( (temp * tempInvMFext) + ( temp * qdot) ) ;
	
	
	constraintsImpulse =  tConstraintsJacobian * lambda;
	
	//qdot += dt * tempInvMFext + dt*SM2Mat<float>( invM * constraintsImpulse );
	qdot += tempInvMFext + invM * constraintsImpulse;//SM2Mat<float>(  );
	
	q += dt*( S*qdot );
	
	q.afficher();
	
}
