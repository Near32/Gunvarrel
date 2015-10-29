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
	
	
<<<<<<< HEAD
	virtual bool isFixe() =0 ;
=======
	virtual bool isFixe()=0;
>>>>>>> ebe99669e863ee1fc796e9143fe141ffeac82d57
	
	
};


#endif
