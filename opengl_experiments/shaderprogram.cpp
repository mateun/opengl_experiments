#include "stdafx.h"
#include "shaderprogram.h"
#include <SDL.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

static const std::string readCodeFromFile(const char* file) {
	std::ifstream inFile(file, std::ios::in);
	if (!inFile) {
		return 0;
	}

	std::ostringstream code;
	while (inFile.good()) {
		int c = inFile.get();
		if (!inFile.eof()) code << (char)c;
	}
	inFile.close();

	return code.str();
}

fb::ShaderProgram::ShaderProgram(const char * vsFileName, const char * fsFileName)
{
	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	const std::string vsstring = readCodeFromFile(vsFileName);
	const GLchar* vssource_char = vsstring.c_str();
	
	glShaderSource(vshader, 1, &vssource_char, NULL);
	glCompileShader(vshader);
	GLint compileStatus;
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &compileStatus);
	SDL_Log("vshader compile status: %d", compileStatus);
	if (GL_FALSE == compileStatus) {
		SDL_Log("problem with vshader!!");
		SDL_Log("vs source: %s", vssource_char);
		exit(1);
	}


	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
	const std::string fsstring = readCodeFromFile(fsFileName);
	const GLchar* fssource_char = fsstring.c_str();
	glShaderSource(fshader, 1, &fssource_char, NULL);
	glCompileShader(fshader);
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &compileStatus);
	SDL_Log("fshader compile status: %d", compileStatus);
	if (GL_FALSE == compileStatus) {
		SDL_Log("problem with fshader!!");
		SDL_Log("fs source: %s", fssource_char);
		exit(1);
	}

	_progHandle = glCreateProgram();
	glAttachShader(_progHandle, vshader);
	glAttachShader(_progHandle, fshader);
	glLinkProgram(_progHandle);
	glGetProgramiv(_progHandle, GL_LINK_STATUS, &compileStatus);
	GLenum err = glGetError();
	if (err != 0) SDL_Log("error: %d", err);
	SDL_Log("linkstatus: %d", compileStatus);

	glDeleteShader(vshader);
	glDeleteShader(fshader);
}

fb::ShaderProgram fb::ShaderManager::createShader(const std::string & vsFileName, const std::string & fsFileName, const std::string & shaderName)
{
	ShaderProgram sp(vsFileName.c_str(), fsFileName.c_str());
	_shaderMap[shaderName] = sp;
	return sp;
}

fb::ShaderProgram fb::ShaderManager::getShader(const std::string & shaderName)
{
	return _shaderMap[shaderName];
}
