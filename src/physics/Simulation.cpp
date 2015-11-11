#include "Simulation.h"
#include "eval/IUpdater.h"
#include "eval/IConstraintsSolver.h"
#include "eval/CollisionDetector.h"
#include "eval/IIntegrator.h"

Simulation::Simulation() : updater(new Updater(this, new ExplicitEulerIntegrater(this)), constraintsSolver( new SimultaneousImpulseBasedConstraintsSolver(this)), collisionDetector(new CollisionDetector(this))
{

}
//Simulation(Environnement* env);
	
Simulation::~Simulation()
{

}
