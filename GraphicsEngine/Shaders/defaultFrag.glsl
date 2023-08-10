#version 130

in vec4 fragmentColor;
in vec2 fragmentPosition;
in vec2 fragUV;

out vec4 fragColor;

uniform float time;
uniform sampler2D mySampler;

void main() {

vec4 textureColor= texture(mySampler, fragUV);



fragColor =  vec4(fragmentColor.r * (sin(fragmentPosition.y * 4.0 + time) + 2.0) * 0.5, 
				  fragmentColor.g * (cos(fragmentPosition.y * 8.0 + time) + 1.0) * 0.5,
				  fragmentColor.b * (cos(fragmentPosition.y * 2.0 + time) + 1.0) * 0.5,
				  fragmentColor.a) * textureColor;
  }