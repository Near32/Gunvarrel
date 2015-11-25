#include "Simulation.h"
#include "eval/IUpdater.h"
#include "eval/IConstraintsSolver.h"
#include "eval/CollisionDetector.h"
#include "eval/IIntegrator.h"

Simulation::Simulation() : updater(new Updater(this, new ExplicitEulerIntegrater(this)), constraintsSolver( new SimultaneousImpulseBasedConstraintsSolver(this)), collisionDetector(new CollisionDetector(this))
{

}

Simulation::Simulation(Environnement* env)
{
	Simulation();
	
	//--------------------------
	int id = 0;
	
	
	std::vector<std::unique_ptr<IElement> >::iterator element = env->ListeElements.begin();
	
	while( element != env->ListeElements.end() )
	{
		// let us create the RigidBody that ought to be created given the Environnement :
		
		switch( element->isFixe() )
		{
			case true :
			//the element is an IElementFixe :
			
			break;
			
			case false :
			//the element is an IElementMobile :
			switch( (IElementMobile)element->isComposit() )
			{
				case true :
				
				break;
				
				case false:
				
				break;
				
				
			}
			break;
			
		}
	}
	
}
	
Simulation::~Simulation()
{

}
