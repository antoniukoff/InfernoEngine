#include "GLSLProgram.h"
#include <fstream>
#include"Errors.h"
#include <vector>

namespace Vladgine {

	GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0)
	{
	}


	GLSLProgram::~GLSLProgram()
	{
	}

	void GLSLProgram::compileShaders(const string& vertFile, const string& fragFile)
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
		compileShader(vertFile, _vertexShaderID);
		compileShader(fragFile, _fragmentShaderID);
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

	void GLSLProgram::compileShader(const string& filePath, GLuint id)
	{
		fstream vertexFile(filePath);
		if (vertexFile.fail()) {
			perror(filePath.c_str());
			fatalError("failed to open " + filePath);
		}

		string fileContents = "";
		string line;
		while (getline(vertexFile, line)) {
			fileContents += line + "\n";
		}

		vertexFile.close();

		const char* contentsPtr = fileContents.c_str();

		glShaderSource(id, 1, &contentsPtr, nullptr);

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
			fatalError("shader " + filePath + " failed to compile");
		}
	}

}