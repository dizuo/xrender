// @module:		map-render/opengl_frame_buffer
// @file:		opengl_frame_buffer.h
// @author:		peteryfren
// @date:		2013/3/14
// @version:	1.0
// @desc:		.

#ifndef OPENGL_FRAME_BUFFER_201303141201_H
#define OPENGL_FRAME_BUFFER_201303141201_H


#ifndef GL_GLEXT_PROTOTYPES
	#define GL_GLEXT_PROTOTYPES
#endif

#ifdef ES_ANDROID
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <android/log.h>
#else

	#include <cstdio>
	#include <gl/glut.h>
	#include "glext.h"
#endif

class OpenGLFrameBuffer
{
public:
	OpenGLFrameBuffer(int w, int h);

#ifdef _WIN32
	// Framebuffer object
	PFNGLGENFRAMEBUFFERSPROC                     glGenFramebuffers;                      // FBO name generation procedure
	PFNGLDELETEFRAMEBUFFERSPROC                  glDeleteFramebuffers;                   // FBO deletion procedure
	PFNGLBINDFRAMEBUFFERPROC                     glBindFramebuffer;                      // FBO bind procedure
	PFNGLCHECKFRAMEBUFFERSTATUSPROC              glCheckFramebufferStatus;               // FBO completeness test procedure
	PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;  // return various FBO parameters
	PFNGLGENERATEMIPMAPPROC                      glGenerateMipmap;                       // FBO automatic mipmap generation procedure
	PFNGLFRAMEBUFFERTEXTURE2DPROC                glFramebufferTexture2D;                 // FBO texdture attachement procedure
	PFNGLFRAMEBUFFERRENDERBUFFERPROC             glFramebufferRenderbuffer;              // FBO renderbuffer attachement procedure
	// Renderbuffer object
	PFNGLGENRENDERBUFFERSPROC                    glGenRenderbuffers;                     // renderbuffer generation procedure
	PFNGLDELETERENDERBUFFERSPROC                 glDeleteRenderbuffers;                  // renderbuffer deletion procedure
	PFNGLBINDRENDERBUFFERPROC                    glBindRenderbuffer;                     // renderbuffer bind procedure
	PFNGLRENDERBUFFERSTORAGEPROC                 glRenderbufferStorage;                  // renderbuffer memory allocation procedure
	PFNGLGETRENDERBUFFERPARAMETERIVPROC          glGetRenderbufferParameteriv;           // return various renderbuffer parameters
	PFNGLISRENDERBUFFERPROC                      glIsRenderbuffer;                       // determine renderbuffer object type

#endif	
	GLuint fboId;							// ID of FBO

	GLuint textureId;						// 
	GLuint depth_stencil_rboId;                     // ID of Renderbuffer object
	GLuint color_rboId;
	
	GLuint width;							// 画布宽度
	GLuint height;							// 画布高度

	bool fboUsed;

	void doInit(bool fboSupported);
	void doDestroy();

	void enterFBO();
	void exitFBO();

private:

	bool checkFramebufferStatus();

};


#endif  // OPENGL_FRAME_BUFFER_201303141201_H
