#pragma once

#include <GLEW/glew.h>

namespace Inferno {


    struct Position {
        float x, y;
    };


    struct ColorRGB8 {
        GLubyte r, g, b, a;

        // Default constructor initializes color to transparent black.
        ColorRGB8() : r(0), g(0), b(0), a(0) {}

        ColorRGB8(GLubyte R, GLubyte G, GLubyte B, GLubyte A)
            : r(R), g(G), b(B), a(A) {}
    };

    struct UV {
        float u, v;
    };

    // Structure Vertex with position, color, and texture data.
    struct Vertex {
        Position position = { 0, 0 };   //< Position of the vertex 
        ColorRGB8 color = { 0, 0, 0, 0 };  //< Color of the vertex 
        UV uv;  //< Texture coordinates of the vertex.

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
