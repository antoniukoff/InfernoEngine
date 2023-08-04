#include "Sprite.h"

Sprite::Sprite()
{
	_vboID = 0;
}

Sprite::~Sprite()
{
	if (_vboID != 0) {
		glDeleteBuffers(1, &_vboID);
	}
}

void Sprite::init(float x, float y, float width, float height)
{
	_x = x;
	_y = y;
	_width = width;
	_height = height;

	

	if (_vboID == 0) {
		glGenBuffers(1, &_vboID);
	}

	float  vertexData[12] = {
		//FIRST TRIANGLE\\
		// vertex 1
		x + width,
		y + width,
		// vertex 2
		x,
		y + height,
		// vertex 3
		x,
		y,
		//SECOND TRIANGLE\\
		// vertex 1
		x,
		y,
		// vertex 2
		x + width,
		y,
		// vertex 3
		x + width,
		y + height
	};

	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Sprite::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
