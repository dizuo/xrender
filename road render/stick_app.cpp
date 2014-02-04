#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include <gl/glut.h>

#include "stick.h"

int windowWidth = 512, windowHeight = 512;

GLenum checkForError(char *loc);
int getSphereFactor(int i, int j, int width, int height);
int getAlphaFactor(int i, int j, int width, int height);

void makeTexture(GLubyte* buffer, int width, int height);
void drawTextureQuad(int width, int height, unsigned int texture);

unsigned int textureId = 0;

Stick::PathType gPath;

void Redraw(void)
{
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// turn on texture
	glEnable (GL_TEXTURE_2D);
	
	Stick sticker;
	int ret = sticker.extrudeLines(gPath, 8);
	if (Stick::EXTRUDE_SUCCESS==ret)
	{
		sticker.render(textureId);
	}

	// drawTextureQuad(windowWidth, windowHeight, textureId);
	
	glutSwapBuffers();

	checkForError("swap");
}

void Button(int button, int state, int x, int y)
{
	if (state != GLUT_UP)
		return;

	switch (button) 
	{
	case GLUT_LEFT_BUTTON:
		gPath.push_back( gtl::Vec3f(x, windowHeight-y, 0) );

		glutPostRedisplay();
		break;
	
	case GLUT_RIGHT_BUTTON:
		gPath.clear();

		glutPostRedisplay();
		break;
	}
}

void Keyboard(unsigned char key, int x, int y)
{

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
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Stick");

	// set up world space to screen mapping
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat) windowWidth, 0.0f, 
		(GLfloat) windowHeight, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, windowWidth, windowHeight);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutDisplayFunc(Redraw);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Button);
	glutKeyboardFunc(Keyboard);

	glEnable(GL_CULL_FACE);

	gPath.push_back( gtl::Vec3f(50,50,0) );
	gPath.push_back( gtl::Vec3f(200,300,0) );
	gPath.push_back( gtl::Vec3f(400,450,0) );

	// make a texture map
	int texWidth = 128;
	int texHeight = 128;

	GLubyte* texBuffer = new GLubyte[4*texWidth*texHeight];

	makeTexture(texBuffer, texWidth, texHeight);

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

	glutMainLoop();

	return 0;
}

void drawTextureQuad(int width, int height, unsigned int texture)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	float tileVertices[] = {0,0, width,0, width,height, 0,height}; 
	float tileTexVertices[] = {0,0, 1,0, 1,1, 0,1};

	glVertexPointer(2, GL_FLOAT, 0, tileVertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, tileTexVertices);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); 

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
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

void makeTexture(GLubyte* buffer, int width, int height)
{
	int i, j, c, a;

	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j++) 
		{
			c = getSphereFactor(i, j, width, height);
			
			c = c > 255 ? 255 : c;

			buffer[ (i*width+j)*4 + 0 ] = (GLubyte) 128;
			buffer[ (i*width+j)*4 + 1 ] = (GLubyte) 128;
			buffer[ (i*width+j)*4 + 2 ] = (GLubyte) 0;
			buffer[ (i*width+j)*4 + 3 ] = (GLubyte) c;
		}
	}
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

