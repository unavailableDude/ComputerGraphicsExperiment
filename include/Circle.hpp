#pragma once
#include "SDL2/SDL.h"
#include "Vec2int.hpp"
#include "IDisplayable.hpp"


class Circle : public IDisplayable{
    private:
    Vec2int pos;
    int r;
    
    public:
    //construct
    Circle(int x, int y, int r);

    //display circle
    virtual void Display(SDL_Renderer *renderer);
    virtual void DisplayFilled(SDL_Renderer *renderer);
    
    //translate circle
    void Move(int dx, int dy);
    void SetPos(int x, int y);
    
    void Expand(int dr);
    void Shrink(int dr);
};