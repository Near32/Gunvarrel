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
		//let us go throught the c and assert if there is really a contact :
		
	}
};





#endif
