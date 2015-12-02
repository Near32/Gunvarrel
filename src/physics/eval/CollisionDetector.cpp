#include "CollisionDetector.h"
#include "../Simulation.h"

CollisionDetector::CollisionDetector(Simulation* sim_, float eps) : sim(sim_), epsilon(eps), broadPhase(new BroadPhaseStrategyA( sim)), midNarrowPhase(new MidNarrowPhaseStrategyA( sim)), contactGenerator( new ContactGeneratorA( sim))
{

}

CollisionDetector::~CollisionDetector()
{

}

void CollisionDetector::checkForCollision(float dt)
{
	contacts.clear();
	
	broadPhase->checkForCollisions(contacts);
	midNarrowPhase->checkForCollisions(contacts);
	contactGenerator->generateContactConstraints(contacts);
}



//-------------------------------

std::vector<Contact> CollisionDetector::getContacts()
{	
	return contacts;	
}
