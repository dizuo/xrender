// @module:		stencil render
// @file:		opacityRectRender.cpp
// @author:		peteryfren
// @date:		2013/3/28
// @version:	1.0
// @desc:		通过模板缓冲区实现半透明矩形的常见操作。

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include <gl/glut.h>

const int windowWidth = 256;
const int windowHeight = 256;

GLenum checkForError(char *loc);

void opacitySourceRender(int width, int height);
void opacityRectXorRender(int width, int height);
void opacityRectUnionRender(int width, int height);
void opacityRectIntersectRender(int width, int height);
void opacityRectDifferenceRender(int width, int height);

void Redraw(void)
{
	glCullFace(GL_BACK);

	float destAlpha = 0.5;
	glClearColor(0,0,0, 0.5);	// 指定清理颜色值
	glClearStencil(0x00);		// 指定清理帧缓冲值
	glClearDepth(0.0);			// 指定清理深度缓冲值
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// glEnable (GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	opacitySourceRender(windowWidth, windowHeight);
	
	// opacityRectDifferenceRender(windowWidth, windowHeight);
	// opacityRectIntersectRender(windowWidth, windowHeight);
	// opacityRectUnionRender(windowWidth, windowHeight);
	// opacityRectXorRender(windowWidth, windowHeight);

	glutSwapBuffers();

	checkForError("swap");
}

void opacitySourceRender(int width, int height)
{
	// one way to solve this in generic OpenGL is to 
	//		fill the framebuffer with your desired alpha channel, 
	// switch the blending mode to glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA)
	//		and draw the rectangles. 

	glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);

	float tileVertices[] = {0,height/2, width,height/2, width,height*3/4, 0,height*3/4}; 
	glColor4f(1, 0, 0, 0.5);
	glVertexPointer(2, GL_FLOAT, 0, tileVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glColor4f(1, 1, 1, 1);

	float tileVertices1[] = {width/4,0, width/2,0, width/2,height, width/4,height}; 
	glColor4f(1, 0, 0, 0.5);
	glVertexPointer(2, GL_FLOAT, 0, tileVertices1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glColor4f(1, 1, 1, 1);
}

void opacityRectDifferenceRender(int width, int height)
{
	glEnable(GL_STENCIL_TEST);

	// draw mask area
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);			// 禁模板测试
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);		// 模板值加1

	glColor4f(1, 0, 0, 0.1);
	float tileVertices[] = {0,height/2, width,height/2, width,height*3/4, 0,height*3/4};
	glColor4f(1, 0, 0, 0.5);
	glVertexPointer(2, GL_FLOAT, 0, tileVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	//static unsigned char data[windowWidth*windowHeight];
	//glReadPixels(0, 0, windowWidth, windowHeight, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, data);
	//SaveFileGrayBMP("stencil.bmp", data, windowWidth,  windowHeight);

	// draw main area
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 0x00, 0x01);	// 只能在模板为0的地方绘图
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);	

	glColor4f(1, 0, 0, 0.5);
	float tileVertices1[] = {width/4,0, width/2,0, width/2,height, width/4,height}; 
	glVertexPointer(2, GL_FLOAT, 0, tileVertices1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glColor4f(1, 1, 1, 1);

	glDisable(GL_STENCIL_TEST);
}

