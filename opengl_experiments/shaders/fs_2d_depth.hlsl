#version 430 core

layout (binding = 0) uniform sampler2D sampler;
out vec4 color;
in vec2 fs_uvs;

// 'colorImage' is a sampler2D with the depth image
// read from the current depth buffer bound to it.
//
float LinearizeDepth(in vec2 uv)
{
    float zNear = 0.1;    // TODO: Replace by the zNear of your perspective projection
    float zFar  = 5000.0; // TODO: Replace by the zFar  of your perspective projection
    float depth = texture2D(sampler, uv).x;
    //return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
    //return (depth * -2/(zFar - zNear) - (zFar + zNear) / (zFar - zNear);
    return depth + 0.3f;
}


void main(void ) {
   float c = LinearizeDepth(fs_uvs);
   color = vec4(c, c, c, 1.0);

  
  
}
