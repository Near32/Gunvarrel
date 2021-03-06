#include "Simulation.h"
#include "eval/IUpdater.h"
#include "eval/IConstraintSolverStrategy.h"
#include "eval/CollisionDetector.h"
#include "eval/IIntegrator.h"

#include <mutex>

//#define debug
//#define debuglvl1
//#define debuglvl2

#define benchmark

extern mutex ressourcesMutex;

Simulation::Simulation() : time(0.0f), updater(new Updater(this, new ExplicitEulerIntegrator(this)) ), constraintsSolver( new SimultaneousImpulseBasedConstraintSolverStrategy(this) ), collisionDetector( new CollisionDetector(this) )
{
	Mat<float> g(0.0f,3,1);
	g.set( -9.81f,3,1);
	collectionF.insert( collectionF.begin(), std::unique_ptr<IForceEffect>(new GravityForceEffect(g))  );
	
#ifdef debug
	std::cout << "SIMULATION : minimal initialization : OKAY." << std::endl;
#endif	
	SimulationTimeStep = 5e-1f;
}


Simulation::Simulation(Environnement* env_) : Simulation()
{
	int id = 0;
	env = env_; 
	
	for( auto& element : env->ListeElements )
	{
		// let us create the RigidBody that ought to be created given the Environnement :
		
		switch( element->isFixe() )
		{
			case true :
			//the element is an IElementFixe :					
			switch( ((IElementFixe*)element.get())->isObstacle())
			{
				case true:
				//CAREFUL : HANDLE THE HWD : with BoxShape
				//The element is an obstacle :
				if( !( element->getName() == std::string("ground") ) )
				{
					simulatedObjects.insert( simulatedObjects.end(), std::unique_ptr<ISimulationObject>(new RigidBody( element->getPoseReference(), element->getName(),id, BOX) ) );
					//((RigidBody&)(*simulatedObjects[id])).setPose( (*element)->getPoseReference());
					//((RigidBody&)(*simulatedObjects[id])).setPtrShape( (IShape*)(new BoxShape( (RigidBody*)simulatedObjects[id].get(), ((IElementFixe&)(*(*element))).hwd)) );		
					((BoxShape&)((RigidBody*)(simulatedObjects[id].get()))->getShapeReference()).setHWD( ((IElementFixe*)(element.get()))->hwd );
#ifdef debug
std::cout << "SIMULATION : environnement initialization : ElementFixe : Obstacle : id = " << id << " : ...." << std::endl;
#endif					
				}
				else
				{
					//then it is the ground :
					simulatedObjects.insert( simulatedObjects.end(), std::unique_ptr<ISimulationObject>(new RigidBody( element->getPoseReference(), element->getName(),id,true) ) );
					//((RigidBody&)(*simulatedObjects[id])).setPose( (*element)->getPoseReference());
					((RigidBody*)(simulatedObjects[id].get()))->setPtrShape( (IShape*)(new BoxShape( (RigidBody*)simulatedObjects[id].get(), ((IElementFixe*)(element.get()))->hwd)) );
					
					//IT IS THE UNMOVEABLE GROUND :
					((RigidBody*)(simulatedObjects[id].get()))->isFixed = true;
					((RigidBody*)(simulatedObjects[id].get()))->setIMass( 1e-20f );//numeric_limit<float>::epsilon() );
					
					
#ifdef debug
	std::cout << "SIMULATION : environnement initialization : GROUND : id = " << id << " : OKAY." << std::endl;
#endif
				}
				
				
				break;
				
				case false :
				//the element is an OrbeBonus :
				simulatedObjects.insert( simulatedObjects.end(), std::unique_ptr<ISimulationObject>(new RigidBody( element->getPoseReference(), element->getName(),id,true) ) );
				//((RigidBody&)(*simulatedObjects[id])).setPose( (*element)->getPoseReference());
					
					//CAREFUL : HANDLE THE RADIUS : with SphereShape
				((RigidBody*)(simulatedObjects[id].get()))->setPtrShape( (IShape*)(new SphereShape( (RigidBody*)simulatedObjects[id].get(), ((IElementMobile*)(element.get()))->hwd.get(1,1) )) );
#ifdef debug
std::cout << "SIMULATION : environnement initialization : ElementFixe OrbeBonus : id = " << id << " : ...." << std::endl;
#endif
				
				break;
			
			}
			break;
			
			case false :
			//the element is an IElementMobile :
			switch( ((IElementMobile*)(element.get()))->isComposit() )
			{
				case true :
				
				break;
				
				case false:
				//CAREFUL : HANDLE THE HWD : with BoxShape
				simulatedObjects.insert( simulatedObjects.end(), std::unique_ptr<ISimulationObject>(new RigidBody( element->getPoseReference(), element->getName(),id, BOX) ) );
				//((RigidBody&)(*simulatedObjects[id])).setPose( (*element)->getPoseReference());
				//((RigidBody&)(*simulatedObjects[id])).setPtrShape( (IShape*)(new BoxShape( (RigidBody*)simulatedObjects[id].get(), ((IElementFixe&)(*(*element))).hwd)) );
#ifdef debug
std::cout << "SIMULATION : environnement initialization : ElementMobile : id = " << id << " : ...." << std::endl;
#endif					
				((BoxShape&)((RigidBody*)(simulatedObjects[id].get()))->getShapeReference()).setHWD( ((IElementMobile*)(element.get()))->hwd );
			
#ifdef benchmark			
					
				if(element->getName() == std::string("picHAUT"))
				{
					//let us put an initial velocity :
					((RigidBody*)(simulatedObjects[id].get()))->setLinearVelocity( Mat<float>(100.0f,3,1) );
					((RigidBody*)(simulatedObjects[id].get()))->setIMass( 1e2f );
					//((RigidBody*)(simulatedObjects[id].get()))->setMass( 1e-2f );
				}
#endif
				
				break;
				
				
			}
			break;
			
		}
		
		//TODO : set mass : okay 1.0f by default...
		//TODO : Inertia ...
		Name2ID[ element->getName() ] = id;
		//std::cout << id << " : " << element->getName() << " == " << Name2ID[element->getName()] << std::endl;
		id++;
#ifdef debug
	std::cout << "SIMULATION : post-increment : id = " << id << " : ...." << std::endl;
#endif
		
	}
	
	initializedQQdotInvMFext = false;
	invM = SparseMat<float>( 6*id);
	S = SparseMat<float>( 7*id, 6*id);
	
#ifdef debug
	std::cout << "SIMULATION : environnement initialization : OKAY." << std::endl;
#endif	
}


