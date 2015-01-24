#include "Game.h"

int main()
{
	Game * game = Game::GetInstance();
	game->Init();
	game->Loop();

	return 0;
}