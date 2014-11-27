
#ifndef __APPLE__
#  include <GL/glew.h>
#endif
#include <GL/freeglut.h>


#ifndef LIGHT_H
#define LIGHT_H


struct light
{
	float colorAmbient[4];
	float colorDiffuse[4];
	float colorSpecula[4];
	float position[4];


	// constructors

	light()
	{
		colorAmbient[0] = colorDiffuse[0] = colorSpecula[0] = position[0] = 0.0;
		colorAmbient[1] = colorDiffuse[1] = colorSpecula[1] = position[1] = 0.0;
		colorAmbient[2] = colorDiffuse[2] = colorSpecula[2] = position[2] = 0.0;
		colorAmbient[3] = colorDiffuse[3] = colorSpecula[3] = position[3] = 1.0;
	}


	light(float intensity)
	{
		colorAmbient[0] = colorAmbient[1] = colorAmbient[2] = 0.0;
		colorDiffuse[0] = colorDiffuse[1] = colorDiffuse[2] = intensity;
		colorSpecula[0] = colorSpecula[1] = colorSpecula[2] = 0.0;
		colorAmbient[3] = colorDiffuse[3] = colorSpecula[3] = 1.0;
		position[0] = position[1] = position[2] = 1.0;
		position[3] = 0.0; // directional
	}


	light(float* amb, float* dif, float* spe, float* pos) 
	{
		for (unsigned int i = 0; i < 4; i++) {
			colorAmbient[i] = amb[i];
			colorDiffuse[i] = dif[i];
			colorSpecula[i] = spe[i];
			position[i] = pos[i];
		}
	}


	// member functions


	void glEnableLight()
	{
		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT0, GL_AMBIENT,  colorAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE,  colorDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, colorSpecula);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glEnable(GL_LIGHT0);
	}


	void glUpdateLight()
	{
		glLightfv(GL_LIGHT0, GL_AMBIENT,  colorAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE,  colorDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, colorSpecula);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
	}


	void glDisableLight()
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}

};

#endif
