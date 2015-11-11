#include "Game.h"

Game::Game() : gameON(true), gameState( MENUINIT ), ptrEtat( new EtatEngine(this, MENUINIT) ), ptrVue( new VueEngine(this, MENUINIT) )
{
	nbrCurrentCommandsHandled = 0;
}

Game::~Game()
{
	delete ptrEtat;
	delete ptrVue;
	delete camera;
}
	
	
void Game::loop()
{
	Uint32 last_time = SDL_GetTicks();
    Uint32 current_time,ellapsed_time;
    Uint32 start_time;
    SDL_Event event;
    
    float angleX = 0.0f;
    float angleZ = 0.0f;
    
	while(gameON)
	{
		start_time = SDL_GetTicks();
		
		//traitement/récupération des events.
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                exit(0);
                break;
                
                case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                	case SDLK_ESCAPE:
                	exit(0);
                	break;
                	
                	default:
                	camera->onKeyboard(event.key);
                	break;
                }
                break;
                
                case SDL_MOUSEMOTION:
                camera->onMouseMotion(event.motion);
                break;
                
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN:
                camera->onMouseButton(event.button);
                break;
            }
        }

		
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
        ptrVue->Dessiner(angleX,angleZ);
        
        
        //--------------------------------------
        //--------------------------------------
        //--------------------------------------
                        
        ellapsed_time = SDL_GetTicks() - start_time;
        if (ellapsed_time < 20)
        {
            SDL_Delay(20 - ellapsed_time);
        }
		
	}
}

void Game::init()
{
    SDL_Init(SDL_INIT_VIDEO);
    //atexit(stop);
    SDL_WM_SetCaption("Gunvarrel", NULL);
    SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);
    
    
    //--------------------
	//Camera 
	camera = new TrackBallCamera();
	
	//-------------------------
	
	glClearColor(1.0f,1.0f,1.0f,1.0f);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(70,(double)800/600,1,1000);
    glEnable(GL_DEPTH_TEST);
    
}


void Game::incrementNbrHandledCommands()
{
	nbrCurrentCommandsHandled++;
}


