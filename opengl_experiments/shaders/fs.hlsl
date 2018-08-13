#version 430 core

uniform sampler2D sampler;
out vec4 color;
in vec2 fs_uvs;

void main(void ) {
	//color = vec4(1,1,0.5,1);
  color = texture(sampler, fs_uvs);
}
