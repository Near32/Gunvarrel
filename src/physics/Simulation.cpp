#include "Simulation.h"
#include "eval/IUpdater.h"
#include "eval/IConstraintsSolver.h"
#include "eval/CollisionDetector.h"
#include "eval/IIntegrator.h"

Simulation::Simulation() : updater(new Updater(this, new ExplicitEulerIntegrater(this)), constraintsSolver( new SimultaneousImpulseBasedConstraintsSolver(this)), collisionDetector(new CollisionDetector(this))
{

}

Simulation::Simulation(Environnement* env) : Simulation()
{
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
				if( !strcmp( element->name, std::string("ground") ) )
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
					simulatedObjects.end()->setiMass( 1e-10f );//numeric_limit<float>::epsilon() );
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
		
		//TODO : set mass : okay 1.0f by default...
		//TODO : Inertia ...
		Name2ID[element->name] = id;
		id++;
	}
	
	initializedQQdotInvMFext = false;
	invM = SparseMat<float>( 6*id);
	S = SparseMat<float>( 7*id, 6*id);
}




Simulation(Environnement* env, const ConstraintsList& cl) : Simulation(env)
{
	//TODO : handle/enforce constraint list :
	ConstraintsList::iterator itC = cl.begin();
	
	switch(itC.ct)
	{
		case CTHingeJoint :
		collectionC.insert( collectionC.end(), 
							new HingeJointConstraint( *simulatedObjects[Name2ID[itC.nameEl1]], 
														*simulatedObjects[Name2ID[itC.nameEl2]],
														extract(itC.data,1,1, 3,1),
														extract(itC.data,1,2, 3,2)
														)
							);
		
		break;
		
		case CTBallAndSocketJoint :
		collectionC.insert( collectionC.end(), 
							new BallAndSocketJointConstraint( *simulatedObjects[Name2ID[itC.nameEl1]], 
														*simulatedObjects[Name2ID[itC.nameEl2]],
														extract(itC.data,1,1, 3,1),
														extract(itC.data,1,2, 3,2)
														)
							);
		break;
	}
	
	while( itC !=cl.end())
	{		
		itC++;
		
		switch(itC.ct)
		{
			case CTHingeJoint :
			collectionC.insert( collectionC.end(), 
								new HingeJointConstraint( *simulatedObjects[Name2ID[itC.nameEl1]], 
															*simulatedObjects[Name2ID[itC.nameEl2]],
															extract(itC.data,1,1, 3,1),
															extract(itC.data,1,2, 3,2)
															)
								);
		
			break;
		
			case CTBallAndSocketJoint :
			collectionC.insert( collectionC.end(), 
								new BallAndSocketJointConstraint( *simulatedObjects[Name2ID[itC.nameEl1]], 
															*simulatedObjects[Name2ID[itC.nameEl2]],
															extract(itC.data,1,1, 3,1),
															extract(itC.data,1,2, 3,2)
															)
								);
			break;
		}	
	}
}
	

	
Simulation::~Simulation()
{

}

void applyForces(float timeStep)
{
	std::vector<IForceEffect>::iterator itF = collectionF.begin();
	
	RigidBody dummy;
	
	if(itF->isGravity())
	{
		std::vector<std::unique_ptr<ISimulatedObject> >::iterator itRB = simulatedObjects.begin();
		
		itF->Apply(timeStep,*itRB);
		
		while(itRB!=simulatedObjects.end())
		{
			itRB++;
			
			itF->Apply(timeStep,*itRB);
		}
	}
	else
	{
		itF->Apply(timeStep,dummy);
	}
	
	while(itF != collectionF.end())
	{
		itF++;
		
		if(itF->isGravity())
		{
			std::vector<std::unique_ptr<ISimulatedObject> >::iterator itRB = simulatedObjects.begin();
		
			itF->Apply(timeStep,*itRB);
		
			while(itRB!=simulatedObjects.end())
			{
				itRB++;
			
				itF->Apply(timeStep,*itRB);
			}
		}
		else
		{
			itF->Apply(timeStep,dummy);
		}
	}
}
void Simulation::updateQQdot()
{
	std::vector<std_unique_ptr<ISimulationObject> >::iterator o = simulatedObjects.begin();
	
	int b1 = 0;
	int b2 = 0;
	(Rigidody*)o->setPosition( extract(q, b1+1,1, b1+3,1) );
	(Rigidody*)o->setMatOrientation( extract(q, b1+4,1, b1+7,1) );
	(Rigidody*)o->setLinearVelocity( extract( qdot, b2+1,1, b2+3,1);
	(Rigidody*)o->setAngularVelocity( extract( qdot, b2+4,1, b1+6,1);
	b1+=7;
	b2+=6;
	
	while( o!=simulatedObjects.end() )
	{
		o++;
		
		(Rigidody*)o->setPosition( extract(q, b1+1,1, b1+3,1) );
		(Rigidody*)o->setMatOrientation( extract(q, b1+4,1, b1+7,1) );
		(Rigidody*)o->setLinearVelocity( extract( qdot, b2+1,1, b2+3,1);
		(Rigidody*)o->setAngularVelocity( extract( qdot, b2+4,1, b1+6,1);
		b1+=7;
		b2+=6;	
		
	}
	
}


void Simulation::updateStates()
{
	updateQQdot();
	
	std::vector<std::unique_ptr<IElement> >::iterator itEl = env->getIteratorElementsBegin();
	
	std::string name( itEl->name);
	int id = 0;
	if( Name2ID.count( name) )
	{
		id = Name2ID[name];
		itEl.setPose( simulatedObjects[id].getPose() );
	}
	
	while(itEl != env->getIteratorElementsEnd())
	{
		itEl++;
		name = itEl->name;
		
		if( Name2ID.count( name) )
		{
			id = Name2ID[name];
			itEl.setPose( simulatedObjects[id].getPose() );
		}				
	}
	
}
