// CMPUT 411 Assignment 4

// Original author: Nathaniel Rossol
// Modified by: Dale Schuurmans

#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdlib.h>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "student_code/shader.h"

using namespace std;


struct Material
{
	bool bAlphaBlended;
	string name;
	shader shad;

	bool bUseTimeVariable;
	float currentTime;
	unsigned int texture_0;
	unsigned int texture_1;
	unsigned int texture_2;
	unsigned int texture_3;
	unsigned int texture_4;
	unsigned int texture_5;
	unsigned int texture_6;
	unsigned int texture_7;

	bool bUniformVariablesCacheNeedsBuilding;

	int cachedTimeVariableLocation;
	int cachedUniformLocationTexture0;
	int cachedUniformLocationTexture1;
	int cachedUniformLocationTexture2;
	int cachedUniformLocationTexture3;
	int cachedUniformLocationTexture4;
	int cachedUniformLocationTexture5;
	int cachedUniformLocationTexture6;
	int cachedUniformLocationTexture7;


	Material(char* vertexShaderFilename, char* fragmentShaderFilename,
           bool alpha_blended = false, bool use_time = false,
           unsigned int texture0 = 0, unsigned int texture1 = 0,
           unsigned int texture2 = 0, unsigned int texture3 = 0,
           unsigned int texture4 = 0, unsigned int texture5 = 0,
           unsigned int texture6 = 0, unsigned int texture7 = 0);
	

	// Binds the shader for rendering
	void bindMaterial();
	

	// Unbinds shader
	void unbindMaterial()
	{
		glUseProgram(0);
	}


	// Get the shader's internal ID number
	unsigned int getID()
	{
		return shad.programHandle;
	}


	// Check to see if this material has been flagged as alphablended
	bool isAlphaBlended()
	{
		return bAlphaBlended;
	}


	// Checks to see if this material is set to use the time variable
	bool usesTimeVariable()
	{
		return bUseTimeVariable;
	}


	// Updates the time of the shader
	void updateTime(float current_time)
	{
		currentTime = current_time;
	}


	void buildUniformVariablesCache();

};

#endif
