// @Note ；道路的贴图两个像素高度即可
// 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include <iostream>
using namespace std;

#include "./map render/color.hpp"
#include "qtriangulatingstroker.h"

#include <gl/glut.h>

// #define TEXTURE_STROKE_APP

#include "road_render.h"

int windowWidth = 512, windowHeight = 512;

Qt::QVectorPath g_vectorPath;
Qt::QPen		g_pen;  
Qt::QTriangulatingStroker g_stroker;

GLenum checkForError(char *loc);
static int getSphereFactor(int i, int j, int width, int height);
int getAlphaFactor(int i, int j, int width, int height);

#ifdef TEXTURE_STROKE_APP
static void makeTexture(GLubyte* buffer, int width, int height, unsigned char color[3]);
void drawTextureQuad(int width, int height, unsigned int texture);
unsigned int textureId = 0;
RoadRender::PathType gPath;
#endif

void Init(void) 
{
	glEnable(GL_CULL_FACE);

	g_pen.setWidth(40);
	g_pen.setCapStyle(Qt::FlatCap);
	g_pen.setJoinStyle(Qt::BevelJoin);

	g_vectorPath.moveTo(100, 100);
	g_vectorPath.lineTo(200, 100);	
	g_vectorPath.moveTo(200, 200);
	g_vectorPath.lineTo(100, 200);

	g_stroker.setInvScale(2.0f);
	g_stroker.process(g_vectorPath, g_pen);
}

void Render(void)
{
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// glClearColor(0,0,0,0);
	Color4f ccl = colorub_2_color4f( int_2_color4ub(0xe8a76eff) );
	glClearColor( ccl.red(), ccl.green(), ccl.blue(), ccl.alpha() );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT, GL_FILL);
	glColor4ub(0xff, 0x00, 0x00, 0xff);
	glVertexPointer(2, GL_FLOAT, 0, g_stroker.vertices());
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, g_stroker.vertexCount()/2);

	glPolygonMode(GL_FRONT, GL_LINE);
	glColor4ub(0xff, 0xff, 0xff, 0xff);
	glVertexPointer(2, GL_FLOAT, 0, g_stroker.vertices());
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, g_stroker.vertexCount()/2);

#ifdef TEXTURE_STROKE_APP
	// turn on texture
	glEnable (GL_TEXTURE_2D);	
	RoadRender renderer;
	int ret = renderer.extrudeLines(gPath, 18);
	if (RoadRender::EXTRUDE_SUCCESS==ret)
	{
		renderer.render(textureId);
	}
	// drawTextureQuad(windowWidth, windowHeight, textureId);
#endif

	glutSwapBuffers();

	checkForError("swap");
}

void Mouse(int button, int state, int x, int y)
{
	if (state != GLUT_UP)
		return;

	switch (button) 
	{
	case GLUT_LEFT_BUTTON:

#ifdef TEXTURE_STROKE_APP
		gPath.push_back( gtl::Vec3f(x, windowHeight-y, 0) );
#endif
		if (g_vectorPath.isEmpty()) 
		{
			g_vectorPath.moveTo(x, windowHeight-y);
		} 
		else 
		{
			g_vectorPath.lineTo(x, windowHeight-y);
			// g_vectorPath.curveTo(x, windowHeight-y);
		}
		g_stroker.process(g_vectorPath, g_pen);
		glutPostRedisplay();
		break;
	
	case GLUT_RIGHT_BUTTON:

#ifdef TEXTURE_STROKE_APP		
		gPath.clear();
#endif
		g_vectorPath.clear();
		g_stroker.clear();
		glutPostRedisplay();
		break;
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		g_pen.setJoinStyle(Qt::BevelJoin);		
		break;
	case '2':
		g_pen.setJoinStyle(Qt::MiterJoin);
		break;
	case '3':
		g_pen.setJoinStyle(Qt::RoundJoin);
		break;
	case '4':
		g_pen.setCapStyle(Qt::FlatCap);
		break;
	case '5':
		g_pen.setCapStyle(Qt::SquareCap);
		break;
	case '6':
		g_pen.setCapStyle(Qt::RoundCap);
		break;
	case 'f':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'F':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	default:
		break;
	}
	g_stroker.process(g_vectorPath, g_pen);
	glutPostRedisplay();
}

void Reshape(int width, int height)
{
	windowWidth = width; windowHeight = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat) windowWidth, 0.0f, 
		(GLfloat) windowHeight, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, windowWidth, windowHeight);
}

int main(int argc, char *argv[])
{
	cout << "\tStrokeRender ReadME" << endl;
	cout << "Left mouse: add point to vector path " << endl;
	cout << "Right mouse: clear vector path " << endl;
	cout << "key 1: switch to Bevel Join Style" << endl;
	cout << "key 2: switch to Miter Join Style" << endl;
	cout << "key 3: switch to Round Join Style" << endl;
	cout << "key f: render stroke in GL_LINE mode" << endl;
	cout << "key F: render stroke in GL_FILL mode" << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("StrokeRenderApp");

	// set up world space to screen mapping
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat) windowWidth, 0.0f, (GLfloat) windowHeight, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, windowWidth, windowHeight);

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);

	Init();

#ifdef TEXTURE_STROKE_APP
	gPath.push_back( gtl::Vec3f(50,50,0) );
	gPath.push_back( gtl::Vec3f(200,300,0) );
	gPath.push_back( gtl::Vec3f(400,450,0) );

	// make a texture map
	int texWidth = 32;
	int texHeight = 32;

	GLubyte* texBuffer = new GLubyte[4*texWidth*texHeight];
	unsigned char color[3] = {128, 128, 0};
	makeTexture(texBuffer, texWidth, texHeight, color);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// initialize texturing
	glTexImage2D(GL_TEXTURE_2D,0,4,texHeight,texWidth,0,GL_RGBA, GL_UNSIGNED_BYTE, texBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] texBuffer;
	texBuffer = NULL;
#endif

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


int getSphereFactor(int i, int j, int width, int height)
{
	float s = (float)i / width;
	float t = (float)j / height;
	float radius = 0.5;

	float dis = sqrt( (s-0.5)*(s-0.5) + (t-0.5)*(t-0.5) );
	if( dis < radius )
	{
		// normalize: norm = dis / radius
		// inverse: 1 - norm
		// transform to color space.
		return (int)( (1-dis/radius)*255 );
	}

	// return (int)(0.2*255);
	return 0;
}

int getAlphaFactor(int i, int j, int width, int height)
{
	float s = (float)i / width;
	float t = (float)j / height;

	float r = sqrt( (s-0.5)*(s-0.5) + (t-0.5)*(t-0.5) );

	return r < 0.5 ? (int)((r)*255) : 0;
}

#ifdef TEXTURE_STROKE_APP

void drawTextureQuad(int width, int height, unsigned int texture)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	float tileVertices[] = {0,0, width,0, width,height, 0,height}; 
	float tileTexVertices[] = {1,0, 0.5,0, 0.5,1, 1,1};

	glVertexPointer(2, GL_FLOAT, 0, tileVertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, tileTexVertices);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); 

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void makeTexture(GLubyte* buffer, int width, int height, unsigned char color[3])
{
	int i, j, c;

	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j++) 
		{
			c = getSphereFactor(i, j, width, height);
			
			c = c > 255 ? 255 : c;

			buffer[ (i*width+j)*4 + 0 ] = color[0];
			buffer[ (i*width+j)*4 + 1 ] = color[1];
			buffer[ (i*width+j)*4 + 2 ] = color[2];
			buffer[ (i*width+j)*4 + 3 ] = (GLubyte) c;
		}
	}
}

#endif
