/*
	WARNING! the code you see here may cause you to have eye cancer
	!YOU HAVE BEEN WARNED!
	todo: refactor this code.
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
#include <../include/VertexBuffer.hpp>
#include <../include/IndexBuffer.hpp>

//third party
#include <../glad/include/glad/glad.h>

#include <../glm/glm.hpp>
#include <../glm/gtc/matrix_transform.hpp>

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

//shader filepaths
const std::string vertShader1Path = "shaders/vertShader1.vert";
const std::string vertShader2Path = "shaders/vertShader2.vert";

const std::string fragShader1Path = "shaders/fragShader1.frag";
const std::string psyShaderFragPath = "shaders/psyShader.frag";
const std::string theGreatSunFragPath = "shaders/theGreatSun.frag";

char customFragShaderPath[256] = "shaders/theGreatSun.frag";
char customVertShaderPath[256] = "shaders/vertShader1.vert";
bool didShaderChange = false;


void PreDraw(ShaderProgram program, float shaderTime, Vec2int u_resolution, glm::mat4& mvp){
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program.Use();
	program.SetUniformTime(shaderTime);
	program.SetUniformResolution(u_resolution);
	program.SetUniformMVP(mvp[0][0]);
}


float deltaTime = 0;
float frameTimeDeltaMiliS = 0;
float framerate = 0;

void ImGuiProfilerFrame(float deltaTime, float framerate){
	ImGui::StyleColorsDark();
	ImGui::Begin("Profiler");
	ImGui::Text("Screen Width: %d, Screen Height: %d", SCREEN_WIDTH, SCREEN_HEIGHT);
	ImGui::Text("Delta Time: %f", deltaTime);
	ImGui::Text("Frame rate: %f", framerate);
	ImGui::End();
}

void ImGuiShaderInputFrame(){
	ImGui::Begin("Shader Editor");
	ImGui::Text("paths to shaders:");
	static char inputFragmentShaderPath[256] = "shaders\\theGreatSun.frag";
	ImGui::InputText("##path1", inputFragmentShaderPath, 256);
	static char inputVertexShaderPath[256] =   "shaders\\vertShader1.vert";
	ImGui::InputText("##path2", inputVertexShaderPath, 256);
	ImGui::Text("Current shaders:");
	ImGui::Text(customFragShaderPath);
	ImGui::Text(customVertShaderPath);
	
	if(ImGui::Button("Load Shader")){
		strcpy(customFragShaderPath, inputFragmentShaderPath);
		strcpy(customVertShaderPath, inputVertexShaderPath);
		didShaderChange = true;
	}
	if(ImGui::Button("reset time")){
		deltaTime = 0;
	}
	ImGui::End();
}

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

	ShaderProgram program1{customVertShaderPath, customFragShaderPath, Vec2int(SCREEN_WIDTH, SCREEN_HEIGHT)};
	glm::mat4 projMat = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f);
	program1.SetUniformMVP(projMat);
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
		program1.SetUniformResolution(Vec2int(SCREEN_WIDTH, SCREEN_HEIGHT));
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
				//these have to be here outside the cases, because c++ throws error: "jump to case label" and note: "crosses initialization of 'bool isBordered'" for bools
				Uint32 window1Flags = SDL_GetWindowFlags(window1);
				bool isBordered = window1Flags & SDL_WINDOW_BORDERLESS;
				bool isAlwaysOnTop = window1Flags & SDL_WINDOW_ALWAYS_ON_TOP;
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
						(SCREEN_WIDTH > SCREEN_HEIGHT) ? SCREEN_HEIGHT = SCREEN_WIDTH : SCREEN_WIDTH = SCREEN_HEIGHT;
						SDL_SetWindowSize(window1, SCREEN_WIDTH, SCREEN_HEIGHT);
						break;
					case SDLK_b:
						SDL_SetWindowBordered(window1, isBordered ? SDL_TRUE : SDL_FALSE);
						break;
					case SDLK_t:
						SDL_SetWindowAlwaysOnTop(window1, isAlwaysOnTop ? SDL_FALSE : SDL_TRUE);
						SDL_SetWindowAlwaysOnTop(profileWindow, isAlwaysOnTop ? SDL_FALSE : SDL_TRUE);
						break;
					case SDLK_x:
						SDL_SetWindowPosition(window1, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
						break;
				}
			}
		}
		//read mouse input
		SDL_GetMouseState(&mouseX, &mouseY);
		mouseX /= SCREEN_SCALE;
		mouseY /= SCREEN_SCALE;

		if(didShaderChange){
			program1.ReloadShader(customVertShaderPath, customFragShaderPath);
			program1.SetUniformMVP(projMat);
			program1.LogInfo();
			didShaderChange = false;
		}

		//specify a triangle
		const std::vector<GLfloat> vertexData{
		   //x, y, z, w
			-1, -1,  0,  0,	//vert 1 bottomLeft
			 1, -1,  1,  0,	//vert 2 bottomRight
			-1,  1,  0,  1,	//vert 3 topLeft
			 1,  1,  1,  1,	//vert 4 topRight
		};
		const std::vector<GLuint> indexData{
			0, 1, 2,
			2, 1, 3
		};

		GLuint vao1 = 0;
		glGenVertexArrays(1, &vao1);																					//generate 1 vao1, and store it in vao1
		glBindVertexArray(vao1);																						//bind vao1, essentially select it to do something...
		
		VertexBuffer vertexBuffer1{vertexData.data(), sizeof(GLfloat) * vertexData.size()};								//create a vertex buffer object with vertexData
		IndexBuffer indexBuffer1{indexData.data(), 6};																	//create an index buffer object with indexData

		glEnableVertexAttribArray(0);																					//enable vertex attribute array 0 which is the position attribute
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);								//0 for position attribute, 3 for number of components, GL_FLOAT for data type, GL_FALSE for normalized, 3 for stride, 0 for offset

		//unbind vao1 and disable open attrib arrays
		glBindVertexArray(0);
		
		glDisableVertexAttribArray(0);

		PreDraw(program1, deltaTime, Vec2int(SCREEN_WIDTH, SCREEN_HEIGHT), projMat);
		glBindVertexArray(vao1);
		indexBuffer1.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		SDL_SetRenderDrawColor(profileRenderer, 0, 0, 0, 1);
		SDL_RenderClear(profileRenderer);

		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();
		ImGuiProfilerFrame(deltaTime, framerate);
		ImGuiShaderInputFrame();
		ImGui::Render();
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
			SDL_Delay(delayTimeMiliS);
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