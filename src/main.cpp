/*
	WARNING! the code you see here may cause you to have eye cancer
	!YOU HAVE BEEN WARNED!
*/

#include <iostream>
#include <stdint.h>
#include <vector>


#include <../include/SDL2/SDL.h>
#include <../include/CGLib.hpp>
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
#define PROFILING
// #define CMD_PROF


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
	int mouseX = SCREEN_WIDTH / 2, mouseY = SCREEN_HEIGHT / 2;
	int keyboardX = SCREEN_WIDTH / 2, keyboardY = SCREEN_HEIGHT / 2;
	unsigned int moveMagnitude = 8;
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
						keyboardY -= moveMagnitude;
						break;
					case SDLK_s:
						keyboardY += moveMagnitude;
						break;  
					case SDLK_a:
						keyboardX -= moveMagnitude;
						break;
					case SDLK_d:
						keyboardX += moveMagnitude;
						break;
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
        
        //for(int i = 0; i < 80; i++){
        //    CGLib::CircleEqu(renderer1, keyboardX, keyboardY, i);
        //}
		CGLib::CircleLines(renderer1, keyboardX, keyboardY, 240);
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
		
		#ifdef CMD_PROF
		std::cout << "frameStartTimePC: " << frameStartTimePC
				  << "\tframeEndTimePC: " << frameEndTimePC 
				  << "\tframeTimeDeltaMiliS: " << frameTimeDeltaMiliS 
				  << "\tdelayTimeMILIS: " << delayTimeMiliS 
				  << "\ttotalFrameTimeMiliS: " << totalFrameTimeMiliS
				  << std::endl;
		#endif
		#endif
	}

	SDL_Quit();
	return 0;
}