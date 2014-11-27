
#ifndef __APPLE__
#  include <GL/glew.h>
#endif
#include <GL/freeglut.h>


#ifndef TIMER_H
#define TIMER_H


struct timer
{
	// animation control
	double loopTime, loopFrac;
	int loopLength;
	long startTick;
	bool paused;


	// member functions


	void pause()
	{
		paused = true;
	}


	void play()
	{
		paused = false;
		startTick = glutGet(GLUT_ELAPSED_TIME);
	}


	void initialize(bool paus, int length, double spacing)
	{
		paused = paus;
		startTick = glutGet(GLUT_ELAPSED_TIME);
		loopFrac = 0.0;
		loopLength = length;
		loopTime = spacing*(double)loopLength;
	}


	void speedUp(double fps) // fps > 0 speed up, fps < 0 slow down
	{
		loopTime /= (1.0 + fps*loopTime/(double)loopLength);
	}


	void stepForward()
	{
		double timeIdx = loopFrac*(double)loopLength;
		int t = (int)round(timeIdx);
		t = (t >= loopLength-1) ? 0 : t+1;
		loopFrac = (double)t/loopLength;
	}


	void stepBackward()
	{
		double timeIdx = loopFrac*(double)loopLength;
		int t = (int)round(timeIdx);
		t = (t <= 0) ? loopLength-1 : t-1;
		loopFrac = (double)t/(double)loopLength;
	}


	void glIdle()
	{
		if (!paused) {
			long currTick = glutGet(GLUT_ELAPSED_TIME);
			long ticks = currTick - startTick;
			startTick = currTick;
			loopFrac += (double)ticks / 1000.0 / loopTime;
			loopFrac -= floor(loopFrac); // wrap frac into [0,1)
		}
		glutPostRedisplay();
	}


	void keyInput(unsigned char key) // timer controls
	{
		switch (key)
		{
			case 'p': play();         break;
			case 'P': pause();        break;
			case 'f': stepForward();  break;
			case 'F': stepBackward(); break;
			case '+': speedUp(10.0);  break;
			case '-': speedUp(-10.0); break; // slow down
		}
	}

};

#endif
