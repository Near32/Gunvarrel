#include "IEngine.h"

#include "Game.h"

IEngine::IEngine()
{
	gameState = NOP;
}

IEngine::IEngine(Game* game_, GameState gameState_) : game(game_), gameState(gameState_)
{

}

IEngine::~IEngine()
{

}
