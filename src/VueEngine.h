#ifndef VUEENGINE_H
#define VUEENGINE_H

#include "IEngine.h"

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>
#include "TrackBallCamera.h"
#include "common/objloader.h"
<<<<<<< HEAD
#include "utils/math.h"
=======
>>>>>>> ebe99669e863ee1fc796e9143fe141ffeac82d57

class VueEngine : IEngine
{
	public :
	
	VueEngine(Game* game_, GameState gameState_);
	~VueEngine();
	
	void loop() override ;
	
	void Dessiner(float angleX, float angleZ);
	
	void drawGunvarrel();
};

#endif
