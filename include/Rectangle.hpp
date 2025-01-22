#pragma once
#include <SDL2/SDL.h>
#include "Vec2int.hpp"
#include "IDisplayable.hpp"

class Rectangle : IDisplayable{
    private:
    Vec2int pos;
    Vec2int size;
    
    public:
    Rectangle(int x, int y, int w, int h);

    virtual void Display(SDL_Renderer *renderer);
    virtual void DisplayFilled(SDL_Renderer *renderer);

    void Move(int dx, int dy);
    void SetPos(int x, int y);

    void Center();
};