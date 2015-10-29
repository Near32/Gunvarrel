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
<<<<<<< HEAD
	void init();
=======
>>>>>>> ebe99669e863ee1fc796e9143fe141ffeac82d57
};

#endif
