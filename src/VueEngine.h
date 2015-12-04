#ifndef VUEENGINE_H
#define VUEENGINE_H

#include "IEngine.h"

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <cstdlib>
#include "TrackBallCamera.h"
#include "common/objloader.h"
#include "utils/math.h"
#include <thread>
#include <mutex>

class VueEngine : public IEngine
{
	public :
	
	SDL_Surface* ecran;
	TrackBallCamera* camera;
	
	
	VueEngine(Game* game_, GameState gameState_);
	~VueEngine();
	
	void loop() override ;
	void init();
	
	void Dessiner(float angleX, float angleZ);
	
	void drawGunvarrel();
	void drawElement(const std::string& path);
};

#endif
