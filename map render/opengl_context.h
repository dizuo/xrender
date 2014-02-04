// @module:		map-render/opengl_context
// @file:		opengl_context.h
// @author:		peteryfren
// @date:		2013/3/14
// @version:	1.0
// @desc:		.

#ifndef OPENGL_CONTEXT_201303141201_H
#define OPENGL_CONTEXT_201303141201_H

#include "opengl_frame_buffer.h"

//--------------------------------------------------------------------------------------
// opengl state
//--------------------------------------------------------------------------------------
struct GLState
{
	typedef unsigned char boolean;

	void save();

	void restore();

	// The attribute GL_SHADE_MODEL.
	int		glShadeModel_;

	// The attribute GL_COLOR_CLEAR_VALUE.
	float	glColorClearValue_[4];
	// The attribute GL_CURRENT_COLOR.
	float	glCurrentColor_[4];

	// The attribute GL_BLEND.
	boolean	glBlend_;
	// The attribute GL_BLEND_DST.
	int		glBlendDst_;
	// The attribute GL_BLEND_SRC.
	int		glBlendSrc_;

	// The attribute GL_CULL_FACE.
	boolean	glCullFace_;
	// The attribute GL_CULL_FACE_MODE.
	int		glCullFaceMode_;

	// The attribute GL_VERTEX_ARRAY.
	boolean	glVertexArray_;
	// The attribute GL_TEXTURE_COORD_ARRAY.
	boolean	glTextureCoordArray_;
	// The attribute GL_COLOR_ARRAY.
	boolean	glColorArray_;

	// The attribute GL_VIEWPORT.
	int		glViewport_[4];
	// The attribute GL_MODELVIEW_MATRIX.
	float	glModelviewMatrix_[16];
	// The attribute GL_PROJECTION_MATRIX.
	float	glProjectionMatrix_[16];
};

//--------------------------------------------------------------------------------------
// opengl context
//--------------------------------------------------------------------------------------
class OpenGLContext
{
public:
	typedef unsigned int Color4;

	OpenGLContext(int width, int height);
	~OpenGLContext();

	void beginPaint();		// save opengl status

	void doPaint(unsigned int& textureId);			// 绘制

	void endPaint();								// 还原旧环境

	void dumpCanvasToFile(int x, int y, const char* fileName);		// for debug

	static OpenGLFrameBuffer renderTarget;			// renderTarget.

private:

	OpenGLContext(const OpenGLContext&);

	OpenGLContext& operator=(const OpenGLContext&);

private:

	GLState glContext;

	unsigned int canvasWidth;
	unsigned int canvasHeight;

};

#endif // OPENGL_CONTEXT_201303141201_H