#include "IElementFixe.h"


IElementFixe::IElementFixe() :  IElement()
{

}

IElementFixe::IElementFixe(std::string name_, std::unique_ptr<se3> pose_) : IElement(name_, std::move(pose_) )
{

}


IElementFixe::~IElementFixe()
{

}

bool IElementFixe::isFixe()
{
  return true;
}
