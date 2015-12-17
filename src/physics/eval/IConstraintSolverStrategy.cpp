#include "IConstraintSolverStrategy.h"
#include "../Simulation.h"

#include "../../utils/LCPSolver/LCPSolver.h"

#define debuglvl1


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
	
	//tJA.afficher();
	//tJB.afficher();
		
	int sl = tJA.getLine();
	int idA = 6 * ( c[0]->rbA.getID() );
	int idB = 6 * ( c[0]->rbB.getID() );
	
	Mat<float> temp((float)0,sl, 6*n );
	
	for(int i=1;i<=sl;i++)
	{
		for(int j=1;j<=6;j++)
		{
			temp.set( tJA.get(i,j) , i, idA+j);
			temp.set( tJB.get(i,j), i, idB+j  );
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
				temp.set( tJA.get(i,j) , i, idA+j);
				temp.set( tJB.get(i,j), i, idB+j  );
			}
		}
		
		constraintsJacobian = operatorC(constraintsJacobian, temp );
	}
}

void SimultaneousImpulseBasedConstraintSolverStrategy::computeConstraintsJacobian(std::vector<std::unique_ptr<IConstraint> >& c, const Mat<float>& q, const Mat<float>& qdot)
{
	//MAJ qdot :
	int b1 = 0;
	int b2 = 0;
	for( auto& o : sim->simulatedObjects ) 
	{	
		//((RigidBody*)(o.get()))->setPosition( extract(q, b1+1,1, b1+3,1) );	
		//((RigidBody*)(o.get()))->setMatOrientation( extract(q, b1+4,1, b1+7,1) );
		((RigidBody*)(o.get()))->setLinearVelocity( extract( qdot, b2+1,1, b2+3,1) );
		((RigidBody*)(o.get()))->setAngularVelocity( extract( qdot, b2+4,1, b1+6,1) );
		
		b1+=7;
		b2+=6;	
	}
	
	//-------------------------------------
	//-------------------------------------
	//-------------------------------------
	
	size_t size = c.size();
	int n = sim->simulatedObjects.size();
	
	c[0]->computeJacobians();
	
	int idA = 6 * ( c[0]->rbA.getID() );
	int idB = 6 * ( c[0]->rbB.getID() );
	
	Mat<float> tJA(c[0]->getJacobianA());
	Mat<float> tJB(c[0]->getJacobianB());
	int sl = tJA.getLine();
	
	Mat<float> temp((float)0,sl, 6*n );
	
	for(int i=1;i<=sl;i++)
	{
		for(int j=1;j<=6;j++)
		{
			temp.set( tJA.get(i,j) , i, idA+j);
			temp.set( tJB.get(i,j), i, idB+j  );
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
				temp.set( tJA.get(i,j) , i, idA+j);
				temp.set( tJB.get(i,j), i, idB+j  );
			}
		}
		
		constraintsJacobian = operatorC(constraintsJacobian, temp );
	}
}

/*
void SimultaneousImpulseBasedConstraintSolverStrategy::Solve(float dt, std::vector<std::unique_ptr<IConstraint> >& c, Mat<float>& q, Mat<float>& qdot, SparseMat<float>& invM, SparseMat<float>& S, const Mat<float>& Fext )
{
	computeConstraintsJacobian(c);
	
	SparseMat<float> tConstraintsJacobian( transpose(constraintsJacobian) );
	
	Mat<float> temp( invGJ( constraintsJacobian * ( invM*tConstraintsJacobian ).SM2mat() ) * constraintsJacobian );//invM*tConstraintsJacobian ) * constraintsJacobian );
	
	Mat<float> tempInvMFext( invM *(dt * Fext) ) ;//SM2Mat<float>( invM * Fext ) );
	//lambda = (-1.0f) * ( temp * tempInvMFext + (1.0f/dt) * ( temp * qdot) ) ;
	lambda = (-1.0f) * ( (temp * tempInvMFext) + ( temp * qdot) ) ;
	
	
	constraintsImpulse =  tConstraintsJacobian * lambda;
	
	Mat<float> tdot( tempInvMFext + invM * constraintsImpulse );
	qdot += tdot;//SM2Mat<float>(  );
	
	
	Mat<float> t( dt*( S*qdot ) );
	q += t;
	
	//S.print();
	//constraintsJacobian.afficher();
	//tempInvMFext.afficher();
	//temp.afficher();
	//lambda.afficher();
	//constraintsImpulse.afficher();
	//tempInvMFext.afficher();
	//tdot.afficher();
	//qdot.afficher();
	//t.afficher();
	//q.afficher();
	
}
*/

