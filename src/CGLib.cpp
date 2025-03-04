#include <../include/CGLib.hpp>
#include <vector>


void CGLib::DrawLine(SDL_Renderer *r, int x1, int y1, int x2, int y2){
    int xi;
    float dy = y2 - y1;
    float dx = x2 - x1;
    float m = dy / dx;
    float b = y1 - (m * x1);

    if(abs(dx) > abs(dy)){
        xi = (dx > 0 ? 1 : -1);
        for(int x = x1; dx != 0; x += xi){
            dx -= xi;
            float y = round(b + (m * x));
            SDL_RenderDrawPoint(r, x, y);
        }
    }
    else{
        std::swap(x1, y1);
        std::swap(x2, y2);

        dy = y2 - y1;
        dx = x2 - x1;
        m = dy / dx;
        b = y1 - (m * x1);
        xi = (dx > 0 ? 1 : -1);

        for(int x = x1; dx != 0; x += xi){
            dx -= xi;
            float y = round(b + (m * x));
            SDL_RenderDrawPoint(r, x, y);
        }
    }
}
void CGLib::SlopeLine(SDL_Renderer *r, int x1, int y1, int x2, int y2){
    int xi;
    float dx = x2 - x1;
    float dy = y2 - y1;
    double m = dy / dx;
    float y = y1;
    if (abs(dx) > abs(dy)){
        if (dx > 0) xi = 1;
        else xi = -1;
        for (int x = x1; dx != 0; x += xi){
            dx -= xi;

            SDL_RenderDrawPoint(r, x, round(y));
            y = y + m;
        }
    }
    else{
        std::swap(x1, y1);
        std::swap(x2, y2);
        dx = x2 - x1;
        dy = y2 - y1;
        double m = dy / dx;
        float y = y1;

        if (dx > 0) xi = 1;
        else xi = -1;
        for (int x = x1; dx != 0; x += xi){
            dx -= xi;
            SDL_RenderDrawPoint(r, round(y), x);
            y = y + m;
        }
    }
}
void CGLib::DDALineStep(SDL_Renderer *r, int x1, int y1, int x2, int y2, int Exch){
    float E = 0;
    int Xinc, Yinc;
    int dx = x2 - x1;
    int dy = y2 - y1;
    float m = fabs(double(dy) / dx);
    float m1 = -(1 - m);
    if (dx < 0)
    {
        dx *= -1;
        Xinc = -1;
    }
    else
        Xinc = 1;
    if (dy < 0)
        Yinc = -1;
    else
        Yinc = 1;
    for (; dx >= 0; --dx)
    {
        if (Exch == 0)
            SDL_RenderDrawPoint(r, x1, y1);
        else
            SDL_RenderDrawPoint(r, y1, x1);
        if (E >= 0)
        {
            y1 = y1 + Yinc;
            E = E + m1;
        }
        else
            E = E + m;
        x1 = x1 + Xinc;
    }
}
void CGLib::DDALine(SDL_Renderer *r, int x1, int y1, int x2, int y2){
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (abs(dx) >= abs(dy))
        DDALineStep(r, x1, y1, x2, y2, 0);
    else
        DDALineStep(r, y1, x1, y2, x2, 1);
}
void CGLib::BresenhamLine(SDL_Renderer *r, int x1,int y1,int x2,int y2){
    int Dx, Dy, Xinc, Yinc, P, Pinc, Ninc;
    int Exch, temp;

    Dx = x2 - x1 ;
    Dy = y2 - y1 ;

    if(Dx > 0)
        Xinc = 1;
    else{
        Xinc = -1;
        Dx = abs(Dx);
    }

    if(Dy > 0)
        Yinc = 1;
    else{
        Yinc = -1;
        Dy = abs(Dy);
    }

    if(Dx > Dy)
        Exch = 0;
    else{
        Exch = 1;
        temp = Dx;
        Dx = Dy;
        Dy = temp;
    }

    P = 2 * Dy - Dx;
    Pinc = 2 * (Dy - Dx);
    Ninc = 2 * Dy;

    while(Dx > 0){
        SDL_RenderDrawPoint(r, x1, y1);
        if(P > 0){
            x1 += Xinc;
            y1 += Yinc;
            P += Pinc;
        }
        else{
            P = P + Ninc;
            if(Exch)
                y1 += Yinc;
            else
                x1 += Xinc;
        }
        Dx -= 1;
    }
}

