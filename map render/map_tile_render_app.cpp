// @module:		map-render
// @file:		map_tile_render_app.cpp
// @author:		peteryfren
// @date:		2013/3/2
// @version:	1.0

#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#include <time.h>

#include "tile_file_loader.h"
#include "road_atlas_texture.h"

#define USE_FBO		// 开启FBO

#ifdef USE_FBO
	#include "share.h"
#endif

#define WINDOW_WIDTH    256
#define WINDOW_HEIGHT   256
#define SEGMENTS 25

int window;
int frames = 0;

int clears = 0;

TileFileLoader g_loader;
RoadAtlasTexture g_texture;

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
	else if (key=='l')
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (key=='f')
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glutPostRedisplay();
}


// Concave Polygons Using the Stencil Buffer
// http://fly.cc.fer.hr/~unreal/theredbook/chapter13.html

// Introduction of Stencil Buffer
// http://www.glprogramming.com/red/chapter10.html

float gConcavePath[] = 
{
	0,0,0,	100,100,0,
	0,100,0,100,0,0,
	0,0,0
};

void drawTextureQuad(int width, int height, unsigned int texture)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	float tileVertices[] = {0,0, 0,height, width,height, width,0}; 
	float tileTexVertices[] = {0,0, 0,1, 1,1, 1,0};

	glVertexPointer(2, GL_FLOAT, 0, tileVertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, tileTexVertices);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); 

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void drawColorQuad(int width, int height)
{
	glDisable(GL_TEXTURE_2D);

	float tileVertices[] = {0,0, 0,height, width,height, width,0}; 
	float tileColorVertices[] = {1,0,0, 0,1,1, 1,1,1, 1,0,1};

	glVertexPointer(2, GL_FLOAT, 0, tileVertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColorPointer(3, GL_FLOAT, 0, tileColorVertices);
	glEnableClientState(GL_COLOR_ARRAY); 

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}


void
scene_render (void)
{
    //if (start_time == 0)
    //    start_time = get_time ();

    glClear (GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

/*
	OpenGLVectorRender ogl_render(256, 256);
	Point lt;		ASSIGN_POINT(lt, 0, 64);
	Point rb;		ASSIGN_POINT(rb, 256, 256);
	ogl_render.set_brush_color( Color4ub(229,226,207,255) );
	ogl_render.render_rectangle(lt, rb);*/

#ifdef USE_FBO
	OpenGLContext* context = Share::get_context();
#endif

	if ( !g_loader.is_loaded() )
	{
		// 采用相对目录
		// g_loader.load_dat_file("./resource/tile.dat");
		// g_loader.load_dat_file("resource/block1.dat");
		g_loader.load_dat_file("resource/block.dat");
	}

	unsigned int texture_id = 0;

#ifdef USE_FBO
	context->beginPaint();
#endif

// 	float tex_buffer[4];
// 	g_texture.query_tile_texture_coord(Color4ub(255,0,0,0), tex_buffer);
// 	g_texture.query_tile_texture_coord(Color4ub(255,255,0,0), tex_buffer);
// 	g_texture.query_tile_texture_coord(Color4ub(255,0,255,0), tex_buffer);

	g_loader.render( g_texture );
	
#ifdef USE_FBO
	context->doPaint(texture_id);
	context->endPaint();
#else
	texture_id = g_texture.get_texture_id();
#endif

	drawTextureQuad(WINDOW_WIDTH, WINDOW_HEIGHT, texture_id);
	// drawColorQuad(WINDOW_WIDTH, WINDOW_HEIGHT);

    glutSwapBuffers();
}

void 
on_idle (void)
{
    // glutPostRedisplay();
}

int
main (int argc, 
      char *argv[])
{
	glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT);

    window = glutCreateWindow (argv[0]);

    glutDisplayFunc (scene_render);
    glutKeyboardFunc (on_key_press);
	glutIdleFunc (on_idle);

	// glClearColor(229/255.0f, 226/255.0f, 207/255.0f, 1.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glViewport(0,0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// 原点左上角，X向右，Y向下
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho (0, WINDOW_WIDTH, WINDOW_HEIGHT,0, -1000.0, 1000.0);

#ifdef USE_FBO
	OpenGLContext::renderTarget.doInit(true);
#endif

    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	// glEnable(GL_CULL_FACE);		// ###禁掉###

    glutMainLoop();
    return 0;
}

void render_quad()
{
	float v_list[] = {0,128, 0,256, 256,128, 256,256};
	// unsigned short i_list[] = {0,2,1, 1,2,3};		// CW
	unsigned short i_list[] = {0,1,2, 1,3,2};			// CCW

	float c_list[] = {229/255.0,226/255.0,207/255.0,1.0, 
		229/255.0,226/255.0,207/255.0,1.0,
		229/255.0,226/255.0,207/255.0,1.0,
		229/255.0,226/255.0,207/255.0,1.0};

	glColorPointer(4, GL_FLOAT, 0, c_list);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, v_list);	
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, i_list);
}