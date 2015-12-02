#include "Simulation.h"
#include "eval/IUpdater.h"
#include "eval/IConstraintSolverStrategy.h"
#include "eval/CollisionDetector.h"
#include "eval/IIntegrator.h"

Simulation::Simulation() : updater(new Updater(this, new ExplicitEulerIntegrator(this)) ), constraintsSolver( new SimultaneousImpulseBasedConstraintSolverStrategy(this) ), collisionDetector( new CollisionDetector(this) )
{
	Mat<float> g(0.0f,3,1);
	g.set( -9.81f,3,1);
	collectionF.insert( collectionF.begin(), std::unique_ptr<IForceEffect>(new GravityForceEffect(g))  );
}

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
					simulatedObjects.insert( simulatedObjects.end(), std::unique_ptr<ISimulationObject>(new RigidBody( (*element)->getName(),id,true) ) );
					((RigidBody&)(*simulatedObjects[id])).setPose( (*element)->getPoseReference());
					((RigidBody&)(*simulatedObjects[id])).setPtrShape( new BoxShape( (RigidBody*)simulatedObjects[id].get(), ((IElementFixe&)(*(*element))).hwd) );
				}
				else
				{
					//then it is the ground :
					simulatedObjects.insert( simulatedObjects.end(), std::unique_ptr<ISimulationObject>(new RigidBody( (*element)->getName(),id,true) ) );
					((RigidBody&)(*simulatedObjects[id])).setPose( (*element)->getPoseReference());
					((RigidBody&)(*simulatedObjects[id])).setPtrShape( new BoxShape( (RigidBody*)simulatedObjects[id].get(), ((IElementFixe&)(*(*element))).hwd) );
					
					//IT IS THE UNMOVEABLE GROUND :
					((RigidBody&)(*simulatedObjects[id])).isFixed = true;
					((RigidBody&)(*simulatedObjects[id])).setIMass( 1e-10f );//numeric_limit<float>::epsilon() );
				}
				
				
				break;
				
				case false :
				//the element is an OrbeBonus :
				simulatedObjects.insert( simulatedObjects.end(), std::unique_ptr<ISimulationObject>(new RigidBody( (*element)->getName(),id,true) ) );
				((RigidBody&)(*simulatedObjects[id])).setPose( (*element)->getPoseReference());
					
					//CAREFUL : HANDLE THE RADIUS : with SphereShape
				((RigidBody&)(*simulatedObjects[id])).setPtrShape( new SphereShape( (RigidBody*)simulatedObjects[id].get(), ((IElementMobile&)(*(*element))).hwd.get(1,1) ) );
				
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
				
				break;
				
				
			}
			break;
			
		}
		
		//TODO : set mass : okay 1.0f by default...
		//TODO : Inertia ...
		Name2ID[ (*element)->getName() ] = id;
		id++;
	}
	
	initializedQQdotInvMFext = false;
	invM = SparseMat<float>( 6*id);
	S = SparseMat<float>( 7*id, 6*id);
}




Simulation::Simulation(Environnement* env, ConstraintsList& cl) : Simulation(env)
{
	//TODO : handle/enforce constraint list :
	ConstraintsList::iterator itC = cl.begin();
	
	switch( (*itC).ct)
	{
		case CTHingeJoint :
		collectionC.insert( collectionC.end(), std::unique_ptr<IConstraint>( new HingeJoint( (RigidBody&)*simulatedObjects[Name2ID[ (*itC).nameEl1]], (RigidBody&)*simulatedObjects[Name2ID[ (*itC).nameEl2]], extract( (*itC).data,1,1, 3,1), extract( (*itC).data,1,2, 3,2) ) ) );
		
		break;
		
		case CTBallAndSocketJoint :
		collectionC.insert( collectionC.end(), 
							std::unique_ptr<IConstraint>(new BallAndSocketJoint( (RigidBody&)*simulatedObjects[Name2ID[ (*itC).nameEl1]], 
														(RigidBody&)*simulatedObjects[Name2ID[ (*itC).nameEl2]],
														extract( (*itC).data,1,1, 3,1),
														extract( (*itC).data,1,2, 3,2)
														))
							);
		break;
	}
	
	while( itC !=cl.end())
	{		
		itC++;
		
		switch( (*itC).ct)
		{
			case CTHingeJoint :
			collectionC.insert( collectionC.end(), 
								std::unique_ptr<IConstraint>(new HingeJoint( (RigidBody&)*simulatedObjects[Name2ID[ (*itC).nameEl1]], 
															(RigidBody&)*simulatedObjects[Name2ID[ (*itC).nameEl2]],
															extract( (*itC).data,1,1, 3,1),
															extract( (*itC).data,1,2, 3,2)
															))
								);
		
			break;
		
			case CTBallAndSocketJoint :
			collectionC.insert( collectionC.end(), 
								std::unique_ptr<IConstraint>(new BallAndSocketJoint( (RigidBody&)*simulatedObjects[Name2ID[ (*itC).nameEl1]], 
															(RigidBody&)*simulatedObjects[Name2ID[ (*itC).nameEl2]],
															extract( (*itC).data,1,1, 3,1),
															extract( (*itC).data,1,2, 3,2)
															))
								);
			break;
		}	
	}
}
	

	
Simulation::~Simulation()
{

}




