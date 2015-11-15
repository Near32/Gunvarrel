#ifndef ISIMULATIONOBJECT_H
#define ISIMULATIONOBJECT_H

#include <iostream>
#include <string>
#include "utils/math.h"
#include "TypeSimulationObject.h"

class ISimulationObject 
{
	protected :
	
	std::String name;	//name of the object
	int id;				//unique id of the object
	enum TypeSimulationObject type;
	
	public :

	bool isActive;		//indicates if the object is to be taken care by the simulation, or not.	
	
	//--------------------------------------
	//--------------------------------------
		
	ISimulationObject();
	ISimulationObject(int id_);
	ISimulationObject( std::String name_, int id_, bool isActive_);
	
	~ISimulationObject();
	
	virtual void Render(const se3& WorldTransformation) = 0;	//abstract method that renders information for debug purpose, for instance.
	
	
	//---------------------------------
	enum TypeSimulationObject getType()	{	return type;	}

};
#endif