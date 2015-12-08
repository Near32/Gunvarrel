#include "VueEngine.h"
#include "Game.h"

#define debug
//#define debuglvl1


extern mutex ressourcesMutex;


VueEngine::VueEngine(Game* game_, GameState gameState_) : IEngine(game_,gameState_)
{
	
}

VueEngine::~VueEngine()
{
	delete camera;
	SDL_Quit();
}
	
void VueEngine::loop()
{
	//THE INITIALIZATION PHASE HAS TO BE PUT IN THE SAME THREAD IN ORDER TO HAVE ACCESS TO THE INITIALIZING HEAP SINCE
	//OPENGL DOES USE THIS HEAP TO INITIALIZE THINGS AND THEN TO RENDER THINGS. 
	
	init();
	
	
	ressourcesMutex.lock();
	bool gameON = game->gameON;
	ressourcesMutex.unlock();
		
	while(gameON)
	{
		if( commandsToHandle.size() > 0)
		{
			//let's verify that it is one of those dedicated commands :
			switch( commandsToHandle[0].getCommandType())
			{
				case TCCameraOnMouseMotion :
				{
				camera->onMouseMotion( ((CameraOnMouseMotionCommand&)commandsToHandle[0]).mmevent);
				}
				break;
				
				case TCCameraOnMouseButton :
				{
				camera->onMouseButton( ((CameraOnMouseButtonCommand&)commandsToHandle[0]).mbevent);
				}
				break;
				
				case TCCameraOnKeyboard :
				{
				camera->onKeyboard( ((CameraOnKeyboardCommand&)commandsToHandle[0]).kevent);
				}
				break;
				
				default :
				{
				
				commandsToHandle.erase(commandsToHandle.begin());
				}
				break;
			}
		}
		
		//----------------------------------------
		ressourcesMutex.lock();
		Dessiner(0.0f,0.0f);
		ressourcesMutex.unlock();
		
		ressourcesMutex.lock();
		gameON = game->gameON;
		ressourcesMutex.unlock();
	}
}

void VueEngine::init()
{
    XInitThreads();
	SDL_Init(SDL_INIT_VIDEO);
    //atexit(stop);
    SDL_WM_SetCaption("Gunvarrel", NULL);
    ecran = SDL_SetVideoMode(800, 600, 32, SDL_OPENGL);
    
    
    //--------------------
	//Camera 
	camera = new TrackBallCamera();
	
	//-------------------------
	
	glClearColor(1.0f,1.0f,1.0f,1.0f);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective(70,(double)800/600,1,1000);
    glEnable(GL_DEPTH_TEST);
    
    
    //---------------------------------
    //---------------------------------
    
    std::string pathElement("../res/element.obj");
    std::string gunvarrel("../res/gunvarrel_scaled.obj");
    loadElement(pathElement);
    loadElement(gunvarrel);
    
}

void VueEngine::Dessiner(float angleX, float angleZ)
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

	//---------------------------
	//Mise a jour de la position de la caméra.
	//game->camera->look();
	camera->look();
	//----------------------------

	drawGunvarrel();
	
	Environnement* env = game->ptrEtat->env;//getEnvironnementFromETATENGINE();
	
	std::string pathElement("../res/element.obj");//el10x10x20.obj");

