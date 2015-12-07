#include "IBroadPhaseStrategy.h"
#include "../Simulation.h"

IBroadPhaseStrategy::IBroadPhaseStrategy(Simulation* sim_) : sim(sim_)
{

}

IBroadPhaseStrategy::~IBroadPhaseStrategy()
{

}

//--------------------------------------------
//--------------------------------------------
//--------------------------------------------

//--------------------------------------------
//--------------------------------------------
//--------------------------------------------

BroadPhaseStrategyA::BroadPhaseStrategyA(Simulation* sim_) : IBroadPhaseStrategy(sim_)
{

}

BroadPhaseStrategyA::~BroadPhaseStrategyA()
{

}

void BroadPhaseStrategyA::checkForCollisions(std::vector<Contact>& c)
{
	//let us access all the RigidBodys in the Simulation and check if they are in contact broadly via the boundingRadius of their Shape :
	std::vector<std::unique_ptr<ISimulationObject> >::iterator it = sim->simulatedObjects.begin();
	
	while( it != sim->simulatedObjects.end())
	{
		if( (*it)->getType() == TSORigidBody)
		{
			if( ((RigidBody&)(*(*it))).getCollisionStatus() )
			{
				//let us access to all of the RigidBody that haven't already been checked with regards to this one :
				std::vector<std::unique_ptr<ISimulationObject> >::iterator other = it;
				other++;
			
				ShapeType typeIt = ((RigidBody&)(*(*it))).getShapeType();
				
			
				while( other != sim->simulatedObjects.end())
				{
					ShapeType typeOther = ((RigidBody&)(*(*other))).getShapeType();
					//------------------
					if( ((RigidBody&)(*(*other))).getType() == TSORigidBody)
					{
						if( ((RigidBody&)(*(*other))).getCollisionStatus() )
						{
							Mat<float> midline( ((RigidBody&)(*(*it))).getPosition()-((RigidBody&)(*(*other))).getPosition());
							float magnitude = norme2(midline);
					
							if(magnitude < ((RigidBody&)(*(*it))).getShapeReference().getBRadius() + ((RigidBody&)(*(*other))).getShapeReference().getBRadius() )
							{
								// then there is a potentiel contact :
								Contact contact( (RigidBody*)(it->get()), (RigidBody*)(other->get()) );
								contact.contactPoint.insert( contact.contactPoint.end(), ((RigidBody&)(*(*other))).getPosition()+(1.0f/2.0f)*midline);
								contact.normal.insert( contact.normal.end(), (1.0f/magnitude)*midline);
						
								c.insert(c.end(), contact );
							}
						}
						 
				
					}
				
					other++;
				}
			}
		
		}
	
		it++;
	}
}
