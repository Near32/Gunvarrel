#include "ISimulationObject.h"

ISImulationObject::ISimulationObject() : name(std::string("not mentioned")), id(0), isActive(true), type(TSOUndefined)
{

}

ISImulationObject::ISimulationObject(int id_) : name(String("not mentioned")), id(id_), isActive(true), type(TSOUndefined)
{

}

ISImulationObject::ISimulationObject(std::String name_, int id_, bool isActive_) : name(name_), id(id_), isActive(isActive_), type(TSOUndefined)
{

}

ISimulationObject::~ISimulationObject()
{

}

