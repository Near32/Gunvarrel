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

class LoadMapCommand : public ICommand
{
	private :
	std::string fileName;
	
	public :
	LoadMapCommand(std::string fileName_) : ICommand(TCLoadMap), fileName(fileName_)
	{
	
	}
	
	~LoadMapCommand()
	{
	
	}
	
	//------------------------------------
	const std::string getFileName() const
	{
		return fileName;
	}
};


class NewCommand : public ICommand
{
	public :
	NewCommand() : ICommand(TCNew)
	{
	
	}

	~NewCommand()
	{
	
	}
};


class LoadCommand : public ICommand
{
	private :
	std::string file_name;
	
	public :
	LoadCommand(std::string file_name_) : ICommand(TCLoad), file_name(file_name_)
	{
	
	}
	
	~LoadCommand()
	{
	
	}
	
	//------------------------------------
	const std::string getFileName() const
	{
		return file_name;
	}

};


class PauseCommand : public ICommand
{
	public :
	PauseCommand() : ICommand(TCPause)
	{
	
	}

	~PauseCommand()
	{
	
	}

};

class GoOnCommand : public ICommand
{
	public :
	GoOnCommand() : ICommand(TCGoOn)
	{
	
	}

	~GoOnCommand()
	{
	
	}

};


class QuitCommand : public ICommand
{
	public :
	QuitCommand() : ICommand(TCQuit)
	{
	
	}

	~QuitCommand()
	{
	
	}

};

class RigidBodysCommand : public ICommand
{
	private :
	enum IDJoint joint;
	enum StateJoint jointState;
		
	public :
	RigidBodyCommand()
	{
	
	}
	
	~RigidBodyCammand()
	{
	
	}	
	
	//------------------------------
	const enum IDJoint getIDJoint() const
	{
		return joint;
	}
	
	const enum StateJoint getStateJoint() const
	{
		return jointState;
	}
};

#endif
