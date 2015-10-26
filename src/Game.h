#ifndef GAME_H
#define GMAE_H

#include <iostream>

#include "GameState.h"
#include "IEngine.h"
#include "EtatEngine.h"
#include "VueEngine.h"



class Game
{
	public :
	
	EtatEngine* ptrEtat;
	VueEngine* ptrVue;
	
	GameState gameState;
	bool gameON;
	
	TrackBallCamera* camera;
		
	//---------------------------------------
	//---------------------------------------
	
	Game();
	~Game();
	
	
	void loop();	
	
	void init();
	
};

#endif