/*
Simulation::Simulation(Environnement* env) : Simulation()
{
	int id = 0;
	
	
	std::vector<std::unique_ptr<IElement> >::iterator element = env->ListeElements.begin();
	
	while( element != env->ListeElements.end() )
	{
		// let us create the RigidBody that ought to be created given the Environnement :
		
		switch( (*element)->isFixe() )
		{
			case true :
			//the element is an IElementFixe :					
			switch( ((IElementFixe&)(*(*element))).isObstacle())
			{
				case true:
				//CAREFUL : HANDLE THE HWD : with BoxShape
				//The element is an obstacle :
				if( !( (*element)->getName() == std::string("ground") ) )
				{
					simulatedObjects.insert( simulatedObjects.end(), std::unique_ptr<ISimulationObject>(new RigidBody( (*element)->getPoseReference(), (*element)->getName(),id, BOX) ) );
					//((RigidBody&)(*simulatedObjects[id])).setPose( (*element)->getPoseReference());
					//((RigidBody&)(*simulatedObjects[id])).setPtrShape( (IShape*)(new BoxShape( (RigidBody*)simulatedObjects[id].get(), ((IElementFixe&)(*(*element))).hwd)) );		
					((BoxShape&)((RigidBody&)(*simulatedObjects[id])).getShapeReference()).setHWD( ((IElementFixe&)(*(*element))).hwd );
#ifdef debug
std::cout << "SIMULATION : environnement initialization : ElementFixe : Obstacle : id = " << id << " : ...." << std::endl;
#endif					
				}
				else
				{
					//then it is the ground :
					simulatedObjects.insert( simulatedObjects.end(), std::unique_ptr<ISimulationObject>(new RigidBody( (*element)->getPoseReference(), (*element)->getName(),id,true) ) );
					//((RigidBody&)(*simulatedObjects[id])).setPose( (*element)->getPoseReference());
					((RigidBody&)(*simulatedObjects[id])).setPtrShape( (IShape*)(new BoxShape( (RigidBody*)simulatedObjects[id].get(), ((IElementFixe&)(*(*element))).hwd)) );
					
					//IT IS THE UNMOVEABLE GROUND :
					((RigidBody&)(*simulatedObjects[id])).isFixed = true;
					((RigidBody&)(*simulatedObjects[id])).setIMass( 1e-10f );//numeric_limit<float>::epsilon() );
					
#ifdef debug
	std::cout << "SIMULATION : environnement initialization : GROUND : id = " << id << " : OKAY." << std::endl;
#endif
				}
				
				
				break;
				
				case false :
				//the element is an OrbeBonus :
				simulatedObjects.insert( simulatedObjects.end(), std::unique_ptr<ISimulationObject>(new RigidBody( (*element)->getPoseReference(), (*element)->getName(),id,true) ) );
				//((RigidBody&)(*simulatedObjects[id])).setPose( (*element)->getPoseReference());
					
					//CAREFUL : HANDLE THE RADIUS : with SphereShape
				((RigidBody&)(*simulatedObjects[id])).setPtrShape( (IShape*)(new SphereShape( (RigidBody*)simulatedObjects[id].get(), ((IElementMobile&)(*(*element))).hwd.get(1,1) )) );
#ifdef debug
std::cout << "SIMULATION : environnement initialization : ElementFixe OrbeBonus : id = " << id << " : ...." << std::endl;
#endif
				
				break;
			
			}
			break;
			
			case false :
			//the element is an IElementMobile :
			switch( ((IElementMobile&)(*(*element))).isComposit() )
			{
				case true :
				
				break;
				
				case false:
				//CAREFUL : HANDLE THE HWD : with BoxShape
				simulatedObjects.insert( simulatedObjects.end(), std::unique_ptr<ISimulationObject>(new RigidBody( (*element)->getPoseReference(), (*element)->getName(),id, BOX) ) );
				//((RigidBody&)(*simulatedObjects[id])).setPose( (*element)->getPoseReference());
				//((RigidBody&)(*simulatedObjects[id])).setPtrShape( (IShape*)(new BoxShape( (RigidBody*)simulatedObjects[id].get(), ((IElementFixe&)(*(*element))).hwd)) );
#ifdef debug
std::cout << "SIMULATION : environnement initialization : ElementMobile : id = " << id << " : ...." << std::endl;
#endif					
				((BoxShape&)((RigidBody&)(*simulatedObjects[id])).getShapeReference()).setHWD( ((IElementMobile&)(*(*element))).hwd );
				
				
				break;
				
				
			}
			break;
			
		}
		
		//TODO : set mass : okay 1.0f by default...
		//TODO : Inertia ...
		Name2ID[ (*element)->getName() ] = id;
		id++;
#ifdef debug
	std::cout << "SIMULATION : post-increment : id = " << id << " : ...." << std::endl;
#endif
		
		element++;
	}
	
	initializedQQdotInvMFext = false;
	invM = SparseMat<float>( 6*id);
	S = SparseMat<float>( 7*id, 6*id);
	
#ifdef debug
	std::cout << "SIMULATION : environnement initialization : OKAY." << std::endl;
#endif	
}*/




Simulation::Simulation(Environnement* env_, ConstraintsList& cl) : Simulation(env_)
{
	//TODO : handle/enforce constraint list :
	//ConstraintsList::iterator itC = cl.begin();

	for( auto& itC : cl)
	{				
		
		switch( itC.ct)
		{
			case CTHingeJoint :
			collectionC.insert( collectionC.end(), 
								std::unique_ptr<IConstraint>(new HingeJoint( *((RigidBody*)simulatedObjects[Name2ID[ itC.nameEl1]].get()), 
															*((RigidBody*)simulatedObjects[Name2ID[ itC.nameEl2]].get()),
															extract( itC.data,1,1, 3,1),
															extract( itC.data,1,2, 3,2)
															))
								);
		
			break;
		
			case CTBallAndSocketJoint :
			collectionC.insert( collectionC.end(), 
								std::unique_ptr<IConstraint>(new BallAndSocketJoint( *((RigidBody*)simulatedObjects[Name2ID[ itC.nameEl1]].get()), 
															*((RigidBody*)simulatedObjects[Name2ID[ itC.nameEl2]].get()),
															extract( itC.data,1,1, 3,1),
															extract( itC.data,1,2, 3,2)
															))
								);
			break;
		}	
	}
	
#ifdef debug
	std::cout << "SIMULATION : constraint on rigidbodies initialization : OKAY." << std::endl;
#endif	

}
	

	
Simulation::~Simulation()
{

}




void Simulation::run(float timeStep, float endTime)
{
	if(timeStep <= 0.0f)	timeStep = 1e-4f;
	
	SimulationTimeStep = timeStep;
	
	if( endTime > time)
	{
		while( time < endTime)
		{
			ressourcesMutex.lock();
			//clock_t time = clock();
			ressourcesMutex.unlock();
			
			runStride( timeStep);
			time += timeStep;
			
			ressourcesMutex.lock();
			//std::cout << "SIMULATION : " << timeStep << "seconds took : " << (float)(clock()-time)/CLOCKS_PER_SEC << " seconds of computation. " << std::endl;
			ressourcesMutex.unlock();
		}
	}
}

