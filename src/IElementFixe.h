#ifndef IELEMENTFIXE_H
#define IELEMENTFIXE_H

#include "IElement.h"

class IElementFixe : public IElement
{  
	public:
		
//----------------------------------------------------------------
//----------------------------------------------------------------
	IElementFixe();
	IElementFixe(std::string name_, std::unique_ptr<se3> pose_);
			
	virtual ~IElementFixe();
			
	virtual bool isFixe() override ; 
	
	virtual bool isObstacle() = 0 ;

};


#endif
