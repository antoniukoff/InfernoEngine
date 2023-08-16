#version 130

in vec4 fragmentColor;
in vec2 fragmentPosition;
in vec2 fragUV;

out vec4 fragColor;

uniform sampler2D mySampler;

void main() {

vec4 textureColor= texture(mySampler, fragUV);



fragColor =  fragmentColor * textureColor;
  }