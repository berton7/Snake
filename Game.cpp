#include "Game.hpp"
#include "Snake.hpp"

Game::Game(): mLogger("Game"), mSnake(4,4)
{
	mLogger.setLevel(logging::Level::Debug);
	mLogger.setOption(logging::Options::ENABLE_COLOR, true);
	setFrameRate(60);
}

Game::~Game()
{
}

void Game::setFrameRate(int fps)
{
	mMaxLoopTimeForFPS = 1000 / fps;
}

void Game::gameSettingsToData(const GameSettings &settings)
{
	mData.cellsSize = (float)settings.winSize / (float)settings.nCells;
	mData.nCellsX = settings.nCells;
	mData.winSize = settings.winSize;
}

int Game::init(const GameSettings &settings)
{
	gameSettingsToData(settings);

	int rc;
	if ((rc=SDL_Init(SDL_INIT_VIDEO)))
	{
		mLogger.error("Failed to init SDL: ", SDL_GetError());
		return rc;
	}
	mLogger.info("Successfully initialized SDL.", "");
	mSDLInitialized = true;

	mWindow = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mData.winSize, mData.winSize, SDL_WINDOW_SHOWN);
	if (!mWindow)
	{
		mLogger.error("Failed to initialize window: ", SDL_GetError());
		return 1;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, 0);
	if (!mRenderer)
	{
		mLogger.error("Failed to initialize renderer: ", SDL_GetError());
		return 2;
	}

	mLogger.debug("Game is starting");
	resetGame();
	mIsRunning = true;
	return 0;
}

void Game::resetGame()
{
	mNextDirection = Snake::Direction::Right;
	mSnake.reset();
	mEdible.reset();
	mEdible.move(mData.nCellsX, mData.nCellsX);
}

void Game::end()
{
	if (mWindow)
		SDL_DestroyWindow(mWindow);
	if (mRenderer)
		SDL_DestroyRenderer(mRenderer);
	if (mSDLInitialized)
		SDL_Quit();
}

void Game::tick()
{
	handleEvents();
	update();
}

void Game::handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			scheduleQuit();
			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
			case SDLK_q:
				scheduleQuit();
				break;
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_r:
				if (!mSnake.isAlive())
					resetGame();
				break;
			case SDLK_UP:
				mNextDirection = Snake::Direction::Up;
				break;
			case SDLK_DOWN:
				mNextDirection = Snake::Direction::Down;
				break;
			case SDLK_LEFT:
				mNextDirection = Snake::Direction::Left;
				break;
			case SDLK_RIGHT:
				mNextDirection = Snake::Direction::Right;
				break;
			}
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				int mouseX, mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				mLogger.debug("Left mouse button pressed at ", mouseX, " ", mouseY);
			}
			break;
		}
	}
}

void Game::update()
{
	if (mSnake.isAlive())
	{
		mSnake.setDirection(mNextDirection);
		mSnake.update(mEdible);
		int winX, winY;
		SDL_GetWindowSize(mWindow, &winX, &winY);
		switch (mNextDirection)
		{
		case Snake::Direction::Up:
			if (mSnake.getHeadY() < 0)
				mSnake.kill();
			break;
		case Snake::Direction::Down:
			if (mSnake.getHeadY() >= mData.nCellsX)
				mSnake.kill();
			break;
		case Snake::Direction::Left:
			if (mSnake.getHeadX() < 0)
				mSnake.kill();
			break;
		case Snake::Direction::Right:
			if (mSnake.getHeadX() >= mData.nCellsX)
				mSnake.kill();
			break;
		}
		if (mSnake.hasEaten())
			mEdible.move(mData.nCellsX, mData.nCellsX);
	}
	//else mLogger.info("dead");
}

void Game::render()
{
	if (SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255))
	{
		mLogger.error("Failed to set renderer draw color: ", SDL_GetError());
	}
	if (SDL_RenderClear(mRenderer))
	{
		mLogger.error("Failed to clear renderer: ", SDL_GetError());
	}
	mEdible.draw(mRenderer, mData.cellsSize,mData.cellsSize);
	mSnake.draw(mRenderer, mData.cellsSize,mData.cellsSize);
	SDL_RenderPresent(mRenderer);
}

void Game::loop()
{
	const Uint64 begin = SDL_GetTicks64();
	if (!(mFrames%6))
	{
		tick();
	}
	render();
	++mFrames;
	mDeltaTime = SDL_GetTicks64() - begin;
	if (mMaxLoopTimeForFPS > mDeltaTime)
		SDL_Delay(mMaxLoopTimeForFPS - mDeltaTime);
	else
		mLogger.warn("Max loop time exceeded: frame: ", mFrames, ", delta: ", mDeltaTime);
	//mLogger.debug("Loop done in ", mDeltaTime, " ms.");
}

void Game::scheduleQuit()
{
	mLogger.debug("Scheduled Quit.");
	mIsRunning = false; mIsRunning = false;
};