//----------------------------------------------------------------
//----------------------------------------------------------------

void Simulation::runStride( float timeStep)
{
	//check Collisions and create the corresponding entities to deal with those :
#ifdef debug
std::cout << "SIMULATION : runStride : collision check : ..." << std::endl;
#endif	
	collisionDetector->checkForCollision(timeStep);
#ifdef debug
std::cout << "SIMULATION : runStride : collision check : DONE." << std::endl;
#endif		
	//apply forces to the RigidBodies:
#ifdef debug
std::cout << "SIMULATION : runStride : apply Forces : ..." << std::endl;
#endif		
	applyForces(timeStep);
#ifdef debug
std::cout << "SIMULATION : runStride : apply Forces : DONE" << std::endl;
#endif			
	//construct the system to be solved...
	if(initializedQQdotInvMFext)
	{
		//TODO : update QQdotInvMFext, for now on, we do not care at all about reallocation and optimization, so the matrixes are being reconstructed every time...
#ifdef debug
std::cout << "SIMULATION : runStride : updating matrices : ..." << std::endl;
#endif					
		updateInvMSFext();
	}
	else
	{
#ifdef debug
std::cout << "SIMULATION : runStride : initializing matrices : ..." << std::endl;
#endif			
		constructQQdotInvMSFext();
#ifdef debug
std::cout << "SIMULATION : runStride : initializing matrices : DONE." << std::endl;
#endif					
	}
	//solve the system and update it :
#ifdef debug
std::cout << "SIMULATION : runStride : solving system : ..." << std::endl;
#endif				
	constraintsSolver->Solve(timeStep, collectionC, q, qdot, invM,S, Fext);
#ifdef debug
std::cout << "SIMULATION : runStride : solving system : DONE." << std::endl;
#endif					
	//apply changes in the state :		
	updateStates();
	
}

void Simulation::constructQ()
{
	std::vector<std::unique_ptr<ISimulationObject> >::iterator o = simulatedObjects.begin();
	
	//there alwas at least one element : the ground :
	q = operatorC( ((RigidBody&)(*o)).getPosition(), ((RigidBody&)(*o)).getMatOrientation() );
	
	while( o != simulatedObjects.end() )
	{
		q = operatorC( q, 
						operatorC( ((RigidBody&)(*o)).getPosition(), ((RigidBody&)(*o)).getMatOrientation()
						)
						);
						
		o++;
						
	}
	
	
}

void Simulation::constructQdot()
{
	std::vector<std::unique_ptr<ISimulationObject> >::iterator o = simulatedObjects.begin();
	
	//there alwas at least one element : the ground :
	qdot = operatorC( ((RigidBody&)(*o)).getLinearVelocity(), ((RigidBody&)(*o)).getAngularVelocity() );
	
	while( o != simulatedObjects.end() )
	{
		qdot = operatorC( qdot, 
						operatorC( ((RigidBody&)(*o)).getLinearVelocity(), ((RigidBody&)(*o)).getAngularVelocity()
						)
						);
		
		o++;				
	}
}

