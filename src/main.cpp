/*
    WARNING! the code you see here may cause you to have eye cancer
    !YOU HAVE BEEN WARNED!
*/


#include <iostream>
#include <stdint.h>

#include <../include/SDL2/SDL.h>
#include <../include/Circle.hpp>
#include <../include/Rectangle.hpp>
#include <../include/ICoordable.hpp>


const int SCREEN_SCALE = 1;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;
uint16_t DESIRED_FPS = 60;  //try UINT16_MAX, it's fun
//const uint16_t MAX_FPS = 240;
//const uint16_t MIN_FPS = 1;
//#define PROFILING

void DisplayShapes(IDisplayable* shape, SDL_Renderer* renderer){
    shape->Display(renderer);
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

    Circle circle1(128, 128, 32);
    Rectangle rect(128, 128, 64, 64);
    ICoordable *currShape = &circle1;
    int mouseX = 0, mouseY = 0;
    while(running){
        Uint64 frameStartTimePC = SDL_GetPerformanceCounter();
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
                        currShape = &circle1;
                        break;
                    case SDLK_d:
                        currShape = &rect;
                        break;
                }
            }
        }

        //read mouse input
        SDL_GetMouseState(&mouseX, &mouseY);
        mouseX /= SCREEN_SCALE;
        mouseY /= SCREEN_SCALE;
        currShape->SetPos(mouseX, mouseY);

        //refresh screen
        SDL_SetRenderDrawColor(renderer1, 0, 0, 0, 255);
        SDL_RenderClear(renderer1);
        SDL_SetRenderDrawColor(renderer1, 255, 255, 255, 255);

        DisplayShapes(dynamic_cast<IDisplayable*>(currShape), renderer1);

        SDL_RenderPresent(renderer1);

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