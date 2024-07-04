//Created by IDz on 20240116
#version 410 core

layout (location = 0) in vec3 position;

out vec4 color;
uniform mat4 mvp;

void main() 
{
  gl_Position = mvp * vec4(position, 1.0);
  color=normalize(vec4(0.0, 1.0, 0.0, 1.0)+abs(vec4(position, 0.0))*0.8*vec4(1.0, 0.0, 0.0, 1.0));
}
