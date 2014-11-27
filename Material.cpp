// CMPUT 411 Assignment 4

// Original author: Nathaniel Rossol
// Modified by: Dale Schuurmans

#include "Material.h"

using namespace std;


Material::Material(char* vertexShaderFilename, char* fragmentShaderFilename, 
                   bool alpha_blended, bool use_time,
                   unsigned int tex0, unsigned int tex1,
                   unsigned int tex2, unsigned int tex3,
                   unsigned int tex4, unsigned int tex5,
                   unsigned int tex6, unsigned int tex7)
{
	name = (string) fragmentShaderFilename;
	name.erase(name.end()-5, name.end()); // remove filename extension
	bAlphaBlended = alpha_blended;

	currentTime = 0;
	bUseTimeVariable = use_time;
	
	texture_0 = tex0;
	texture_1 = tex1;
	texture_2 = tex2;
	texture_3 = tex3;
	texture_4 = tex4;
	texture_5 = tex5;
	texture_6 = tex6;
	texture_7 = tex7;

	shad.setShaders(vertexShaderFilename, fragmentShaderFilename);
	bUniformVariablesCacheNeedsBuilding = true;
}


//Binds the shader for rendering
void Material::bindMaterial()
{
	glUseProgram(shad.programHandle);

	if (bUniformVariablesCacheNeedsBuilding)
		buildUniformVariablesCache();

	if (bUseTimeVariable && cachedTimeVariableLocation != -1)
		glUniform1f(cachedTimeVariableLocation, currentTime);

	if (texture_0 != 0) {
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(cachedUniformLocationTexture0, 0);
		glBindTexture(GL_TEXTURE_2D, texture_0);
	}
	if (texture_1 != 0) {
		glActiveTexture(GL_TEXTURE1);
		glUniform1i(cachedUniformLocationTexture1, 1);
		glBindTexture(GL_TEXTURE_2D, texture_1);
	}
	if (texture_2 != 0) {
		glActiveTexture(GL_TEXTURE2);
		glUniform1i(cachedUniformLocationTexture2, 2);
		glBindTexture(GL_TEXTURE_2D, texture_2);
	}
	if (texture_3 != 0) {
		glActiveTexture(GL_TEXTURE3);
		glUniform1i(cachedUniformLocationTexture3, 3);
		glBindTexture(GL_TEXTURE_2D, texture_3);
	}
	if (texture_4 != 0) {
		glActiveTexture(GL_TEXTURE4);
		glUniform1i(cachedUniformLocationTexture4, 4);
		glBindTexture(GL_TEXTURE_2D, texture_4);
	}
	if (texture_5 != 0) {
		glActiveTexture(GL_TEXTURE5);
		glUniform1i(cachedUniformLocationTexture5, 5);
		glBindTexture(GL_TEXTURE_2D, texture_5);
	}
	if (texture_6 != 0) {
		glActiveTexture(GL_TEXTURE6);
		glUniform1i(cachedUniformLocationTexture6, 6);
		glBindTexture(GL_TEXTURE_2D, texture_6);
	}
	if (texture_7 != 0) {
		glActiveTexture(GL_TEXTURE7);
		glUniform1i(cachedUniformLocationTexture7, 7);
		glBindTexture(GL_TEXTURE_2D, texture_7);
	}
	glActiveTexture(GL_TEXTURE0);
}


void Material::buildUniformVariablesCache()
{
	cachedTimeVariableLocation = -1;
	cachedUniformLocationTexture0 =-1;
	cachedUniformLocationTexture1 =-1;
	cachedUniformLocationTexture2 =-1;
	cachedUniformLocationTexture3 =-1;
	cachedUniformLocationTexture4 =-1;
	cachedUniformLocationTexture5 =-1;
	cachedUniformLocationTexture6 =-1;
	cachedUniformLocationTexture7 =-1;
	
	if (bUseTimeVariable)
		cachedTimeVariableLocation = 
			glGetUniformLocation(shad.programHandle, "currentTime");

	if (texture_0 != 0)
		cachedUniformLocationTexture0 = 
			glGetUniformLocation(shad.programHandle, "textureSample_0");

	if (texture_1 != 0)
		cachedUniformLocationTexture1 = 
			glGetUniformLocation(shad.programHandle, "textureSample_1");

	if (texture_2 != 0)
		cachedUniformLocationTexture2 = 
			glGetUniformLocation(shad.programHandle, "textureSample_2");

	if (texture_3 != 0)
		cachedUniformLocationTexture3 = 
			glGetUniformLocation(shad.programHandle, "textureSample_3");

	if (texture_4 != 0)
		cachedUniformLocationTexture4 = 
			glGetUniformLocation(shad.programHandle, "textureSample_4");

	if (texture_5 != 0)
		cachedUniformLocationTexture5 = 
			glGetUniformLocation(shad.programHandle, "textureSample_5");

	if (texture_6 != 0)
		cachedUniformLocationTexture6 = 
			glGetUniformLocation(shad.programHandle, "textureSample_6");

	if (texture_7 != 0)
		cachedUniformLocationTexture7 = 
			glGetUniformLocation(shad.programHandle, "textureSample_7");

	bUniformVariablesCacheNeedsBuilding = false;
}

