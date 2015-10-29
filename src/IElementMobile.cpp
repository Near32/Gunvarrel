#include "IElementMobile.h"

IElementMobile::IElementMobile() :IElement()
{
	
}


IElementMobile::IElementMobile(std::string name_, std::unique_ptr<se3> pose_) : IElement( name_, std::move(pose_) )
{

}

IElementMobile::~IElementMobile()
{

}

bool IElementMobile::isFixe()
{
  return false;
}

