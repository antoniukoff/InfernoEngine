#pragma once
#include <string>
#include <GLEW/glew.h>

using namespace std;
namespace Inferno {

	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();


		void compileShadersFromSource(const char* vertSource, const char* fragSource);

		void compileShaders(const string& vertFile, const string& fragFile);

		void linkShaders();

		GLuint getUniformLocation(const string& uniformName);

		void addAttribure(const char* attributeName);
		void use();
		void unuse();

		void dispose();
	private:
		int _numAttributes;

		void compileShader(const char* source, const std::string& name, GLuint id);
		GLuint _programID;

		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
	};

}