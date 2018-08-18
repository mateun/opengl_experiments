#version 430 core

uniform sampler2D sampler;
out vec4 color;
in vec2 fs_uvs;
in vec3 fs_normals;
in vec3 fs_pos;

layout (location = 0) uniform vec3 u_light_pos;

void main(void ) {
	
  //vec3 lightVector = normalize((u_light_pos) * -1);
  vec3 lightVector = normalize(u_light_pos - vec3(0, 0, 0));
  float diffuse = max(dot(fs_normals, lightVector), 0.1);
  
  color = texture(sampler, fs_uvs) * diffuse;
  color.a = 1;
  
}
