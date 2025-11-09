#include "Game.h"


int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_VIDEO);
	Game mGame = Game(1400, 800);
	mGame.init();
	mGame.run();
	SDL_Quit();
	return 0;
}