#ifndef IELEMENTMOBILE_H
#define IELEMENTMOBILE_H

#include "IElement.h"

class IElementMobile : public IElement
{
	public:  

		 IElementMobile();
		 
		 IElementMobile(std::string name_, std::unique_ptr<se3> pose_);
		 
		 virtual ~IElementMobile(); // Le desctructeur d'une interface doit être virtuel (c.f. wikipedia objet composite)
		
		  
		 virtual bool isFixe() override;
		  
		  
};

#endif