void Simulation::constructQQdotInvMSFextDEBUG()
{
	//TODO : uncomment once the other strategy is set on track...
	//initializedQQdotInvMFext = true;
	
	std::vector< std::unique_ptr< ISimulationObject> >::const_iterator o = simulatedObjects.cbegin();
	
	
	//------------------------------------------------------------------
	//------------------------------------------------------------------
	
	//there alwas at least one element : the ground :
	//qdot = operatorC( ((IMoveable&)(*o)).getLinearVelocity(), ((IMoveable&)(*o)).getAngularVelocity() );
	ressourcesMutex.lock();
	qdot = operatorC( ((RigidBody*)(o->get()))->getLinearVelocity(), ((RigidBody*)(o->get()))->getAngularVelocity() );
	q = operatorC( ((RigidBody*)(o->get()))->getPosition(), ((RigidBody*)(o->get()))->getMatOrientation() );
	ressourcesMutex.unlock();
	
	//------------------------------------------------------------------
	//------------------------------------------------------------------
	
	ressourcesMutex.lock();
	if(!((RigidBody&)(*o)).getFixedStatus())
	{
		//then we can initialize Fext :
		Fext = operatorC(	((RigidBody&)(*o)).getForceAccumulator(), 
							((RigidBody&)(*o)).getTorqueAccumulator() 
							);
					
	}
	else
	{
		//then we can initialize Fext :
		Fext = Mat<float>(0.0f, 6,1);
	}
	ressourcesMutex.unlock();
	
	//------------------------------------------------------------------
	//------------------------------------------------------------------
	
	//Mat<float> Identity3(0.0f,3,3);
	//for(int i=1;i<=3;i++)	Identity3.set( 1.0f, i,i);
	//Mat<float> Zero3(0.0f,3,3);
	//Mat<float> Zero
	
	//------------------------------------------------------------------
	size_t nbrB = 1;
	//invM.addLine(6);
	//invM.addColumn(6);
	//the size of those matrixes has been deduced in the constructor...
	ressourcesMutex.lock();
	for(int k=1;k<=3;k++)
	{
		invM.set( (nbrB-1)*6+k, (nbrB-1)*6+k, ((RigidBody&)(*o)).getIMass() );
	}
	Mat<float> tempIIW( ((RigidBody&)(*o)).getInverseInertialWorld() );
	for(int i=1;i<=3;i++)
	{
		for(int j=1;j<=3;j++)
		{
			invM.set( (nbrB-1)*6+3+i, (nbrB-1)*6+3+j,  tempIIW.get(i,j) );
		}
	}
	
	/*invM = operatorC( 	operatorL( ((RigidBody*)o->getIMass())*Identity, Zero3),
						operatorL( Zero3, (RigidBody*)o->getInverseInertiaWorld() )
						);*/
	ressourcesMutex.unlock();
	//------------------------------------------------------------------
	
	for(int k=1;k<=3;k++)
	{
		S.set( (nbrB-1)*7+k, (nbrB-1)*6+k, 1.0f );
	}
	ressourcesMutex.lock();
	Quat tempq = ((RigidBody*)(o->get()))->getOrientation();
	ressourcesMutex.unlock();
	tempq.x *= 0.5f;
	tempq.y *= 0.5f;
	tempq.z *= 0.5f;
	tempq.w *= 0.5f;
	S.set( (nbrB-1)*7+4, (nbrB-1)*6+4, -tempq.x );
	S.set( (nbrB-1)*7+4, (nbrB-1)*6+5, -tempq.y );
	S.set( (nbrB-1)*7+4, (nbrB-1)*6+6, -tempq.z );
	
	S.set( (nbrB-1)*7+5, (nbrB-1)*6+4, tempq.w );
	S.set( (nbrB-1)*7+5, (nbrB-1)*6+5, tempq.z );
	S.set( (nbrB-1)*7+5, (nbrB-1)*6+6, -tempq.y);
	
	S.set( (nbrB-1)*7+6, (nbrB-1)*6+4, -tempq.z );
	S.set( (nbrB-1)*7+6, (nbrB-1)*6+5, tempq.w );
	S.set( (nbrB-1)*7+6, (nbrB-1)*6+6, tempq.x );
	
	S.set( (nbrB-1)*7+7, (nbrB-1)*6+4, tempq.y );
	S.set( (nbrB-1)*7+7, (nbrB-1)*6+5, -tempq.x );
	S.set( (nbrB-1)*7+7, (nbrB-1)*6+6, tempq.w );
	
	//---------------------------------------------------
	//---------------------------------------------------------------------
#ifdef debug
std::cout << "SIMULATION : simulated object : " << nbrB << " : DONE." << std::endl;
#endif

	nbrB++;
	o++;
	
	
	while( o != simulatedObjects.cend() )
	{
		ressourcesMutex.lock();
		qdot = operatorC( 	qdot, 
							operatorC( ((RigidBody*)(o->get()))->getLinearVelocity(), ((RigidBody*)(o->get()))->getAngularVelocity())
							);
		
		q = operatorC( 	q, 
						operatorC( ((RigidBody*)(o->get()))->getPosition(), ((RigidBody*)(o->get()))->getMatOrientation())
						);
		ressourcesMutex.unlock();						
		
#ifdef debug
std::cout << "SIMULATION : simulated object : " << nbrB << " : Q." << std::endl;
#endif
		
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		
		
		ressourcesMutex.lock();
		if( ! ((RigidBody&)(*o)).getFixedStatus())
		{
			Mat<float> temp1( ((RigidBody&)(*o)).getForceAccumulator() ); 
			Mat<float> temp2( ((RigidBody&)(*o)).getTorqueAccumulator() );
			
			std::cout << temp1.getLine() << " " << temp1.getColumn() << std::endl;
			std::cout << temp2.getLine() << " " << temp2.getColumn() << std::endl;
			
			//exit(1);
#ifdef debug
std::cout << "SIMULATION : simulated object : " << nbrB << " : Q." << std::endl;
#endif			
			Fext = operatorC(	&Fext,
								operatorC(	temp1,
											temp2 
											)
								);
		}
		else
		{
			Fext = operatorC(&Fext, Mat<float>(0.0f, 6,1) );
		}
		ressourcesMutex.unlock();
#ifdef debug
std::cout << "SIMULATION : simulated object : " << nbrB << " : Fext." << std::endl;
#endif
		
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		
		//invM.addLine(6);
		//invM.addColumn(6);
		ressourcesMutex.lock();
		for(int k=1;k<=3;k++)
		{
			invM.set( (nbrB-1)*6+k, (nbrB-1)*6+k, ((RigidBody*)(o->get()))->getIMass() );
		}
		
		tempIIW = ((RigidBody*)(o->get()))->getInverseInertialWorld() ;
		ressourcesMutex.unlock();
		for(int i=1;i<=3;i++)
		{
			for(int j=1;j<=3;j++)
			{
				invM.set( (nbrB-1)*6+3+i, (nbrB-1)*6+3+j,  tempIIW.get(i,j) );
			}
		}	
				
		
#ifdef debug
std::cout << "SIMULATION : simulated object : " << nbrB << " : InvM." << std::endl;
#endif
		
		//------------------------------------------------------------------
		//------------------------------------------------------------------
	
		for(int k=1;k<=3;k++)
		{
			S.set( (nbrB-1)*7+k, (nbrB-1)*6+k, 1.0f );
		}
		ressourcesMutex.lock();
		Quat tempq = ((RigidBody*)(o->get()))->getOrientation();
		ressourcesMutex.unlock();
		tempq.x *= 0.5f;
		tempq.y *= 0.5f;
		tempq.z *= 0.5f;
		tempq.w *= 0.5f;
		S.set( (nbrB-1)*7+4, (nbrB-1)*6+4, -tempq.x );
		S.set( (nbrB-1)*7+4, (nbrB-1)*6+5, -tempq.y );
		S.set( (nbrB-1)*7+4, (nbrB-1)*6+6, -tempq.z );

		S.set( (nbrB-1)*7+5, (nbrB-1)*6+4, tempq.w );
		S.set( (nbrB-1)*7+5, (nbrB-1)*6+5, tempq.z );
		S.set( (nbrB-1)*7+5, (nbrB-1)*6+6, -tempq.y);

		S.set( (nbrB-1)*7+6, (nbrB-1)*6+4, -tempq.z );
		S.set( (nbrB-1)*7+6, (nbrB-1)*6+5, tempq.w );
		S.set( (nbrB-1)*7+6, (nbrB-1)*6+6, tempq.x );

		S.set( (nbrB-1)*7+7, (nbrB-1)*6+4, tempq.y );
		S.set( (nbrB-1)*7+7, (nbrB-1)*6+5, -tempq.x );
		S.set( (nbrB-1)*7+7, (nbrB-1)*6+6, tempq.w );

		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		
		nbrB++;
		o++;
		
#ifdef debug
std::cout << "SIMULATION : simulated object : " << nbrB << " : DONE." << std::endl;
#endif
		
	}
}


