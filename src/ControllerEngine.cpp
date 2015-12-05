#include "ControllerEngine.h"
#include "Game.h"

extern std::mutex ressourcesMutex;

ControllerEngine::ControllerEngine(Game* game_, EtatEngine* ptrEtat_, VueEngine* ptrVue_, const GameState& gameState_) : IEngine(game_,gameState_), ptrEtat(ptrEtat_), ptrVue(ptrVue_)
{
	init();
}

ControllerEngine::~ControllerEngine()
{

}
	
void ControllerEngine::loop()
{
	Uint32 last_time = SDL_GetTicks();
    Uint32 current_time,ellapsed_time;
    Uint32 start_time;
    SDL_Event event;
    
    float angleX = 0.0f;
    float angleZ = 0.0f;
    
    ressourcesMutex.lock();
    bool gameON = game->gameON;
    ressourcesMutex.unlock();
    
	while(gameON)
	{
		start_time = SDL_GetTicks();
		
		//traitement/récupération des events.
		ressourcesMutex.lock();
        while (SDL_PollEvent(&event))
        {
        	ressourcesMutex.unlock();
            switch(event.type)
            {
                case SDL_QUIT:
                //exit(0);
                gameON = false;
                break;
                
                case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                	case SDLK_ESCAPE:
                	//exit(0);
                	gameON = false;
                	break;
                	
                	case SDLK_SPACE:
                	{
#ifdef debug
std::cout << "GAME : add command : TCSimulateStride : DEBUG." << std::endl;
#endif                	
                	ptrEtat->addCommandToHandle( (const ICommand&)SimulateStrideCommand() );
                	}
                	break;
                	
                	default:
                	{
                	//camera->onKeyboard(event.key);
                	ptrVue->addCommandToHandle( (const ICommand&)CameraOnKeyboardCommand(event.key));
                	}
                	break;
                }
                break;
                
                case SDL_MOUSEMOTION:
                {
                //camera->onMouseMotion(event.motion);
                ptrVue->addCommandToHandle( (const ICommand&)CameraOnMouseMotionCommand(event.motion) );
                }
                break;
                
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN:
                {
                //camera->onMouseButton(event.button);
                ptrVue->addCommandToHandle( (const ICommand&)CameraOnMouseButtonCommand(event.button) );
                }
                break;
            }
            
            ressourcesMutex.lock();
        }
        
        ressourcesMutex.unlock();

		
        //--------------------------------------
        //--------------------------------------
        //--------------------------------------
        
        current_time = SDL_GetTicks();
        ellapsed_time = current_time - last_time;
        last_time = current_time;
        
        
        //--------------------------------------
        //--------------------------------------
        //--------------------------------------
        
        
        //MAJ DES ENGINES :
        
        angleZ += 0.05 * ellapsed_time;
        angleX += 0.05 * ellapsed_time;
        //--------------------------------------
        //--------------------------------------
        //--------------------------------------
                        
        ellapsed_time = SDL_GetTicks() - start_time;
        if (ellapsed_time < 20)
        {
            SDL_Delay(20 - ellapsed_time);
        }
        
        ressourcesMutex.lock();
        game->gameON = game;
        ressourcesMutex.unlock();
		
	}
}


void ControllerEngine::init()
{

}
