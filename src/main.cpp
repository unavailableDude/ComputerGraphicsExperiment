/*
	WARNING! the code you see here may cause you to have eye cancer
	!YOU HAVE BEEN WARNED!
*/

#include <iostream>
#include <stdint.h>
#include <vector>


#include <../include/SDL2/SDL.h>
#include <../include/Circle.hpp>
#include <../include/Rectangle.hpp>
#include <../include/ICoordable.hpp>
#include <../include/IShape.hpp>


const int SCREEN_SCALE = 1;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;
uint16_t DESIRED_FPS = 60;  //try UINT16_MAX, it's fun
//const uint16_t MAX_FPS = 240;
//const uint16_t MIN_FPS = 1;
//#define PROFILING


void DrawLine(SDL_Renderer *r, int x1, int y1, int x2, int y2){
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
void SlopeLine(SDL_Renderer *r, int x1, int y1, int x2, int y2){
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
void DDALineStep(SDL_Renderer *r, int x1, int y1, int x2, int y2, int Exch){
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
void DDALine(SDL_Renderer *r, int x1, int y1, int x2, int y2){
    int dx = x2 - x1;
    int dy = y2 - y1;
    if (abs(dx) >= abs(dy))
        DDALineStep(r, x1, y1, x2, y2, 0);
    else
        DDALineStep(r, y1, x1, y2, x2, 1);
}
void BresenhamLine(SDL_Renderer *r, int x1,int y1,int x2,int y2){
    int Dx,Dy,Yinc,Xinc,Pinc,Ninc,P;
    int Exch ,temp;

    Dx = x2 - x1 ;
    Dy = y2 - y1 ;

    if ( Dx > 0)
        Xinc = 1 ;
    else{
        Xinc = -1;
        Dx = abs(Dx) ;
    }

    if ( Dy > 0)
        Yinc = 1 ;
    else{
        Yinc = -1;
        Dy =abs(Dy) ;
    }

    if ( Dx > Dy )
        Exch = 0 ;
    else{
        Exch = 1 ;
        temp = Dx ;
        Dx = Dy ;
        Dy = temp ;
    }

    P = 2*Dy - Dx ;
    Pinc = 2*(Dy-Dx);
    Ninc = 2*Dy;

    while(Dx > 0){
        SDL_RenderDrawPoint(r, x1, y1);
        if( P > 0 ){
            x1 = x1 + Xinc;
            y1 = y1 + Yinc;
            P = P + Pinc;
        }
        else{
            P = P + Ninc;
            if (Exch)
                y1 = y1 + Yinc;
            else
                x1 = x1 + Xinc;
        }
        Dx = Dx - 1 ;
    }
}

void LineRect(SDL_Renderer *r, int cx, int cy, int width, int height){
    int sx = cx - (width / 2);
    int sy = cy - (height / 2);
    int ex = cx + (width / 2);
    int ey = cy + (height / 2);

    BresenhamLine(r, sx, sy, ex, sy);
    BresenhamLine(r, ex, sy, ex, ey);
    BresenhamLine(r, ex, ey, sx, ey);
    BresenhamLine(r, sx, ey, sx, sy);
}
void LineRectFilled(SDL_Renderer *r, int cx, int cy, int width, int height){
    for(int i = width; i > 0; i--){
        LineRect(r, cx, cy, i, height);
    }
}


void DisplayShapes(IDisplayable* shape, SDL_Renderer* renderer){
	shape->Display(renderer);
}

void ProfilerOnWindowTitle(SDL_Window *wind, float totalFrameTimeMiliS, float frameTimeDeltaMiliS, float delayTimeMiliS){
	SDL_SetWindowTitle(wind, ("totalFrameTimeMiliS: "    + std::to_string(totalFrameTimeMiliS) + 
							  "\t frameTimeDeltaMiliS: " + std::to_string(frameTimeDeltaMiliS) + 
							  "\t delayTimeMiliS: "      + std::to_string(delayTimeMiliS)).c_str());
}

void DrawHW1(SDL_Renderer *r, int x, int y){
	SDL_Rect rect1 = {x, y, 100, 50};
	SDL_Rect rect2 = {x + 200, y, 100, 50};
	SDL_RenderDrawRect(r, &rect1);
	SDL_RenderDrawRect(r, &rect2);
	SDL_RenderDrawLine(r, x + 100, y + 25, x + 200, y + 25);
}

int main(int argc, char* argv[]){
	if(SDL_Init(SDL_INIT_EVERYTHING)){
		std::cout << "SDL_INIT failed with error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	SDL_Window *window1 = nullptr;
	SDL_Renderer *renderer1 = nullptr;
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH * SCREEN_SCALE, SCREEN_HEIGHT * SCREEN_SCALE, SDL_WINDOW_RESIZABLE, &window1, &renderer1);
	if(window1 == nullptr){
		std::cout << "SDL_CreateWindow failed with error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}
	if(renderer1 == nullptr){
		std::cout << "SDL_CreateRenderer failed with error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}
	
	SDL_RenderSetScale(renderer1, SCREEN_SCALE, SCREEN_SCALE);
	SDL_SetRenderDrawColor(renderer1, 0, 0, 0, 255);
	SDL_RenderClear(renderer1);
	SDL_SetRenderDrawColor(renderer1, 255, 255, 255, 255);
	SDL_SetWindowTitle(window1, "Window 1");
	
	SDL_Event event;
	bool running = true;

	Vec2int currShapeSize(64, 64);
	std::vector<Circle> strokes;
	int mouseX = SCREEN_WIDTH / 2, mouseY = SCREEN_HEIGHT / 2;
	while(running){
		Uint64 frameStartTimePC = SDL_GetPerformanceCounter();
		//read keyboard input
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = false;
				SDL_DestroyWindow(window1);
				SDL_DestroyRenderer(renderer1);
			}
			else if(event.type == SDL_KEYDOWN){
				switch(event.key.keysym.sym){
					case SDLK_w:
						break;
					case SDLK_s:
						break;  
					case SDLK_a:
						break;
					case SDLK_d:
						break;
				}
			}
			//get clicket points
			if(event.button.type == SDL_MOUSEBUTTONDOWN){
				if(event.button.button == SDL_BUTTON_LEFT){
					Circle circle{mouseX, mouseY, 20};
					strokes.push_back(circle);
				}
			}
		}

		//read mouse input
		SDL_GetMouseState(&mouseX, &mouseY);
		mouseX /= SCREEN_SCALE;
		mouseY /= SCREEN_SCALE;

		//refresh screen
		SDL_SetRenderDrawColor(renderer1, 0, 0, 0, 255);
		SDL_RenderClear(renderer1);
		SDL_SetRenderDrawColor(renderer1, 255, 255, 255, 255);

		DrawHW1(renderer1, 50, 50);
		LineRect(renderer1, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 100, 100);

		SDL_RenderPresent(renderer1);

		#ifdef PROFILING
		//calculate frame time to render, and delay by <desired frame time> - <current frame time> to make up the rest of the delay,
		//then calculate total frame (delay + render) time and print it
		uint64_t frameEndTimePC = SDL_GetPerformanceCounter();
		float frameTimeDeltaMiliS = (frameEndTimePC - frameStartTimePC) / (float)SDL_GetPerformanceFrequency();
		float delayTimeMiliS = (1000 / DESIRED_FPS) - frameTimeDeltaMiliS;
		SDL_Delay(delayTimeMiliS);
		//maybe causing a bit more delay (~2 msec)
		float totalFrameTimeMiliS = ((SDL_GetPerformanceCounter() - frameStartTimePC) / (float)SDL_GetPerformanceFrequency()) * 1000;
		ProfilerOnWindowTitle(window1, totalFrameTimeMiliS, frameTimeDeltaMiliS, delayTimeMiliS);
		
		std::cout << "frameStartTimePC: " << frameStartTimePC
				  << "\tframeEndTimePC: " << frameEndTimePC 
				  << "\tframeTimeDeltaMiliS: " << frameTimeDeltaMiliS 
				  << "\tdelayTimeMILIS: " << delayTimeMiliS 
				  << "\ttotalFrameTimeMiliS: " << totalFrameTimeMiliS
				  << std::endl;
		#endif
	}

	SDL_Quit();
	return 0;
}