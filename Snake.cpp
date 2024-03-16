#include "Snake.hpp"
#include "Logger.hpp"

Snake::Snake(int startX, int startY): mStartX(startX), mStartY(startY), mLogger("Snake")
{
}

void Snake::setDirection(Direction d)
{
    if (d != Direction::None && d != mDirection){
        switch(mDirection)
        {
        case Direction::Up:
            if (d==Direction::Down) {
                mLogger.debug("thats not possible!");
                return;
            }
            break;
        case Direction::Down:
            if (d==Direction::Up) {
                mLogger.debug("thats not possible!");
                return;
            }
            break;
        case Direction::Right:
            if (d==Direction::Left) {
                mLogger.debug("thats not possible!");
                return;
            }
            break;
        case Direction::Left:
            if (d==Direction::Right) {
                mLogger.debug("thats not possible!");
                return;
            }
            break;
        default:
            return;
        }
        mDirection = d;
    }
}

void Snake::kill()
{
    mLogger.info("I'M Dead!");
    mIsAlive = false;
    goBack();
};

void Snake::goBack()
{
    mCells.pop_back();
    mCells.emplace_front(prevLastCell);
}

void Snake::update(const Edible &edible)
{
    mHasEaten = false;
    const auto &prevHead = mCells.back();
    prevLastCell = mCells.front();
    switch (mDirection)
    {
    case Direction::Up:
        mCells.emplace_back(std::make_pair(prevHead.first, prevHead.second-1));
        break;
    case Direction::Down:
        mCells.emplace_back(std::make_pair(prevHead.first, prevHead.second+1));
        break;
    case Direction::Right:
        mCells.emplace_back(std::make_pair(prevHead.first+1, prevHead.second));
        break;
    case Direction::Left:
        mCells.emplace_back(std::make_pair(prevHead.first-1, prevHead.second));
        break;
    default:
        break;
    }

    const auto &newHead = mCells.back();
    bool maybeDead = false;
    for (const auto &cell: mCells)
    {
        // delay 1 cycle to not check head
        if (maybeDead)
        {
            kill();
            return;
        }
        if (newHead.first == cell.first && newHead.second == cell.second)
        {
            maybeDead = true;
        }
    }

    if (newHead.first == edible.getX() && newHead.second == edible.getY()) mHasEaten = true;
    if (!mHasEaten)
        mCells.pop_front();
}

void Snake::draw(SDL_Renderer *renderer, int cellW, int cellH)
{
    SDL_Rect r;
    r.w = cellW;
    r.h = cellH;
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (const auto &cell: mCells)
    {
        r.x = cell.first * cellW;
        r.y = cell.second * cellH;
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &r);
        // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        // SDL_RenderDrawRect(renderer, &r);
    }
    r.x = mCells.back().first * cellW;
    r.y = mCells.back().second * cellH;
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(renderer, &r);
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // SDL_RenderDrawRect(renderer, &r);
}

void Snake::reset()
{
    // start with 3
    mLogger.setLevel(logging::Level::Debug);
    mCells.clear();
    mDirection = Direction::Right;
    mHasEaten = false;
    mIsAlive = true;
    constexpr int startLen = 3;
    prevLastCell.first = mStartX-startLen;
    prevLastCell.second = mStartY;
    for (int i=mStartX-startLen+1; i<=mStartX; ++i)
    {
        mCells.emplace_back(std::make_pair(i, mStartY));
    }
    mLogger.info("I'M Alive!");
}
