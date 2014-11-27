////////////////////////////////////////////////////////////////////
// helloSquare.cpp
//
// This program, based on square.cpp, is a simple GLSL program to
// show shader functionality. It adds to square.cpp minimal code
// to activate a vertex shader and a fragment shader. 
// The output is a red square.
//
////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <stdexcept>

#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;


void setup();
void drawScene();
void resize(int, int);
void keyInput(unsigned char, int, int);
char* readTxtFile(char*);
int glVerifyHandle(GLuint);


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500); 
	glutInitWindowPosition(100, 100);
	glutCreateWindow("redSquare.cpp");
	glutDisplayFunc(drawScene); 
	glutReshapeFunc(resize);	
	glutKeyboardFunc(keyInput);
	glewInit();
	setup();
	glutMainLoop();
	return 0;	 
}


void setup() 
{
	// set up vertex and fragment shaders
	GLuint vertShaderHandle(glCreateShader(GL_VERTEX_SHADER)); 
	GLuint fragShaderHandle(glCreateShader(GL_FRAGMENT_SHADER)); 
	
	char* vertShaderSource(readTxtFile("passThrough.vert"));
	char* fragShaderSource(readTxtFile("red.frag"));
	
	glShaderSource(vertShaderHandle, 1, (const char**) &vertShaderSource, NULL);
	glShaderSource(fragShaderHandle, 1, (const char**) &fragShaderSource, NULL);
	if (vertShaderSource != NULL) free(vertShaderSource);
	if (fragShaderSource != NULL) free(fragShaderSource);
	
	glCompileShader(vertShaderHandle);
	glCompileShader(fragShaderHandle);

	if (!glVerifyHandle(vertShaderHandle))
		throw runtime_error("Failed to compile vertex shader");
	if (!glVerifyHandle(fragShaderHandle))
		throw runtime_error("Failed to compile fragment shader");

	// set up final shading program
	GLuint programHandle(glCreateProgram()); 

	glAttachShader(programHandle, vertShaderHandle);
	glAttachShader(programHandle, fragShaderHandle);

	glLinkProgram(programHandle);

	if (!glVerifyHandle(programHandle))
		throw runtime_error("Failed to link program");

	glDetachShader(programHandle, vertShaderHandle);
	glDetachShader(programHandle, fragShaderHandle);

	glUseProgram(programHandle);

	glClearColor(1.0, 1.0, 1.0, 0.0);
}


void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_POLYGON);
		glVertex3f(20.0, 20.0, 0.0);
		glVertex3f(80.0, 20.0, 0.0);
		glVertex3f(80.0, 80.0, 0.0);
		glVertex3f(20.0, 80.0, 0.0);
	glEnd();

	glFlush();
}


void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void keyInput(unsigned char key, int x, int y)
{
	switch(key)
	{		
		case 'q': exit(0); break;
		default: break;
	}
}


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
