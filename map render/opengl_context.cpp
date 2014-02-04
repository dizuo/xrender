// @module:		map-render/opengl_context
// @file:		opengl_context.cpp
// @author:		peteryfren
// @date:		2013/3/14
// @version:	1.0
// @desc:		.

#include "opengl_context.h"

#include "../texture_editor/bmp.h"

#include <cmath>

#define fbo_log_i 

//--------------------------------------------------------------------------------------
// defines
//--------------------------------------------------------------------------------------

#define TEXTURE_WIDTH 256
#define TEXTURE_HEIGHT 256

#define SET_GL_STATE(flag, glEnum) { if (flag) { glEnable(glEnum);}	else { glDisable(glEnum); } }
#define SET_GL_CLIENT_STATE(flag, glEnum) { if (flag){ glEnableClientState(glEnum); }	else {	glDisableClientState(glEnum); } }

//--------------------------------------------------------------------------------------
// global functions
//--------------------------------------------------------------------------------------

// declare
static void renderTileQuad(const unsigned char* rBuffer, int, int);
static void stencilRenderTest();

//--------------------------------------------------------------------------------------
// opengl state
//--------------------------------------------------------------------------------------
void GLState::save()
{
	glGetFloatv( GL_COLOR_CLEAR_VALUE , glColorClearValue_ );

	glGetBooleanv( GL_BLEND , &glBlend_ );
	glGetIntegerv( GL_BLEND_DST , &glBlendDst_ );
	glGetIntegerv( GL_BLEND_SRC , &glBlendSrc_ );

	glGetBooleanv( GL_CULL_FACE , &glCullFace_ );
	glGetIntegerv( GL_CULL_FACE_MODE , &glCullFaceMode_ );
	glGetFloatv( GL_CURRENT_COLOR , glCurrentColor_ );

	glGetIntegerv( GL_VIEWPORT , glViewport_ );
	glGetIntegerv( GL_SHADE_MODEL , &glShadeModel_ );
	glGetFloatv( GL_PROJECTION_MATRIX , glProjectionMatrix_ );
	glGetFloatv( GL_MODELVIEW_MATRIX , glModelviewMatrix_ );

	glGetBooleanv( GL_VERTEX_ARRAY , &glVertexArray_ );
	glGetBooleanv( GL_TEXTURE_COORD_ARRAY, &glTextureCoordArray_);
	glGetBooleanv( GL_COLOR_ARRAY, &glColorArray_);
}

void GLState::restore()
{
	// fbo_log_i("restore 1: %d", glGetError());

	glClearColor(glColorClearValue_[0], glColorClearValue_[1], glColorClearValue_[2], glColorClearValue_[3]);

	// restore viewport
	glViewport(glViewport_[0], glViewport_[1], glViewport_[2], glViewport_[3]);

	// restore projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(glProjectionMatrix_);

	// restore modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(glModelviewMatrix_);

	// restore current color
	// glColor4fv(glCurrentColor_);
	glColor4f(glCurrentColor_[0], glCurrentColor_[1], glCurrentColor_[2], glCurrentColor_[3]);

	SET_GL_STATE(glBlend_, GL_BLEND);

	glBlendFunc(glBlendSrc_, glBlendDst_);

	SET_GL_STATE(glCullFace_, GL_CULL_FACE);
	glCullFace(glCullFaceMode_);
	glShadeModel(glShadeModel_);

	SET_GL_CLIENT_STATE(glVertexArray_, GL_VERTEX_ARRAY);
	SET_GL_CLIENT_STATE(glTextureCoordArray_, GL_TEXTURE_COORD_ARRAY);
	SET_GL_CLIENT_STATE(glColorArray_, GL_COLOR_ARRAY);

}

//--------------------------------------------------------------------------------------
// OpenGLFrameBuffer
//--------------------------------------------------------------------------------------

OpenGLFrameBuffer OpenGLContext::renderTarget(TEXTURE_WIDTH, TEXTURE_HEIGHT);

OpenGLContext::OpenGLContext(int width, int height)
: canvasWidth(width), canvasHeight(height)
{
}

OpenGLContext::~OpenGLContext()
{
}