void Simulation::constructQQdotInvMSFext()
{
	//TODO : uncomment once the other strategy is set on track...
	initializedQQdotInvMFext = true;
	
	//------------------------------------------------------------------
	//------------------------------------------------------------------	
	//there alwas at least one element : the ground :
	//qdot = operatorC( ((IMoveable&)(*o)).getLinearVelocity(), ((IMoveable&)(*o)).getAngularVelocity() );
	ressourcesMutex.lock();
	qdot = operatorC( ((RigidBody*)simulatedObjects[0].get())->getLinearVelocity(), ((RigidBody*)simulatedObjects[0].get())->getAngularVelocity() );
	q = operatorC( ((RigidBody*)simulatedObjects[0].get())->getPosition(), ((RigidBody*)simulatedObjects[0].get())->getMatOrientation() );
	ressourcesMutex.unlock();

	//------------------------------------------------------------------
	//------------------------------------------------------------------

	ressourcesMutex.lock();
	if(! ((RigidBody*)simulatedObjects[0].get())->getFixedStatus())
	{
		//then we can initialize Fext :
		Fext = operatorC(	((RigidBody*)simulatedObjects[0].get())->getForceAccumulator(), 
							((RigidBody*)simulatedObjects[0].get())->getTorqueAccumulator() 
							);
				
	}
	else
	{
		//then we can initialize Fext :
		Fext = Mat<float>(0.0f, 6,1);
	}
	ressourcesMutex.unlock();

	//------------------------------------------------------------------
	//------------------------------------------------------------------

	//Mat<float> Identity3(0.0f,3,3);
	//for(int i=1;i<=3;i++)	Identity3.set( 1.0f, i,i);
	//Mat<float> Zero3(0.0f,3,3);
	//Mat<float> Zero

	//------------------------------------------------------------------
	size_t nbrB = 1;
	//invM.addLine(6);
	//invM.addColumn(6);
	//the size of those matrixes has been deduced in the constructor...
	ressourcesMutex.lock();
	for(int k=1;k<=3;k++)
	{
		invM.set( (nbrB-1)*6+k, (nbrB-1)*6+k, ((RigidBody*)simulatedObjects[0].get())->getIMass() );
	}
	Mat<float> tempIIW( ((RigidBody*)simulatedObjects[0].get())->getInverseInertialWorld() );
	for(int i=1;i<=3;i++)
	{
		for(int j=1;j<=3;j++)
		{
			invM.set( (nbrB-1)*6+3+i, (nbrB-1)*6+3+j,  tempIIW.get(i,j) );
		}
	}

	/*invM = operatorC( 	operatorL( ((RigidBody*)o->getIMass())*Identity, Zero3),
						operatorL( Zero3, (RigidBody*)o->getInverseInertiaWorld() )
						);*/
	ressourcesMutex.unlock();
	//------------------------------------------------------------------

	for(int k=1;k<=3;k++)
	{
		S.set( (nbrB-1)*7+k, (nbrB-1)*6+k, 1.0f );
	}
	ressourcesMutex.lock();
	Quat tempq = ((RigidBody*)simulatedObjects[0].get())->getOrientation();
	ressourcesMutex.unlock();
	tempq.x *= 0.5f;
	tempq.y *= 0.5f;
	tempq.z *= 0.5f;
	tempq.w *= 0.5f;
	S.set( (nbrB-1)*7+4, (nbrB-1)*6+4, -tempq.x );
	S.set( (nbrB-1)*7+4, (nbrB-1)*6+5, -tempq.y );
	S.set( (nbrB-1)*7+4, (nbrB-1)*6+6, -tempq.z );

	S.set( (nbrB-1)*7+5, (nbrB-1)*6+4, tempq.w );
	S.set( (nbrB-1)*7+5, (nbrB-1)*6+5, tempq.z );
	S.set( (nbrB-1)*7+5, (nbrB-1)*6+6, -tempq.y);

	S.set( (nbrB-1)*7+6, (nbrB-1)*6+4, -tempq.z );
	S.set( (nbrB-1)*7+6, (nbrB-1)*6+5, tempq.w );
	S.set( (nbrB-1)*7+6, (nbrB-1)*6+6, tempq.x );

	S.set( (nbrB-1)*7+7, (nbrB-1)*6+4, tempq.y );
	S.set( (nbrB-1)*7+7, (nbrB-1)*6+5, -tempq.x );
	S.set( (nbrB-1)*7+7, (nbrB-1)*6+6, tempq.w );

	//---------------------------------------------------
	//---------------------------------------------------------------------
#ifdef debug
std::cout << "SIMULATION : simulated object : " << nbrB << " : DONE." << std::endl;
#endif

	nbrB++;
	
	for(int i=1;i<simulatedObjects.size();i++)
	{
		ressourcesMutex.lock();
		qdot = operatorC( 	qdot, 
							operatorC( ((RigidBody*)simulatedObjects[i].get())->getLinearVelocity(), ((RigidBody*)simulatedObjects[i].get())->getAngularVelocity())
							);
		
		q = operatorC( 	q, 
						operatorC( ((RigidBody*)simulatedObjects[i].get())->getPosition(), ((RigidBody*)simulatedObjects[i].get())->getMatOrientation())
						);
		ressourcesMutex.unlock();						
		
#ifdef debug
std::cout << "SIMULATION : simulated object : " << nbrB << " : Q." << std::endl;
#endif
		
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		
		
		ressourcesMutex.lock();
		if( ! ((RigidBody*)simulatedObjects[i].get())->getFixedStatus())
		{
			Mat<float> temp1( ((RigidBody*)simulatedObjects[i].get())->getForceAccumulator() ); 
			Mat<float> temp2( ((RigidBody*)simulatedObjects[i].get())->getTorqueAccumulator() );
		
			Fext = operatorC(	&Fext,
								operatorC(	temp1,
											temp2 
											)
								);
		}
		else
		{
			Fext = operatorC(&Fext, Mat<float>(0.0f, 6,1) );
		}
		ressourcesMutex.unlock();
#ifdef debug
std::cout << "SIMULATION : simulated object : " << nbrB << " : Fext." << std::endl;
#endif
		
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		
		//invM.addLine(6);
		//invM.addColumn(6);
		ressourcesMutex.lock();
		for(int k=1;k<=3;k++)
		{
			invM.set( (nbrB-1)*6+k, (nbrB-1)*6+k, ((RigidBody*)simulatedObjects[i].get())->getIMass() );
		}
		
		tempIIW = ((RigidBody*)simulatedObjects[i].get())->getInverseInertialWorld() ;
		ressourcesMutex.unlock();
		for(int i=1;i<=3;i++)
		{
			for(int j=1;j<=3;j++)
			{
				invM.set( (nbrB-1)*6+3+i, (nbrB-1)*6+3+j,  tempIIW.get(i,j) );
			}
		}			
		
#ifdef debug
std::cout << "SIMULATION : simulated object : " << nbrB << " : InvM." << std::endl;
#endif
		
		//------------------------------------------------------------------
		//------------------------------------------------------------------
	
		for(int k=1;k<=3;k++)
		{
			S.set( (nbrB-1)*7+k, (nbrB-1)*6+k, 1.0f );
		}
		ressourcesMutex.lock();
		Quat tempq = ((RigidBody*)simulatedObjects[i].get())->getOrientation();
		ressourcesMutex.unlock();
		tempq.x *= 0.5f;
		tempq.y *= 0.5f;
		tempq.z *= 0.5f;
		tempq.w *= 0.5f;
		S.set( (nbrB-1)*7+4, (nbrB-1)*6+4, -tempq.x );
		S.set( (nbrB-1)*7+4, (nbrB-1)*6+5, -tempq.y );
		S.set( (nbrB-1)*7+4, (nbrB-1)*6+6, -tempq.z );

		S.set( (nbrB-1)*7+5, (nbrB-1)*6+4, tempq.w );
		S.set( (nbrB-1)*7+5, (nbrB-1)*6+5, tempq.z );
		S.set( (nbrB-1)*7+5, (nbrB-1)*6+6, -tempq.y);

		S.set( (nbrB-1)*7+6, (nbrB-1)*6+4, -tempq.z );
		S.set( (nbrB-1)*7+6, (nbrB-1)*6+5, tempq.w );
		S.set( (nbrB-1)*7+6, (nbrB-1)*6+6, tempq.x );

		S.set( (nbrB-1)*7+7, (nbrB-1)*6+4, tempq.y );
		S.set( (nbrB-1)*7+7, (nbrB-1)*6+5, -tempq.x );
		S.set( (nbrB-1)*7+7, (nbrB-1)*6+6, tempq.w );

		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		
		nbrB++;
		
#ifdef debug
std::cout << "SIMULATION : simulated object : " << nbrB << " : DONE." << std::endl;
#endif
	}
}

