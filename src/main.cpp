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
#include <../include/ShaderLoader.hpp>

//third party
#include <../glad/include/glad/glad.h>


const int SCREEN_SCALE = 1;
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 480;
uint16_t DESIRED_FPS = 60;  //try UINT16_MAX, it's fun

void ProfilerOnWindowTitle(SDL_Window *wind, float totalFrameTimeMiliS, float frameTimeDeltaMiliS, float delayTimeMiliS){
	SDL_SetWindowTitle(wind, ("totalFrameTimeMiliS: "    + std::to_string(totalFrameTimeMiliS) + 
							  "\t frameTimeDeltaMiliS: " + std::to_string(frameTimeDeltaMiliS) + 
							  "\t delayTimeMiliS: "      + std::to_string(delayTimeMiliS)).c_str());
}

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
}

GLuint CompileShader(GLenum shaderType, const std::string &shaderSrc){
	GLuint shaderObject;
	
	if(shaderType == GL_VERTEX_SHADER){
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
	
	}else if(shaderType == GL_FRAGMENT_SHADER){
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	}
	const char *src = shaderSrc.c_str();
	glShaderSource(shaderObject, 1, &src, nullptr);
	glCompileShader(shaderObject);

	return shaderObject;
}

GLuint CreateShader(const std::string &vShaderSrc, const std::string &fShaderSrc){
	GLuint programObject = glCreateProgram();
	//make shaders
	GLuint vertexShader   = CompileShader(GL_VERTEX_SHADER, vShaderSrc);
	GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fShaderSrc);
	//attach shaders to program
	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);
	glLinkProgram(programObject);
	//check for errors
	glValidateProgram(programObject);

	return programObject;
}


//shader filenames const
const std::string vertShader1Path = "shaders/vertShader1.vert";
const std::string fragShader1Path = "shaders/fragShader1.frag";

//shader loader
ShaderLoader shaderLoader;

//load shaders
const std::string vertexShader1 = shaderLoader.LoadShaderFromFile(vertShader1Path);
const std::string fragmentShader1 = shaderLoader.LoadShaderFromFile(fragShader1Path);


void PreDraw(){
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(CreateShader(vertexShader1, fragmentShader1));
}

void Draw(GLuint vertexArrayObject, GLuint vertexBufferObject){
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

float deltaTime = 0;
int main(int argc, char* argv[]){
	if(SDL_Init(SDL_INIT_EVERYTHING)){
		std::cout << "SDL_INIT failed with error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	SDL_Window *window1 = SDL_CreateWindow("Window 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_GLContext glContext = SDL_GL_CreateContext(window1);	//create opengl context object
	if(window1 == nullptr){
		std::cout << "SDL_CreateWindow failed with error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}
	if(glContext == nullptr){
		std::cout << "SDL_GL_CreateContext failed with error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	OGLSetup();

	SDL_Event event;
	bool running = true;
	int mouseX = SCREEN_WIDTH / 2, mouseY = SCREEN_HEIGHT / 2;
	while(running){
		SDL_GetWindowSizeInPixels(window1, &SCREEN_WIDTH, &SCREEN_HEIGHT);
		Uint64 frameStartTimePC = SDL_GetPerformanceCounter();
		//read keyboard input
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = false;
				SDL_DestroyWindow(window1);
				Cleanup();
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
		}
		//read mouse input
		SDL_GetMouseState(&mouseX, &mouseY);
		mouseX /= SCREEN_SCALE;
		mouseY /= SCREEN_SCALE;

		//specify a triangle
		const std::vector<GLfloat> triangle1Verts{
		   //x,  y, z
			-1, -1, 0,	//vert 1
			 1, -1, 0,	//vert 2
			 0,  1, 0	//vert 3
		};
		const std::vector<GLfloat> triangle1Colors{
		   //R, G, B
			 1, 0, 0,
			 0, 1, 0,
			 0, 0, 1
		};

		GLuint vao1 = 0;
		GLuint vbo1 = 0;
		GLuint vbo2 = 0;
		glGenVertexArrays(1, &vao1);			//generate 1 vao1, and store it in vao1
		glBindVertexArray(vao1);				//bind vao1, essentially select it to do something...
		glGenBuffers(1, &vbo1);					//generate 1 buffer, and store it in vbo1
		glBindBuffer(GL_ARRAY_BUFFER, vbo1);	//bind vbo1, essentially select it to do something...
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * triangle1Verts.size(), triangle1Verts.data(), GL_STATIC_DRAW);	//copy data to vbo1
		glEnableVertexAttribArray(0);			//enable vertex attribute array 0 which is the position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	//0 for position attribute, 3 for number of components, GL_FLOAT for data type, GL_FALSE for normalized, 0 for stride, 0 for offset

		glGenBuffers(1, &vbo2);					//generate 1 buffer, and store it in vbo2
		glBindBuffer(GL_ARRAY_BUFFER, vbo2);	//bind vbo2, essentially select it to do something...
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * triangle1Colors.size(), triangle1Colors.data(), GL_STATIC_DRAW);	//copy data to vbo2
		glEnableVertexAttribArray(1);			//enable vertex attribute array 1 which is the color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	//1 for color attribute, 3 for number of components, GL_FLOAT for data type, GL_FALSE for normalized, 0 for stride, 0 for offset

		//unbind vao1 and disable open attrib arrays
		glBindVertexArray(0);
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		PreDraw();
		Draw(vao1, vbo1);
		Draw(vao1, vbo2);


		SDL_GL_SwapWindow(window1);
		//calculate frame time to render, and delay by <desired frame time> - <current frame time> to make up the rest of the delay,
		//then calculate total frame (delay + render) time and print it
		{
			uint64_t frameEndTimePC = SDL_GetPerformanceCounter();
			float frameTimeDeltaMiliS = (frameEndTimePC - frameStartTimePC) / (float)SDL_GetPerformanceFrequency();
			float delayTimeMiliS = (1000 / DESIRED_FPS) - frameTimeDeltaMiliS;
			deltaTime += frameTimeDeltaMiliS;
			SDL_Delay(delayTimeMiliS);
			//maybe causing a bit more delay (~2 msec)
			float totalFrameTimeMiliS = ((SDL_GetPerformanceCounter() - frameStartTimePC) / (float)SDL_GetPerformanceFrequency()) * 1000;
			ProfilerOnWindowTitle(window1, totalFrameTimeMiliS, frameTimeDeltaMiliS, delayTimeMiliS);
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