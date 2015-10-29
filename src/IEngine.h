#ifndef IENGINE_H
#define IENGINE_H

#include <iostream>
#include <memory>
#include "GameState.h"

class Game;

class IEngine
{
	public :
	
	Game* game;
	GameState gameState;
	
	//-----------------------------------------------------------
	//-----------------------------------------------------------
	
	IEngine();
	IEngine(Game* game_, GameState gameState_);
	virtual ~IEngine();
	
	virtual void loop()=0;
	
};

#endif
