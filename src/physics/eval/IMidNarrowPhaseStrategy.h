#ifndef IMIDNARROWPHASESTRATEGY_H
#define IMIDNARROWPHASESTRATEGY_H

#include "Contact.h"



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
				
				case SPHERE :
				
				switch(typeB)
				{
					case ABSTRACT :
					
					break;
				
					case SPHERE :
					//nothing to do.
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
				
				case BOX :
				
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