void Simulation::updateInvMSFext()
{
	//TODO : uncomment once the other strategy is set on track...
	initializedQQdotInvMFext = true;
	
	//------------------------------------------------------------------
	//------------------------------------------------------------------	
	//there alwas at least one element : the ground :
	//qdot = operatorC( ((IMoveable&)(*o)).getLinearVelocity(), ((IMoveable&)(*o)).getAngularVelocity() );
	//ressourcesMutex.lock();
	//qdot = operatorC( ((RigidBody*)simulatedObjects[0].get())->getLinearVelocity(), ((RigidBody*)simulatedObjects[0].get())->getAngularVelocity() );
	//q = operatorC( ((RigidBody*)simulatedObjects[0].get())->getPosition(), ((RigidBody*)simulatedObjects[0].get())->getMatOrientation() );
	//ressourcesMutex.unlock();

	//------------------------------------------------------------------
	//------------------------------------------------------------------

	ressourcesMutex.lock();
	if(! ((RigidBody*)simulatedObjects[0].get())->getFixedStatus())
	{
		//then we can initialize Fext :
		Fext = operatorC(	((RigidBody*)simulatedObjects[0].get())->getForceAccumulator(), 
							((RigidBody*)simulatedObjects[0].get())->getTorqueAccumulator() 
							);
				
	}
	else
	{
		//then we can initialize Fext :
		Fext = Mat<float>(0.0f, 6,1);
	}
	ressourcesMutex.unlock();

	//------------------------------------------------------------------
	//------------------------------------------------------------------

	//Mat<float> Identity3(0.0f,3,3);
	//for(int i=1;i<=3;i++)	Identity3.set( 1.0f, i,i);
	//Mat<float> Zero3(0.0f,3,3);
	//Mat<float> Zero

	//------------------------------------------------------------------
	size_t nbrB = 1;
	//invM.addLine(6);
	//invM.addColumn(6);
	//the size of those matrixes has been deduced in the constructor...
	ressourcesMutex.lock();
	for(int k=1;k<=3;k++)
	{
		invM.set( (nbrB-1)*6+k, (nbrB-1)*6+k, ((RigidBody*)simulatedObjects[0].get())->getIMass() );
	}
	Mat<float> tempIIW( ((RigidBody*)simulatedObjects[0].get())->getInverseInertialWorld() );
	for(int i=1;i<=3;i++)
	{
		for(int j=1;j<=3;j++)
		{
			invM.set( (nbrB-1)*6+3+i, (nbrB-1)*6+3+j,  tempIIW.get(i,j) );
		}
	}

	/*invM = operatorC( 	operatorL( ((RigidBody*)o->getIMass())*Identity, Zero3),
						operatorL( Zero3, (RigidBody*)o->getInverseInertiaWorld() )
						);*/
	ressourcesMutex.unlock();
	//------------------------------------------------------------------

	for(int k=1;k<=3;k++)
	{
		S.set( (nbrB-1)*7+k, (nbrB-1)*6+k, 1.0f );
	}
	ressourcesMutex.lock();
	Quat tempq = ((RigidBody*)simulatedObjects[0].get())->getOrientation();
	ressourcesMutex.unlock();
	tempq.x *= 0.5f;
	tempq.y *= 0.5f;
	tempq.z *= 0.5f;
	tempq.w *= 0.5f;
	S.set( (nbrB-1)*7+4, (nbrB-1)*6+4, -tempq.x );
	S.set( (nbrB-1)*7+4, (nbrB-1)*6+5, -tempq.y );
	S.set( (nbrB-1)*7+4, (nbrB-1)*6+6, -tempq.z );

	S.set( (nbrB-1)*7+5, (nbrB-1)*6+4, tempq.w );
	S.set( (nbrB-1)*7+5, (nbrB-1)*6+5, tempq.z );
	S.set( (nbrB-1)*7+5, (nbrB-1)*6+6, -tempq.y);

	S.set( (nbrB-1)*7+6, (nbrB-1)*6+4, -tempq.z );
	S.set( (nbrB-1)*7+6, (nbrB-1)*6+5, tempq.w );
	S.set( (nbrB-1)*7+6, (nbrB-1)*6+6, tempq.x );

	S.set( (nbrB-1)*7+7, (nbrB-1)*6+4, tempq.y );
	S.set( (nbrB-1)*7+7, (nbrB-1)*6+5, -tempq.x );
	S.set( (nbrB-1)*7+7, (nbrB-1)*6+6, tempq.w );

	//---------------------------------------------------
	//---------------------------------------------------------------------
#ifdef debug
std::cout << "SIMULATION : simulated object : " << nbrB << " : DONE." << std::endl;
#endif

	nbrB++;
	
	for(int i=1;i<simulatedObjects.size();i++)
	{
		//ressourcesMutex.lock();
		//qdot = operatorC( qdot,operatorC( ((RigidBody*)simulatedObjects[i].get())->getLinearVelocity(), ((RigidBody*)simulatedObjects[i].get())->getAngularVelocity()) );
		
		//q = operatorC( q,operatorC( ((RigidBody*)simulatedObjects[i].get())->getPosition(), ((RigidBody*)simulatedObjects[i].get())->getMatOrientation()));
		//ressourcesMutex.unlock();								
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		
		
		ressourcesMutex.lock();
		if( ! ((RigidBody*)simulatedObjects[i].get())->getFixedStatus())
		{
			Mat<float> temp1( ((RigidBody*)simulatedObjects[i].get())->getForceAccumulator() ); 
			Mat<float> temp2( ((RigidBody*)simulatedObjects[i].get())->getTorqueAccumulator() );
		
			Fext = operatorC(	&Fext,
								operatorC(	temp1,
											temp2 
											)
								);
		}
		else
		{
			Fext = operatorC(&Fext, Mat<float>(0.0f, 6,1) );
		}
		ressourcesMutex.unlock();
#ifdef debug
std::cout << "SIMULATION : simulated object : " << nbrB << " : Fext." << std::endl;
#endif
		
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		
		//invM.addLine(6);
		//invM.addColumn(6);
		ressourcesMutex.lock();
		for(int k=1;k<=3;k++)
		{
			invM.set( (nbrB-1)*6+k, (nbrB-1)*6+k, ((RigidBody*)simulatedObjects[i].get())->getIMass() );
		}
		
		tempIIW = ((RigidBody*)simulatedObjects[i].get())->getInverseInertialWorld() ;
		ressourcesMutex.unlock();
		for(int i=1;i<=3;i++)
		{
			for(int j=1;j<=3;j++)
			{
				invM.set( (nbrB-1)*6+3+i, (nbrB-1)*6+3+j,  tempIIW.get(i,j) );
			}
		}			
		
#ifdef debug
std::cout << "SIMULATION : simulated object : " << nbrB << " : InvM." << std::endl;
#endif
		
		//------------------------------------------------------------------
		//------------------------------------------------------------------
	
		for(int k=1;k<=3;k++)
		{
			S.set( (nbrB-1)*7+k, (nbrB-1)*6+k, 1.0f );
		}
		ressourcesMutex.lock();
		Quat tempq = ((RigidBody*)simulatedObjects[i].get())->getOrientation();
		ressourcesMutex.unlock();
		tempq.x *= 0.5f;
		tempq.y *= 0.5f;
		tempq.z *= 0.5f;
		tempq.w *= 0.5f;
		S.set( (nbrB-1)*7+4, (nbrB-1)*6+4, -tempq.x );
		S.set( (nbrB-1)*7+4, (nbrB-1)*6+5, -tempq.y );
		S.set( (nbrB-1)*7+4, (nbrB-1)*6+6, -tempq.z );

		S.set( (nbrB-1)*7+5, (nbrB-1)*6+4, tempq.w );
		S.set( (nbrB-1)*7+5, (nbrB-1)*6+5, tempq.z );
		S.set( (nbrB-1)*7+5, (nbrB-1)*6+6, -tempq.y);

		S.set( (nbrB-1)*7+6, (nbrB-1)*6+4, -tempq.z );
		S.set( (nbrB-1)*7+6, (nbrB-1)*6+5, tempq.w );
		S.set( (nbrB-1)*7+6, (nbrB-1)*6+6, tempq.x );

		S.set( (nbrB-1)*7+7, (nbrB-1)*6+4, tempq.y );
		S.set( (nbrB-1)*7+7, (nbrB-1)*6+5, -tempq.x );
		S.set( (nbrB-1)*7+7, (nbrB-1)*6+6, tempq.w );

		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		
		nbrB++;
		
#ifdef debug
std::cout << "SIMULATION : simulated object : " << nbrB << " : DONE." << std::endl;
#endif
	}
}


