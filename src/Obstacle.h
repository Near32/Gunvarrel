#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "IElementFixe.h"

class Obstacle : IElementFixe
{
	public :
	
	Obstacle();
	Obstacle(std::string name_, std::unique_ptr<se3> pose_);
	
	~Obstacle();
	
	virtual bool isObstacle() override ;
};

#endif
