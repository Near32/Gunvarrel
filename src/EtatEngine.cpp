#include "EtatEngine.h"
#include "Game.h"

EtatEngine::EtatEngine(Game* game_, GameState gameState_) : IEngine(game_,gameState_)
{
	env = new Environnement();
}

EtatEngine::~EtatEngine()
{
	delete env;
}
	
void EtatEngine::loop()
{
	while(game->gameON)
	{
	
	}
}
