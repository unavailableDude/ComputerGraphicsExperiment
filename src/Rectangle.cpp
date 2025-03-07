#include <../include/Rectangle.hpp>


Rectangle::Rectangle(int x, int y, int w, int h)
: size(w, h){
	this->coords = Vec2int(x, y);
}

void Rectangle::Display(SDL_Renderer *renderer){
	SDL_Rect rect = {coords.x, coords.y, size.x, size.y};
	SDL_RenderDrawRect(renderer, &rect);
}
void Rectangle::DisplayFilled(SDL_Renderer *renderer){
	SDL_Rect rect = {coords.x, coords.y, size.x, size.y};
	SDL_RenderFillRect(renderer, &rect);
}

void Rectangle::SetPos(int x, int y){
	ICoordable::SetPos(x, y);
	Center();
}

void Rectangle::MovePos(int dx, int dy){
	ICoordable::MovePos(dx, dy);
}

void Rectangle::SetSize(int w, int h){
	size.x = w;
	size.y = h;
	Center();
}

void Rectangle::ChangeSize(int dw, int dh){
	size.x += dw;
	size.y += dh;
	Center();
}

void Rectangle::Center(){
	coords.x -= size.x / 2;
	coords.y -= size.y / 2;
}