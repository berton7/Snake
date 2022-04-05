#pragma once

#include <SDL2/SDL.h>

class Edible
{
private:
    int x, y, mMaxX, mMaxY;

public:
    Edible();
    void reset();
    ~Edible() = default;

    int getX() const { return x; }
    int getY() const { return y; }
    void move(int maxX, int maxY);

    void draw(SDL_Renderer *renderer, int cellW, int cellH);
};