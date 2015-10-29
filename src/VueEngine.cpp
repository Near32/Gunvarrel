#include "VueEngine.h"
#include "Game.h"

VueEngine::VueEngine(Game* game_, GameState gameState_) : IEngine(game_,gameState_)
{
	
}

VueEngine::~VueEngine()
{

}
	
void VueEngine::loop()
{
	while(game->gameON)
	{
		
	}
}

void VueEngine::Dessiner(float angleX, float angleZ)
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

	//---------------------------
	game->camera->look();
	//----------------------------

	drawGunvarrel();
	
	Environnement* env = game->ptrEtat->env;//getEnvironnementFromETATENGINE();
	
	for(int i=0;i<env->ListeElements.size();i++)
	{
		Mat<float> poseElement = env->ListeElements[i]->pose->exp();
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

    glFlush();
    SDL_GL_SwapBuffers();
}



void VueEngine::drawGunvarrel()
{
	std::vector<glm::vec3> v;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> n;
	bool res = loadOBJ("../res/gunvarrel_scaled.obj", v,uv,n);
	
	
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
