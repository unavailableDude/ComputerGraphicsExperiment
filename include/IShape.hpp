#pragma once
#include "ICoordable.hpp"
#include "IDisplayable.hpp"


class IShape : public ICoordable, public IDisplayable{
	public:
	virtual ~IShape(){}

	//from ICoordable
	virtual void SetPos(int x, int y) = 0;
	virtual void MovePos(int dx, int dy) = 0;
	
	//from IDisplayable
	virtual void Display(SDL_Renderer *renderer) = 0;
	virtual void DisplayFilled(SDL_Renderer *renderer) = 0;

	virtual void SetSize(int w, int h) = 0;
	virtual void ChangeSize(int dw, int dh) = 0;
};