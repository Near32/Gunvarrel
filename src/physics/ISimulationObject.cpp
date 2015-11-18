#include "ISimulationObject.h"

ISImulationObject::ISimulationObject() : name(std::string("not mentioned")), id(0), isActive(true), type(TSOUndefined)
{

}

ISImulationObject::ISimulationObject(int id_) : name(std::string("not mentioned")), id(id_), isActive(true), type(TSOUndefined)
{

}

ISImulationObject::ISimulationObject(std::string name_, int id_, bool isActive_) : name(name_), id(id_), isActive(isActive_), type(TSOUndefined)
{

}

ISimulationObject::~ISimulationObject()
{

}

