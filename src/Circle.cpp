#include <../include/Circle.hpp>


Circle::Circle(int x, int y, int r) : pos(x, y), r(r) {}

void Circle::Display(SDL_Renderer *renderer){
    const int diameter = (r * 2);
    int xCurr = r - 1;
    int yCurr = 0;
    int tX = 1;
    int tY = 1;
    int error = tX - diameter;

    while(xCurr >= yCurr){
        //render 8 octants of the circle
        SDL_RenderDrawPoint(renderer, pos.x + xCurr, pos.y - yCurr);
        SDL_RenderDrawPoint(renderer, pos.x + xCurr, pos.y + yCurr);
        SDL_RenderDrawPoint(renderer, pos.x - xCurr, pos.y - yCurr);
        SDL_RenderDrawPoint(renderer, pos.x - xCurr, pos.y + yCurr);
        SDL_RenderDrawPoint(renderer, pos.x + yCurr, pos.y - xCurr);
        SDL_RenderDrawPoint(renderer, pos.x + yCurr, pos.y + xCurr);
        SDL_RenderDrawPoint(renderer, pos.x - yCurr, pos.y - xCurr);
        SDL_RenderDrawPoint(renderer, pos.x - yCurr, pos.y + xCurr);
        
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
        SDL_RenderDrawLine(renderer, pos.x + xCurr, pos.y - yCurr, pos.x + xCurr, pos.y + yCurr);
        SDL_RenderDrawLine(renderer, pos.x - xCurr, pos.y - yCurr, pos.x - xCurr, pos.y + yCurr);
        SDL_RenderDrawLine(renderer, pos.x + yCurr, pos.y - xCurr, pos.x + yCurr, pos.y + xCurr);
        SDL_RenderDrawLine(renderer, pos.x - yCurr, pos.y - xCurr, pos.x - yCurr, pos.y + xCurr);
        
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

void Circle::Move(int dx, int dy){
    pos.x += dx;
    pos.y += dy;
}
void Circle::SetPos(int x, int y){
    pos.x = x;
    pos.y = y;
}

void Circle::Shrink(int dr){
    r ? r -= dr : r;
}

void Circle::Expand(int dr){
    r ? r += dr : r += dr;
}