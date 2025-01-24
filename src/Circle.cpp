#include <../include/Circle.hpp>


Circle::Circle(int x, int y, int r){
    coords = Vec2int(x, y);
    this->r = r;
}

void Circle::Display(SDL_Renderer *renderer){
    const int diameter = (r * 2);
    int xCurr = r - 1;
    int yCurr = 0;
    int tX = 1;
    int tY = 1;
    int error = tX - diameter;

    while(xCurr >= yCurr){

        //render 8 octants of the circle
        SDL_RenderDrawPoint(renderer, coords.x + xCurr, coords.y - yCurr);
        SDL_RenderDrawPoint(renderer, coords.x + xCurr, coords.y + yCurr);
        SDL_RenderDrawPoint(renderer, coords.x - xCurr, coords.y - yCurr);
        SDL_RenderDrawPoint(renderer, coords.x - xCurr, coords.y + yCurr);
        SDL_RenderDrawPoint(renderer, coords.x + yCurr, coords.y - xCurr);
        SDL_RenderDrawPoint(renderer, coords.x + yCurr, coords.y + xCurr);
        SDL_RenderDrawPoint(renderer, coords.x - yCurr, coords.y - xCurr);
        SDL_RenderDrawPoint(renderer, coords.x - yCurr, coords.y + xCurr);
        
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
    const int diameter = (r * 2);
    int xCurr = r - 1;
    int yCurr = 0;
    int tX = 1;
    int tY = 1;
    int error = tX - diameter;

    while(xCurr >= yCurr){
        //render 8 octants of the circle
        SDL_RenderDrawLine(renderer, coords.x + xCurr, coords.y - yCurr, coords.x + xCurr, coords.y + yCurr);
        SDL_RenderDrawLine(renderer, coords.x - xCurr, coords.y - yCurr, coords.x - xCurr, coords.y + yCurr);
        SDL_RenderDrawLine(renderer, coords.x + yCurr, coords.y - xCurr, coords.x + yCurr, coords.y + xCurr);
        SDL_RenderDrawLine(renderer, coords.x - yCurr, coords.y - xCurr, coords.x - yCurr, coords.y + xCurr);
        
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
    coords.x = x;
    coords.y = y;
}

void Circle::MovePos(int dx, int dy){
    coords.x += dx;
    coords.y += dy;
}

void Circle::Shrink(int dr){
    r ? r -= dr : r;
}

void Circle::Expand(int dr){
    r ? r += dr : r += dr;
}