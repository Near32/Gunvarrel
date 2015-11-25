#ifndef ICONTACTGENERATOR_H
#define ICONTACTGENERATOR_H

#include "Contact.h"
#include "../Simulation.h"



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




/*This contact generator turns every collision/contact into a SpringForceEffect between the two bodies and the two connections points are used as anchors. The restLength of the spring will be 2 times the value of the penetrationDepth that is to be computed here, and the springConstant will be set by default to 1.0f, for starters...*/
/*WARNING : endTime's of those IForceEffect are to be set to the next simulation time.*/

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
		//let us create a ContactConstraint in the simulation for every contact in c in order to have penetrationDepth :
		std::vector<Contact>::iterator contact = c.begin();
		int size = c.size();
		float* pD = (float*)malloc(sizeof(float)*size);
		int i = 0;
		while(contact != c.end() )
		{
			//TODO : retrieve the penetration depths...
			
			pD[i] = 1.0f;
			contact++;
			i++;
		}
		
		//--------------------------------------
		//let us create the SpringForceEffect required into the simulation with endTime being the next simulation step time :
		
		
	}
};



#endif
