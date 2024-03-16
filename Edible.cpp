#include "Edible.hpp"
#include <random>
#include <ctime>

Edible::Edible()
{
}

void Edible::reset()
{
    std::srand(std::time(nullptr));
}

void Edible::move(int maxX, int maxY)
{
    x = std::rand() % maxX;
    y = std::rand() % maxY;
}

void Edible::draw(SDL_Renderer *renderer, int cellW, int cellH)
{
    SDL_Rect r;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    r.x = x * cellW;
    r.y = y * cellH;
    r.w = cellW;
    r.h = cellH;
    SDL_RenderFillRect(renderer, &r);
    // SDL_RenderDrawRect(renderer, &r);
}