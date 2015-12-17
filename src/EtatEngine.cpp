#include "EtatEngine.h"
#include "Game.h"

#undef NDEBUG

extern std::mutex ressourcesMutex;


EtatEngine::EtatEngine(Game* game_, GameState gameState_) : IEngine(game_,gameState_)
{
	env = new Environnement();
	
	init();
	
}

EtatEngine::~EtatEngine()
{
	delete env;
	delete sim;
}
	
void EtatEngine::loop()
{
	ressourcesMutex.lock();
	bool gameON = game->gameON;
	ressourcesMutex.unlock();
	
	while(gameON)
	{
		if( commandsToHandle.size() > 0)
		{
			//let's verify that it is one of those dedicated commands :
			switch( (commandsToHandle[0].get())->getCommandType())
			{
				//DEBUGGING :
				case TCSimulateStride:
				{
				float timestep = 1e-1f;
				float time = sim->getTime();
#ifdef debug
std::cout << "SIMULATION : run : ..." << std::endl;
#endif					
				sim->run(timestep,time+10*timestep);
#ifdef debug
std::cout << "SIMULATION : run : successfully !!" << std::endl;
#endif				
				
				commandsToHandle.erase(commandsToHandle.begin());
				}
				break;
				
				
				default :
				{
				commandsToHandle.erase(commandsToHandle.begin());
				}
				break;
			}
		}
		
		ressourcesMutex.lock();
		gameON = game->gameON;
		ressourcesMutex.unlock();
	}
}

void EtatEngine::init()
{
	//let's create the Elements that we need.
	Mat<float> hwd(500.0f,3,1);
	Mat<float> t((float)0,3,1);
	
	ConstraintsList cl;
	
	//--------------------------------
	//create the elements :
	//map : ground :
	//ground :
	t.set( -hwd.get(3,1)/2,3,1);
	env->addElement( env->fabriques->fabriquer(ELFObstacle, std::string("ground"), new se3(t), hwd ) );
	//
	//sa position est bien à l'origine..
	//resetting :
	t *= 0.0f;
	hwd = Mat<float>(1.0f,3,1);
	//--------------------------------
	
	
	//Gunvarrel :
	hwd *= 10.0f;
	hwd.set(20.0f,3,1);
	t.set( hwd.get(3,1)/2+1.0f, 3,1);	
	env->addElement( new ElementMobile(std::string("picBAS"), new se3(t), hwd) );
	
	t.set( t.get(3,1)+hwd.get(3,1)+2.0f, 3,1);
	
	t.set( t.get(2,1)+0.0f, 2,1);
		
	env->addElement( new ElementMobile(std::string("picHAUT"), new se3(t), hwd) );
	
	//constraints :
	Mat<float> AnchorAL(0.0f,3,1);
	AnchorAL.set( -hwd.get(3,1)/2-2.0f, 3,1);
	//Mat<float> HJAxis(0.0f,3,1);
	//HJAxis.set( 1.0f, 1,1);
	Mat<float> AnchorBL(0.0f,3,1);
	AnchorBL.set( hwd.get(3,1)/2+2.0f, 3,1);
	cl.insert( cl.end(), ConstraintInfo(std::string("picBAS"),std::string("picHAUT"), CTBallAndSocketJoint, operatorL(AnchorAL,AnchorBL) ) ); 
	
	//resetting :
	t = Mat<float>(0.0f,3,1);
	hwd = Mat<float>(10.0f,3,1);
	//--------------------------------
	
	/*
	//map : obstacles :
	int nbrObstacle = 0;
	//on veut changer sa position avec une hauteur un peu plus grande :
	t.set(20.0f,3,1);
	t.set(10.0f,1,1);
	t.set(30.0f,2,1); 
	env->addElement( env->fabriques->fabriquer(ELFObstacle, std::string("obstacle")+std::to_string(nbrObstacle), new se3(t), hwd ) );
	nbrObstacle++;
	//--------------------------------
	
	
	//map : orbe bonus :
	//orbe bonus :
	int nbrOrbeBonus = 0;
	//on veut changer sa position avec une hauteur un peu plus grande :
	t.set(20.0f,3,1);
	t.set(30.0f,1,1);
	t.set(30.0f,2,1); 
	env->addElement( env->fabriques->fabriquer(ELFOrbeBonus, std::string("orbebonus")+std::to_string(nbrOrbeBonus), new se3(t), hwd ) );
	nbrOrbeBonus++;
	*/
	//--------------------------------
	
	//-------------------------------
	
	sim = new Simulation(env,cl);
		
}
