#include "IContactGenerator.h"
#include "../Simulation.h"

#define debug


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


ContactGeneratorA::ContactGeneratorA(Simulation* sim_) : IContactGenerator(sim_)
{

}

ContactGeneratorA::~ContactGeneratorA()
{

}

void ContactGeneratorA::generateContactConstraints(std::vector<Contact>& c)
{
	//let us create a ContactConstraint in the simulation for every contact in c in order to have penetrationDepth :
#ifdef debug
std::cout << "COLLISION DETECTOR : CONTACT GENERATOR : initialization : ..." << std::endl;
#endif
	
	std::vector<Contact>::iterator contact = c.begin();
	int size = c.size();
	float* pD = (float*)malloc(sizeof(float)*size);
	
#ifdef debug
std::cout << "COLLISION DETECTOR : CONTACT GENERATOR : initialization : OKAY." << std::endl;
#endif	
	int i = 0;
	
#ifdef debug
std::cout << "COLLISION DETECTOR : CONTACT GENERATOR : penetration depths computations : ..." << std::endl;
#endif	
	while(contact != c.end() )
	{
		//TODO : retrieve the penetration depths...
		//in the mean time : we assume that we want the body's origin to be separated of the sum of the BRadius.
		pD[i] = 1.0f;
		//pD[i] = contact->rbA->getShapeReference().getBRadius()+contact->rbB->getShapeReference().getBRadius();
				
		contact++;
		i++;
	}

#ifdef debug
std::cout << "COLLISION DETECTOR : CONTACT GENERATOR : contact forces creations : ..." << std::endl;
#endif	
	//--------------------------------------
	//let us create the SpringForceEffect required into the simulation with endTime being the next simulation step time :
	
	for(;i--;)
	{
		sim->collectionF.insert( sim->collectionF.begin(), std::unique_ptr<IForceEffect>( new SpringForceEffect( /*in LOCAL : c[i].rbA->getPosition()*/c[i].rbA->getPointInLocal(c[i].contactPoint[0]), /*in LOCAL : c[i].rbB->getPosition()*/c[i].rbB->getPointInLocal(c[i].contactPoint[0]), *(c[i].rbA), *(c[i].rbB), pD[i] ) ) );
		//sim->collectionF[sim->collectionF.size()-1]->setEndTime( sim->getTime()+sim->getTimeStep());
		sim->collectionF[0]->setEndTime( sim->getTime()+sim->getTimeStep());
		//this collision force is only here for one timestep and it will be recreated if needed on the next time step.
	}

	
}
