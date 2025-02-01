/*
	WARNING! the code you see here may cause you to have eye cancer
	!YOU HAVE BEEN WARNED!
*/
//#define PROFILING

//c++
#include <iostream>
#include <stdint.h>
#include <vector>

//first party
#include <../include/SDL2/SDL.h>
#include <../include/Circle.hpp>
#include <../include/Rectangle.hpp>
#include <../include/ICoordable.hpp>
#include <../include/IShape.hpp>

//third party
#include <../glad/include/glad/glad.h>


const int SCREEN_SCALE = 1;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;
uint16_t DESIRED_FPS = 60;  //try UINT16_MAX, it's fun

void DisplayShapes(IDisplayable* shape, SDL_Renderer* renderer){
	shape->Display(renderer);
}

void SetupOpenGL(){
	//specify opengl version 4.1 profile compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	//enable double buffering
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);		//set depth buffer size to 24 bits
}

void ProfilerOnWindowTitle(SDL_Window *wind, float totalFrameTimeMiliS, float frameTimeDeltaMiliS, float delayTimeMiliS){
	SDL_SetWindowTitle(wind, ("totalFrameTimeMiliS: "    + std::to_string(totalFrameTimeMiliS) + 
							  "\t frameTimeDeltaMiliS: " + std::to_string(frameTimeDeltaMiliS) + 
							  "\t delayTimeMiliS: "      + std::to_string(delayTimeMiliS)).c_str());
}


int main(int argc, char* argv[]){
	if(SDL_Init(SDL_INIT_EVERYTHING)){
		std::cout << "SDL_INIT failed with error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	//setup opengl
	SetupOpenGL();

	SDL_Window *window1 = SDL_CreateWindow("Window 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_Renderer *renderer1 = SDL_CreateRenderer(window1, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_GLContext glContext = SDL_GL_CreateContext(window1);	//create opengl context object
	gladLoadGLLoader(SDL_GL_GetProcAddress);					//load opengl functions, segfault if ignored!
	if(window1 == nullptr){
		std::cout << "SDL_CreateWindow failed with error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}
	if(renderer1 == nullptr){
		std::cout << "SDL_CreateRenderer failed with error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}
	if(glContext == nullptr){
		std::cout << "SDL_GL_CreateContext failed with error: " << SDL_GetError() << std::endl;
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
	Circle circle1(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, currShapeSize.x / 2);				//r = w / 2 because r represents the radius, not the diameter
	Rectangle rect(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, currShapeSize.x, currShapeSize.y);
	IShape *currShape = &circle1;
	std::vector<Vec2int> clickPoints;

	int mouseX = SCREEN_WIDTH / 2, mouseY = SCREEN_HEIGHT / 2;
	while(running){
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
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
						currShapeSize.x += 1;
						currShapeSize.y += 1;
						currShape->SetSize(currShapeSize.x, currShapeSize.y);
						break;
					case SDLK_s:
						currShapeSize.x -= 1;
						currShapeSize.y -= 1;
						currShape->SetSize(currShapeSize.x, currShapeSize.y);
						break;  
					case SDLK_a:
						currShape = &circle1;
						currShape->SetSize(currShapeSize.x, currShapeSize.y);
						break;
					case SDLK_d:
						currShape = &rect;
						currShape->SetSize(currShapeSize.x, currShapeSize.y);
						break;
				}
			}
			//get clicket points
			if(event.button.type == SDL_MOUSEBUTTONDOWN){
				if(event.button.button == SDL_BUTTON_LEFT){
					clickPoints.push_back(Vec2int(mouseX, mouseY));
				}
			}
		}

		//read mouse input
		SDL_GetMouseState(&mouseX, &mouseY);
		mouseX /= SCREEN_SCALE;
		mouseY /= SCREEN_SCALE;
		currShape->SetPos(mouseX, mouseY);

		//refresh screen
		//clear color and depth buffer gl
		glClearColor(1, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SDL_GL_SwapWindow(window1);

		//calculate frame time to render, and delay by <desired frame time> - <current frame time> to make up the rest of the delay,
		//then calculate total frame (delay + render) time and print it
		uint64_t frameEndTimePC = SDL_GetPerformanceCounter();
		float frameTimeDeltaMiliS = (frameEndTimePC - frameStartTimePC) / (float)SDL_GetPerformanceFrequency();
		float delayTimeMiliS = (1000 / DESIRED_FPS) - frameTimeDeltaMiliS;
		SDL_Delay(delayTimeMiliS);
		//maybe causing a bit more delay (~2 msec)
		float totalFrameTimeMiliS = ((SDL_GetPerformanceCounter() - frameStartTimePC) / (float)SDL_GetPerformanceFrequency()) * 1000;
		ProfilerOnWindowTitle(window1, totalFrameTimeMiliS, frameTimeDeltaMiliS, delayTimeMiliS);
		
		#ifdef PROFILING
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