#version 430 core

layout (location=0) in vec3 position;

void main(void ) {
	//gl_Position = vec4(0,0,0.5,1);
  gl_Position = vec4(position, 1);
}
