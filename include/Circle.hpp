#pragma once
#include <SDL2/SDL.h>
#include <Vec2int.hpp>
#include <IDisplayable.hpp>
#include <ICoordable.hpp>


class Circle : public IDisplayable, public ICoordable{
    private:
    float r;
        
    public:
    Circle(int x, int y, int r);

    virtual void Display(SDL_Renderer *renderer) override;
    virtual void DisplayFilled(SDL_Renderer *renderer) override;

    virtual void SetPos(int x, int y) override;
    virtual void MovePos(int dx, int dy) override;
    
    void Expand(int dr);
    void Shrink(int dr);
};