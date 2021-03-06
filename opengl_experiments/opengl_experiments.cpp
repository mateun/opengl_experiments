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
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"


#include "model.h"
#include "fbmath.h"
#include "texture.h"
#include "shaderprogram.h"
#include "camera.h"
#include "geometry.h"
#include "sprite.h"
#include "framebuffer.h"


using std::ifstream;
using std::ios;
using std::ostringstream;



// Imgui variables
ImGuiTextBuffer logBuf;
bool showWindow = true;
bool scrollLogToBottom;

fb::Model importModelFromFile(const char* fileName) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate /*| aiProcess_FlipWindingOrder | aiProcess_GenUVCoords | aiProcess_FixInfacingNormals | aiProcess_MakeLeftHanded*/);

	if (!scene) {
		SDL_Log("can not find model file!");
		exit(-1);
	}

	aiMesh* mesh = scene->mMeshes[0];

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

	fb::Model m(vertices, mesh->mNumVertices, normals, mesh->mNumVertices, uvs, mesh->mNumVertices);
	return m;
}


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
			uvs[u] = (t.x);
			uvs[u + 1] = (1 - t.y);		// otherwise the texture is wrong direction coming from blender
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
		SDL_Log("problem with vshader!!");
		exit(1);
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

void vectorTests() {
	fb::Vec4 v1(1, 2, 3, 1);
	fb::Vec4 v2(2, 5, 7, 1);
	fb::Vec4 v3 = v1 + v2;
	assert(v3.x == 3);
	assert(v3.y == 7);
	assert(v3.z == 10);

	v3 = v2 - v1;
	assert(v3.x == 1);
	fb::Vec4 v3n = v3.normalize();
	float v1len = v1.length();

	float matvalues[16] = {
		1, 0, 0, 0, 
		0, 1, 0, 0, 
		0, 0, 1, 0, 
		0, 0, 0, 1
	};
	fb::Mat4 mat(matvalues);
	assert(mat.m11 == 1);
	assert(mat.m12 == 0);
	assert(mat.m21 == 0);
	assert(mat.m22 == 1);

	fb::Vec4 vresult = mat * v1;
	assert(vresult.x == v1.x);
	assert(vresult.y == v1.y);
	assert(vresult.z == v1.z);
	
}

void AddLog(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	logBuf.appendfv(fmt, args);
	va_end(args);
	scrollLogToBottom = true;
}

