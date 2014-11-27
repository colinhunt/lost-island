// CMPUT 411 Assignment 4, Question 1

// Based on an original assignment design by Nathaniel Rossol
// Modified by Dale Schuurmans

#include <stdlib.h>
#include <iostream>
#include <string>
#include <cmath>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "camera.h"
#include "light.h"
#include "timer.h"
#include "Mesh.h"

#include "student_code/shader.h"

#define DEG2RAD 0.017453292519943 /* rad = deg*PI/180 = deg*DEG2RAD */

using namespace std;


/* Global variables */
camera cam;
timer tim;
Mesh* airplaneMesh;
shader shad;

	// light
float ambient[4]  = { 0.1, 0.1, 0.3, 1.0 };
float diffuse[4]  = { 1.0, 1.0, 1.0, 1.0 };
float specular[4] = { 1.0, 0.1, 0.1, 1.0 };
float position[4] = { 0.5, 0.5, 0.7071, 0.0 };
light lit(ambient, diffuse, specular, position);


int main(int, char**);
void setup();
void drawScene();
void idle();
void resize(int, int);
void keyInput(unsigned char, int, int);


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (1280, 768);
	glutInitWindowPosition (50, 50);
	glutCreateWindow("shader q1");
	glutDisplayFunc(drawScene);
	glutIdleFunc(idle);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glewInit();
	setup();
	glutMainLoop();
	return 0;
}


void setup()
{
	// light
	lit.glEnableLight();

	// camera volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	cam.initialize(persp, -16.7, 16.7, -10.0, 10.0, 10.0, 200000.0);
	cam.glVolume();

	// move camera into initial viewing position
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cam.zTransl(-100);
	cam.xRotate(90);
	cam.zTransl(300);
	cam.yRotate(180);
	cam.zRotate(180);

	// object
	airplaneMesh = new Mesh("Meshes/airplane.obj", "Toon shaded airplane",
	                        NULL, 0, 0, 0, 40, 40, 40);

	// timer
	tim.initialize(false,1000,0.01);

	// shader
	shad.setShaders("student_code/toon.vert", "student_code/toon.frag");

	// gl
	glShadeModel(GL_SMOOTH);

	glEnable(GL_NORMALIZE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glAlphaFunc(GL_GREATER,0.5);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	glClearColor(0.0, 0.0, 0.0, 0.0);  
}


void idle()
{	
	tim.glIdle();
	airplaneMesh->yaw = 360.0*(float)tim.loopFrac;
	glutPostRedisplay();
}


void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cam.glPosition();
	airplaneMesh->drawMesh();
	glutSwapBuffers();
}


void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}


void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'q': exit(0); break;
		default: break;
	}
	cam.keyInput(key); // camera controls
	tim.keyInput(key); // timer controls

	glutPostRedisplay();
}


