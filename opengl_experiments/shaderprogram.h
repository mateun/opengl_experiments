#pragma once
#include <GL/glew.h>
#include <map>
#include <string>

namespace fb {

	class ShaderProgram {
	public:
		ShaderProgram() {}
		ShaderProgram(const char* vsFileName, const char* fsFileName);
		GLuint _progHandle;
	};

	/**
		The ShaderManager provides a convenient way to reuse shader programs
		by storing them in a map. 
	*/
	class ShaderManager {
	public:
		ShaderManager() {}
		ShaderProgram createShader(const std::string& vsFileName, const std::string& fsFileName, const std::string& shaderName);
		ShaderProgram getShader(const std::string& shaderName);
	private:
		std::map<std::string, ShaderProgram> _shaderMap;
	};

	
}

