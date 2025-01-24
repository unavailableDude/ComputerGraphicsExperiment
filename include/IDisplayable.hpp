#pragma once
#include <SDL2/SDL.h>


class IDisplayable{
	public:
	virtual ~IDisplayable(){}
	
	virtual void Display(SDL_Renderer *renderer) = 0;
	virtual void DisplayFilled(SDL_Renderer *renderer) = 0;
};