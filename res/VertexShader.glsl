#version 410 core

layout(location=0) in vec2 a_position;
layout(location=1) in vec2 a_texCoords;

out vec2 texCoords;

uniform mat4 u_transformMatrix;

void main(){
  gl_Position = vec4(a_position, 0.0, 1.0) * u_transformMatrix;
  texCoords = a_texCoords;
}