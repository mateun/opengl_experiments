#version 430 core

layout (location=0) in vec3 position;
layout (location = 1) in vec2 uvs;

out vec2 fs_uvs;

void main(void ) {
	//gl_Position = vec4(0,0,0.5,1);
  gl_Position = vec4(position, 1);
  fs_uvs = uvs;
}
