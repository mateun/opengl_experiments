#version 430 core

layout (binding = 0) uniform sampler2D sampler;
layout (binding = 1) uniform sampler2D shadowMap;
out vec4 color;
in vec2 fs_uvs;
in vec3 fs_normals;
in vec3 fs_pos;
in vec4 fs_pos_in_light;

layout (location = 0) uniform vec3 u_light_pos;

float calcShadowFactor() {
  vec3 pos_corr = vec3(fs_pos_in_light.xyz/ fs_pos_in_light.w) * vec3(0.5) + vec3(0.5);
  float shadow_factor = 1;
  if (texture(shadowMap, pos_corr.xy).r > pos_corr.z) {
    shadow_factor = 0.3;
  }
  
  return shadow_factor;
}

void main(void ) {
	
  float shadow_factor = calcShadowFactor();
  vec3 lightVector = normalize(u_light_pos - vec3(0, 0, 0));
  float diffuse = max(dot(fs_normals, lightVector), 0.1);
  
  color = texture(sampler, fs_uvs) * diffuse * shadow_factor;
  color.a = 1;
  
}