void opacityRectXorRender(int width, int height)
{
	glEnable(GL_STENCIL_TEST);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// draw mask area 
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	glStencilFunc(GL_ALWAYS, 0x00, 0xFF);		// 禁用模板

	float tileVertices[] = {0,height/2, width,height/2, width,height*3/4, 0,height*3/4};
	glColor4f(1, 0, 0, 0.5);
	glVertexPointer(2, GL_FLOAT, 0, tileVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glColor4f(1, 0, 0, 0.5);
	float tileVertices1[] = {width/4,0, width/2,0, width/2,height, width/4,height}; 
	glVertexPointer(2, GL_FLOAT, 0, tileVertices1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glColor4f(1, 1, 1, 1);

	//static unsigned char data[windowWidth*windowHeight];
	//glReadPixels(0, 0, windowWidth, windowHeight, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, data);
	//SaveFileGrayBMP("stencil.bmp", data, windowWidth,  windowHeight);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 0x01, 0x01);		// 模板值为1的区域才绘制
	glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);		// 用0替换当前值

	glColor4f(1, 0, 0, 0.5);
	glVertexPointer(2, GL_FLOAT, 0, tileVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glColor4f(1, 1, 1, 1);

	glColor4f(1, 0, 0, 0.5);
	glVertexPointer(2, GL_FLOAT, 0, tileVertices1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glColor4f(1, 1, 1, 1);

	glDisable(GL_STENCIL_TEST);
}

void opacityRectIntersectRender(int width, int height)
{
	glEnable(GL_STENCIL_TEST);
	
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	
	// draw mask area 
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	glStencilFunc(GL_ALWAYS, 0x00, 0xFF);		// 禁用模板

	float tileVertices[] = {0,height/2, width,height/2, width,height*3/4, 0,height*3/4};
	glColor4f(1, 0, 0, 0.5);
	glVertexPointer(2, GL_FLOAT, 0, tileVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glColor4f(1, 0, 0, 0.5);
	float tileVertices1[] = {width/4,0, width/2,0, width/2,height, width/4,height}; 
	glVertexPointer(2, GL_FLOAT, 0, tileVertices1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glColor4f(1, 1, 1, 1);

	//static unsigned char data[windowWidth*windowHeight];
	//glReadPixels(0, 0, windowWidth, windowHeight, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, data);
	//SaveFileGrayBMP("stencil.bmp", data, windowWidth,  windowHeight);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glStencilFunc(GL_EQUAL, 0x02, 0x03);		// 模板值为2的区域才绘制 掩码0000 0011
	glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);

	glColor4f(1, 0, 0, 0.5);
	glVertexPointer(2, GL_FLOAT, 0, tileVertices1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glColor4f(1, 1, 1, 1);

	glDisable(GL_STENCIL_TEST);
}

void opacityRectUnionRender(int width, int height)
{
	// 模板缓冲区就一个字节

	glEnable (GL_STENCIL_TEST);		

	// glStencilFunc(GL_ALWAYS,0,0xFF);	// 禁用模板测试

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);	// 开启写入颜色缓冲区

	// fail = KEEP  模版测试失败后保持模板缓冲区中值 不变
	// zfail =  KEEP 深度测试失败后保持模版缓冲区中值 不变
	// zpass = INCR 深度测试通过后对模版缓冲区中值 加1操作
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);		
	
	// glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// func = EQUAL 只有与参考值相等才能通过模版测试
	// ref = 0x00	参考值
	// mask = 0x01	只比较最低位
	glStencilFunc(GL_EQUAL, 0x0, 0x1);	// 开启模板测试

	glEnable (GL_BLEND);		// 开启混合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// sfactor = 1 - dstA
	// dfactor = dstA
	// glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);

	float tileVertices[] = {0,height/2, width,height/2, width,height*3/4, 0,height*3/4};
	glColor4f(1, 0, 0, 0.5);
	glVertexPointer(2, GL_FLOAT, 0, tileVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glColor4f(1, 0, 0, 0.5);
	float tileVertices1[] = {width/4,0, width/2,0, width/2,height, width/4,height}; 
 	glVertexPointer(2, GL_FLOAT, 0, tileVertices1);
 	glEnableClientState(GL_VERTEX_ARRAY);
 	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glColor4f(1, 1, 1, 1);

	//static unsigned char data[windowWidth*windowHeight];
	//glReadPixels(0, 0, windowWidth, windowHeight, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, data);
	//SaveFileGrayBMP("stencil.bmp", data, windowWidth,  windowHeight);

	glDisable (GL_STENCIL_TEST);
}

void Button(int button, int state, int x, int y)
{
	if (state != GLUT_UP)
		return;

	switch (button) 
	{
	case GLUT_LEFT_BUTTON:

		glutPostRedisplay();
		break;
	
	case GLUT_RIGHT_BUTTON:

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
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH|GLUT_STENCIL);

	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("Region");

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

