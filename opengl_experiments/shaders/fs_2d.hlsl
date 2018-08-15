#version 430 core

uniform sampler2D sampler;
out vec4 color;
in vec2 fs_uvs;

void main(void ) {
  vec3 col = texture(sampler, fs_uvs).rgb;
  if (col.r == 0 && col.g == 0 && col.b == 0)
    color = vec4(col, 0);
  else
    color = vec4(col, 1);
  
  
  
}
