#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include "../Simulation.h"

#include "IBroadPhaseStrategy.h"
#include "IMidNarrowPhaseStrategy.h"
#include "IContactGenerator.h"

#include "Contact.h"


class CollisionDetector
{
	private :
	
	std::shared_ptr<Simulation> sim;
	
	float epsilon;
	std::unique_ptr<IBroadPhaseStrategy> broadPhase;
	std::unique_ptr<IMidNarrowPhaseStrategy> midNarrowPhase;
	std::unique_ptr<IContactGenerator> contactGenerator;
	
	std::vector<Contact> contacts;
	//------------------------------
	//------------------------------
	
	public :
	
	CollisionDetector(Simulation* sim_, float eps = (float)1e-10) : sim(sim_), epsilon(eps)
	{
		broadPhase = new BroadPhaseStrategyA( sim);
		midNarrowPhase = new MidNarrowPhaseStrategyA( sim);
		contactGenerator = new ContactGeneratorA( sim);
	}
	
	~CollisionDetector()
	{
	
	}
	
	void checkForCollision(float dt = 0.0001f)
	{
		contacts.clear();
		
		broadPhase.checkForCollisions(contacts);
		midNarrowPhase.checkForCollisions(contacts);
		contactGenerator.generateContactConstraints(contacts);
	}
	
	
	
	//-------------------------------
	
	std::vector<Contact> getContacts()	{	return contacts;	} 	
	
	
};

#endif
