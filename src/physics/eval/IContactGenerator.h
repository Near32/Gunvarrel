#ifndef ICONTACTGENERATOR_H
#define ICONTACTGENERATOR_H

#include "Contact.h"



class IContactGenerator
{
	private :
	
	Simulation* sim;
	
	
	//----------------------------------------
	//----------------------------------------	
	public :
	
	IContactGenerator(Simulation* sim_) : sim(sim_)
	{
	
	}
	
	virtual ~IContactGenerator()
	{
	
	}
	
	virtual void generateContactConstraints(std::vector<Contact>& c) = 0;
};





class ContactGeneratorA
{
	private :
	
	Simulation* sim;
	
	
	//----------------------------------------
	//----------------------------------------	
	public :
	
	ContactGeneratorA(Simulation *sim_) : IContactGenerator(sim_)
	{
	
	}
	
	virtual ~ContactGeneratorA()
	{
	
	}
	
	virtual void generateContactConstraints(std::vector<Contact>& c) override
	{
		//let us create a ContactConstraint in the simulation for every contact in c :
		
	}
};



#endif