void CGLib::LineRect(SDL_Renderer *r, int cx, int cy, int width, int height){
    int sx = cx - (width / 2);
    int sy = cy - (height / 2);
    int ex = cx + (width / 2);
    int ey = cy + (height / 2);

    BresenhamLine(r, sx, sy, ex, sy);
    BresenhamLine(r, ex, sy, ex, ey);
    BresenhamLine(r, ex, ey, sx, ey);
    BresenhamLine(r, sx, ey, sx, sy);
}
void CGLib::LineRectFilled(SDL_Renderer *r, int cx, int cy, int width, int height){
    for(int i = width; i > 0; i--){
        LineRect(r, cx, cy, i, height);
    }
}

void CGLib::CircleEqu(SDL_Renderer *r, int xc, int yc, int radius){
    int x = 0, y = 0;
    for (x = 0; x <= y; x++){
        y = sqrt(radius * radius - (x * x));
		
        SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
		SDL_RenderDrawPoint(r, xc + x, yc + y);
        SDL_SetRenderDrawColor(r, 255, 255, 0, 255);
		SDL_RenderDrawPoint(r, xc + x, yc - y);
        SDL_SetRenderDrawColor(r, 255, 255, 0, 255);
		SDL_RenderDrawPoint(r, xc - x, yc + y);
        SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
		SDL_RenderDrawPoint(r, xc - x, yc - y);
        SDL_SetRenderDrawColor(r, 0, 255, 255, 255);
		SDL_RenderDrawPoint(r, xc + y, yc + x);
        SDL_SetRenderDrawColor(r, 0, 0, 255, 255);
		SDL_RenderDrawPoint(r, xc + y, yc - x);
        SDL_SetRenderDrawColor(r, 255, 0, 255, 255);
		SDL_RenderDrawPoint(r, xc - y, yc + x);
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
		SDL_RenderDrawPoint(r, xc - y, yc - x);
    }
}
void CGLib::CircleFilled(SDL_Renderer *r, int xc, int yc, int radius){	
}

void CGLib::CircleLines(SDL_Renderer *r, int xc, int yc, int radius){
	int x = 0, y = 0;
    for (x = 0; x <= y; x++){
        y = sqrt(radius * radius - (x * x));
		
		//SDL_RenderDrawPoint(r, xc + x, yc + y);
		//SDL_RenderDrawPoint(r, xc + x, yc - y);
		//SDL_RenderDrawPoint(r, xc - x, yc + y);
		//SDL_RenderDrawPoint(r, xc - x, yc - y);
		//SDL_RenderDrawPoint(r, xc + y, yc + x);
		//SDL_RenderDrawPoint(r, xc + y, yc - x);
		//SDL_RenderDrawPoint(r, xc - y, yc + x);
		//SDL_RenderDrawPoint(r, xc - y, yc - x);

		SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
		SDL_RenderDrawLine(r, xc, yc, xc + x, yc + y);
		SDL_SetRenderDrawColor(r, 255, 255, 0, 255);
		SDL_RenderDrawLine(r, xc, yc, xc + x, yc - y);
		SDL_SetRenderDrawColor(r, 255, 0, 255, 255);
		SDL_RenderDrawLine(r, xc, yc, xc - x, yc + y);
		SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
		SDL_RenderDrawLine(r, xc, yc, xc - x, yc - y);
		SDL_SetRenderDrawColor(r, 0, 255, 255, 255);
		SDL_RenderDrawLine(r, xc, yc, xc + y, yc + x);
		SDL_SetRenderDrawColor(r, 0, 255, 0, 255);
		SDL_RenderDrawLine(r, xc, yc, xc + y, yc - x);
		SDL_SetRenderDrawColor(r, 0, 0, 255, 255);
		SDL_RenderDrawLine(r, xc, yc, xc - y, yc + x);
		SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
		SDL_RenderDrawLine(r, xc, yc, xc - y, yc - x);

    }
}

void CGLib::CirclePolar(SDL_Renderer *r, int xc, int yc, int radius){
    int x = 0, y = 0;
    for (int i = 0; i <= 360; i++){
        float theta = i * M_PI / 180 - 0.4;
        x = radius * cos(theta);
        y = radius * sin(theta);
        SDL_RenderDrawPoint(r, x + xc, y + yc);

		theta = i * M_PI / 180 + 0.4;
        x = radius * cos(theta);
        y = radius * sin(theta);
        SDL_RenderDrawPoint(r, x + xc, y + yc);
    }
}