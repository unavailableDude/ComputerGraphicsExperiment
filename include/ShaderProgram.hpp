#pragma once
#include <../glad/include/glad/glad.h>
#include <ShaderLoader.hpp>
#include <Vec2int.hpp>
#include <iostream>


class ShaderProgram{
	private:
	GLuint program;
	float u_time;
	Vec2int u_resolution;
	GLint u_timeLocation;
	GLint u_resolutionLocation;

	public:
	ShaderProgram(const std::string &vShaderSrcPath, const std::string &fShaderSrcPath, Vec2int resolution);
	~ShaderProgram();

	void SetUniformTime(float time);
	void SetUniformResolution(const Vec2int resolution);

	void Use();
	GLuint GetProgram();

	void LogInfo();
	void ReloadShader(const std::string &vShaderSrcPath, const std::string &fShaderSrcPath);

	private:
	GLuint CompileShader(GLenum shaderType, const std::string &shaderSrc);
	GLuint CreateShader(const std::string &vShaderSrc, const std::string &fShaderSrc);
};