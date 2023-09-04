#include "GLSLProgram.h"
#include <fstream>
#include "VladgineErrors.h"
#include <vector>
#include "IOManager.h"

namespace Vladgine {

	GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
	{
	}


	GLSLProgram::~GLSLProgram()
	{
	}

	void GLSLProgram::compileShadersFromSource(const char* vertSource, const char* fragSource)
	{
		_programID = glCreateProgram();

		_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (_vertexShaderID == 0) {
			fatalError("***vertex shader failed to be created***");
		}
		_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (_fragmentShaderID == 0) {
			fatalError("***fragment shader failed to be created***");
		}
		compileShader(vertSource, "Vertex shader", _vertexShaderID);
		compileShader(fragSource, "Fragment shader", _fragmentShaderID);
	}

	void GLSLProgram::compileShaders(const string& vertFile, const string& fragFile)
	{
		std::string vertSource;
		std::string fragSource;

		IOManager::reafFileToBuffer(vertFile, vertSource);
		IOManager::reafFileToBuffer(fragFile, fragSource);

		compileShadersFromSource(vertSource.c_str(), fragSource.c_str());
	}

	void GLSLProgram::linkShaders()
	{
		// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.


		// Attach our shaders to our program
		glAttachShader(_programID, _vertexShaderID);
		glAttachShader(_programID, _fragmentShaderID);

		// Link our program
		glLinkProgram(_programID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			vector<char> errorLog(maxLength);
			glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);



			// We don't need the program anymore.
			glDeleteProgram(_programID);
			// Don't leak shaders either.
			glDeleteShader(_vertexShaderID);
			glDeleteShader(_fragmentShaderID);

			// Use the infoLog as you see fit.

			printf("%s\n", &errorLog[0]);
			fatalError("shaders failed to link!");
		}

		// Always detach shaders after a successful link.
		glDetachShader(_programID, _vertexShaderID);
		glDetachShader(_programID, _fragmentShaderID);
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);
	}

	GLuint GLSLProgram::getUniformLocation(const string& uniformName)
	{
		GLint location = glGetUniformLocation(_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX) {
			fatalError("Uniform " + uniformName + " not found in shader");
		}
		return location;
	}

	void GLSLProgram::addAttribure(const string& attributeName)
	{
		glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
	}

	void GLSLProgram::use()
	{
		glUseProgram(_programID);
		for (int i = 0; i < _numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse()
	{
		glUseProgram(0);
		for (int i = 0; i < _numAttributes; i++) {
			glDisableVertexAttribArray(i);
		}
	}

	void GLSLProgram::dispose()
	{
		if(_programID) glDeleteProgram(_programID);
	}

	void GLSLProgram::compileShader(const char* source, const std::string& name, GLuint id)
	{
		

		glShaderSource(id, 1, &source, nullptr);

		glCompileShader(id);

		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
			vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(id);

			printf("%s\n", &errorLog[0]);
			fatalError("shader " + name + " failed to compile");
		}
	}

}