/*
void Simulation::updateQQdotInvMSFext()
{
	//TODO : finishing it...
	std::vector<std_unique_ptr<ISimulationObject> >::iterator o = simulatedObjects.begin();
	
	
	//------------------------------------------------------------------
	//------------------------------------------------------------------
	
	
	//there alwas at least one element : the ground :
	Mat<float> lvel((RigidBody*)o->getLinearVelocity());
	for(int i=1;i<=lvel.getLine();i++)
	{
		qdot.set( lvel.get(i,1), i,1);
		
	}
	Mat<float> avel((RigidBody*)o->getAngularVelocity() );
	for(int i=1;i<=avel.getLine();i++)
	{
		qdot.set( avel.get(i,1), lvel.getLine()+i,1);
	}
	
	Mat<float> pos((RigidBody*)o->getPosition());
	for(int i=1;i<=pos.getLine();i++)
	{
		q.set( pos.get(i,1), i,1);
		
	}
	Mat<float> orient((RigidBody*)o->getMatOrientation() );
	for(int i=1;i<=orient.getLine();i++)
	{
		q.set( orient.get(i,1), pos.getLine()+i,1);
		
	}
	
	
	//------------------------------------------------------------------
	//------------------------------------------------------------------
	//TODO finishing .... in the mean time, we do reallocation...
	
	if(!(RigidBody*)o->getFixedStatus())
	{
		//then we can initialize Fext :
		Fext = operatorL(	(RigidBody*)o->getForceAccumulator(), 
							(RigidBody*)o->getTorqueAccumulator() 
							);
					
	}
	else
	{
		//then we can initialize Fext :
		Fext = Mat<float>(0.0f, 6,1);
	}
	
	
	//------------------------------------------------------------------
	//------------------------------------------------------------------
	
	//Mat<float> Identity3(0.0f,3,3);
	//for(int i=1;i<=3;i++)	Identity3.set( 1.0f, i,i);
	//Mat<float> Zero3(0.0f,3,3);
	//Mat<float> Zero
	
	//------------------------------------------------------------------
	size_t nbrB = 1;
	//invM.addLine(6);
	//invM.addColumn(6);
	//the size of those matrixes has been deduced in the constructor...
	for(int k=1;k<=3;k++)
	{
		invM.set( (nbrB-1)*6+k, (nbrB-1)*6+k, (RigidBody*)o->getIMass() );
	}
	Mat<float> tempIIW( (RigidBody*)o->getInverseInertiaWorld() );
	for(int i=1;i<=3;i++)
	{
		for(int j=1;j<=3;j++)
		{
			invM.set( (nbrB-1)*6+3+i, (nbrB-1)*6+3+j,  tempIIW(i,j) );
		}
	}
	
	//------------------------------------------------------------------
	
	for(int k=1;k<=3;k++)
	{
		S.set( (nbrB-1)*7+k, (nbrB-1)*6+k, 1.0f );
	}
	Quat tempq = 0.5f*((RigidBody*)o->getOrientation());
	S.set( (nbrB-1)*7+4, (nbrB-1)*6+4, -tempq.x );
	S.set( (nbrB-1)*7+4, (nbrB-1)*6+5, -tempq.y );
	S.set( (nbrB-1)*7+4, (nbrB-1)*6+6, -tempq.z );
	
	S.set( (nbrB-1)*7+5, (nbrB-1)*6+4, tempq.w );
	S.set( (nbrB-1)*7+5, (nbrB-1)*6+5, tempq.z );
	S.set( (nbrB-1)*7+5, (nbrB-1)*6+6, -tempq.y);
	
	S.set( (nbrB-1)*7+6, (nbrB-1)*6+4, -tempq.z );
	S.set( (nbrB-1)*7+6, (nbrB-1)*6+5, tempq.w );
	S.set( (nbrB-1)*7+6, (nbrB-1)*6+6, tempq.x );
	
	S.set( (nbrB-1)*7+7, (nbrB-1)*6+4, tempq.y );
	S.set( (nbrB-1)*7+7, (nbrB-1)*6+5, -tempq.x );
	S.set( (nbrB-1)*7+7, (nbrB-1)*6+6, tempq.w );
	
	//---------------------------------------------------
	//---------------------------------------------------------------------
	nbrB++;
	
	
	while( o != simulatedObjects.end() )
	{
		
		qdot = operatorC( 	qdot, 
							operatorC( o->getLinearVelocity(), o->getAngularVelocity()
							);
		
		q = operatorC( 	q, 
						operatorC( o->getPosition(), o->getMatOrientation()
						);
		
		
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		
		
		
		if(!(RigidBody*)o->getFixedStatus())
		{
			Fext = operatorL(	Fext,
								operatorL(	(RigidBody*)o->getForceAccumulator(), 
											(RigidBody*)o->getTorqueAccumulator() 
											)
								);
		}
		else
		{
			Fext = operatorL(Fext, Mat<float>(0.0f, 6,1) );
		}
		
		
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		
		//invM.addLine(6);
		//invM.addColumn(6);
		for(int k=1;k<=3;k++)
		{
			invM.set( (nbrB-1)*6+k, (nbrB-1)*6+k, (RigidBody*)o->getIMass() );
		}
		tempIIW = (RigidBody*)o->getInverseInertiaWorld() ;
		for(int i=1;i<=3;i++)
		{
			for(int j=1;j<=3;j++)
			{
				invM.set( (nbrB-1)*6+3+i, (nbrB-1)*6+3+j,  tempIIW(i,j) );
			}
		}			
		//------------------------------------------------------------------
		//------------------------------------------------------------------
	
		for(int k=1;k<=3;k++)
		{
			S.set( (nbrB-1)*7+k, (nbrB-1)*6+k, 1.0f );
		}
		Quat tempq = 0.5f*((RigidBody*)o->getOrientation());
		S.set( (nbrB-1)*7+4, (nbrB-1)*6+4, -tempq.x );
		S.set( (nbrB-1)*7+4, (nbrB-1)*6+5, -tempq.y );
		S.set( (nbrB-1)*7+4, (nbrB-1)*6+6, -tempq.z );

		S.set( (nbrB-1)*7+5, (nbrB-1)*6+4, tempq.w );
		S.set( (nbrB-1)*7+5, (nbrB-1)*6+5, tempq.z );
		S.set( (nbrB-1)*7+5, (nbrB-1)*6+6, -tempq.y);

		S.set( (nbrB-1)*7+6, (nbrB-1)*6+4, -tempq.z );
		S.set( (nbrB-1)*7+6, (nbrB-1)*6+5, tempq.w );
		S.set( (nbrB-1)*7+6, (nbrB-1)*6+6, tempq.x );

		S.set( (nbrB-1)*7+7, (nbrB-1)*6+4, tempq.y );
		S.set( (nbrB-1)*7+7, (nbrB-1)*6+5, -tempq.x );
		S.set( (nbrB-1)*7+7, (nbrB-1)*6+6, tempq.w );

		//---------------------------------------------------------------------
		//---------------------------------------------------------------------
		
		nbrB++;
		o++;
	}
}*/






