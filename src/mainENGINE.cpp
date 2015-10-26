#include "Game.h"

void stop();



int main(int argc, char* argv[])
{
	Game game;
	game.init();
	
	game.loop();
	
	stop();
	
	return 0;
}


void stop()
{
	SDL_Quit();
}
