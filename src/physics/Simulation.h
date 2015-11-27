#ifndef SIMULATION_H
#define SIMULATION_H


#include <memory>
#include "ISimulationObject.h"
#include "IConstraint.h"
#include "IForceEffect.h"

class IUpdater;
class IConstraintsSolver;
class CollisionDetector;

class Simulation
{
	public :
	
	std::vector<std::unique_ptr<ISimulationObject> > simulatedObjects;
	std::vector<std::unique_ptr<IConstaint> > collectionC;
	std::vector<std::unique_ptr<IForceEffect> > collectionF;
	
	std::unique_ptr<IConstraintsSolver> constraintsSolver;
	std::unique_ptr<IUpdater> updater;
	std::unique_ptr<CollisionDetector> collisionDetector;
	
	Mat<float> q;
	Mat<float> qdot;
	Mat<float> Fext;
	
	SparseMat<float> invM;
	SparseMat<float> S;
	
	//-------------------------------------------------------------
	//-------------------------------------------------------------
	
	
	Simulation();
	//Simulation(IConstraintsSolver* cs, IUpdater* u, CollisionDetector* cd);
	Simulation(Environnement* env);
	
	~Simulation();
	
	void run(float timeStep, float endTime)
	{
		float time = 0.0f;
		if(timeStep <= 0.0f)	timeStep = 1e-2f;
		
		if( endTime > 0.0f)
		{
			while( time < endTime)
			{
				runStride( timeStep);
				time += timeStep;
			}
		}
	}
	
	//----------------------------------------------------------------
	//----------------------------------------------------------------
	
	protected :
	
	void runStride( float timeStep)
	{
		//check Collisions and create the corresponding entities to deal with those :
		collisionDetector->checkForCollision(timeStep);
		
		//apply forces to the RigidBodies:
		//TODO
		
		//construct the system to be solved...
		constructQQdotInvMSFext();
		//solve the system and update it :
		constraintsSolver->Solve(timeStep, collectionC, q, qdot, invM, Fext);
		
		//apply changes in the state :		
		//TODO
		
	}
	
	void constructQ()
	{
		std::vector<std_unique_ptr<ISimulationObject> >::iterator o = simulatedObjects.begin();
		
		//there alwas at least one element : the ground :
		q = operatorC( o->getPosition(), o->getMatOrientation() );
		
		while( o != simulatedObjects.end() )
		{
			q = operatorC( q, 
							operatorC( o->getPosition(), o->getMatOrientation()
							);
							
			o++;
							
		}
		
		
	}
	
	void constructQdot()
	{
		std::vector<std_unique_ptr<ISimulationObject> >::iterator o = simulatedObjects.begin();
		
		//there alwas at least one element : the ground :
		qdot = operatorC( o->getLinearVelocity(), o->getAngularVelocity() );
		
		while( o != simulatedObjects.end() )
		{
			qdot = operatorC( qdot, 
							operatorC( o->getLinearVelocity(), o->getAngularVelocity()
							);
			
			o++;				
		}
	}
	
	void constructQQdotInvMSFext()
	{
		std::vector<std_unique_ptr<ISimulationObject> >::iterator o = simulatedObjects.begin();
		
		
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		
		
		//there alwas at least one element : the ground :
		qdot = operatorC( (RigidBody*)o->getLinearVelocity(), (RigidBody*)o->getAngularVelocity() );
		q = operatorC( (RigidBody*)o->getPosition(), (RigidBody*)o->getMatOrientation() );
		
		
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		
		
		if(!(RigidBody*)o->getFixedStatus())
		{
			//then we can initialize Fext :
			Fext = operatorL(	(RigidBody*)o->getForceAccumulator(), 
								(RigidBody*)o->getTorqueAccumulator() 
								);
						
		}
		else
		{
			//then we can initialize Fext :
			Fext = Mat<float>(0.0f, 6,1);
		}
		
		
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		
		//Mat<float> Identity3(0.0f,3,3);
		//for(int i=1;i<=3;i++)	Identity3.set( 1.0f, i,i);
		//Mat<float> Zero3(0.0f,3,3);
		//Mat<float> Zero
		
		//------------------------------------------------------------------
		size_t nbrB = 1;
		//invM.addLine(6);
		//invM.addColumn(6);
		for(int k=1;k<=3;k++)
		{
			invM.set( (nbrB-1)*6+k, (nbrB-1)*6+k, (RigidBody*)o->getIMass() );
		}
		Mat<float> tempIIW( (RigidBody*)o->getInverseInertiaWorld() );
		for(int i=1;i<=3;i++)
		{
			for(int j=1;j<=3;j++)
			{
				invM.set( (nbrB-1)*6+3+i, (nbrB-1)*6+3+j,  tempIIW(i,j) );
			}
		}
		nbrB++;
		
		/*invM = operatorC( 	operatorL( ((RigidBody*)o->getIMass())*Identity, Zero3),
							operatorL( Zero3, (RigidBody*)o->getInverseInertiaWorld() )
							);*/
		
		//------------------------------------------------------------------
		
		//TODO : compute S
		
		
		
		
		
		
		//---------------------------------------------------------------------
		
		
		while( o != simulatedObjects.end() )
		{
			
			qdot = operatorC( 	qdot, 
								operatorC( o->getLinearVelocity(), o->getAngularVelocity()
								);
			
			q = operatorC( 	q, 
							operatorC( o->getPosition(), o->getMatOrientation()
							);
			
			
			//------------------------------------------------------------------
			//------------------------------------------------------------------
			
			
			
			if(!(RigidBody*)o->getFixedStatus())
			{
				Fext = operatorL(	Fext,
									operatorL(	(RigidBody*)o->getForceAccumulator(), 
												(RigidBody*)o->getTorqueAccumulator() 
												)
									);
			}
			else
			{
				Fext = operatorL(Fext, Mat<float>(0.0f, 6,1) );
			}
			
			
			//------------------------------------------------------------------
			//------------------------------------------------------------------
			
			//invM.addLine(6);
			//invM.addColumn(6);
			for(int k=1;k<=3;k++)
			{
				invM.set( (nbrB-1)*6+k, (nbrB-1)*6+k, (RigidBody*)o->getIMass() );
			}
			tempIIW = (RigidBody*)o->getInverseInertiaWorld() ;
			for(int i=1;i<=3;i++)
			{
				for(int j=1;j<=3;j++)
				{
					invM.set( (nbrB-1)*6+3+i, (nbrB-1)*6+3+j,  tempIIW(i,j) );
				}
			}
			nbrB++;
			
			/*
			invM = operator(operatorC( 	operatorL( ((RigidBody*)o->getIMass())*Identity, Zero3),
							operatorL( Zero3, (RigidBody*)o->getInverseInertiaWorld() )
							);			
			*/
			
			//------------------------------------------------------------------
			//------------------------------------------------------------------
		
			//TODO : compute S
		
		
		
		
		
		
			//---------------------------------------------------------------------
			//---------------------------------------------------------------------
			
			
			o++;
		}
	}
	
	
	/*
	void majQ(const Mat<float>& add)
	{
		q += add;
	}
	
	void majQdot(const Mat<float>& add)
	{
		qdot += add;
	}
	*/
	
	
};
#endif
