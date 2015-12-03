#include "IMidNarrowPhaseStrategy.h"
#include "../Simulation.h"

IMidNarrowPhaseStrategy::IMidNarrowPhaseStrategy(Simulation* sim_) : sim(sim_)
{

}

IMidNarrowPhaseStrategy::~IMidNarrowPhaseStrategy()
{

}


//----------------------------------------
//----------------------------------------

//----------------------------------------
//----------------------------------------


MidNarrowPhaseStrategyA::MidNarrowPhaseStrategyA(Simulation *sim_) : IMidNarrowPhaseStrategy(sim_)
{

}

MidNarrowPhaseStrategyA::~MidNarrowPhaseStrategyA()
{

}

void MidNarrowPhaseStrategyA::checkForCollisions(std::vector<Contact>& c)
{
	//let us iterate over the contact set :
	std::vector<Contact>::iterator contact = c.begin();
	bool erase = false;
	while( contact != c.end())
	{
		ShapeType typeA = contact->rbA->getShapeType();
		ShapeType typeB = contact->rbB->getShapeType();
					
		int nbrContact = 0;					
		switch(typeA)
		{
			case ABSTRACT :
			//TODO : exception ?
			switch(typeB)
			{
				case ABSTRACT :
				//TODO : exception ?
				break;
			
				case SPHERE :
				
				break;
			
				case PLANE :
				
				break;
			
				case BOX :
				
				break;
			
				case COMPOSIT :
				
				break;
			
				default :
				
				break;
			
			}
			
			break;
			
			case SPHERE :
			
			switch(typeB)
			{
				case ABSTRACT :
				//TODO : exception ?
				break;
			
				case SPHERE :
				//nothing to do, it has already been handled if a bounding sphere has been used.
				//TODO : handle the case when the IBroadPhaseStrategy is different of the one using spheres...
				break;
			
				case PLANE :
				//let us compute the distance between the center of the sphere and the plane :
				
				
				break;
			
				case BOX :
				
				break;
			
				case COMPOSIT :
				
				break;
			
				default :
				
				break;
			
			}
			
			break;
			
			case PLANE :
			
			switch(typeB)
			{
				case ABSTRACT :
				//TODO : exception ?
				break;
			
				case SPHERE :
				// 
				break;
			
				case PLANE :
				// let us think of it as a really thin box :
				//TODO : handle the infinity size ?
				break;
			
				case BOX :
				{
				bool testIntersection = testOBBPlane( *(contact->rbB), *(contact->rbA) );
				if(testIntersection)
				{
					//the plane and the box are intersecting : let us fill in the Contact structure :
					//TODO //TODO 
				}
				else
				{
					//the plane and the box are not intersecting.
					c.erase(contact);
					erase = true;
				}
				}
				break;
			
				case COMPOSIT :
				
				break;
			
				default :
				
				break;
			
			}
			
			break;
			
			case BOX :
			
			switch(typeB)
			{
				case ABSTRACT :
				//TODO :
				break;
			
				case SPHERE :
				
				break;
			
				case PLANE :
				{
				//TODO : no plane for now ...
				bool testIntersection = testOBBPlane( *(contact->rbA), *(contact->rbB));
				if(testIntersection)
				{
					//the plane and the box are intersecting : let us fill in the Contact structure :
					//let us compute the intersection point :
					Mat<float> pointA( contact->rbA->getPosition());
					Mat<float> normal(  ((PlaneShape&)(contact->rbA->getShapeReference())).getNormal() );
					
					
					//pointA = (*contact)->rbA.getPointInLocal(pointA+
					
				}
				else
				{
					//the plane and the box are not intersecting.
					c.erase(contact);
					erase = true;
				}
				}
				break;
			
				case BOX :
				{
				bool testIntersection = false;
				Mat<float> intersectingPointsWOfA( testOBBOBB( *(contact->rbA), *(contact->rbB), testIntersection) );
				
				if(testIntersection)
				{
					//the two boxes are intersecting : let us fill in the Contact structure :
					for(int i=intersectingPointsWOfA.getColumn();i--;)
					{
						contact->contactPoint.insert( contact->contactPoint.begin(), extract(intersectingPointsWOfA, 1,i+1, 3,i+1) );
					}
				}
				else
				{
					//the two boxes are not intersecting.
					c.erase(contact);
					erase = true;
				}
				}
				break;
			
				case COMPOSIT :
				
				break;
			
				default :
				
				break;
			
			}
			
			break;
			
			case COMPOSIT :
			
			switch(typeB)
			{
				case ABSTRACT :
				
				break;
			
				case SPHERE :
				
				break;
			
				case PLANE :
				
				break;
			
				case BOX :
				
				break;
			
				case COMPOSIT :
				
				break;
			
				default :
				
				break;
			
			}
			
			break;
			
			default :
			
			switch(typeB)
			{
				case ABSTRACT :
				
				break;
			
				case SPHERE :
				
				break;
			
				case PLANE :
				
				break;
			
				case BOX :
				
				break;
			
				case COMPOSIT :
				
				break;
			
				default :
				
				break;
			
			}
			
			break;
			
		}
		
		if(!erase)
			contact++;
		
		erase = false;
	}
	
}
