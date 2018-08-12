// opengl_experiments.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <SDL.h>
#include <GL/glew.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using std::ifstream;
using std::ios;
using std::ostringstream;

GLuint importModelFromFile(const char* fileName, int& nrVerts) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate /*| aiProcess_FlipWindingOrder | aiProcess_GenUVCoords | aiProcess_FixInfacingNormals | aiProcess_MakeLeftHanded*/);

	if (!scene) {
		SDL_Log("can not find model file!");
		exit(-1);
	}

	aiMesh* mesh = scene->mMeshes[0];

	nrVerts = mesh->mNumVertices;

	float* vertices = new float[mesh->mNumVertices * 3];
	float* normals = new float[mesh->mNumVertices * 3];
	float* uvs = new float[mesh->mNumVertices * 2];
	uint32_t* indeces = new uint32_t[mesh->mNumFaces * 3];
	int p = 0;
	int u = 0;
	int i = 0;
	for (int m = 0; m < mesh->mNumVertices; m++) {
		aiVector3D v = mesh->mVertices[m];
		vertices[p] = v.x;
		vertices[p + 1] = v.y;
		vertices[p + 2] = v.z;


		aiVector3D n = mesh->mNormals[m];
		normals[p] = n.x;
		normals[p + 1] = n.y;
		normals[p + 2] = n.z;

		p += 3;


		if (mesh->HasTextureCoords(0)) {
			aiVector3D t = mesh->mTextureCoords[0][m];
			uvs[u] = t.x;
			uvs[u + 1] = 1 - t.y;		// otherwise the texture is wrong direction coming from blender
			u += 2;
		}
	}

	for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
		aiFace face = mesh->mFaces[f];
		int findices = face.mNumIndices;
		indeces[i] = face.mIndices[0];
		indeces[i + 1] = face.mIndices[1];
		indeces[i + 2] = face.mIndices[2];
		i += 3;
	}

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbuf;
	glGenBuffers(1, &vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, vbuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * nrVerts, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	return vao;

	//_vb = new VertexBuffer(renderer, vertices, mesh->mNumVertices, VertexBufferType::Position);
	//_vbuvs = new VertexBuffer(renderer, uvs, mesh->mNumVertices, VertexBufferType::UVs);
	//_normals = new VertexBuffer(renderer, normals, mesh->mNumVertices, VertexBufferType::Normal);
	//_ib = new IndexBuffer(renderer, indeces, mesh->mNumFaces * 3);




}

bool fileExists(const char* fileName)
{
	struct stat info;
	int ret = -1;

	ret = stat(fileName, &info);
	return 0 == ret;
}

const std::string readCodeFromFile(const char* file) {
	ifstream inFile(file, ios::in);
	if (!inFile) {
		return 0;
	}

	ostringstream code;
	while (inFile.good()) {
		int c = inFile.get();
		if (!inFile.eof()) code << (char)c;
	}
	inFile.close();

	return code.str();
}

GLuint createShaderProgram(const char* vsfile, const char* fsfile) {
	
	

	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	const std::string vsstring = readCodeFromFile(vsfile);
	const GLchar* vssource_char = vsstring.c_str();
	SDL_Log("vs source: %s", vssource_char);
	glShaderSource(vshader, 1, &vssource_char, NULL);
	glCompileShader(vshader);
	GLint compileStatus;
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &compileStatus);
	SDL_Log("vshader compile status: %d", compileStatus);
	if (GL_FALSE == compileStatus) {
		SDL_Log("problem with shader!!");
	}


	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
	const std::string fsstring = readCodeFromFile(fsfile);
	const GLchar* fssource_char = fsstring.c_str();
	glShaderSource(fshader, 1, &fssource_char, NULL);
	glCompileShader(fshader);
	glGetShaderiv(fshader, GL_COMPILE_STATUS, &compileStatus);
	SDL_Log("fshader compile status: %d", compileStatus);

	GLuint program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &compileStatus);
	GLenum err = glGetError();
	if (err != 0) SDL_Log("error: %d", err);
	SDL_Log("linkstatus: %d", compileStatus);

	glDeleteShader(vshader);
	glDeleteShader(fshader);

	return program;
}

int main(int argc, char** args) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("error init %s", SDL_GetError());
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_Window* window = SDL_CreateWindow("RoboHunt",
		100, 100,
		800, 600,
		SDL_WINDOW_OPENGL);



	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	const GLubyte* version = glGetString(GL_VERSION);
	SDL_Log("GL Version: %s", version);
	version = glGetString(GL_SHADING_LANGUAGE_VERSION);
	SDL_Log("Shader version: %s", version);

	bool gameRunning = true;
	SDL_Event event;
	glClearColor(0.1, 0.1, 0.1, 1);

	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if (GLEW_OK != error) {
		SDL_Log("glew init failed %s", glewGetErrorString(error));
		return -1;
	}

	GLint majVer, minVer;
	glGetIntegerv(GL_MAJOR_VERSION, &majVer);
	glGetIntegerv(GL_MINOR_VERSION, &minVer);
	SDL_Log("GL Version %d/%d", majVer, minVer);

	// Init GL stuff
	GLfloat positions[] = {
		-0.5, -0.5, 0, 1,
		0.5, -0.5, 0, 1,
		0, 0.5, 0, 1,
	};
	glViewport(0, 0, 800, 600);
	GLuint shader = createShaderProgram("D:/Projects/C++/opengl_experiments/opengl_experiments/shaders/vs.hlsl", "D:/Projects/C++/opengl_experiments/opengl_experiments/shaders/fs.hlsl");
	error = glGetError();
	SDL_Log("error after creating shader prog: %d", error);
	SDL_Log("prog handle: %u", shader);
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbuf;
	glGenBuffers(1, &vbuf);
	glBindBuffer(GL_ARRAY_BUFFER, vbuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int nrVertices = 0;
	int nrVertices2 = 0;
	GLuint vaoModel = importModelFromFile("D:/Projects/C++/opengl_experiments/Debug/thing.obj", nrVertices);
	GLuint vaoModel2 = importModelFromFile("D:/Projects/C++/opengl_experiments/Debug/thing2.obj", nrVertices2);

	GLenum err = glGetError();
	SDL_Log("glerror: %d", err);

	while (gameRunning) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				gameRunning = false;
				break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader);
		glBindVertexArray(vaoModel);
		glDrawArrays(GL_TRIANGLES, 0, nrVertices);

		glBindVertexArray(vaoModel2);
		glDrawArrays(GL_TRIANGLES, 0, nrVertices2);

		err = glGetError();
#if false
		SDL_Log("glError in loop: %d", err);
#endif
		SDL_GL_SwapWindow(window);
	}


	return 0;

}