// 保存、设置新的OpenGL状态量
// 视口、模型视图、投影、混合状态
void OpenGLContext::beginPaint()			// 保存旧、初始化新环境
{
	glContext.save();

	renderTarget.enterFBO();
	
	// glDisable(GL_CULL_FACE);		// ###禁掉###

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_ALWAYS); 

	glEnable(GL_SCISSOR_TEST);
	glScissor(0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT);
	glClearColor(1,1,1,1);			// 设置当前清理颜色
#ifdef _WIN32
	glClearDepth(0.0);				// 设置当前清理深度值
#else 
	glClearDepthf(0.0);				// 设置当前清理深度值
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	
	// 用当前缓冲区清楚值(glClearColor, glClearDepth, glClearStencil) 
	
	// 通过GL_SCISSOR_TEST 清理指定缓冲区

	// 原点位于屏幕左下角，X右 Y上
	glViewport(0, 0, (GLsizei)canvasWidth, (GLsizei)canvasHeight);

	// 切换到正交模式
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity(); 

#ifdef ES_ANDROID
	glOrthof(0, canvasWidth, canvasHeight, 0, 0, 1.0f);
#else
	glOrtho(0, canvasWidth, canvasHeight, 0, 0, 1.0f);
#endif

	glMatrixMode(GL_MODELVIEW);  

	glLoadIdentity();  

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// stencilRenderTest();

	fbo_log_i(" beginPaint }} error = %d ", glGetError());
}

// 绘图完成将FBO的纹理对象传到glmap层管理
void OpenGLContext::doPaint(unsigned int& textureId)
{
	fbo_log_i(" doPaint {{ error = %d ", glGetError());

	textureId = renderTarget.color_rboId;		// 

	fbo_log_i(" doPaint }} error = %d ", glGetError());
}

// 还原旧环境
void OpenGLContext::endPaint()				
{
	fbo_log_i(" endPaint {{ error = %d ", glGetError());

	renderTarget.exitFBO();
	glContext.restore();

	fbo_log_i(" endPaint }} error = %d ", glGetError());
}

void OpenGLContext::dumpCanvasToFile(int x, int y, const char* fileName)
{
 	unsigned char* canvasBuf = new unsigned char[canvasHeight*canvasWidth*4];
 	glReadPixels(x,y,canvasWidth,canvasHeight, GL_RGBA, GL_UNSIGNED_BYTE, canvasBuf);
 	SaveFileBMP(fileName, canvasBuf, canvasWidth, canvasHeight, 4);
 	delete[] canvasBuf;	canvasBuf = NULL;
}

//--------------------------------------------------------------------------------------
// global functions
//--------------------------------------------------------------------------------------

void stencilRenderTest()
{
	GLdouble dRadius = 0.1; // Initial radius of spiral
	GLdouble dAngle;        // Looping variable
	float x = 100;
	float y = 100;
	float rsize = 25;

	// Clear blue window
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);

	// Use 0 for clear stencil, enable stencil test
	glClearStencil(0.0f);
	glEnable(GL_STENCIL_TEST);

	// Clear color and stencil buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// All drawing commands fail the stencil test, and are not
	// drawn, but increment the value in the stencil buffer. 
	glStencilFunc(GL_NEVER, 0x0, 0x0);
	glStencilOp(GL_INCR, GL_INCR, GL_INCR);

	// Spiral pattern will create stencil pattern
	// Draw the spiral pattern with white lines. We 
	// make the lines  white to demonstrate that the 
	// stencil function prevents them from being drawn
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for(dAngle = 0; dAngle < 400.0; dAngle += 0.1)
	{
		glVertex2d(dRadius * cos(dAngle), dRadius * sin(dAngle));
		dRadius *= 1.002;
	}
	glEnd();

	// Now, allow drawing, except where the stencil pattern is 0x1
	// and do not make any further changes to the stencil buffer
	glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// Now draw red bouncing square
	// (x and y) are modified by a timer function
	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(x, y, x + rsize, y - rsize);

}

void renderTileQuad(const unsigned char* rBuffer, int canvasWidth, int canvasHeight)
{
	// float marginX = 5;
	// float marginY = 5;

	float marginX = 0;
	float marginY = 0;
	float tileVertices[] = {-marginX,-marginY, canvasWidth+marginX,-marginY,
		canvasWidth+marginX,canvasHeight+marginY, -marginX,canvasHeight+marginY}; 
	float tileTexVertices[] = {0,0, 1,0, 1,1, 0,1};

	glVertexPointer(2, GL_FLOAT, 0, tileVertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, tileTexVertices);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); 

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
