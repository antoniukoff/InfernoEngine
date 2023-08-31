#pragma once
#include <glm/glm.hpp>
#include "Vertex.h"
#include <vector>
#include "GLSLProgram.h"

namespace Vladgine {
	class DebugRenderer
	{
	public:

		DebugRenderer();
		~DebugRenderer();
		void init();
		void end();
		void drawLine(const glm::vec2& a, const glm::vec2& b, const ColorRGB8& color);
		void drawBox(const glm::vec4& destRect, const ColorRGB8& color, float angle);
		void drawCircle(const glm::vec2& center, const ColorRGB8& color, float radius);
		void render(const glm::mat4& projectionMatrix, float lineWidth);
		void dispose();

		struct  DebugVertex
		{	
			glm::vec2 position;
			Vladgine::ColorRGB8 color;
		};

	private:
		Vladgine::GLSLProgram m_program;
		std::vector<DebugVertex> m_verts;
		GLuint m_vbo = 0, m_vao = 0, m_ibo = 0;
		std::vector <GLuint> m_indeces;
		int m_numElements = 0;
	};

}