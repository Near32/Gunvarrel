#ifndef IBROADPHASESTRATEGY_H
#define IBROADPHASESTRATEGY_H

#include "../Simulation.h"
#include "Contact.h"

class IBroadPhaseStrategy 
{
	private :
	
	Simulation* sim;
	
	
	//----------------------------------------
	//----------------------------------------	
	public :
	
	IBroadPhaseStrategy(Simulation* sim_) : sim(sim_)
	{
	
	}
	
	virtual ~IBroadPhaseStrategy()
	{
	
	}
	
	virtual void checkForCollisions(std::vector<Contact>& c) = 0;
};













class BroadPhaseStrategyA
{
	private :
	
	
	
	//----------------------------------------
	//----------------------------------------	
	public :
	
	BroadPhaseStrategyA(Simulation* sim_) : IBroadPhaseStrategy(sim_)
	{
	
	}
	
	virtual ~BroadPhaseStrategyA()
	{
	
	}
	
	virtual void checkForCollisions(std::vector<Contact>& c) override
	{
		//let us access all the RigidBodys in the Simulation and check if they are in contact broadly via the boundingRadius of their Shape :
		std::vector<ISimulationObject>::iterator it = sim->simulatedObjects.begin();
		
		while( it != sim->simulatedObjects.end())
		{
			if( (*it)->getType() == TSORigidBody)
			{
				//let us access to all of the RigidBody that haven't already been checked with regards to this one :
				std::vector<ISimulationObject>::iterator other = it;
				other++;
				
				ShapeType typeIt = (*it)->getShapeType();
				ShapeType typeOther = (*other)->getShapeType();
				
				while( (*other) != sim->simulatedObjects.end())
				{
					/*
					int nbrContact = 0;					
					switch(typeIt)
					{
						case ABSTRACT :
						
						switch(typeOther)
						{
							case ABSTRACT :
							
							break;
						
							case SPHERE :
							
							break;
						
							case PLANE :
							
							break;
						
							case BOX :
							
							break;
						
							case COMPOSIT :
							
							break;
						
							default :
							
							break;
						
						}
						
						break;
						
						case SPHERE :
						
						switch(typeOther)
						{
							case ABSTRACT :
							
							break;
						
							case SPHERE :
							
							break;
						
							case PLANE :
							
							break;
						
							case BOX :
							
							break;
						
							case COMPOSIT :
							
							break;
						
							default :
							
							break;
						
						}
						
						break;
						
						case PLANE :
						
						switch(typeOther)
						{
							case ABSTRACT :
							
							break;
						
							case SPHERE :
							
							break;
						
							case PLANE :
							
							break;
						
							case BOX :
							
							break;
						
							case COMPOSIT :
							
							break;
						
							default :
							
							break;
						
						}
						
						break;
						
						case BOX :
						
						switch(typeOther)
						{
							case ABSTRACT :
							
							break;
						
							case SPHERE :
							
							break;
						
							case PLANE :
							
							break;
						
							case BOX :
							
							break;
						
							case COMPOSIT :
							
							break;
						
							default :
							
							break;
						
						}
						
						break;
						
						case COMPOSIT :
						
						switch(typeOther)
						{
							case ABSTRACT :
							
							break;
						
							case SPHERE :
							
							break;
						
							case PLANE :
							
							break;
						
							case BOX :
							
							break;
						
							case COMPOSIT :
							
							break;
						
							default :
							
							break;
						
						}
						
						break;
						
						default :
						
						switch(typeOther)
						{
							case ABSTRACT :
							
							break;
						
							case SPHERE :
							
							break;
						
							case PLANE :
							
							break;
						
							case BOX :
							
							break;
						
							case COMPOSIT :
							
							break;
						
							default :
							
							break;
						
						}
						
						break;
						
					}
					*/
					
					//------------------
					if( (*other)->getType() == TSORigidBody)
					{
						Mat<float> midline( (*it)->getPosition()-(*other)->getPosition());
						float magnitude = norme2(midline);
						
						if(magnitude < (*it=->getShapeReference().getBRadius() + (*other)->getShapeReference().getBRadius() )
						{
							// then there is a potentiel contact :
							Contact contact;
							contact.rbA = **it;
							contact.rbB = **other;
							contact.contactPoint.insert( contact.contactPoint.end(), (*other)->getPosition()+(1.0f/2.0f)*midline);
							contact.contactNormal.insert( contact.contactNormal.end(), (1.0f/magnitude)*midline);
							
							c.insert(c.end(), contact);
						}
						 
					
					}
					
					other++;
				}
			
			}
		
			it++;
		}
	}
};

#endif
