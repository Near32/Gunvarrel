#ifndef ELEMENTMOBILE_H
#define ELEMENTMOBILE_H

#include"IElementMobile.h"

class ElementMobile : public IElementMobile
{
	public:
		ElementMobile();
		ElementMobile(std::string name_, std::unique_ptr <se3> pose_);
		
		~ElementMobile();
		
		virtual bool isFixe() override;
		
};


#endif