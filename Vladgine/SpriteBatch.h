#pragma once
#include <GLEW/glew.h>
#include "Vertex.h"
#include <GLEW/glm/glm.hpp>
#include <vector>

namespace Vladgine {
	
	enum class GlyphSortType{ NONE, FRONT_TO_BACK, BACK_TO_FRONT, TEXTURE};

	class Glyph {
	public:
		Glyph():depth(0), texture(0) {};
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, ColorRGB8 color);
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, ColorRGB8 color, float angle);

		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
		//float m_angle;
	private:
		glm::vec2 rotatePoint(glm::vec2 pos, float angle);

	};

	class RenderBatch {
	public: 
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : 
					offset(Offset), numVertices(NumVertices), texture(Texture){}

		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	class SpriteBatch
	{
	public: 
		SpriteBatch();
		~SpriteBatch();

		void init();
		void dispose();

		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void end();

		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, ColorRGB8 color);
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, ColorRGB8 color, float angle);
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, ColorRGB8 color, const glm::vec2& dir);

		void renderBatch();

	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GLuint _vbo;
		GLuint _vao;
		GlyphSortType _sortType;

		std::vector<Glyph*> _glyphPointers;//for sorting
		std::vector<Glyph> _glyphs; // actual glyphs
		std::vector<RenderBatch> _renderBatches;

		

	};

}