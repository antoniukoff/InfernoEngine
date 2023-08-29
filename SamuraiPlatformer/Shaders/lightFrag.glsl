#version 130

in vec4 fragmentColor;
in vec2 fragmentPosition;
in vec2 fragUV;

out vec4 fragColor;



void main() {

float distance = length(fragUV);

fragColor =  vec4(fragmentColor.rgb, fragmentColor.a * (pow(0.01,distance) - 0.01));

}