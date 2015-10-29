#ifndef ETATENGINE_H
#define ETATENGINE_H

#include "IEngine.h"
#include "Environnement.h"

class EtatEngine : IEngine
{
	public :
	
	Environnement* env;
	
	//---------------------------------------------
	//---------------------------------------------
	
	EtatEngine(Game* game_, GameState gameState_);
	~EtatEngine();
	
	void loop() override ;

	void init();

};

#endif
