#include "SpriteBatch.h"
#include <algorithm>


namespace Inferno {


	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, ColorRGB8 color) :
		texture(Texture), depth(Depth) {

		topLeft.color = color;
		topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x, destRect.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

	}

	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, ColorRGB8 color, float angle) :
		texture(Texture), depth(Depth) {

		glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

		//get points centered at the origin
		glm::vec2 tl(-halfDims.x, halfDims.y);
		glm::vec2 bl(-halfDims.x, -halfDims.y);
		glm::vec2 br(halfDims.x, -halfDims.y);
		glm::vec2 tr(halfDims.x, halfDims.y);

		//rotate the points
		tl = rotatePoint(tl, angle) + halfDims;
		bl = rotatePoint(bl, angle) + halfDims;
		br = rotatePoint(br, angle) + halfDims;
		tr = rotatePoint(tr, angle) + halfDims;
		
		topLeft.color = color;
		topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

	}

	glm::vec2 Glyph::rotatePoint(glm::vec2 pos, float angle)
	{
		glm::vec2 newv = glm::vec2{};
		newv.x = pos.x * cos(angle) - pos.y * sin(angle);
		newv.y = pos.x * sin(angle) + pos.y * cos(angle);
		return newv;
	}

	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0), _sortType(GlyphSortType::TEXTURE)
	{
	}

	SpriteBatch::~SpriteBatch()
	{
	}

	void SpriteBatch::init()
	{
		createVertexArray();
	}

	void SpriteBatch::dispose()
	{
		if (_vao != 0) {
			glDeleteVertexArrays(1, &_vao);
		}
		if (_vbo != 0) {
			glDeleteBuffers(1, &_vbo);
		}
	}

	// setting up our sprite batch before drawing we are coing to clear the allocated memory
	//for render batches and glyphs and are going to delete the glyphs on the heap
	void SpriteBatch::begin(GlyphSortType sortType/* = GlyphSortType::TEXTURE*/)
	{
		_sortType = sortType;
		_renderBatches.clear();
		_glyphs.clear();
	}

	void SpriteBatch::end()
	{
		_glyphPointers.resize(_glyphs.size());
		for (uint32_t i = 0; i < _glyphs.size(); i++) {
			_glyphPointers[i] = &_glyphs[i];
		}
		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, ColorRGB8 color)
	{
		// creating a sprite and setting up its attributes(vertexpos, vertexuv, vertex color, depth ,texture)
		//to the passed in parameters
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, ColorRGB8 color, float angle)
	{
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, ColorRGB8 color, const glm::vec2& dir)
	{
		const glm::vec2 rightDir(1.0f, 0.0f);
		float angle = acos(glm::dot(rightDir, dir));
		if (dir.y < 0.0f) angle = -angle;
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	//renders the batches
	void SpriteBatch::renderBatch()
	{
		// bind the vao we created in the createArrayBuffer()
		glBindVertexArray(_vao);
		// loop through all the render batches
		for (uint32_t i = 0; i < _renderBatches.size(); i++) {
			//bind texture of the current array and specifies the beginning of that render batch 
			//as well as number of vertices to render until the next batch 
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches() {

		// This method organizes the glyphs into render batches. 
		// Render batches are grouped based on texture to minimize texture swapping during rendering.
		// Note: Currently, it sorts only by texture.

		// Allocate memory for a vertices vector to hold vertex data for all the glyphs.
		// Each glyph requires 6 vertices (two triangles).
		std::vector<Vertex> vertices;
		vertices.resize(_glyphPointers.size() * 6);

		// Return early if there are no glyphs to process.
		if (_glyphPointers.empty()) return;

		// The 'offset' tracks the number of vertices processed, used to determine where each batch starts.
		int offset = 0;
		int cv = 0; // 'cv' is the current vertex index in the vertices vector.

		// Initialize the first render batch based on the first glyph's texture.
		_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);
		// Load the vertex data of the first glyph into the vertices vector.
		vertices[cv++] = _glyphPointers[0]->topLeft;
		vertices[cv++] = _glyphPointers[0]->bottomLeft;
		vertices[cv++] = _glyphPointers[0]->bottomRight;
		vertices[cv++] = _glyphPointers[0]->bottomRight;
		vertices[cv++] = _glyphPointers[0]->topRight;
		vertices[cv++] = _glyphPointers[0]->topLeft;
		offset += 6; // Increment the offset for the next batch.

		// Process each glyph and organize them into batches.
		for (uint32_t cg = 1; cg < _glyphs.size(); cg++) {
			// Check if the current glyph has a different texture from the previous one. If so, start a new batch.
			if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture) {
				_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture);
			}
			else {
				// Otherwise, continue adding to the current batch.
				_renderBatches.back().numVertices += 6;
			}
			// Load the current glyph's vertex data into the vertices vector.
			vertices[cv++] = _glyphPointers[cg]->topLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomRight;
			vertices[cv++] = _glyphPointers[cg]->bottomRight;
			vertices[cv++] = _glyphPointers[cg]->topRight;
			vertices[cv++] = _glyphPointers[cg]->topLeft;
			offset += 6; // Increment the offset for the next batch.
		}

		// Upload the vertices data to the GPU for rendering.
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		// 'Orphan' the buffer: Allocate new memory for the buffer to improve performance.
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		// Copy the vertices data into the buffer.
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	void SpriteBatch::createVertexArray()
	{
		// This function sets up a Vertex Array Object (VAO) and a Vertex Buffer Object (VBO).
		// The VAO stores the configuration of the vertex attributes, and the VBO stores the actual vertex data.
		// This setup is necessary for efficient rendering in OpenGL.
		
		// Generate and bind a Vertex Array Object if it has not been created yet.
		if (_vao == 0) {
			glGenVertexArrays(1, &_vao);
		}
		glBindVertexArray(_vao);

		// Generate and bind a Vertex Buffer Object if it has not been created yet.
		// This buffer will store our vertex data.
		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		// Enable the vertex attribute arrays. 
		// Here, we are enabling three attributes: position, color, and UV coordinates.
		glEnableVertexAttribArray(0); // Position attribute
		glEnableVertexAttribArray(1); // Color attribute
		glEnableVertexAttribArray(2); // UV attribute

		// Specify the layout of the vertex data.
		// The first argument corresponds to the attribute index (as enabled above).
		// The next arguments specify the type, size, and offset of each attribute in the vertex data.

		// Position attribute pointer: 2 floats (x, y)
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

		// Color attribute pointer: 4 bytes (RGBA), normalized to the range [0, 1]
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

		// UV attribute pointer: 2 floats (u, v)
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		// Unbind the VAO to prevent accidental modification.
		glBindVertexArray(0);

	}

	// sort teh glyphs either by depth(front to back or back to front) or by the texture
	void SpriteBatch::sortGlyphs()
	{
		switch (_sortType) {
			case GlyphSortType::BACK_TO_FRONT :
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront);
				break;
			case GlyphSortType::FRONT_TO_BACK:
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
				break;
			case GlyphSortType::TEXTURE :
				std::stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
				break;
		}
		
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
	{
		return (a->depth < b->depth);
	}

	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
	{
		return (a->depth > b->depth);
	}

	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b)
	{
		return (a->texture < b->texture);
	}

}