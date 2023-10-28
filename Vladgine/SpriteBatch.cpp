#include "SpriteBatch.h"
#include <algorithm>


namespace Vladgine {


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

	void SpriteBatch::createRenderBatches()
	{

		// here we create batches for rendering. now it only is going to create render batches based on the texture so it is crucial to sort by texture before creating render batches to avoid multiple render batches for the same texture
		//TODO: implement depth batching. to properly organize the batches

		//allocating the total amount of memory for vertices vector to hold an appropriate number of vertices per total amount of glyphs

		std::vector<Vertex> vertices;

		vertices.resize(_glyphPointers.size() * 6);
	
		//return if no glyphs in the vector
		if (_glyphPointers.empty()) return;

		// offset used to specify the amount of verices between the render batches to minimize draw calls 
		int offset = 0;
		int cv = 0;// current vertex
		// puses back only the paramaters of the render batch constructor and sets up the first render batch
		_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);
		// upload the vertices data of the first glyph into the vertex vector
		vertices[cv++] = _glyphPointers[0]->topLeft;
		vertices[cv++] = _glyphPointers[0]->bottomLeft;
		vertices[cv++] = _glyphPointers[0]->bottomRight;
		vertices[cv++] = _glyphPointers[0]->bottomRight;
		vertices[cv++] = _glyphPointers[0]->topRight;
		vertices[cv++] = _glyphPointers[0]->topLeft;
		//increment offset by 6 vertices(offset used inly for the new render batch)
		offset += 6;
		// loop through all the glyphs
		for (uint32_t cg = 1; cg < _glyphs.size(); cg++) {
			// if the texture of the previous glyph is not the same as of the current one - create new render batch
			if (_glyphPointers[cg]->texture != _glyphPointers[cg - 1]->texture) {
				_renderBatches.emplace_back(offset, 6, _glyphPointers[cg]->texture);
			}
			// else increase the number of vertecies of the render batch to render
			else {
				_renderBatches.back().numVertices += 6;
			}
			//store the vertecies of subsequent glyphs in the vector
			vertices[cv++] = _glyphPointers[cg]->topLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomLeft;
			vertices[cv++] = _glyphPointers[cg]->bottomRight;
			vertices[cv++] = _glyphPointers[cg]->bottomRight;
			vertices[cv++] = _glyphPointers[cg]->topRight;
			vertices[cv++] = _glyphPointers[cg]->topLeft;
			//increment offset by 6 vertices
			offset += 6;
		}
		// upload the data the data to the gpu
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);


	}

	void SpriteBatch::createVertexArray()
	{
		// creates a vertex array object - storing the configuration
		//in the VAO that saves time for open gl to set up the settings 
		//about the vertex data to  render

		if (_vao == 0) {
			glGenVertexArrays(1, &_vao);
		}

		glBindVertexArray(_vao);

		if (_vbo == 0) {
			glGenBuffers(1, &_vbo);
		}
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//this is a color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//uv attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

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