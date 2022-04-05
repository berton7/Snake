#pragma once
#include <SDL2/SDL.h>
#include "Logger.hpp"
#include "Snake.hpp"
#include <memory>

typedef struct
{
	int winSize, nCells;
} GameSettings;

/*
typedef struct
{
	int winSize, winH, cellsSize, cellsH, nCellsX, nCellsY
} GameData;
*/
typedef struct
{
	float cellsSize;
	int winSize, nCellsX;
} GameData;

class Game
{
private:
	SDL_Window *mWindow{ nullptr };
	SDL_Renderer *mRenderer{ nullptr };
	logging::Logger mLogger;
	bool mSDLInitialized{ false };
	Uint64 mDeltaTime { 0 };
	Uint64 mMaxLoopTimeForFPS { 0 };
	int mFrames {0};

	bool mIsRunning{ false };

	GameData mData;
	Snake::Direction mNextDirection {Snake::Direction::Right};
	Snake mSnake;
	Edible mEdible;

	void gameSettingsToData(const GameSettings &settings);

public:
	Game();
	~Game();

	void setFrameRate(int fps);

	int init(const GameSettings &settings);
	void resetGame();
	void tick();
	void handleEvents();
	void update();
	void render();
	void loop();
	void end();

	bool isRunning() { return mIsRunning; }
	void scheduleQuit();
};

