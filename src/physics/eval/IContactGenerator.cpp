#include "IContactGenerator.h"
#include "../Simulation.h"

IContactGenerator::IContactGenerator(Simulation* sim_) : sim(sim_)
{

}

IContactGenerator::~IContactGenerator()
{

}

//----------------------------
//----------------------------
//----------------------------

//----------------------------
//----------------------------
//----------------------------


ContactGeneratorA::ContactGeneratorA(Simulation *sim_) : IContactGenerator(sim_)
{

}

ContactGeneratorA::~ContactGeneratorA()
{

}

void ContactGeneratorA::generateContactConstraints(std::vector<Contact>& c)
{
	//let us create a ContactConstraint in the simulation for every contact in c in order to have penetrationDepth :
	std::vector<Contact>::iterator contact = c.begin();
	int size = c.size();
	float* pD = (float*)malloc(sizeof(float)*size);
	int i = 0;
	while(contact != c.end() )
	{
		//TODO : retrieve the penetration depths...
		//in the mean time :
		pD[i] = 1.0f;
				
		contact++;
		i++;
	}
	
	//--------------------------------------
	//let us create the SpringForceEffect required into the simulation with endTime being the next simulation step time :
	for(int i=size;i--;)
	{
		sim->collectionF.insert( sim->collectionF.end(), std::unique_ptr<IForceEffect>( new SpringForceEffect(c[i].contactPoint[0], c[i].contactPoint[1], *(c[i].rbA), *(c[i].rbB), pD[i] ) ) );
	}
	
}