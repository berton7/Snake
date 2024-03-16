#pragma once
#include <SDL2/SDL.h>
#include <list>
#include <utility>

#include "Edible.hpp"
#include "Logger.hpp"

class Snake
{
public:
    enum class Direction
    {
        None, Up, Down, Left, Right
    };

private:
    typedef std::pair<int,int> Cell;
    Cell prevLastCell;
    int mStartX, mStartY;
    Direction mDirection;
    std::list<Cell> mCells;
    bool mHasEaten, mIsAlive;
    logging::Logger mLogger;

public:
    Snake(int startX, int startY);
    void reset();
    ~Snake() = default;

    int getHeadX() const { return mCells.back().first; };
    int getHeadY() const { return mCells.back().second; };

    void setDirection(Direction d);
    Direction getCurrentDirection() { return mDirection; };
    bool hasEaten() { return mHasEaten; };
    bool isAlive() { return mIsAlive; };
    void kill();
    void goBack();
    void update(const Edible &edible);
    void draw(SDL_Renderer *renderer, int cellW, int cellH);
};