void Simulation::run(float timeStep, float endTime)
{
	float time = 0.0f;
	if(timeStep <= 0.0f)	timeStep = 1e-2f;
	
	if( endTime > 0.0f)
	{
		while( time < endTime)
		{
			runStride( timeStep);
			time += timeStep;
		}
	}
}

//----------------------------------------------------------------
//----------------------------------------------------------------

void Simulation::runStride( float timeStep)
{
	//check Collisions and create the corresponding entities to deal with those :
	collisionDetector->checkForCollision(timeStep);
	
	//apply forces to the RigidBodies:
	applyForces(timeStep);
	
	//construct the system to be solved...
	if(initializedQQdotInvMFext)
	{
		//TODO : update QQdotInvMFext, for now on, we do not care at all about reallocation and optimization, so the matrixes are being reconstructed every time...
	}
	else
	{
		constructQQdotInvMSFext();
	}
	//solve the system and update it :
	constraintsSolver->Solve(timeStep, collectionC, q, qdot, invM,S, Fext);
	
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

void Simulation::constructQQdotInvMSFext()
{
	//TODO : uncomment once the other strategy is set on track...
	//initializedQQdotInvMFext = true;
	
	std::vector<std::unique_ptr<ISimulationObject> >::iterator o = simulatedObjects.begin();
	
	
	//------------------------------------------------------------------
	//------------------------------------------------------------------
	
	
	//there alwas at least one element : the ground :
	qdot = operatorC( ((RigidBody&)(*o)).getLinearVelocity(), ((RigidBody&)(*o)).getAngularVelocity() );
	q = operatorC( ((RigidBody&)(*o)).getPosition(), ((RigidBody&)(*o)).getMatOrientation() );
	
	
	//------------------------------------------------------------------
	//------------------------------------------------------------------
	
	
	if(!((RigidBody&)(*o)).getFixedStatus())
	{
		//then we can initialize Fext :
		Fext = operatorL(	((RigidBody&)(*o)).getForceAccumulator(), 
							((RigidBody&)(*o)).getTorqueAccumulator() 
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
	
	//------------------------------------------------------------------
	
	for(int k=1;k<=3;k++)
	{
		S.set( (nbrB-1)*7+k, (nbrB-1)*6+k, 1.0f );
	}
	Quat tempq = ((RigidBody&)(*o)).getOrientation();
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
	nbrB++;
	o++;
	
	
	while( o != simulatedObjects.end() )
	{
		
		qdot = operatorC( 	qdot, 
							operatorC( ((RigidBody&)(*o)).getLinearVelocity(), ((RigidBody&)(*o)).getAngularVelocity())
							);
		
		q = operatorC( 	q, 
						operatorC( ((RigidBody&)(*o)).getPosition(), ((RigidBody&)(*o)).getMatOrientation())
						);
		
		
		//------------------------------------------------------------------
		//------------------------------------------------------------------
		
		
		
		if(! ((RigidBody&)(*o)).getFixedStatus())
		{
			Fext = operatorL(	Fext,
								operatorL(	((RigidBody&)(*o)).getForceAccumulator(), 
											((RigidBody&)(*o)).getTorqueAccumulator() 
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
			invM.set( (nbrB-1)*6+k, (nbrB-1)*6+k, ((RigidBody&)(*o)).getIMass() );
		}
		tempIIW = ((RigidBody&)(*o)).getInverseInertialWorld() ;
		for(int i=1;i<=3;i++)
		{
			for(int j=1;j<=3;j++)
			{
				invM.set( (nbrB-1)*6+3+i, (nbrB-1)*6+3+j,  tempIIW.get(i,j) );
			}
		}			
		//------------------------------------------------------------------
		//------------------------------------------------------------------
	
		for(int k=1;k<=3;k++)
		{
			S.set( (nbrB-1)*7+k, (nbrB-1)*6+k, 1.0f );
		}
		Quat tempq = ((RigidBody&)(*o)).getOrientation();
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
	
	if( (*itF)->isGravity())
	{
		std::vector<std::unique_ptr<ISimulationObject> >::iterator itRB = simulatedObjects.begin();
		
		(*itF)->Apply(timeStep, (RigidBody&)(*(*itRB)) );
		
		while(itRB!=simulatedObjects.end())
		{
			itRB++;
			
			(*itF)->Apply(timeStep,(RigidBody&)(*(*itRB)));
		}
	}
	else
	{
		(*itF)->Apply(timeStep,dummy);
	}
	
	while(itF != collectionF.end())
	{
		itF++;
		
		if( (*itF)->isGravity())
		{
			std::vector<std::unique_ptr<ISimulationObject> >::iterator itRB = simulatedObjects.begin();
		
			(*itF)->Apply(timeStep, (RigidBody&)(*(*itRB)));
		
			while(itRB!=simulatedObjects.end())
			{
				itRB++;
			
				(*itF)->Apply(timeStep, (RigidBody&)(*(*itRB)));
			}
		}
		else
		{
			(*itF)->Apply(timeStep,dummy);
		}
	}
}
void Simulation::updateQQdot()
{
	std::vector<std::unique_ptr<ISimulationObject> >::iterator o = simulatedObjects.begin();
	
	int b1 = 0;
	int b2 = 0;
	((RigidBody&)(*o)).setPosition( extract(q, b1+1,1, b1+3,1) );
	((RigidBody&)(*o)).setMatOrientation( extract(q, b1+4,1, b1+7,1) );
	((RigidBody&)(*o)).setLinearVelocity( extract( qdot, b2+1,1, b2+3,1) );
	((RigidBody&)(*o)).setAngularVelocity( extract( qdot, b2+4,1, b1+6,1) );
	b1+=7;
	b2+=6;
	
	while( o!=simulatedObjects.end() )
	{
		o++;
		
		((RigidBody&)(*o)).setPosition( extract(q, b1+1,1, b1+3,1) );
		((RigidBody&)(*o)).setMatOrientation( extract(q, b1+4,1, b1+7,1) );
		((RigidBody&)(*o)).setLinearVelocity( extract( qdot, b2+1,1, b2+3,1) );
		((RigidBody&)(*o)).setAngularVelocity( extract( qdot, b2+4,1, b1+6,1) );
		b1+=7;
		b2+=6;	
		
	}
	
}


void Simulation::updateStates()
{
	updateQQdot();
	
	std::vector<std::unique_ptr<IElement> >::iterator itEl = env->getIteratorElementsBegin();
	
	std::string name( (*itEl)->getName());
	int id = 0;
	if( Name2ID.count( name) )
	{
		id = Name2ID[name];
		(*itEl)->setPose( ((RigidBody&)simulatedObjects[id]).getPose() );
		((RigidBody&)simulatedObjects[id]).clearUser();
	}
	
	while(itEl != env->getIteratorElementsEnd())
	{
		itEl++;
		name = (*itEl)->getName();
		
		if( Name2ID.count( name) )
		{
			id = Name2ID[name];
			(*itEl)->setPose( ((RigidBody&)simulatedObjects[id]).getPose() );
			((RigidBody&)simulatedObjects[id]).clearUser();
		}				
	}
	
}
