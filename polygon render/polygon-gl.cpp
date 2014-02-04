// @ author : ren.yafee@gmail.com
// @ desc : polygon render
// @ note : 模版缓冲绘制多边形，需要禁掉GL_CULL_FACE，gConcavePath中参数为BAD_CASE

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#include <time.h>

#include "concave_polygon_render.h"
#include "random_polygon.h"

#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480
#define SEGMENTS 25

#define FLOWER_COUNT	2

#define MIN(a, b) ( (a<b) ? (a):(b) )
#define MAX(a, b) ( (a<b) ? (b):(a) )

int window;
// gint64 start_time = 0;
int frames = 0;

float* contour = NULL;
unsigned int pointNum = 0;

int clears = 0;

void
on_key_press (unsigned char key, 
              int x, 
              int y)
{
	if (key=='q')
	{
		glutDestroyWindow (window);
		exit (0);
	}
	else if (key=='r')
	{
		RandomPolygon polygon(20, WINDOW_WIDTH, WINDOW_HEIGHT);
		polygon.getPolygonContour(contour, pointNum);
	}
}


// Concave Polygons Using the Stencil Buffer
// http://fly.cc.fer.hr/~unreal/theredbook/chapter13.html

// Introduction of Stencil Buffer
// http://www.glprogramming.com/red/chapter10.html

float gConcavePath[] = 
{
	//0,0,0,	100,100,0,
	//0,100,0,100,0,0,
	//0,0,0

	100,150,0,
	50,80,0,
	100,0,0,
	10,10,0,
	0,150,0,
	100,150,0

	//10,10,0,
	//0,150,0,
	//50,80,0,
	//100,150,0,
	//0,150,0,
	//10,10,0
};

void
scene_render (void)
{
    //if (start_time == 0)
    //    start_time = get_time ();

    if (clears < 2)
        glClear (GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    else
        glClear (GL_COLOR_BUFFER_BIT);

    // clears++;

	ConcavePolygonRender myRender;
	float topColor[4] = {1,0,0,1};
	float botColor[4] = {1,1,0,1};
	myRender.renderHandler(gConcavePath, 5, topColor, botColor);
	
	// myRender.renderHandler(contour, pointNum, topColor, botColor);

    glutSwapBuffers();
    frames++;
	
	// reRandom angin.
    // randomize_flower_list (data);

    if (frames == 25) {
        // g_debug ("FPS %.5f s", (float) frames / ((get_time () - start_time) / (float) G_USEC_PER_SEC));
        frames = 0;
        // start_time = 0;
    }
}

void 
on_idle (void)
{
    glutPostRedisplay();
}

int
main (int argc, 
      char *argv[])
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_STENCIL);
    glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);

    window = glutCreateWindow (argv[0]);

    glutDisplayFunc (scene_render);
    glutKeyboardFunc (on_key_press);
	glutIdleFunc (on_idle);

    /* GL settings */
    // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	
	// glEnable(GL_CULL_FACE);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0, 640, 480, 0, -1000.0, 1000.0);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

	srand(time(0));

	RandomPolygon polygon(20, WINDOW_WIDTH, WINDOW_HEIGHT);
	polygon.getPolygonContour(contour, pointNum);

    glutMainLoop();
    return 0;
}

void 
renderConcavePolygon(float r1, float g1, float b1, float a1,
					 float r2, float g2, float b2, float a2)
{
	float x0, y0, x1, y1;

	x0 = 0;
	x1 = 100;
	y0 = 0;
	y1 = 100;

	// Draw to stencil
	glDisable (GL_BLEND);
	glEnable (GL_STENCIL_TEST);
	glStencilMask (0x01);
	glStencilOp (GL_KEEP, GL_KEEP, GL_INVERT);
	glStencilFunc (GL_ALWAYS, 0, ~0);
	glColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glColor4f (1.0, 1.0, 1.0, 1.0);
	glBegin (GL_TRIANGLE_FAN);
	glVertex3f (0, 0, 0);
	// sketch_flower (&x0, &y0, &x1, &y1, NULL);
	for (int i=0; i<5; i++)
		glVertex3fv(gConcavePath + 3*i);

	glEnd ();

	// Draw aliased off-pixels to real
	glColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glStencilFunc (GL_EQUAL, 0x00, 0x01);
	glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

	glEnable(GL_LINE_SMOOTH);
	glBegin (GL_LINES);
	// sketch_flower (&x0, &y0, &x1, &y1, color_set_func);
	for (int i=0; i<5; i++)
		glVertex3fv(gConcavePath + 3*i);

	glEnd ();
	glDisable (GL_LINE_SMOOTH);

	// Draw fill
	glStencilFunc (GL_EQUAL, 0x01, 0x01);
	glStencilOp (GL_ZERO, GL_ZERO, GL_ZERO);

	glBegin (GL_QUADS);
	glColor4f (r1, g1, b1, a1);
	glVertex3f (x0, y0, 0.0); 
	glVertex3f (x1, y0, 0.0); 
	glColor4f (r2, g2, b2, a2);
	glVertex3f (x1, y1, 0.0); 
	glVertex3f (x0, y1, 0.0); 
	glEnd (); 

	glDisable (GL_STENCIL_TEST);
}