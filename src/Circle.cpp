#include <../include/Circle.hpp>


Circle::Circle(int x, int y, int r){
	_coords = Vec2int(x, y);
	this->_r = r;
}

void Circle::Display(SDL_Renderer *renderer){
	const int diameter = (_r * 2);
	int xCurr = _r - 1;
	int yCurr = 0;
	int tX = 1;
	int tY = 1;
	int error = tX - diameter;

	while(xCurr >= yCurr){

		//render 8 octants of the circle
		SDL_RenderDrawPoint(renderer, _coords._x + xCurr, _coords._y - yCurr);
		SDL_RenderDrawPoint(renderer, _coords._x + xCurr, _coords._y + yCurr);
		SDL_RenderDrawPoint(renderer, _coords._x - xCurr, _coords._y - yCurr);
		SDL_RenderDrawPoint(renderer, _coords._x - xCurr, _coords._y + yCurr);
		SDL_RenderDrawPoint(renderer, _coords._x + yCurr, _coords._y - xCurr);
		SDL_RenderDrawPoint(renderer, _coords._x + yCurr, _coords._y + xCurr);
		SDL_RenderDrawPoint(renderer, _coords._x - yCurr, _coords._y - xCurr);
		SDL_RenderDrawPoint(renderer, _coords._x - yCurr, _coords._y + xCurr);
		
		//update coords
		if(error <= 0){
			++yCurr;
			error += tY;
			tY += 2;
		}

		if(error > 0){
			--xCurr;
			tX += 2;
			error += (tX - diameter);
		}
	}
}
void Circle::DisplayFilled(SDL_Renderer *renderer){
	const int diameter = (_r * 2);
	int xCurr = _r - 1;
	int yCurr = 0;
	int tX = 1;
	int tY = 1;
	int error = tX - diameter;

	while(xCurr >= yCurr){
		//render 8 octants of the circle
		SDL_RenderDrawLine(renderer, _coords._x + xCurr, _coords._y - yCurr, _coords._x + xCurr, _coords._y + yCurr);
		SDL_RenderDrawLine(renderer, _coords._x - xCurr, _coords._y - yCurr, _coords._x - xCurr, _coords._y + yCurr);
		SDL_RenderDrawLine(renderer, _coords._x + yCurr, _coords._y - xCurr, _coords._x + yCurr, _coords._y + xCurr);
		SDL_RenderDrawLine(renderer, _coords._x - yCurr, _coords._y - xCurr, _coords._x - yCurr, _coords._y + xCurr);
		
		//update coords
		if(error <= 0){
			++yCurr;
			error += tY;
			tY += 2;
		}

		if(error > 0){
			--xCurr;
			tX += 2;
			error += (tX - diameter);
		}
	}
}

void Circle::SetPos(int x, int y){
	ICoordable::SetPos(x, y);
}
void Circle::MovePos(int dx, int dy){
	ICoordable::MovePos(dx, dy);
}

void Circle::SetSize(int w, int h){
	_r = w / 2;
}

void Circle::ChangeSize(int dw, int dh){
	_r += dw;
}