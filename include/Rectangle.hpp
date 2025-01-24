#pragma once
#include <SDL2/SDL.h>
#include <Vec2int.hpp>
#include <IDisplayable.hpp>
#include <ICoordable.hpp>


class Rectangle : public IDisplayable, public ICoordable{
    private:
    Vec2int size;
    
    public:
    Rectangle(int x, int y, int w, int h);

    virtual void Display(SDL_Renderer *renderer) override;
    virtual void DisplayFilled(SDL_Renderer *renderer) override;

    virtual void SetPos(int x, int y) override;
    virtual void MovePos(int dx, int dy) override;

    void Center();
};