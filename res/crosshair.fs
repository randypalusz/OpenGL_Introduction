#version 410 core
layout(location = 0) out vec4 color;

in vec2 texCoords;

uniform vec4 u_Color;
uniform sampler2D texture1;

void main()
{
    if(texture2D(texture1, texCoords).a == 0.0f) {
      discard;
    }
    color = texture(texture1, texCoords) * u_Color;
    // color = texture(texture1, texCoords) * vec4(1.0f, 0.0f, 0.0f, 0.5f);
}