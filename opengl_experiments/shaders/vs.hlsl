#version 430 core

layout (location=0) in vec3 position;
layout (location = 1) in vec2 uvs;
layout (location = 2) in vec3 normals;

layout (location = 3) uniform mat4 mwv;
layout (location = 4) uniform mat4 mp;

out vec2 fs_uvs;
out vec3 fs_normals;
out vec3 fs_pos;

void main(void ) {
	gl_Position = mp * mwv  * (vec4(position, 1));
  fs_normals = vec3(mwv * vec4(normals, 0.0));
  fs_uvs = uvs;
  
  fs_pos = position;
}
