#version 410 core
layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
  //color = vec4(u_Color.r, u_Color.g, u_Color.b, u_Color.a);
  color = vec4(u_Color.r, u_Color.g, gl_FragCoord.x/2560 , u_Color.a);
}