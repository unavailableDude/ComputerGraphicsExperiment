#include "../include/Rectangle.hpp"


Rectangle::Rectangle(int x, int y, int w, int h): pos(x, y), size(w, h){}

//display rect
void Rectangle::Display(SDL_Renderer *renderer){
    SDL_Rect rect = {pos.x, pos.y, size.x, size.y};
    SDL_RenderDrawRect(renderer, &rect);
}

void Rectangle::DisplayFilled(SDL_Renderer *renderer){
    SDL_Rect rect = {pos.x, pos.y, size.x, size.y};
    SDL_RenderFillRect(renderer, &rect);
}

//move rect
void Rectangle::Move(int dx, int dy){
    pos.x += dx;
    pos.y += dy;
}
void Rectangle::SetPos(int x, int y){
    this->pos.x = x;
    this->pos.y = y;
}

//shift it up left by half it's size
void Rectangle::Center(){
    pos.x -= size.x / 2;
    pos.y -= size.y / 2;
}