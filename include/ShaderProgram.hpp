#pragma once
#include <../glad/include/glad/glad.h>
#include <../glm/glm.hpp>
#include <ShaderLoader.hpp>
#include <Vec2int.hpp>
#include <iostream>


class ShaderProgram{
	private:
	GLuint _program;
	GLint u_timeLocation;
	GLint u_resolutionLocation;
	GLint u_MVPLocation;

	public:
	ShaderProgram(const std::string &vShaderSrcPath, const std::string &fShaderSrcPath, Vec2int resolution);
	~ShaderProgram();

	void SetUniformTime(float time);
	void SetUniformResolution(const Vec2int resolution);
	void SetUniformMVP(const glm::mat4& mat);
	void SetUniformM4f(const std::string &matrixName, const glm::mat4 &matrix);
	void SetUniformF(const std::string &floatName, float value);

	void Use();
	GLuint GetProgram();

	void LogInfo();
	void ReloadShader(const std::string &vShaderSrcPath, const std::string &fShaderSrcPath);

	private:
	GLuint CompileShader(GLenum shaderType, const std::string &shaderSrc);
	GLuint CreateShader(const std::string &vShaderSrc, const std::string &fShaderSrc);
};