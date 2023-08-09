#version 130



in vec2 vertexPos;
in vec4 vertexColor;
in vec2 vertUV;

out vec4 fragmentColor;
out vec2 fragmentPosition;
out vec2 fragUV;

void main(){
	gl_Position.xy = vertexPos;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;

	fragmentPosition = vertexPos;

	fragmentColor = vertexColor;

	vec2 flippedUV = vec2(vertUV.x, 1 - vertUV.y);

	fragUV = flippedUV;
}
