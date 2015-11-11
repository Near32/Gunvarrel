#ifndef GAME_H
#define GMAE_H

#include <iostream>

#include "GameState.h"
#include "EtatEngine.h"
#include "VueEngine.h"
#include "ICommand.h"



class Game
{
	public :
	
	EtatEngine* ptrEtat;
	VueEngine* ptrVue;
	
	GameState gameState;
	bool gameON;
	
	TrackBallCamera* camera;
	
	std::vector<ICommand> currentCommands;
	std::vector<ICommand> waitingCommands;
	int nbrCurrentCommandsHandled;
	
	//---------------------------------------
	//---------------------------------------
	
	Game();
	~Game();
	
	
	void loop();	
	
	void init();
	
	
	void incrementNbrHandledCommands();
	
};

#endif
