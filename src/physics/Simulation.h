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
	
	//-------------------------------------------------------------
	//-------------------------------------------------------------
	
	
	Simulation();
	//Simulation(IConstraintsSolver* cs, IUpdater* u, CollisionDetector* cd);
	//Simulation(Environnement* env);
	
	~Simulation();
	
	
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
							
		}
		
		
	}
	
	void constructQdot()
	{
		std::vector<std_unique_ptr<ISimulationObject> >::iterator o = simulatedObjects.begin();
		
		//there alwas at least one element : the ground :
		qdot = operatorC( o->getLinearVelocity(), o->getAngularVelocity() );
		
		while( o != simulatedObjects.end() )
		{
			q = operatorC( q, 
							operatorC( o->getLinearVelocity(), o->getAngularVelocity()
							);
							
		}
	}
	
	void constructQQdotFext()
	{
		std::vector<std_unique_ptr<ISimulationObject> >::iterator o = simulatedObjects.begin();
		
		//there alwas at least one element : the ground :
		qdot = operatorC( o->getLinearVelocity(), o->getAngularVelocity() );
		
		while( o != simulatedObjects.end() )
		{
			q = operatorC( q, 
							operatorC( o->getLinearVelocity(), o->getAngularVelocity()
							);
							
		}
	}
	
	void majQ(const Mat<float>& add)
	{
		q += add;
	}
	
	void majQdot(const Mat<float>& add)
	{
		qdot += add;
	}
};
#endif
