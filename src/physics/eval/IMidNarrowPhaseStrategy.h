#ifndef IMIDNARROWPHASESTRATEGY_H
#define IMIDNARROWPHASESTRATEGY_H

#include "Contact.h"
#include "../Simulation.h"
#include "HelperFunctions.h"



class IMidNarrowPhaseStrategy 
{
	private :
	
	Simulation* sim;
	
	
	//----------------------------------------
	//----------------------------------------	
	public :
	
	IMidNarrowPhaseStrategy(Simulation* sim_) : sim(sim_)
	{
	
	}
	
	virtual ~IMidNarrowPhaseStrategy()
	{
	
	}
	
	virtual void checkForCollisions(std::vector<Contact>& c) = 0;
};





class MidNarrowPhaseStrategyA 
{
	private :
	
	
	//----------------------------------------
	//----------------------------------------	
	public :
	
	MidNarrowPhaseStrategyA(Simulation *sim_) : IMidNarrowPhaseStrategy(sim_)
	{
	
	}
	
	virtual ~MidNarrowPhaseStrategyA()
	{
	
	}
	
	virtual void checkForCollisions(std::vector<Contact>& c) override
	{
		//let us iterate over the contact set :
		std::vector<IContact>::iterator contact = c.begin();
		
		while( contact != c.end())
		{
			ShapeType typeA = contact->rbA.getShapeType();
			ShapeType typeB = contact->rbB.getShapeType();
						
			int nbrContact = 0;					
			switch(typeA)
			{
				case ABSTRACT :
				//TODO : exception ?
				switch(typeB)
				{
					case ABSTRACT :
					//TODO : exception ?
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
				
				switch(typeB)
				{
					case ABSTRACT :
					//TODO : exception ?
					break;
				
					case SPHERE :
					//nothing to do, it has already been handled if a bounding sphere has been used.
					//TODO : handle the case when the IBroadPhaseStrategy is different of the one using spheres...
					break;
				
					case PLANE :
					//let us compute the distance between the center of the sphere and the plane :
					
					
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
				
				switch(typeB)
				{
					case ABSTRACT :
					//TODO : exception ?
					break;
				
					case SPHERE :
					// 
					break;
				
					case PLANE :
					// let us think of it as a really thin box :
					//TODO : handle the infinity size ?
					break;
				
					case BOX :
					
					bool testIntersection = testOBBPlane( (*contact)->rbB, (*contact)->rbA);
					if(testIntersection)
					{
						//the plane and the box are intersecting : let us fill in the Contact structure :
						
					}
					else
					{
						//the plane and the box are not intersecting.
						c.erase(contact);
					}
					
					break;
				
					case COMPOSIT :
					
					break;
				
					default :
					
					break;
				
				}
				
				break;
				
				case BOX :
				
				switch(typeB)
				{
					case ABSTRACT :
					//TODO :
					break;
				
					case SPHERE :
					
					break;
				
					case PLANE :
					
					bool testIntersection = testOBBPlane( (*contact)->rbA, (*contact)->rbB);
					if(testIntersection)
					{
						//the plane and the box are intersecting : let us fill in the Contact structure :
						
					}
					else
					{
						//the plane and the box are not intersecting.
						c.erase(contact);
					}
					break;
				
					case BOX :
					
					bool testIntersection = false;
					Mat<float> intersectingPointsWOfA( testOBBOBB( (*contact)->rbA, (*contact)->rbB) );
					
					if(testIntersection)
					{
						//the two boxes are intersecting : let us fill in the Contact structure :
						
					}
					else
					{
						//the two boxes are not intersecting.
						c.erase(contact);
					}
					break;
				
					case COMPOSIT :
					
					break;
				
					default :
					
					break;
				
				}
				
				break;
				
				case COMPOSIT :
				
				switch(typeB)
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
				
				switch(typeB)
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
			
			contact++;
		}
		
	}
};





#endif
