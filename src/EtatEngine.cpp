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

void EtatEngine::init()
{
	//let's create the Elements that we need.
	env->addElement( env->fabriques->fabriquer(ELFObstacle, std::string("sol"), new se3() ) );
	//sa position est bien à l'origine..
	
	env->addElement( env->fabriques->fabriquer(ELFOrbeBonus, std::string("orbebonus0"), new se3() ) );
	//on veut changer sa position avec une hauteur un peu plus grande :
	Mat<float> t1(0.0f,3,1);
	t1.set(1.0f,3,1);
	env->ListeElements[1]->pose->setT(t1);
	
	
	
}