#ifdef debug
//ressourcesMutex.lock();
std::cout << " VUE : " << env->ListeElements.size() << " element(s) to draw." << std::endl;
//ressourcesMutex.unlock();
#endif	
	
	for(int i=0;i<env->ListeElements.size();i++)
	{
		//ressourcesMutex.lock();
		Mat<float> poseElement = env->ListeElements[i]->pose->exp();
		//ressourcesMutex.unlock();
		Mat<float> SO3( extract( poseElement, 1,1, 3,3) );
		Mat<float> EulerAngles(3,1);
		Rot2Euler(SO3, EulerAngles);
		
		
		//let us go in the correct configuration to draw the Element :
		glTranslated( poseElement.get(1,4), poseElement.get(2,4), poseElement.get(3,4));
		glRotated( EulerAngles.get(1,1), 1,0,0);
		glRotated( EulerAngles.get(2,1), 0,1,0);
		glRotated( EulerAngles.get(3,1), 0,0,1);
		//-----------------------------------
		
		
		//--------------------------------
		//--------------------------------
		//let us draw the element once we have identified it...
		//ressourcesMutex.lock();
		if( env->ListeElements[i]->name != std::string("ground") )
		{
			//ressourcesMutex.unlock();
			//drawElement( pathElement );
			drawElement( containerV[pathElement], containerUV[pathElement], containerN[pathElement] );
		}
		else
		{
			//ressourcesMutex.unlock();
			//ground...
			glBegin(GL_QUADS);
			glColor3ub(230,230,230);
			glVertex3d(-100,-100,-2);
			glVertex3d(100,-100,-2);
			glVertex3d(100,100,-2);
			glVertex3d(-100,100,-2);
			glEnd();
		}
#ifdef debuglvl1		
//ressourcesMutex.lock();
std::cout << " VUE : element : " << env->ListeElements[i]->name << " has been drawn." << std::endl;
//ressourcesMutex.unlock();
#endif
		//--------------------------------
		//--------------------------------
		
		//let us come back to the identity configuration :
		glRotated( -EulerAngles.get(3,1), 0,0,1);
		glRotated( -EulerAngles.get(2,1), 0,1,0);
		glRotated( -EulerAngles.get(1,1), 1,0,0);
		glTranslated( -poseElement.get(1,4), -poseElement.get(2,4), -poseElement.get(3,4));
		//-----------------------------------
		
		
	}
	
	/*-----------------------------------*/	
	/*test code that will be deleted....*/
	/*-----------------------------------*/
	
	//ground...
	glBegin(GL_QUADS);
	glColor3ub(230,230,230);
    glVertex3d(-100,-100,-2);
    glVertex3d(100,-100,-2);
    glVertex3d(100,100,-2);
    glVertex3d(-100,100,-2);
	glEnd();
	
	
	
    glRotated(angleX,1,0,0);	
    glRotated(angleZ,0,0,1);


	//OrbeBonus...
    glBegin(GL_QUADS);
    glColor3ub(0,0,0); 
    glVertex3d(1,1,1);
    glVertex3d(1,1,-1);
    glVertex3d(-1,1,-1);
    glVertex3d(-1,1,1);

    glColor3ub(10,10,10); 
    glVertex3d(1,-1,1);
    glVertex3d(1,-1,-1);
    glVertex3d(1,1,-1);
    glVertex3d(1,1,1);

    glColor3ub(20,20,20); 
    glVertex3d(-1,-1,1);
    glVertex3d(-1,-1,-1);
    glVertex3d(1,-1,-1);
    glVertex3d(1,-1,1);

    glColor3ub(30,30,30);
    glVertex3d(-1,1,1);
    glVertex3d(-1,1,-1);
    glVertex3d(-1,-1,-1);
    glVertex3d(-1,-1,1);

    glColor3ub(40,40,40);
    glVertex3d(1,1,-1);
    glVertex3d(1,-1,-1);
    glVertex3d(-1,-1,-1);
    glVertex3d(-1,1,-1);

    glColor3ub(50,50,50);
    glVertex3d(1,-1,1);
    glVertex3d(1,1,1);
    glVertex3d(-1,1,1);
    glVertex3d(-1,-1,1);
    
    /*--------------------------------------------*/	
	/*END OF : test code that will be deleted....*/
	/*------------------------------------------*/

    glEnd();

    //glFlush();
//ressourcesMutex.lock();    
    SDL_GL_SwapBuffers();
//ressourcesMutex.unlock();    
}



void VueEngine::drawGunvarrel()
{
	std::string gunvarrel("../res/gunvarrel_scaled.obj");
	std::vector<glm::vec3> v = containerV[gunvarrel];
	std::vector<glm::vec2> uv = containerUV[gunvarrel];
	std::vector<glm::vec3> n = containerN[gunvarrel];
	//bool res = loadOBJ("../res/gunvarrel_scaled.obj", v,uv,n);
	
	
	//glBegin(GL_TRIANGLES);
	glRotated(90,1,0,0);
	glTranslated(2,2.8,0);
	
	for(int i=0;i<v.size();i++)
	{
		if(i%3 == 0)
		{
			glBegin(GL_TRIANGLES);
			int color = (i)%255;
			glColor3ub(color,color,color);
		}
		
		glVertex3d( v[i].x, v[i].y, v[i].z);
		
		if( (i+1)%3 == 0)
		{
			glEnd();
		}
	}
	
	//glEnd();
	glTranslated(-2,-2.8,0);
	glRotated(-90,1,0,0);
	
	//------------------------
	//------------------------
}

void VueEngine::drawElement(const std::string& path)
{
	std::vector<glm::vec3> v;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> n;
	bool res = loadOBJ(path.c_str(), v,uv,n);
	
	if(res)
	{
		for(int i=0;i<v.size();i++)
		{
			if(i%3 == 0)
			{
				glBegin(GL_TRIANGLES);
				int color = (i)%255;
				glColor3ub(color,color,color);
			}
		
			glVertex3d( v[i].x, v[i].y, v[i].z);
		
			if( (i+1)%3 == 0)
			{
				glEnd();
			}
		}
	}
	else
	{
		cerr << "Impossible de charger l'element : " << path << endl;
	}
}

void VueEngine::drawElement(const std::vector<glm::vec3>& v, const std::vector<glm::vec2>& uv, const std::vector<glm::vec3>& n)
{	
	for(int i=0;i<v.size();i++)
	{
		if(i%3 == 0)
		{
			glBegin(GL_TRIANGLES);
			int color = (i)%255;
			glColor3ub(color,color,color);
		}
	
		glVertex3d( v[i].x, v[i].y, v[i].z);
	
		if( (i+1)%3 == 0)
		{
			glEnd();
		}
	}
} 

void VueEngine::loadElement( const std::string& path)
{
	std::vector<glm::vec3> v;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> n;
	bool res = loadOBJ(path.c_str(), v,uv,n);
	
	if( res)
	{
		std::cout << "SUCCESSFULL LOADING : " << path << endl;
		containerV[path] = v;
		containerUV[path] = uv;
		containerN[path] = n;
	}
	else
	{
		std::cout << "ERROR WHILE LOADING : " << path << endl;
	}
}
