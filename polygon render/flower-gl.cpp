#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#include <time.h>

#include "flower-common.h"
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

FlowerData *data;
typedef void (*ColorSetFunc) (float x, float y);

float* contour = NULL;
unsigned int pointNum = 0;

float r_set_1;
float g_set_1;
float b_set_1;
float a_set_1;
float r_set_2;
float g_set_2;
float b_set_2;
float a_set_2;

int clears = 0;

void
on_key_press (unsigned char key, 
              int x, 
              int y)
{
    glutDestroyWindow (window);
    exit (0);
}

void 
draw_curve (float x0, 
            float y0, 
            float x1, 
            float y1,
            float x2, 
            float y2, 
            float x3,
            float y3, 
            int segments, 
            float *xx0, 
            float *yy0,
            float *xx1, 
            float *yy1, 
            ColorSetFunc func)
{
    float lvl = 0.0;
    float x;
    float y;

    x = (x0 * (1.0 - lvl) * (1.0 - lvl) * (1.0 - lvl)) +
        (x1 * 3.0 * lvl * (1.0 - lvl) * (1.0 - lvl)) +
        (x2 * 3.0 * lvl * lvl * (1.0 - lvl)) +
        (x3 * lvl * lvl * lvl);

    y = (y0 * (1.0 - lvl) * (1.0 - lvl) * (1.0 - lvl)) +
        (y1 * 3.0 * lvl * (1.0 - lvl) * (1.0 - lvl)) +
        (y2 * 3.0 * lvl * lvl * (1.0 - lvl)) +
        (y3 * lvl * lvl * lvl);

    int i;
    for (i = 0; i < segments; i++) {
        
        *xx0 = MIN (*xx0, x);
        *xx1 = MAX (*xx1, x);
        *yy0 = MIN (*yy0, y);
        *yy1 = MAX (*yy1, y);

        if (func)
            func (x, y);
        glVertex3f (x, y, 0);

        lvl = ((i + 1) / (float) segments);
        
        x = (x0 * (1.0 - lvl) * (1.0 - lvl) * (1.0 - lvl)) +
            (x1 * 3.0 * lvl * (1.0 - lvl) * (1.0 - lvl)) +
            (x2 * 3.0 * lvl * lvl * (1.0 - lvl)) +
            (x3 * lvl * lvl * lvl);

        y = (y0 * (1.0 - lvl) * (1.0 - lvl) * (1.0 - lvl)) +
            (y1 * 3.0 * lvl * (1.0 - lvl) * (1.0 - lvl)) +
            (y2 * 3.0 * lvl * lvl * (1.0 - lvl)) +
            (y3 * lvl * lvl * lvl);

        *xx0 = MIN (*xx0, x);
        *xx1 = MAX (*xx1, x);
        *yy0 = MIN (*yy0, y);
        *yy1 = MAX (*yy1, y);
 
        if (func)
            func (x, y);
        glVertex3f (x, y, 0);
    }

}

void 
color_set_func (float x, 
                float y)
{
    float v = ((y + 1) / 2.0);
    glColor4f (r_set_1 + ((r_set_2 - r_set_1) * v), 
               g_set_1 + ((g_set_2 - g_set_1) * v),
               b_set_1 + ((b_set_2 - b_set_1) * v),
               a_set_1 + ((a_set_2 - a_set_1) * v));
}

void
sketch_flower (float *x0, 
               float *y0,
               float *x1, 
               float *y1, 
               ColorSetFunc func)
{
    draw_curve (0.0, 0.0, 
                -0.9, 0.0,
                -0.9, -0.9,
                -0.9, -0.9,
                SEGMENTS,
                x0, y0, x1, y1, 
                func);

    draw_curve (-0.9, -0.9, 
                0.0, -0.9,
                0.0, 0.0,
                0.0, 0.0,
                SEGMENTS,
                x0, y0, x1, y1, 
                func);

    draw_curve (0.0, 0.0, 
                0.9, 0.0,
                0.9, -0.9,
                0.9, -0.9,
                SEGMENTS,
                x0, y0, x1, y1, 
                func);

    draw_curve (0.9, -0.9, 
                0.0, -0.9,
                0.0, 0.0,
                0.0, 0.0,
                SEGMENTS,
                x0, y0, x1, y1, 
                func);

    draw_curve (0.0, 0.0, 
                0.9, 0.0,
                0.9, 0.9,
                0.9, 0.9,
                SEGMENTS,
                x0, y0, x1, y1, 
                func);

    draw_curve (0.9, 0.9, 
                0.0, 0.9,
                0.0, 0.0,
                0.0, 0.0,
                SEGMENTS,
                x0, y0, x1, y1, 
                func);

    draw_curve (0.0, 0.0, 
                -0.9, 0.0,
                -0.9, 0.9,
                -0.9, 0.9,
                SEGMENTS,
                x0, y0, x1, y1, 
                func);

    draw_curve (-0.9, 0.9, 
                0.0, 0.9,
                0.0, 0.0,
                0.0, 0.0,
                SEGMENTS,
                x0, y0, x1, y1, 
                func);
}

// Concave Polygons Using the Stencil Buffer
// http://fly.cc.fer.hr/~unreal/theredbook/chapter13.html

// Introduction of Stencil Buffer
// http://www.glprogramming.com/red/chapter10.html