/*
void SimultaneousImpulseBasedConstraintSolverStrategy::Solve(float dt, std::vector<std::unique_ptr<IConstraint> >& c, Mat<float>& q, Mat<float>& qdot, SparseMat<float>& invM, SparseMat<float>& S, const Mat<float>& Fext )
{
	Mat<float> tempInvMFext( dt*(invM * Fext) ) ;
	qdot += tempInvMFext;
	
	for(int i=0;i<=1;i++)
	{
	computeConstraintsJacobian(c,q,qdot);
	
	SparseMat<float> tConstraintsJacobian( transpose(constraintsJacobian) );
	Mat<float> invMtJ( ( invM*tConstraintsJacobian ).SM2mat() );
	
	Mat<float> temp( invGJ( constraintsJacobian * ( invM*tConstraintsJacobian ).SM2mat() ) * constraintsJacobian );//invM*tConstraintsJacobian ) * constraintsJacobian );
	
	lambda = (-1.0f) * ( (temp * tempInvMFext) + ( temp * qdot) ) ;
	
	
	constraintsImpulse =  tConstraintsJacobian * lambda;
	
	Mat<float> tdot(invM * constraintsImpulse );
	qdot += tdot;//SM2Mat<float>(  );
	
	
#ifdef debuglvl1
	std::cout << "J = " << std::endl;
	constraintsJacobian.afficher();

	Mat<float> Cdot(constraintsJacobian*qdot);
	std::cout << "Cdot = Jqdot : norme2 =  " << norme2(Cdot) << std::endl;	
	Cdot.afficher();
	tdot.afficher();
	if( norme2(Cdot) <= 1e-20f )
		i=100;		
#endif
	}	
	
	Mat<float> t( dt*( S*qdot ) );
	q += t;
	
	//S.print();
	//constraintsJacobian.afficher();
	//tempInvMFext.afficher();
	//temp.afficher();
	//lambda.afficher();
	//constraintsImpulse.afficher();
	//tempInvMFext.afficher();
	//tdot.afficher();
	//qdot.afficher();
	//t.afficher();
	//q.afficher();
	
}
*/

/*
void SimultaneousImpulseBasedConstraintSolverStrategy::Solve(float dt, std::vector<std::unique_ptr<IConstraint> >& c, Mat<float>& q, Mat<float>& qdot, SparseMat<float>& invM, SparseMat<float>& S, const Mat<float>& Fext )
{
	Mat<float> tempInvMFext( dt*(invM * Fext) ) ;
	qdot += tempInvMFext;
	computeConstraintsJacobian(c,q,qdot);
	
	Mat<float> tConstraintsJacobian( transpose(constraintsJacobian) );
	Mat<float> M( invGJ( invM.SM2mat()) );
	//Mat<float> A( operatorC( operatorL( M, (-1.0f)*tConstraintsJacobian), operatorL( constraintsJacobian, Mat<float>((float)0, constraintsJacobian.getLine(), constraintsJacobian.getLine()) ) ) );
	Mat<float> b( operatorC( (-1.0f)*(M*qdot+dt*Fext), Mat<float>(0.0f, 3,1) ) );
	Mat<float> x( operatorC( qdot, Mat<float>(0.0f,3, 1) ) );
		
	int N = 10;
	LCPSolver instanceLCPSolver( N, A,b,x);
	x = instanceLCPSolver.solve(N);
	

	lambda = extract( x, qdot.getLine()+1, 1, x.getLine(), 1);
	
	constraintsImpulse =  tConstraintsJacobian * lambda;
	
	Mat<float> tdot(invM * constraintsImpulse );
	qdot += tdot;//SM2Mat<float>(  );
	
	A.afficher();
	std::cout << " qdot+ = " << std::endl;
	qdot.afficher();
	std::cout << " computation gave : " << std::endl;
	extract(x, 1,1, qdot.getLine(), 1).afficher();
	
	
	Mat<float> t( dt*( S*qdot ) );
	q += t;
	
	//S.print();
	//constraintsJacobian.afficher();
	//tempInvMFext.afficher();
	//temp.afficher();
	lambda.afficher();
	//constraintsImpulse.afficher();
	//tempInvMFext.afficher();
	//tdot.afficher();
	//qdot.afficher();
	//t.afficher();
	//q.afficher();
	
}
*/

