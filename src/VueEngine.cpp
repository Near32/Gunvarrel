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
	
	
	glBegin(GL_QUADS);
	glColor3ub(230,230,230);
    glVertex3d(-100,-100,-2);
    glVertex3d(100,-100,-2);
    glVertex3d(100,100,-2);
    glVertex3d(-100,100,-2);
	glEnd();
	
	
    glRotated(angleZ,0,0,1);
    glRotated(angleX,1,0,0);

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
