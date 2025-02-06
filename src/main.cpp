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
#include <../include/ShaderLoader.hpp>
#include <../include/ShaderProgram.hpp>

//third party
#include <../glad/include/glad/glad.h>

#include <../include/imgui/imgui.h>
#include <../include/imgui/imgui_impl_sdl2.h>
#include <../include/imgui/imgui_impl_sdlrenderer2.h>


const int SCREEN_SCALE = 1;
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 480;
uint16_t DESIRED_FPS = 60;  //try UINT16_MAX, it's fun


void OGLGetInfo(){
	std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}
void OGLSetup(){
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){					//load opengl functions, segfault if ignored!
		std::cout << "Failed to initialize GLAD" << std::endl;
	}
	OGLGetInfo();
}

void Cleanup(){
	SDL_Quit();
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}


//shader filenames const
const std::string vertShader1Path = "shaders/vertShader1.vert";
const std::string fragShader1Path = "shaders/fragShader1.frag";
const std::string psyShaderFragPath = "shaders/psyShader.frag";
const std::string theGreatSunFragPath = "shaders/theGreatSun.frag";


void PreDraw(ShaderProgram program, float shaderTime, Vec2int u_resolution){
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program.Use();
	program.SetUniformTime(shaderTime);
	program.SetUniformResolution(u_resolution);
}

