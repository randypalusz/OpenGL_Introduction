#version 410 core

layout(location=0) in vec3 a_position;
layout(location=1) in vec2 a_texCoords;

out vec2 texCoords;

uniform mat4 u_model;

void main(){
  gl_Position = u_model * vec4(a_position, 1.0f);
  texCoords = a_texCoords;
}