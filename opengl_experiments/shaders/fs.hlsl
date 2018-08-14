#version 430 core

uniform sampler2D sampler;
out vec4 color;
in vec2 fs_uvs;
in vec3 fs_normals;
in vec3 fs_pos;

void main(void ) {
	//color = vec4(1,1,0.5,1);
  //color = texture(sampler, fs_uvs);
  
  vec3 lightPos = vec3(0, 2, 5);
  vec3 lightVector = normalize(lightPos - fs_pos);
  float diffuse = max(dot(fs_normals, lightVector), 0.1);
  
  color = texture(sampler, fs_uvs) * diffuse;
  //color = vec4(fs_normals, 1);
  //color = vec4(0.5, 0.5,0.5,1) * diffuse;
}
