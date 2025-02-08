#include <../include/Rectangle.hpp>


Rectangle::Rectangle(int x, int y, int w, int h)
: _size(w, h){
	this->_coords = Vec2int(x, y);
}

void Rectangle::Display(SDL_Renderer *renderer){
	SDL_Rect rect = {_coords._x, _coords._y, _size._x, _size._y};
	SDL_RenderDrawRect(renderer, &rect);
}
void Rectangle::DisplayFilled(SDL_Renderer *renderer){
	SDL_Rect rect = {_coords._x, _coords._y, _size._x, _size._y};
	SDL_RenderFillRect(renderer, &rect);
}

void Rectangle::SetPos(int x, int y){
	ICoordable::SetPos(x, y);
	Center();
}
void Rectangle::MovePos(int dx, int dy){
	ICoordable::MovePos(dx, dy);
	Center();
}

void Rectangle::SetSize(int w, int h){
	_size._x = w;
	_size._y = h;
	Center();
}
void Rectangle::ChangeSize(int dw, int dh){
	_size._x += dw;
	_size._y += dh;
	Center();
}

void Rectangle::Center(){
	_coords._x -= _size._x / 2;
	_coords._y -= _size._y / 2;
}