void
do_flower (float r1, 
           float g1, 
           float b1,
           float a1,
           float r2, 
           float g2, 
           float b2, 
           float a2)
{
    float x0, y0, x1, y1;

    x0 = 100000;
    x1 = -100000;
    y0 = 100000;
    y1 = -100000;

    // Draw to stencil
    glDisable (GL_BLEND);
    glEnable (GL_STENCIL_TEST);
    glStencilMask (0x01);
    glStencilOp (GL_KEEP, GL_KEEP, GL_INVERT);
	
	// The reference value is compared to the value in the stencil buffer using the comparison function,
	// but the comparison applies only to those bits where the corresponding bits of the mask are 1.
	// (func, ref, mask)
	glStencilFunc (GL_ALWAYS, 0, ~0);
    glColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    glColor4f (1.0, 1.0, 1.0, 1.0);
    glBegin (GL_TRIANGLE_FAN);
    glVertex3f (0, 0, 0);
    sketch_flower (&x0, &y0, &x1, &y1, NULL);
    glEnd ();

    // Draw aliased off-pixels to real
    glColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glStencilFunc (GL_EQUAL, 0x00, 0x01);
    glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

    r_set_1 = r1;
    g_set_1 = g1;
    b_set_1 = b1;
    a_set_1 = a1;
    r_set_2 = r2;
    g_set_2 = g2;
    b_set_2 = b2;
    a_set_2 = a2;
    
    glEnable(GL_LINE_SMOOTH);
    glBegin (GL_LINES);
    sketch_flower (&x0, &y0, &x1, &y1, color_set_func);
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

void
do_flower_plain (float r1, 
		   float g1, 
		   float b1,
		   float a1,
		   float r2, 
		   float g2, 
		   float b2, 
		   float a2)
{
	float x0, y0, x1, y1;

	x0 = 100000;
	x1 = -100000;
	y0 = 100000;
	y1 = -100000;

	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f (1.0, 1.0, 1.0, 1.0);
	glBegin (GL_TRIANGLE_FAN);
	glVertex3f (0, 0, 0);
	sketch_flower (&x0, &y0, &x1, &y1, NULL);
	glEnd ();

	r_set_1 = r1;
	g_set_1 = g1;
	b_set_1 = b1;
	a_set_1 = a1;
	r_set_2 = r2;
	g_set_2 = g2;
	b_set_2 = b2;
	a_set_2 = a2;

	glEnable(GL_LINE_SMOOTH);
	glBegin (GL_LINES);
	sketch_flower (&x0, &y0, &x1, &y1, color_set_func);
	glEnd ();
	glDisable (GL_LINE_SMOOTH);

}

float gConcavePath[] = 
{
	0,0,0,	100,100,0,
	0,100,0,100,0,0,
	0,0,0
};

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

	r_set_1 = r1;
	g_set_1 = g1;
	b_set_1 = b1;
	a_set_1 = a1;
	r_set_2 = r2;
	g_set_2 = g2;
	b_set_2 = b2;
	a_set_2 = a2;

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

void
scene_render (void)
{
    //if (start_time == 0)
    //    start_time = get_time ();

    if (clears < 2)
        glClear (GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    else
        glClear (GL_COLOR_BUFFER_BIT);

    clears++;

    int i;
    for (i = 0; i < FLOWER_COUNT; i++) {
        glPushMatrix ();
        glTranslatef (data [i].x, data [i].y, 0.0);
        glScalef (data [i].scale, data [i].scale, 0.0);
        glRotatef (data [i].rotation * 57.2957795, 0.0, 0.0, 1.0); 

        do_flower (data [i].r1, data [i].g1, data [i].b1, data [i].a1, 
                   data [i].r2, data [i].g2, data [i].b2, data [i].a2);
        
        glPopMatrix ();
    }

	// bezier polygon.
	/*float x0,y0,x1,y1;
	glColor4f(1,0,0,1);
	glBegin(GL_TRIANGLE_FAN);
	draw_curve (0.0, 0.0, 
		100.0, 0.0,
		100.0, 100.0,
		0.0, 100.0,
		SEGMENTS,
		&x0, &y0, &x1, &y1, 
		NULL);
	glEnd(); */

	// Direct render :
	//glColor4f (1.0, 1.0, 1.0, 1.0);
	//glBegin (GL_TRIANGLE_FAN);
	//glVertex3f (0, 0, 0);
	//for (int i=0; i<5; i++)
	//	glVertex3fv(gConcavePath + 3*i);
	//glEnd ();

	// 
	// renderConcavePolygon(1,0,0,1, 1,1,0,1);

	ConcavePolygonRender myRender;
	float topColor[4] = {1,0,0,1};
	float botColor[4] = {1,1,0,1};
	// myRender.renderHandler(gConcavePath, 5, topColor, botColor);
	
	myRender.renderHandler(contour, pointNum, topColor, botColor);

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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0, 640, 480, 0, -1000.0, 1000.0);

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

	srand(time(0));
	
    data = (FlowerData*) malloc (sizeof (FlowerData) * FLOWER_COUNT);
    randomize_flower_list (data, FLOWER_COUNT);

	RandomPolygon polygon(20, WINDOW_WIDTH, WINDOW_HEIGHT);
	polygon.getPolygonContour(contour, pointNum);

    glutMainLoop();
    return 0;
}