void Simulation::applyForces(float timeStep)
{
	std::vector<std::unique_ptr<IForceEffect> >::iterator itF = collectionF.begin();
	
	RigidBody dummy;
	bool nextOne = true;
	
	while(itF != collectionF.end())
	{
		if( (itF->get())->isGravity())
		{
			std::vector<std::unique_ptr<ISimulationObject> >::iterator itRB = simulatedObjects.begin();
			
			while(itRB!=simulatedObjects.end())
			{			
				(itF->get())->Apply(timeStep, (RigidBody&)(*(itRB->get())));
				itRB++;
			}
		}
		else
		{
			(itF->get())->Apply(timeStep,dummy);
			
			//deletion of the collision forces :
			if( (itF->get())->getEndTime() >= time+SimulationTimeStep)
			{
				collectionF.erase(itF);
				nextOne = false;
			}
		}
		
		
		if(nextOne)
			itF++;
		else
		{
			nextOne = true;
		}
	}

	/*
	Mat<float> forceLATERAL( (float)0,3,1);
	forceLATERAL.set( 1.0f, 1,1);
	Mat<float> bodyPoint( (float)0,3,1);
	bodyPoint.set( 5.0f,1,1);
	bodyPoint.set( 10.0f,3,1);
	((RigidBody*)simulatedObjects[2].get())->addForceAtBodyPoint( forceLATERAL, bodyPoint); 
	*/
#ifdef debuglvl2	
	for( auto& it : simulatedObjects)
	{
		((RigidBody*)(it.get()))->getForceAccumulator().afficher();
		((RigidBody*)(it.get()))->getTorqueAccumulator().afficher();
	}
#endif
}
void Simulation::updateQQdot()
{
	
	int b1 = 0;
	int b2 = 0;
	
	
	for( auto& o : simulatedObjects ) 
	{	
		((RigidBody*)(o.get()))->setPosition( extract(q, b1+1,1, b1+3,1) );
#ifdef debuglvl1		
		std::cout << " SIM : TEST AVANT MODIF : " << std::endl;
		((RigidBody*)(o.get()))->getMatOrientation().afficher();
#endif		
		((RigidBody*)(o.get()))->setMatOrientation( extract(q, b1+4,1, b1+7,1) );
		((RigidBody*)(o.get()))->setLinearVelocity( extract( qdot, b2+1,1, b2+3,1) );
		((RigidBody*)(o.get()))->setAngularVelocity( extract( qdot, b2+4,1, b1+6,1) );
		
#ifdef debuglvl1		
		std::cout << " SIM : APRES MODIF : " << std::endl;
		((RigidBody*)(o.get()))->getMatOrientation().afficher();
#endif		
		b1+=7;
		b2+=6;	
	}
	
}


void Simulation::updateStates()
{
	updateQQdot();
	
	std::string name;
	int id = 0;
	
	ressourcesMutex.lock();
	for( auto& itEl : env->ListeElements)
	{
		ressourcesMutex.unlock();
		ressourcesMutex.lock();
		name = itEl->getName();
		ressourcesMutex.unlock();
		
		
		if( Name2ID.count( name) )
		{
			id = Name2ID[name];
			ressourcesMutex.lock();
			itEl->setPose( ((RigidBody*)(simulatedObjects[id].get()))->getPose() );
			ressourcesMutex.unlock();
			
			((RigidBody*)simulatedObjects[id].get())->clearUser();
		}
		
		ressourcesMutex.lock();
	}
	ressourcesMutex.unlock();
	
}




//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------
//---------------------------------------------------




float Simulation::getTime()	const
{
	return time;
}

float Simulation::getTimeStep()	const
{
	return SimulationTimeStep;
}
