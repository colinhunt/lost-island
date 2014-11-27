
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <stdexcept>

#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;


#ifndef SHADER_H
#define SHADER_H



struct shader
{
	GLuint vertShaderHandle, fragShaderHandle, programHandle;

	~shader()
	{
		glDeleteShader(vertShaderHandle);
		glDeleteShader(fragShaderHandle);
		glDeleteProgram(programHandle);
	}


	void setShaders(char*  vertShaderFileName, char* fragShaderFileName)
	{
		// WRITE ALL OF YOUR QUESTION 1 CODE IN HERE! 
		// (it should be approximately 20 lines of code)
		// (Note: you can use the utility function "readTxtFile" to load the 
		// shader code from a text file into a C_string)


		//*** Step 1: load and compile the vertex and fragment shaders ***//

		// cout << "Setting " << vertShaderFileName << " and " << fragShaderFileName << endl;

			// create shader handles
		vertShaderHandle = glCreateShader(GL_VERTEX_SHADER); 
		fragShaderHandle = glCreateShader(GL_FRAGMENT_SHADER); 
	
			// read shader programs into c strings
		char* vertShaderSource(readTxtFile(vertShaderFileName));
		char* fragShaderSource(readTxtFile(fragShaderFileName));
		char* sharedVertSource(readTxtFile("student_code/shared.vert"));

		char* vertShaderSources[] = {sharedVertSource, vertShaderSource};
		char* fragShaderSources[] = {fragShaderSource};
	
			// set strings as shader sources
		glShaderSource(vertShaderHandle, 2, (const char**) vertShaderSources, NULL);
		glShaderSource(fragShaderHandle, 1, (const char**) fragShaderSources, NULL);

			// free shader source memory (no longer needed once loaded into GPU)
		if (vertShaderSource != NULL) free(vertShaderSource);
		if (fragShaderSource != NULL) free(fragShaderSource);
		if (sharedVertSource != NULL) free(sharedVertSource);
	
			// compile shader programs
		glCompileShader(vertShaderHandle);
		glCompileShader(fragShaderHandle);

			// verify
		if (!glVerifyHandle(vertShaderHandle))
			throw runtime_error("Failed to compile vertex shader");
		if (!glVerifyHandle(fragShaderHandle))
			throw runtime_error("Failed to compile fragment shader");


		//*** Step 2: link and install the shading program ***//
	
			// create program handle
		programHandle = glCreateProgram(); 
	
			// attach shaders
		glAttachShader(programHandle, vertShaderHandle);
		glAttachShader(programHandle, fragShaderHandle);
	
			// link program 
		glLinkProgram(programHandle);

			// verify
		if (!glVerifyHandle(programHandle))
			throw runtime_error("Failed to link program");

			// detach shaders (no longer needed once linked into program)
		glDetachShader(programHandle, vertShaderHandle);
		glDetachShader(programHandle, fragShaderHandle);

			// install program into rendering state
		glUseProgram(programHandle);

		glClearColor(1.0, 1.0, 1.0, 0.0);
	} // NO MORE CHANGES PAST THIS POINT!


	char* readTxtFile(char* fileName) // reads a text file into a C string
	{
		ifstream infile(fileName, ios::binary);
		if (!infile) throw runtime_error(string("Cannot open file ") + fileName);
		infile.seekg(0, ios::end);
		size_t length(infile.tellg());
		char* data(new char[length + 1]);
		infile.seekg(0, ios::beg);
		infile.read(data, length);
		data[length] = '\0'; // end of string
		infile.close();
		return data;
	}


	int glVerifyHandle(GLuint handle) // verifies program attached to handle
	{
		int logLen, checkLen;
		glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &logLen);
		char* infoLog(new char[logLen]);
		glGetInfoLogARB(handle, logLen, &checkLen, &infoLog[0]);
		if (checkLen > 0) {
			cerr << "Error:" << endl << infoLog << endl;
			return 0; // failed
		}
		return 1; // okay
	}

};

#endif
