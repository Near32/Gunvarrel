#ifndef IELEMENT_H
#define IELEMENT_H

#include <iostream>
#include <memory>

#include <string>
#include "utils/math.h"


class IElement
{
	public :
	
	std::string name;
	std::unique_ptr<se3> pose;
	
	//-----------------------------
	//-----------------------------
	
	
	IElement();
	IElement( std::string name, std::unique_ptr<se3> pose );
	
	~IElement();
	
	virtual bool isFixe() =0 ;
	
	se3& getPoseReference() const
	{
		return *pose;
	}
	
	void setPose( const se3& p)
	{
		(*pose) = p;
	}
	
};


#endif
