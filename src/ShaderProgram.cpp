#include <../include/ShaderProgram.hpp>

//public:
ShaderProgram::ShaderProgram(const std::string &vShaderSrcPath, const std::string &fShaderSrcPath, Vec2int resolution) : program(CreateShader(vShaderSrcPath, fShaderSrcPath)), u_time(0.0f), u_resolution(resolution){
	u_timeLocation = glGetUniformLocation(program, "u_time");
	u_resolutionLocation = glGetUniformLocation(program, "u_resolution");
}
ShaderProgram::~ShaderProgram(){
	glDeleteProgram(program);
}

void ShaderProgram::SetUniformTime(float time){
	u_time = time;
	glUniform1f(u_timeLocation, time);
}
void ShaderProgram::SetUniformResolution(const Vec2int resolution){
	u_resolution = resolution;
	glUniform2f(u_resolutionLocation, (float)resolution.x, (float)resolution.y);
}

void ShaderProgram::Use(){
	glUseProgram(program);
}
GLuint ShaderProgram::GetProgram(){
	return program;
}

void ShaderProgram::LogInfo(){
	std::cout << "Shader Program Info: " << std::endl;
	std::cout << "GLuint program: " << program << std::endl;
	std::cout << "float u_time: " << u_time << std::endl;
	std::cout << "Vec2int u_resolution: x = " << u_resolution.x << "  y = " << u_resolution.y << std::endl;
	std::cout << "GLint u_timeLocation: " << u_timeLocation << std::endl;
	std::cout << "GLint u_resolutionLocation: " << u_resolutionLocation << std::endl;
}

//private:
GLuint ShaderProgram::CompileShader(GLenum shaderType, const std::string &shaderSrc){
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
GLuint ShaderProgram::CreateShader(const std::string &vShaderSrcPath, const std::string &fShaderSrcPath){
	GLuint programObject = glCreateProgram();
	//make shaders
	ShaderLoader shaderLoader;
	GLuint vertexShader   = CompileShader(GL_VERTEX_SHADER, shaderLoader.LoadShaderFromFile(vShaderSrcPath));
	GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, shaderLoader.LoadShaderFromFile(fShaderSrcPath));
	//attach shaders to program
	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);
	glLinkProgram(programObject);
	//delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//check for errors
	glValidateProgram(programObject);

	return programObject;
}