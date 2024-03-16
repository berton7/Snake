#include "Game.hpp"

int main()
{
	Game game;
	game.init({400, 30});
	game.setFrameRate(60);
	while (game.isRunning())
	{
		game.loop();
	}
	return 0;
}
