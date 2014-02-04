#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <gl/glut.h>
#include <iostream>
using namespace std;

#include "pack/pack.h"

#define RECTS 50
const int g_window_size = 512;
int g_window_width = g_window_size;
int g_window_height = g_window_size;

GLenum checkForError(char *loc);

rect_xywhf g_rects[RECTS], *g_ptr_rects[RECTS];
vector<bin> g_bins;
bool g_process_status = false;
GLuint g_color_stable[] =   
{  
	(0xFF0000), //red   
	(0xDC143C), //crimson  
	(0x9400D3), //violet  
	(0xFFA000), //orange  
	(0xFFFF00), //yellow  
	(0xFFD700), //glod  
	(0x00FF00), //green  
	(0x90FF90), //lightgreen  
	(0x006400), //darkgreen  
	(0x556B2F), //olivegreen        //very good colorÂÌéÏé­  
	(0x0000FF), //blue  
	(0x00008B), //darkblue  
	(0x4169E1), //royalblue  
	(0x5555FF), //skyblue  
	(0x191970), //midnightbue  
	(0xFF00FF), //fuchsia  
	(0x00FFFF), //aqua  
	(0xffb6c1), //pink  
	(0xFF91A4), //salmonpink  
	(0x30D5C8), //turquoise  
	(0x008080), //darkturpuoise  
	(0xA9A9A9), //gray  
	(0xD3D3D3), //lightgray  
	(0x808080)  //darkgray  
};  

void getColorFromTable(GLuint rgb, float vec[3])  
{  
	GLubyte  r = GLubyte ((rgb >> 16) & 0xFF);  
	GLubyte  g = GLubyte ((rgb >> 8) & 0xFF);  
	GLubyte  b = GLubyte (rgb & 0xFF);  
	vec[0] = r / 255.0f;  
	vec[1] = g / 255.0f;  
	vec[2] = b / 255.0f;  
}

void Init(void) 
{	
	const int maxSize = 128;		// 50
	for(int i = 0; i < RECTS; ++i) {
		g_rects[i] = rect_xywhf(0, 0, 7+rand()%maxSize,  7+rand()%maxSize);
		g_ptr_rects[i] = g_rects+i;
	}
		
	clock_t begin = clock();
	g_process_status = pack(g_ptr_rects, RECTS, g_window_size, g_bins);
	printf("time = %d\n", clock() - begin);

	glEnable(GL_CULL_FACE);	
}

void Render(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int colorTableSize = sizeof(g_color_stable) / sizeof(g_color_stable[0]);

	for(int i = 0; g_bins.size() >= 1 && i < 1; ++i) {
		printf("\n\nbin: %dx%d, rects: %d\n", g_bins[i].size.w, g_bins[i].size.h,
			g_bins[i].rects.size());

		for(int r = 0; r < g_bins[i].rects.size(); ++r) {
			rect_xywhf* rect = g_bins[i].rects[r];
			int colorIndex = r % colorTableSize;
			float colorCache[3];
			getColorFromTable(g_color_stable[colorIndex], colorCache);
			glColor3fv(colorCache);
			glRectf(rect->x, rect->y, rect->x + rect->w, rect->y + rect->h);
			printf("rect %d: x: %d, y: %d, w: %d, h: %d, was flipped: %s\n", r, rect->x, rect->y, rect->w, rect->h, rect->flipped ? "yes" : " no"); 
		}
	}

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

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(g_window_width, g_window_height);
	glutCreateWindow("RectangleBinPack");

	// set up world space to screen mapping
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat)g_window_size, 0.0f, (GLfloat)g_window_size, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, g_window_size, g_window_size);

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);

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


