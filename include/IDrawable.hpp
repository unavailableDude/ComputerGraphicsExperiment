#pragma once
#include "SDL2/SDL.h"


class IDrawable{
	public:
	virtual ~IDrawable(){}
	virtual void Display(SDL_Renderer *renderer) = 0;
	virtual void DisplayFilled(SDL_Renderer *renderer) = 0;
};