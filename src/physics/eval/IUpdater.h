#ifndef IUPDATER_H
#define IUPDATER_H

#include "../Simulation.h"

#include "IIntegrator.h"

class IUpdater
{
	private :
	
	std::shared_ptr<Simulation> simulation;	
	std::unique_ptr<IIntegrator> integrator;
	
	
	//----------------------------------------------
	//----------------------------------------------
	public :
	
	IUpdater(Simulation* sim, IIntegrator* integ) : simulation(sim), integrator( integ )
	{
	
	}
	
	~IUpdater()
	{
	
	}
	
	
		
};


class Updater : public IUpdater
{
	public :
	
	Updater(Simulation* sim, IIntegrator* integ) : IUpdater(sim,integ)
	{
	
	}
	
	~Updater()
	{
	
	}
};



#endif