int main(int argc, char** args) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_Log("error init %s", SDL_GetError());
		AddLog("error init SDL %s", SDL_GetError());
		return -1;
	}

	AddLog("SDL Init ok!\n");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_Window* window = SDL_CreateWindow("RoboHunt",
		100, 100,
		800, 600,
		SDL_WINDOW_OPENGL);



	SDL_GLContext glContext = SDL_GL_CreateContext(window);

	const GLubyte* version = glGetString(GL_VERSION);
	AddLog("GL Version: %s\n", version);
	version = glGetString(GL_SHADING_LANGUAGE_VERSION);
	AddLog("Shader version: %s\n", version);

	bool gameRunning = true;
	SDL_Event event;
	glClearColor(0, 0, 0, 1);

	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if (GLEW_OK != error) {
		AddLog("glew init failed %s\n", glewGetErrorString(error));
		return -1;
	}

	GLint majVer, minVer;
	glGetIntegerv(GL_MAJOR_VERSION, &majVer);
	glGetIntegerv(GL_MINOR_VERSION, &minVer);
	AddLog("GL Version %d/%d\n", majVer, minVer);

	// Init IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	const char* glsl_version = "#version 430";
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();

	// Init GL stuff
	fb::initStaticGeometry();
	glViewport(0, 0, 800, 600);
	
	fb::ShaderManager shaderManager;

	fb::ShaderProgram shader = shaderManager.createShader("D:/Projects/C++/opengl_experiments/opengl_experiments/shaders/vs.hlsl", "D:/Projects/C++/opengl_experiments/opengl_experiments/shaders/fs.hlsl", "defaultShader");
	error = glGetError();
	AddLog("error after creating shader prog: %d\n", error);
	AddLog("prog handle: %u\n", shader);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	fb::Model model2 = importModelFromFile("D:/Projects/C++/opengl_experiments/Debug/box.obj");
	fb::Model model3 = importModelFromFile("D:/Projects/C++/opengl_experiments/Debug/hunter_ship.obj");
	fb::Model ground = importModelFromFile("D:/Projects/C++/opengl_experiments/Debug/ground.obj");
	fb::Model wall = importModelFromFile("D:/Projects/C++/opengl_experiments/Debug/wall.obj");
	fb::Model gun = importModelFromFile("D:/Projects/C++/opengl_experiments/Debug/gun.obj");
	fb::Model axis = importModelFromFile("D:/Projects/C++/opengl_experiments/Debug/axis.obj");
	fb::Model coordinateSystem = importModelFromFile("D:/Projects/C++/opengl_experiments/Debug/coordinate_system.obj");

	GLenum err = glGetError();
	AddLog("glerror: %d\n", err);

	vectorTests();

	fb::Texture texBox("D:/Projects/C++/opengl_experiments/Debug/boxtex.bmp", false);
	fb::Texture texShip("D:/Projects/C++/opengl_experiments/Debug/shiptex.bmp", false);
	
	glm::mat4 mwv = glm::mat4(1.0f);
	static float angle = 0.0f;
	
	glm::mat4 scalem = glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 2));
	glm::mat4 translatem = glm::translate(glm::mat4(1.0f), glm::vec3(-0, 0, 0));
	
	//fb::Camera camera(glm::vec3(0, 10, 60), glm::vec3(0, 0, 0));
	fb::Camera camera(glm::vec3(0, 10, 40), glm::vec3(0, 0, -1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
	camera.initializePerspective(70);
	//camera.initializeOrthographic(-10, 10, -10, 10);

	fb::ShaderProgram shader2d = shaderManager.createShader("D:/Projects/C++/opengl_experiments/opengl_experiments/shaders/vs_2d.hlsl", "D:/Projects/C++/opengl_experiments/opengl_experiments/shaders/fs_2d.hlsl", "spriteShader");
	shaderManager.createShader("D:/Projects/C++/opengl_experiments/opengl_experiments/shaders/vs_2d.hlsl", "D:/Projects/C++/opengl_experiments/opengl_experiments/shaders/fs_2d_depth.hlsl", "depthShader");

	fb::Texture fontTexture("D:/Projects/C++/opengl_experiments/Debug/font.bmp", false);
	fb::Texture marsTexture("D:/Projects/C++/opengl_experiments/Debug/mars-texture.bmp", true);
	fb::Texture crossHairTex("D:/Projects/C++/opengl_experiments/Debug/crosshair.bmp", false);
	fb::Texture axisBlueTex("D:/Projects/C++/opengl_experiments/Debug/axis_blue.bmp", false);
	fb::Texture axisRedTex("D:/Projects/C++/opengl_experiments/Debug/axis_red.bmp", false);
	fb::Texture axisGreenTex("D:/Projects/C++/opengl_experiments/Debug/axis_green.bmp", false);
	fb::Texture axisAll("D:/Projects/C++/opengl_experiments/Debug/axis_all.bmp", false);
	fb::Sprite crossHairSprite(crossHairTex, shaderManager, 800, 600);

	// End orthographic stuff

	// Sprite setup 
	fb::Texture xyzTex("D:/Projects/C++/opengl_experiments/Debug/xyz_tex.bmp", false);
	fb::Texture titleTex("D:/Projects/C++/opengl_experiments/Debug/robohunt_titlescreen_800x600.bmp", false);
	fb::Sprite sprite1(xyzTex, shaderManager, 800, 600);
	fb::Sprite fontSprite(fontTexture, shaderManager, 800, 600);
	fb::Sprite titleSprite(titleTex, shaderManager, 800, 600);
	fontSprite.scale(0.25f, 0.25f);
	// end sprite setup

	// Framebuffer setup
	fb::FrameBuffer fb1(800, 600);

	// setup sprite to debug-render the shadow map
	fb::Sprite offScreenSprite(fb1._depthTexture, shaderManager, 800, 600);
	
	glm::vec3 lightPos(10, 30, 10);

	const Uint8 *kbState = SDL_GetKeyboardState(NULL);
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	float camSpeed = 0.8f;
	float camRotSpeed = 0.01f;
	

	glm::mat4 gunRot = glm::rotate(glm::mat4(1), -camRotSpeed, camera._up);
	glm::vec3 gf = glm::vec3(0, 0, 1);
	glm::vec3 gr = glm::vec3(1, 0, 0);
	glm::vec3 gu = glm::vec3(0, 1, 0);
	float gunRotY = 0.0f;

	gunRot = glm::column(gunRot, 0, glm::vec4(gr, 0));
	gunRot = glm::column(gunRot, 1, glm::vec4(gu, 0));
	gunRot = glm::column(gunRot, 2, glm::vec4(gf, 0));

	float effectiveMouseX = 0, effectiveMouseY = 0;

	while (gameRunning) {
		while (SDL_PollEvent(&event)) {

			ImGui_ImplSDL2_ProcessEvent(&event);

			if (event.type == SDL_QUIT) {
				gameRunning = false;
				break;
			}
		}

		// Imgui rendering
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		
		if (showWindow) {
			ImGui::Begin("Program Log", &showWindow);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::TextUnformatted(logBuf.begin());
			if (scrollLogToBottom)
				ImGui::SetScrollHere(1.0f);
			scrollLogToBottom = false;
			ImGui::End();
		}
		
		ImGui::Render();

		// end imgui rendering
		

		glViewport(0, 0, 800, 600);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int mouseX = 0, mouseY =0;
		
		
		
		if (SDL_GetRelativeMouseState(&mouseX, &mouseY) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
			effectiveMouseX = mouseX;
			effectiveMouseY = mouseY;
			camera._forward = glm::rotate(glm::mat4(1.0f), -camRotSpeed * mouseX, camera._up) * glm::vec4(camera._forward.x, camera._forward.y, camera._forward.z, 1);
			//gf = glm::rotate(glm::mat4(1.0f), -camRotSpeed * mouseX, gu) * glm::vec4(gf, 1);
			
			camera._right = glm::rotate(glm::mat4(1.0f), -camRotSpeed*mouseX, camera._up) * glm::vec4(camera._right.x, camera._right.y, camera._right.z, 1);
			camera._right.y = 0;

			//gr = glm::rotate(glm::mat4(1), -camRotSpeed * mouseX, gu) * glm::vec4(gr, 1);
			
			
			camera._forward = glm::rotate(glm::mat4(1.0f), -camRotSpeed * mouseY, camera._right) * glm::vec4(camera._forward.x, camera._forward.y, camera._forward.z, 1);
			//gf = glm::rotate(glm::mat4(1.0f), -camRotSpeed * mouseY, gr) * glm::vec4(gf, 1);
			//gu = glm::rotate(glm::mat4(1), -camRotSpeed * mouseY, gr) * glm::vec4(gu, 1);
			
			
		}
		if (kbState[SDL_SCANCODE_ESCAPE]) {
			gameRunning = false;
		}

		// Camera movement
		if (kbState[SDL_SCANCODE_I]) {

			/*vec<3, T, Q> const f(normalize(center - eye));
			vec<3, T, Q> const s(normalize(cross(up, f)));
			vec<3, T, Q> const u(cross(f, s));*/

			// pitch = rotate about local "right" axis
			camera._forward = glm::rotate(glm::mat4(1.0f), -camRotSpeed, camera._right) * glm::vec4(camera._forward.x, camera._forward.y, camera._forward.z, 1);
			//camera._up = glm::rotate(glm::mat4(1.0f), -0.01f, camera._right) * glm::vec4(camera._up.x, camera._up.y, camera._up.z, 1);
		}

		if (kbState[SDL_SCANCODE_J]) {
			// pitch = rotate about local "right" axis
			camera._forward = glm::rotate(glm::mat4(1.0f), camRotSpeed, camera._right) * glm::vec4(camera._forward.x, camera._forward.y, camera._forward.z, 1);
			//camera._up = glm::rotate(glm::mat4(1.0f), 0.01f, camera._right) * glm::vec4(camera._up.x, camera._up.y, camera._up.z, 1);
		}

		if (kbState[SDL_SCANCODE_D]) {
			// strafe = move along the right axis
			camera.pos += camera._right * camSpeed;
			//camera._right = glm::rotate(glm::mat4(1.0f), -0.01f, camera._up) * glm::vec4(camera._right.x, camera._right.y, camera._right.z, 1);
		}

		if (kbState[SDL_SCANCODE_A]) {
			// strafe = move along the right axis
			camera.pos -= camera._right * camSpeed;
			//camera._forward = glm::rotate(glm::mat4(1.0f), 0.01f, camera._up) * glm::vec4(camera._forward.x, camera._forward.y, camera._forward.z, 1);
			//camera._right = glm::rotate(glm::mat4(1.0f), 0.01f, camera._up) * glm::vec4(camera._right.x, camera._right.y, camera._right.z, 1);
		}


		if (kbState[SDL_SCANCODE_W]) {
			camera.pos += camera._forward * camSpeed;
		}

		if (kbState[SDL_SCANCODE_S]) {
			camera.pos -= camera._forward * camSpeed;
		}
		
		if (kbState[SDL_SCANCODE_R]) {
			gunRotY += 0.01f;
		}

		camera.updateFromDirections();
		

		// end cam movement
		
		angle += 0.01f;
		glm::mat4 rm = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));

		// Render the whole world from the lights perspective into an offscreen buffer
		fb1.bind();
		glClearColor(0, 0, 1, 1);
		glViewport(0, 0, 800, 600);
		GLfloat cleardepth[] = { 1 };
		glClearBufferfv(GL_DEPTH, 0, cleardepth);
		
		fb::Camera lightCam(lightPos, glm::vec3(0, 0, 0));
		lightCam.initializeOrthographic(-70, 70, -70, 70);
		
		model2.render(glm::translate(glm::mat4(1.0f), glm::vec3(5, 7, -2)), lightCam.view, lightCam.proj, rm, scalem, shader, texBox, lightPos, nullptr);
		model2.render(glm::translate(glm::mat4(1.0f), glm::vec3(-5, 1.5, 0)), lightCam.view, lightCam.proj, rm, scalem, shader, texBox, lightPos, nullptr);
		model3.render(glm::translate(glm::mat4(1.0f), glm::vec3(5, 15, -5)), lightCam.view, lightCam.proj, rm, scalem, shader, texShip, lightPos, nullptr);
		model3.render(glm::translate(glm::mat4(1.0f), glm::vec3(-15, 3, 5)), lightCam.view, lightCam.proj, rm, scalem, shader, texShip, lightPos, nullptr);

		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(-0, -1, 0)), lightCam.view, lightCam.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, nullptr);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(-0, -1, -40)), lightCam.view, lightCam.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, nullptr);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(-40, -1, 0)), lightCam.view, lightCam.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, nullptr);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(40, -1, 0)), lightCam.view, lightCam.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, nullptr);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(40, -1, -40)), lightCam.view, lightCam.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, nullptr);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(-40, -1, -40)), lightCam.view, lightCam.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, nullptr);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(-0, -1, 40)), lightCam.view, lightCam.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, nullptr);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(-40, -1, 40)), lightCam.view, lightCam.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, nullptr);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(40, -1, 40)), lightCam.view, lightCam.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, nullptr);

		wall.render(glm::translate(glm::mat4(1.0f), glm::vec3(25, -0.6, 0)), lightCam.view, lightCam.proj, glm::rotate(glm::mat4(1), 1.57f, glm::vec3(0, 1, 0)), scalem, shader, texShip, lightPos, nullptr);
		fb1.unbind();
		// end light rendering

		// The 3d world
		glViewport(0, 0, 800, 600);
		model2.render(glm::translate(glm::mat4(1.0f), glm::vec3(5, 7, -2)), camera.view, camera.proj, rm, scalem, shader, texBox, lightPos, nullptr);
		model2.render(glm::translate(glm::mat4(1.0f), glm::vec3(-5, 1.5, 0)), camera.view, camera.proj, rm, scalem, shader, texBox, lightPos, nullptr);
		model3.render(glm::translate(glm::mat4(1.0f), glm::vec3(5, 15, -5)), camera.view, camera.proj, rm, scalem, shader, texShip, lightPos, &offScreenSprite._tex);
		model3.render(glm::translate(glm::mat4(1.0f), glm::vec3(-15, 3, 5)), camera.view, camera.proj, rm, scalem, shader, texShip, lightPos, &offScreenSprite._tex);
		
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(-0, -1, 0)), camera.view, camera.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, &offScreenSprite._tex);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(-0, -1, -40)), camera.view, camera.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, &offScreenSprite._tex);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(-40, -1, 0)), camera.view, camera.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, &offScreenSprite._tex);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(40, -1, 0)), camera.view, camera.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, &offScreenSprite._tex);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(40, -1, -40)), camera.view, camera.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, &offScreenSprite._tex);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(-40, -1, -40)), camera.view, camera.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, &offScreenSprite._tex);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(-0, -1, 40)), camera.view, camera.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, &offScreenSprite._tex);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(-40, -1, 40)), camera.view, camera.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, &offScreenSprite._tex);
		ground.render(glm::translate(glm::mat4(1.0f), glm::vec3(40, -1, 40)), camera.view, camera.proj, glm::mat4(1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, 1)), shader, marsTexture, lightPos, &offScreenSprite._tex);

		wall.render(glm::translate(glm::mat4(1.0f), glm::vec3(25, -0.6, 0)), camera.view, camera.proj, glm::rotate(glm::mat4(1), 1.57f, glm::vec3(0, 1, 0)), scalem, shader, texShip, lightPos, nullptr);
		
		
		gr = glm::rotate(glm::mat4(1), -camRotSpeed * effectiveMouseX, gu) * glm::vec4(gr, 0);
		gf = glm::rotate(glm::mat4(1), -camRotSpeed * effectiveMouseX, gu) * glm::vec4(gf, 0);
		gf = glm::rotate(glm::mat4(1), -camRotSpeed * effectiveMouseY, gr) * glm::vec4(gf, 0);
		gunRot = glm::column(gunRot, 0, glm::vec4(gr, 0));
		gunRot = glm::column(gunRot, 1, glm::vec4(gu, 0));
		gunRot = glm::column(gunRot, 2, glm::vec4(gf, 0));

		float angleForwardRight = glm::acos(glm::dot(gf, gr));
		AddLog("angleForwardRight: %f\n", angleForwardRight);

		//gunRot = glm::rotate(gunRot, -camRotSpeed * effectiveMouseX, glm::vec3(0, 1, 0));
		//gunRot = glm::rotate(glm::mat4(1), -camRotSpeed * effectiveMouseY, glm::vec3(1, 0, 0));
		
		gun.render(glm::translate(glm::mat4(1.0f), camera.pos + gf * -2.5f + glm::vec3(0.5, -2, 0)), camera.view, camera.proj, gunRot, scalem, shader, texShip, lightPos, nullptr);

		coordinateSystem.render(glm::translate(glm::mat4(1), glm::vec3(0, 2, 3)), camera.view, camera.proj, gunRot, scalem, shader, axisAll, lightPos, nullptr);
		//axis.render(glm::translate(glm::mat4(1), glm::vec3(0, 2, 3)), camera.view, camera.proj, gunRot, scalem, shader, axisRedTex, lightPos, nullptr);
		
		// Render the coordinate axis
		//fb::getXAxis().render(glm::mat4(1), camera.view, camera.proj, glm::mat4(1), glm::scale(glm::mat4(1.0f), glm::vec3(0, 10, 0)), shader);
		
		// UI stuff
		//sprite1.render(450, 200, -30);
		//fontSprite.render(300, 300, -40);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		//offScreenSprite.scale(0.5, 0.5);
		//offScreenSprite.renderFlippedDepth(200, 100, -20);
		crossHairSprite.render(400, 300, -20);
		
		err = glGetError();
#if true
		if (err != 0)
			AddLog("glError in loop: %d", err);
#endif
		SDL_GL_SwapWindow(window);
	}


	return 0;

}
