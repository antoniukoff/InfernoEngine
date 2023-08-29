#version 130


// incoming vertecies
in vec2 vertexPos;
in vec4 vertexColor;
in vec2 vertUV;

// passing to the frag shader
out vec4 fragmentColor;
out vec2 fragmentPosition;
out vec2 fragUV;

uniform mat4 P;

void main(){
	gl_Position.xy = (P * vec4(vertexPos, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;

	fragmentPosition = vertexPos;

	fragmentColor = vertexColor;

	

	fragUV = vertUV;
}
