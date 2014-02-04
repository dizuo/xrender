#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <gl/glut.h>
#include <iostream>
using namespace std;

#include "2dobb.h"

const int g_window_size = 512;
int g_window_width = g_window_size;
int g_window_height = g_window_size;
const char* g_app_string = "OBB2DDemo";

OBB2D*		g_rotateObbPtr;
OBB2D*		g_moveObbPtr;
float		g_obbAngle = 10;
Vector2		g_moveObbCenter(50, 50);

GLenum checkForError(char *loc);

void Init(void) 
{	
	g_rotateObbPtr = new OBB2D( Vector2(100, 100), 50,100, g_obbAngle );
	g_moveObbPtr = new OBB2D( g_moveObbCenter, 50,100, 0 );
	glEnable(GL_CULL_FACE);
}

void Render(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (g_rotateObbPtr->overlaps(*g_moveObbPtr)) 
	{
		glColor3f(0, 1, 0);
	} else 
	{
		glColor3f(1, 0, 0);
	}
	g_rotateObbPtr->render();
	g_moveObbPtr->render();

	glutSwapBuffers();
	checkForError("swap");
}

void Reshape(int width, int height)
{
	g_window_width = width; g_window_height = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat) g_window_width, 0.0f, 
		(GLfloat) g_window_height, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, g_window_width, g_window_height);
}

void keyboard(unsigned char key, int x, int y) {
	int state = -1;

	switch (key)
	{
	case '1':
		g_obbAngle += 0.1;
		g_rotateObbPtr->updateAngle(Vector2(100, 100), 50, 100, g_obbAngle);
		break;
	case 'w':	// move up
		state = 0;
		break;
	case 's':
		state = 1;
		break;
	case 'a':
		state = 3;
		break;
	case 'd':
		state = 2;
		break;
	case 'q':
		delete g_rotateObbPtr;
		delete g_moveObbPtr;
		exit(0);
	}

	if (state >= 0) {
		Vector2::data_type dx = 10 * (state < 2 ? 0 : (state==3 ? -1 : 1) );
		Vector2::data_type dy = 10 * (state < 2 ? (state==0 ? 1 : -1) : 0 );
		g_moveObbCenter += Vector2(dx, dy);
		g_moveObbPtr->moveTo(g_moveObbCenter);
	}

	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(g_window_width, g_window_height);
	glutCreateWindow(g_app_string);

	// set up world space to screen mapping
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat)g_window_size, 0.0f, (GLfloat)g_window_size, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, g_window_size, g_window_size);

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);

	Init();

	glutMainLoop();

	return 0;
}

GLenum checkForError(char *loc)
{
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		printf("OpenGL error: %s",errString);

		if (loc != NULL)
			printf("(%s)",loc);

		printf("\n");
	}

	return errCode;
}


