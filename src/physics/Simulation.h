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
	
	
	//-------------------------------------------------------------
	//-------------------------------------------------------------
	
	
	Simulation();
	//Simulation(IConstraintsSolver* cs, IUpdater* u, CollisionDetector* cd);
	//Simulation(Environnement* env);
	
	~Simulation();
	
	
};
#endif
