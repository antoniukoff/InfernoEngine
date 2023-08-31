#include "DebugRenderer.h"

const float PI = 3.14159265359f;

namespace {
	const char* VERT_SOURCE = R"(#version 130
//The vertex shader operates on each vertex

//input data from the VBO. Each vertex is 2 floats
in vec2 vertexPosition;
in vec4 vertexColor;

out vec2 fragmentPosition;
out vec4 fragmentColor;

uniform mat4 P;

void main() {
    //Set the x,y position on the screen
    gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
    //the z position is zero since we are in 2D
    gl_Position.z = 0.0;
    
    //Indicate that the coordinates are normalized
    gl_Position.w = 1.0;
    
    fragmentPosition = vertexPosition;
    
    fragmentColor = vertexColor;
})";

	const char* FRAG_SOURCE = R"(#version 130
//The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;

//This is the 3 component float vector that gets outputted to the screen
//for each pixel.
out vec4 color;

void main() {

    color = fragmentColor;
})";
}

Vladgine::DebugRenderer::DebugRenderer()
{
	//empty 
}

Vladgine::DebugRenderer::~DebugRenderer()
{
	dispose();
}

void Vladgine::DebugRenderer::init()
{
	// shader init
	m_program.compileShadersFromSource(VERT_SOURCE, FRAG_SOURCE);
	m_program.addAttribure("vertexPosition");
	m_program.addAttribure("vertexColor");
	m_program.linkShaders();


	// set up buffers
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);



	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));



	glBindVertexArray(0);
}

void Vladgine::DebugRenderer::end()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glBufferData(GL_ARRAY_BUFFER, m_verts.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_verts.size() * sizeof(DebugVertex), m_verts.data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indeces.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indeces.size() * sizeof(GLuint), m_indeces.data());
	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_numElements = m_indeces.size();
	m_indeces.clear();
	m_verts.clear();
}

void Vladgine::DebugRenderer::drawLine(const glm::vec2& a, const glm::vec2& b, const ColorRGB8& color)
{
	int i = m_verts.size();
	m_verts.resize(m_verts.size() + 2);
	m_verts[i].position = a;
	m_verts[i].color = color;
	m_verts[i + 1].position = b;
	m_verts[i + 1].color = color;

	m_indeces.push_back(i);
	m_indeces.push_back(i + 1);
}


glm::vec2 rotatePoint(glm::vec2 pos, float angle)
{
	glm::vec2 newv;
	newv.x = pos.x * cos(angle) - pos.y * sin(angle);
	newv.y = pos.x * sin(angle) + pos.y * cos(angle);
	return newv;
}

void Vladgine::DebugRenderer::drawBox(const glm::vec4& destRect, const ColorRGB8& color, float angle)
{
	int i = m_verts.size();
	m_verts.resize(m_verts.size() + 4);

	glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

	//get points centered at the origin
	glm::vec2 tl(-halfDims.x, halfDims.y);
	glm::vec2 bl(-halfDims.x, -halfDims.y);
	glm::vec2 br(halfDims.x, -halfDims.y);
	glm::vec2 tr(halfDims.x, halfDims.y);

	glm::vec2 positionOffset(destRect.x, destRect.y);

	//rotate the points
	m_verts[i].position = rotatePoint(tl, angle) + halfDims + positionOffset;
	m_verts[i + 1].position = rotatePoint(bl, angle) + halfDims + positionOffset;
	m_verts[i + 2].position = rotatePoint(br, angle) + halfDims + positionOffset;
	m_verts[i + 3].position = rotatePoint(tr, angle) + halfDims + positionOffset;
															    
	for (int j = i; j < i + 4; j++) {
		m_verts[j].color = color;
	}

	m_indeces.reserve(m_indeces.size() + 8);

	m_indeces.push_back(i);
	m_indeces.push_back(i + 1);

	m_indeces.push_back(i + 1);
	m_indeces.push_back(i + 2);

	m_indeces.push_back(i + 2);
	m_indeces.push_back(i + 3);

	m_indeces.push_back(i + 3);
	m_indeces.push_back(i);
}

void Vladgine::DebugRenderer::drawCircle(const glm::vec2& center, const ColorRGB8& color, float radius)
{
	//set up vertices
	static const int NUM_VERTICES = 100;
	
	int start = m_verts.size();
	m_verts.resize(m_verts.size() + NUM_VERTICES);
	for (int i = 0; i < NUM_VERTICES; i++) {
		float angle = ((float)i / NUM_VERTICES) * PI * 2.0f;
		m_verts[start + i].position.x = cos(angle) * radius + center.x;
		m_verts[start + i].position.y = sin(angle) * radius + center.y;
		m_verts[start + i].color = color;
	}

	//set up for indexed drawinh
	m_indeces.reserve(m_indeces.size() + NUM_VERTICES * 2);
	for (int i = 0; i < NUM_VERTICES - 1; i++) {
		m_indeces.push_back(start + i);
		m_indeces.push_back(start + i + 1);
	}
	m_indeces.push_back(start + NUM_VERTICES - 1);
	m_indeces.push_back(start);
}

void Vladgine::DebugRenderer::render(const glm::mat4& projectionMatrix, float lineWidth)
{
	m_program.use();

	GLint pUniform = m_program.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	glLineWidth(lineWidth);
	glBindVertexArray(m_vao);

	glDrawElements(GL_LINES, m_numElements, GL_UNSIGNED_INT, 0);


	glBindVertexArray(0);
	m_program.unuse();
}

void Vladgine::DebugRenderer::dispose()
{
	if (m_vao) {
		glDeleteVertexArrays(1, &m_vao);
	}
	if (m_vbo) {
		glDeleteBuffers(1, &m_vbo);
	}
	if (m_ibo) {
		glDeleteBuffers(1, &m_ibo);
	}
	m_program.dispose();
}