void Draw(GLuint vertexArrayObject, const GLuint vertexBufferObject){
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

	// glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

float deltaTime = 0;
float frameTimeDeltaMiliS = 0;
float framerate = 0;
int main(int argc, char* argv[]){
	if(SDL_Init(SDL_INIT_EVERYTHING)){
		std::cout << "SDL_INIT failed with error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	SDL_Window *window1 = SDL_CreateWindow("Window 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_Window *profileWindow = SDL_CreateWindow("Profiler", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	SDL_Renderer *profileRenderer = SDL_CreateRenderer(profileWindow, 1, 0);
	SDL_GLContext glContext = SDL_GL_CreateContext(window1);	//create opengl context object
	if(window1 == nullptr){
		std::cout << "SDL_CreateWindow failed with error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}
	if(profileWindow == nullptr){
		std::cout << "SDL_CreateWindow failed with error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}
	if(glContext == nullptr){
		std::cout << "SDL_GL_CreateContext failed with error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	OGLSetup();

	ShaderProgram program1{vertShader1Path, theGreatSunFragPath, Vec2int(SCREEN_WIDTH, SCREEN_HEIGHT)};
	program1.LogInfo();

	//init imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	ImGui_ImplSDL2_InitForSDLRenderer(profileWindow, profileRenderer);
	ImGui_ImplSDLRenderer2_Init(profileRenderer);

	SDL_Event event;
	bool running = true;
	int mouseX = SCREEN_WIDTH / 2, mouseY = SCREEN_HEIGHT / 2;

	while(running){
		Uint64 frameStartTimePC = SDL_GetPerformanceCounter();
		SDL_GetWindowSizeInPixels(window1, &SCREEN_WIDTH, &SCREEN_HEIGHT);
		//read keyboard input
		while(SDL_PollEvent(&event)){
			ImGui_ImplSDL2_ProcessEvent(&event);
			if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE){
				running = false;
				SDL_DestroyWindow(window1);
				SDL_DestroyWindow(profileWindow);
				Cleanup();
				return 0;
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
					case SDLK_c:
						SCREEN_WIDTH = SCREEN_HEIGHT;
						SDL_SetWindowSize(window1, SCREEN_WIDTH, SCREEN_HEIGHT);
						break;
					case SDLK_b:
						bool isBordered = SDL_GetWindowFlags(window1) & SDL_WINDOW_BORDERLESS; 
						SDL_SetWindowBordered(window1, isBordered ? SDL_TRUE : SDL_FALSE);
						break;
				}
			}
		}
		//read mouse input
		SDL_GetMouseState(&mouseX, &mouseY);
		mouseX /= SCREEN_SCALE;
		mouseY /= SCREEN_SCALE;

		//specify a triangle
		const std::vector<GLfloat> vertexData{
		   //x,  y,  z
		   //R,  G,  B
			-1, -1,  0,	//vert 1 bottomLeft
			 1,  0,  0,
			 1, -1,  0,	//vert 2 bottomRight
			 0,  1,  0,
			-1,  1,  0,	//vert 3 topLeft
			 0,  0,  1,
			 1,  1,  0,	//vert 4 topRight
			 1,  0,  0
		};
		const std::vector<GLuint> indexData{
			0, 1, 2,
			2, 1, 3
		};

		//imgui frame
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGui::StyleColorsDark();
		ImGui::Begin("Profiler");
		ImGui::Text("Screen Width: %d, Screen Height: %d", SCREEN_WIDTH, SCREEN_HEIGHT);
		ImGui::Text("MouseX: %d, MouseY: %d", mouseX, mouseY);
		ImGui::Text("Delta Time: %f", deltaTime);
		ImGui::Text("Frame rate: %f", framerate);
		ImGui::End();
		ImGui::Render();

		GLuint vao1 = 0;
		GLuint vbo1 = 0;
		GLuint ibo1 = 0;
		glGenVertexArrays(1, &vao1);																					//generate 1 vao1, and store it in vao1
		glBindVertexArray(vao1);																						//bind vao1, essentially select it to do something...
		glGenBuffers(1, &vbo1);																							//generate 1 buffer, and store it in vbo1
		glBindBuffer(GL_ARRAY_BUFFER, vbo1);																			//bind vbo1, essentially select it to do something...
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * vertexData.size(), vertexData.data(), GL_STATIC_DRAW);			//copy data to vbo1 (populate vbo1)

		glGenBuffers(1, &ibo1);																							//generate 1 buffer, and store it in ibo1
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo1);																	//bind ibo1, essentially select it to do something...
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,indexData.size() * sizeof(GLuint), indexData.data(), GL_STATIC_DRAW);		//copy data to ibo1 (populate ibo1)

		glEnableVertexAttribArray(0);																					//enable vertex attribute array 0 which is the position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)0);								//0 for position attribute, 3 for number of components, GL_FLOAT for data type, GL_FALSE for normalized, 6 for stride, 0 for offset
		glEnableVertexAttribArray(1);																					//enable vertex attribute array 1 which is the color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));			//1 for color attribute,    3 for number of components, GL_FLOAT for data type, GL_FALSE for normalized, 6 for stride, 3 for offset

		//unbind vao1 and disable open attrib arrays
		glBindVertexArray(0);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		PreDraw(program1, deltaTime, Vec2int(SCREEN_WIDTH, SCREEN_HEIGHT));
		Draw(vao1, vbo1);

		SDL_SetRenderDrawColor(profileRenderer, 0, 0, 0, 1);
		SDL_RenderClear(profileRenderer);
		ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), profileRenderer);
		SDL_RenderPresent(profileRenderer);

		SDL_GL_SwapWindow(window1);

		//calculate frame time to render, and delay by <desired frame time> - <current frame time> to make up the rest of the delay,
		//then calculate total frame (delay + render) time and print it
		// @todo investiminigate why the hell is the frameTimeDeltaMiliS is not consistent
		{
			uint64_t frameEndTimePC = SDL_GetPerformanceCounter();
			frameTimeDeltaMiliS = (frameEndTimePC - frameStartTimePC) / (float)SDL_GetPerformanceFrequency();
			float delayTimeMiliS = (1000 / DESIRED_FPS) - frameTimeDeltaMiliS;
			deltaTime += ImGui::GetIO().DeltaTime;
			framerate = ImGui::GetIO().Framerate;
			// SDL_Delay(delayTimeMiliS);
			//maybe causing extra delay
			//float totalFrameTimeMiliS = ((SDL_GetPerformanceCounter() - frameStartTimePC) / (float)SDL_GetPerformanceFrequency()) * 1000;
		}
		#ifdef PROFILING
		std::cout << "frameStartTimePC: " << frameStartTimePC
				  << "\tframeEndTimePC: " << frameEndTimePC 
				  << "\tframeTimeDeltaMiliS: " << frameTimeDeltaMiliS 
				  << "\tdelayTimeMILIS: " << delayTimeMiliS 
				  << "\ttotalFrameTimeMiliS: " << totalFrameTimeMiliS
				  << std::endl;
		#endif
	}

	Cleanup();
	return 0;
}