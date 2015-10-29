#include "OrbeBonus.h"

OrbeBonus::OrbeBonus() : IElementFixe(), type(NONE)
{
	
}

OrbeBonus::OrbeBonus( std::string name_, std::unique_ptr<se3> pose_) : IElementFixe(name_, std::move(pose_)), type(NONE)
{

}

OrbeBonus::OrbeBonus( TypeOrbeBonus type_, std::string name_, std::unique_ptr<se3> pose_) : IElementFixe(name_, std::move(pose_)), type(type_)
{

}
	
OrbeBonus::~OrbeBonus()
{

}

bool OrbeBonus::isObstacle()
{
	return false;
}