/*
//TIME STEPPING IMPULSE BASED METHOD : with Baumgarte Stabilization....
void SimultaneousImpulseBasedConstraintSolverStrategy::Solve(float dt, std::vector<std::unique_ptr<IConstraint> >& c, Mat<float>& q, Mat<float>& qdot, SparseMat<float>& invM, SparseMat<float>& S, const Mat<float>& Fext )
{
	computeConstraintsJacobian(c);
	Mat<float> tempInvMFext( dt*(invM * Fext) ) ;
	qdot += tempInvMFext;
	//computeConstraintsJacobian(c,q,qdot);
	Mat<float> b((float)10,3,1);
	Mat<float> C( ((RigidBody*)(sim->simulatedObjects[1].get()))->getPointInWorld(c[0]->AnchorAL )-((RigidBody*)(sim->simulatedObjects[2].get()))->getPointInWorld(c[0]->AnchorBL) );
	b = (0.5f/dt)*C;
	//BAUMGARTE STABILIZATION ....
	
	
	std::cout << "Constraints BASJoint : norme  = " << norme2(C) << std::endl;
	C.afficher();
	c[0]->AnchorAL.afficher();
	((RigidBody*)(sim->simulatedObjects[1].get()))->getPointInWorld(c[0]->AnchorAL ).afficher();
	c[0]->AnchorBL.afficher();
	((RigidBody*)(sim->simulatedObjects[2].get()))->getPointInWorld(c[0]->AnchorBL ).afficher();
	
	
	SparseMat<float> tConstraintsJacobian( transpose(constraintsJacobian) );
	Mat<float> invJinvMtJ( invGJ( constraintsJacobian * ( invM*tConstraintsJacobian ).SM2mat() ) );//invM*tConstraintsJacobian ) * constraintsJacobian );
	
	lambda = (-1.0f) * (invJinvMtJ * (constraintsJacobian*qdot+b) );
	
	
	constraintsImpulse =  tConstraintsJacobian * lambda;
	
	//Mat<float> tdot( (1.0f)*tempInvMFext + invM * constraintsImpulse );
	Mat<float> tdot(invM * constraintsImpulse );
	qdot += tdot;//SM2Mat<float>(  );
	
	
	Mat<float> t( dt*( S*qdot ) );
	q += t;
	
	//S.print();
	//constraintsJacobian.afficher();
	//temp.afficher();
	//invJinvMtJ.afficher();
	std::cout << " lambda = : " << std::endl;
	lambda.afficher();
	std::cout << " Pc : " << std::endl;
	constraintsImpulse.afficher();
	//tempInvMFext.afficher();
	//tdot.afficher();
	//qdot.afficher();
	//t.afficher();
	//q.afficher();
	
}
*/


//KKT :

void SimultaneousImpulseBasedConstraintSolverStrategy::Solve(float dt, std::vector<std::unique_ptr<IConstraint> >& c, Mat<float>& q, Mat<float>& qdot, SparseMat<float>& invM, SparseMat<float>& S, const Mat<float>& Fext )
{
	
	//computeConstraintsJacobian(c);
	Mat<float> tempInvMFext( dt*(invM * Fext) ) ;
	//qdot += tempInvMFext;
	computeConstraintsJacobian(c,q,qdot);
	
	Mat<float> offset((float)10,3,1);
	Mat<float> C( ((RigidBody*)(sim->simulatedObjects[1].get()))->getPointInWorld(c[0]->AnchorAL )-((RigidBody*)(sim->simulatedObjects[2].get()))->getPointInWorld(c[0]->AnchorBL) );
	offset = (0.8f/dt)*C;
	//BAUMGARTE STABILIZATION ....
	
	std::cout << "Constraints BASJoint : norme  = " << norme2(C) << std::endl;
	C.afficher();
	c[0]->AnchorAL.afficher();
	((RigidBody*)(sim->simulatedObjects[1].get()))->getPointInWorld(c[0]->AnchorAL ).afficher();
	c[0]->AnchorBL.afficher();
	((RigidBody*)(sim->simulatedObjects[2].get()))->getPointInWorld(c[0]->AnchorBL ).afficher();
	
	Mat<float> tConstraintsJacobian( transpose(constraintsJacobian) );
	Mat<float> A( (-1.0f)*tConstraintsJacobian );
	Mat<float> M( invGJ( invM.SM2mat() ) );
	A = operatorL( M, A);
	A = operatorC( A , operatorL( constraintsJacobian, Mat<float>((float)0,constraintsJacobian.getLine(), constraintsJacobian.getLine()) ) );
	
	Mat<float> invA( invGJ(A) );//invM*tConstraintsJacobian ) * constraintsJacobian );
	
	Mat<float> tempLambda( invA * operatorC( Mat<float>((float)0,invA.getLine()-3,1) , constraintsJacobian*(invM*Fext)+offset ) );
	lambda = extract( &tempLambda, qdot.getLine()+1, 1, tempLambda.getLine(), 1);
	
	qdot = extract(  &tempLambda, 1,1, qdot.getLine(), 1)+tempInvMFext;
	
	
	Mat<float> t( dt*( S*qdot ) );
	q += t;
	
	//S.print();
	std::cout << " computed Pc : " << std::endl;
	(tConstraintsJacobian*lambda).afficher();
	std::cout << "invM*Fext : " << std::endl;
	tempInvMFext.afficher();
	//temp.afficher();
	//(constraintsJacobian*(invM*Fext)).afficher();
	//(invM*Fext).afficher();
	//invA.afficher();
	lambda.afficher();
	qdot.afficher();
	//tempInvMFext.afficher();
	//tdot.afficher();
	//qdot.afficher();
	//t.afficher();
	//q.afficher();
	
}



