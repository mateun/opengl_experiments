#version 430 core

uniform sampler2D sampler;
out vec4 color;
in vec2 fs_uvs;

void main(void ) {
  vec3 col = texture(sampler, fs_uvs).rgb;
  color = vec4(col, col.r);
  
  
  
}
