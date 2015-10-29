#include "IElement.h"


IElement::IElement() : name( std::string("not mentioned") ),  pose( new se3() )
{	

}


IElement::IElement( std::string name_, std::unique_ptr<se3> pose_) : name( name_ ), pose( std::move(pose_) )
{

}
	
IElement::~IElement()
{
<<<<<<< HEAD
	//pas de delete sur pose puisque la destruction du unique_ptr s'en chargera...
=======

>>>>>>> ebe99669e863ee1fc796e9143fe141ffeac82d57
}
