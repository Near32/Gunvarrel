#include "EtatEngine.h"
#include "Game.h"


EtatEngine::EtatEngine(Game* game_, GameState gameState_) : IEngine(game_,gameState_)
{
	env = new Environnement();
	
}

EtatEngine::~EtatEngine()
{
	delete env;
	delete sim;
}
	
void EtatEngine::loop()
{
	while(game->gameON)
	{
		if( commandsToHandle.size() > 0)
		{
			//let's verify that it is one of those dedicated commands :
			switch( commandsToHandle[0].getCommandType())
			{
				default :
				
				commandsToHandle.erase(commandsToHandle.begin(),0);
				break;
			}
		}
	}
}

void EtatEngine::init()
{
	//let's create the Elements that we need.
	Mat<float> hwd(10.0f,3,1);
	Mat<float> t(0.0f,3,1);
	ConstraintsList cl;
	
	//--------------------------------
	//create the elements :
	//map : ground :
	//ground :
	t.set( -hwd.get(3,1)/2,3,1);
	env->addElement( env->fabriques->fabriquer(ELFObstacle, std::string("ground"), new se3(t) ) );
	//
	//sa position est bien à l'origine..
	//resetting :
	t *= 0.0f;
	hwd *= 1.0f/10.0f;
	//--------------------------------
	
	
	//Gunvarrel :
	env->addElement( new ElementMobile(
	
	
	//resetting :
	t = Mat<float>(0.0f,3,1);
	hwd = Mat<float>(1.0f,3,1);
	//--------------------------------
	
	
	//map : obstacles :
	int nbrObstacle = 0;
	//on veut changer sa position avec une hauteur un peu plus grande :
	t.set(2.0f,3,1);
	t.set(1.0f,1,1);
	t.set(3.0f,2,1); 
	env->addElement( env->fabriques->fabriquer(ELFObstacle, std::string("obstacle")+std::string(nbrObstacle), new se3(t), hwd ) );
	nbrObstacle++;
	//--------------------------------
	
	
	//map : orbe bonus :
	//orbe bonus :
	int nbrOrbeBonus = 0;
	//on veut changer sa position avec une hauteur un peu plus grande :
	t.set(2.0f,3,1);
	t.set(1.0f,1,1); 
	env->addElement( env->fabriques->fabriquer(ELFOrbeBonus, std::string("orbebonus")+std::string(nbrOrbeBonus), new se3(t), hwd ) );
	nbrOrbeBonus++;
	//--------------------------------
	
	//-------------------------------
	
	sim = new Simulation(env,cl);
		
}
