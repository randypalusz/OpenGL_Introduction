#version 410 core
layout(location = 0) out vec4 color;

in vec2 texCoords;

uniform vec4 u_Color;
uniform float u_enableBlueGradient;
uniform sampler2D texture1;

void main()
{
  //color = vec4(u_Color.r, u_Color.g, u_Color.b, u_Color.a);

  if (u_enableBlueGradient == 1.0f) {
    color = texture(texture1, texCoords) * vec4(u_Color.r, u_Color.g, clamp((gl_FragCoord.x/2560)*2, 0.0, 1.0), u_Color.a);
  }
  else {
    color = texture(texture1, texCoords);
  }

}