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
			switch((IElementFixe*)element->isObstacle())
			{
				case true:
				//CAREFUL : HANDLE THE HWD : with BoxShape
				//The element is an obstacle :
				if( !strcmp( name, std::string("ground") ) )
				{
					simulatedObjects.insert( simulatedObjects.end(), new RigidBody( name,id,true) );
					simulatedObjects.end()->setPose( element->pose);
					simulatedObjects.end()->setPtrShape( new BoxShape( simulatedObjects.end(), element->hwd) );
				}
				else
				{
					//then it is the ground :
					simulatedObjects.insert( simulatedObjects.end(), new RigidBody( name,id,true) );
					simulatedObjects.end()->setPose( element->pose);
					simulatedObjects.end()->setPtrShape( new BoxShape( simulatedObjects.end(), element->hwd) );
					
					//IT IS THE UNMOVEABLE GROUND :
					simulatedObjects.end()->isFixed = true;
				}
				
				break;
				
				case false :
				//the element is an OrbeBonus :
				simulatedObjects.insert( simulatedObjects.end(), new RigidBody( name,id,true) );
					simulatedObjects.end()->setPose( element->pose);
					
					//CAREFUL : HANDLE THE RADIUS : with SphereShape
					simulatedObjects.end()->setPtrShape( new SphereShape( simulatedObjects.end(), element->hwd.get(1,1) );
				
				break;
			
			}
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
		
		//TODO : set mass Inertia ...
		id++;
	}
	
	
	invM = SparseMat<float>( 6*id);
	S = SparseMat<float>( 7*id, 6*id);
}
	
Simulation::~Simulation()
{

}
