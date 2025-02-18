#include <../include/ShaderProgram.hpp>
#include <../glm/glm.hpp>

//public:
ShaderProgram::ShaderProgram(const std::string &vShaderSrcPath, const std::string &fShaderSrcPath, Vec2int resolution) : _program(CreateShader(vShaderSrcPath, fShaderSrcPath)){
	u_timeLocation = glGetUniformLocation(_program, "u_time");
	u_resolutionLocation = glGetUniformLocation(_program, "u_resolution");
	u_MVPLocation = glGetUniformLocation(_program, "u_MVP");
}
ShaderProgram::~ShaderProgram(){
	glDeleteProgram(_program);
}

void ShaderProgram::SetUniformTime(float time){
	glUniform1f(u_timeLocation, time);
}
void ShaderProgram::SetUniformResolution(const Vec2int resolution){
	glUniform2f(u_resolutionLocation, (float)resolution._x, (float)resolution._y);
}
void ShaderProgram::SetUniformMVP(const glm::mat4& mat){
	glUniformMatrix4fv(u_MVPLocation, 1, GL_TRUE, &mat[0][0]);
}

void ShaderProgram::SetUniformM4f(const std::string &matrixName, const glm::mat4 &matrix){
	glUniformMatrix4fv(glGetUniformLocation(_program, matrixName.c_str()), 1, GL_FALSE, &matrix[0][0]);
}
void ShaderProgram::SetUniformF(const std::string &floatName, float value){
	glUniform1f(glGetUniformLocation(_program, floatName.c_str()), value);
}

void ShaderProgram::Use(){
	glUseProgram(_program);
}
GLuint ShaderProgram::GetProgram(){
	return _program;
}

void ShaderProgram::LogInfo(){
	std::cout << "-------- Shader Program Info --------" << std::endl;
	std::cout << "GLuint program: " << _program << std::endl;
	std::cout << "u_timeLocation: " << u_timeLocation << std::endl;
	std::cout << "u_resolutionLocation: " << u_resolutionLocation << std::endl;
	std::cout << "u_MVP: " << u_MVPLocation << std::endl;
}

void ShaderProgram::ReloadShader(const std::string &vShaderSrcPath, const std::string &fShaderSrcPath){
	glDeleteProgram(_program);
	_program = CreateShader(vShaderSrcPath, fShaderSrcPath);
	u_timeLocation = glGetUniformLocation(_program, "u_time");
	u_resolutionLocation = glGetUniformLocation(_program, "u_resolution");
	u_MVPLocation = glGetUniformLocation(_program, "u_MVP");
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