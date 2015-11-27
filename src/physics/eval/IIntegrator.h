#ifndef IINTEGRATOR_H
#define IINTEGRATOR_H

#include "../Simulation.h"

class IIntegrator
{
	private :
	
	std::shared_ptr<Simulation> sim;
	
	//---------------------------------------------
	//---------------------------------------------
	public :
	
	IIntegrator(Simulation* sim) : simulation(sim)
	{
	
	}
	
	virtual ~IIntegrator()
	{
	
	}	
	
	virtual void integrate(float dt = 0.0001f) =0 ;
	
};



class ExplicitEulerIntegrator : public IIntegrator
{
	private :
	
	
	//---------------------------------------------
	//---------------------------------------------
	public :
	
	
	ExplicitEulerIntegrator(Simulation* sim) : IIntegrator(sim)
	{
	
	}
	
	~ExplicitEulerIntegrator()
	{
	
	}	
	
	virtual void integrate(float dt = 0.0001f) override
	{
		assert(dt > 0.0f);
		
		//int nbrSimulatedObjects = sim->simulatedObjects.size();
		std::vector<ISimulatedObject>::iterator it = sim->simulatedObjects.begin();
		
		while( it != sim->simulatedObjects.end())
		{
			if(it->type == TSORigidBody)
			{
				it->calculateDerivedData();
				
				
				//Linear :
				Mat<float> resultingLinearAcceleration(  it->getInverseMass()*it->getForceAccumulator());
		
				it->setLinearVelocity( it->getLinearVelocity() + dt*resultingLinearAcceleration);
		
				//velocity *= pow( damping, duration);
				
				it->setPosition( it->getPosition() + dt*it->getLinearVelocity);
		
				//Angular :
				Mat<float> resultingAngularAcceleration(  it->getInverseInertiaWorld()*it->getTorqueAccumulator());
		
				it->setAngularVelocity( it->getAngularVelocity() + dt*resultingAngularAcceleration);
		
				//velocity *= pow( damping, duration);
				
				//----------------------------------------------
				//Quaternion : via Euler Angles...
				//Mat<float> deltaQEuler(dt*getAngularVelocity);
				//it->setOrientation( Qt_Mul( Euler2Qt( deltaQEuler.get(1,1), deltaQEuler.get(2,1), deltaQEuler.get(3,1)), it->getOrientation() ) );
				//----------------------------------------------
				//----------------------------------------------
				//Quaternion : via qrond matrix : watch out for the quaternion definition : here : (x y z w)^T...
				Mat<float> qrond(4,3);
				Quat q( it->getOrientation() );
				qrond.set( q.w, 1,1);
				qrond.set( q.w, 2,2);
				qrond.set( q.w, 3,3);
				
				qrond.set( q.x, 2,3);
				qrond.set( -q.x, 3,2);
				qrond.set( -q.x, 4,1);
				
				qrond.set( q.y, 3,1);
				qrond.set( -q.y, 1,3);
				qrond.set( -q.y, 4,2);
				
				qrond.set( q.z, 1,2);
				qrond.set( -q.z, 2,1);
				qrond.set( -q.z, 4,3);
				
				it->setOrientation( it->getOrientation()+Mat2Qt( dt * (qrond*it->getAngularVelocity()) ) );
				//----------------------------------------------
				
				//-------------------
				it->clearUser();
			}
		}
	}
			
};


#endif
