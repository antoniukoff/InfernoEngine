#pragma once
#include <glew.h>
namespace Vladgine {

	struct Position {
		float x, y;
	};

	struct ColorRGB8 {
		ColorRGB8() :r(0), g(0), b(0), a(0) {}
		ColorRGB8(GLubyte R, GLubyte G, GLubyte B,GLubyte A) :
			r(R), g(G), b(B), a(A) {}


		GLubyte r, g, b, a;

	};

	struct UV {
		float u, v;
	};

	// here i compose a vertex that has a position color and uv for texture mapping
	struct Vertex {
		Position position;

		ColorRGB8 color;

		UV uv;

		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;

		}

		void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			color.r = r;
			color.g = g;
			color.b = b;
			color.a = a;
		}
		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}
	};
}