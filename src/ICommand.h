#ifndef ICOMMAND_H
#define ICOMMAND_H

#include "TypeCommand.h"
#include "IDJoint.h"
#include "StateJoint.h"

class ICommand
{
	protected :
	enum TypeCommand type;
	
	public :
	
	ICommand(const enum TypeCommand t) : type(t)
	{
	
	}
	
	virtual ~ICommand()
	{
	
	}
	
	
	//-----------------------------
	
	const enum TypeCommand getCommandType()
	{
		return type;
	}
	
	void setCommandType( const enum TypeCommand t)
	{
		type = t;
	}
	
};




class CameraCommand : public ICommand
{
	private :
	float angleX;
	float angleY;
	float distance;
	
	public :
	
	CameraCommand( float aX = 0.0f, float aY = 0.0f, float dist = 1.0f) : ICommand(TCCamera), angleX(aX), angleY(aY), distance(dist)
	{
	
	}
	
	~CameraCommand()
	{
	
	}
	
	
	//----------------------------------------
	const float getAngleX() const
	{
		return angleX;
	}
	
	const float getAngleY() const
	{
		return angleY;
	}
	
	const float getDistance() const
	{
		return distance;
	}
	
};



#endif
