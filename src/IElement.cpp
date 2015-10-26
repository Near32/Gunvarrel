#include "IElement.h"


IElement::IElement() : name( std::string("not mentioned") ),  pose( new se3() )
{	

}


IElement::IElement( std::string name_, std::unique_ptr<se3> pose_) : name( name_ ), pose( std::move(pose_) )
{

}
	
IElement::~IElement()
{

}
