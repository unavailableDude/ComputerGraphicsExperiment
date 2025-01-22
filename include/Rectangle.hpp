#pragma once
#include <SDL2/SDL.h>
#include "Vec2int.hpp"


class Rectangle{
    private:
    Vec2int pos;
    Vec2int size;
    
    public:
    Rectangle(int x, int y, int w, int h);

    void Display(SDL_Renderer *renderer);
    void DisplayFilled(SDL_Renderer *renderer);

    void Move(int dx, int dy);
    void SetPos(int x, int y);

    void Center();
};