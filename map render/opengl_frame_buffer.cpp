// @module:		map-render/opengl_frame_buffer
// @file:		opengl_frame_buffer.h
// @author:		peteryfren
// @date:		2013/3/14
// @version:	1.0
// @desc:		.

#include "opengl_frame_buffer.h"

#define fbo_log
#define fbo_log_i 

OpenGLFrameBuffer::OpenGLFrameBuffer(int w, int h)
{
#ifdef _WIN32
	// Framebuffer object
	glGenFramebuffers = 0;                      // FBO name generation procedure
	glDeleteFramebuffers = 0;                   // FBO deletion procedure
	glBindFramebuffer = 0;                      // FBO bind procedure
	glCheckFramebufferStatus = 0;               // FBO completeness test procedure
	glGetFramebufferAttachmentParameteriv = 0;  // return various FBO parameters
	glGenerateMipmap = 0;                       // FBO automatic mipmap generation procedure
	glFramebufferTexture2D = 0;                 // FBO texdture attachement procedure
	glFramebufferRenderbuffer = 0;              // FBO renderbuffer attachement procedure
	// Renderbuffer object
	glGenRenderbuffers = 0;                     // renderbuffer generation procedure
	glDeleteRenderbuffers = 0;                  // renderbuffer deletion procedure
	glBindRenderbuffer = 0;                     // renderbuffer bind procedure
	glRenderbufferStorage = 0;                  // renderbuffer memory allocation procedure
	glGetRenderbufferParameteriv = 0;           // return various renderbuffer parameters
	glIsRenderbuffer = 0;       
#endif

	fboId = 0;
	color_rboId = 0;		// 颜色
	depth_stencil_rboId = 0;
	textureId = 0;	
		 
	width = w;
	height = h;
}

void OpenGLFrameBuffer::doInit(bool fboSupported)
{
	if (!fboSupported)
	{
		return;
	}

	const int targetWidth = width;
	const int targetHeight = height;

#ifdef _WIN32

	// check if FBO is supported by your video card
	// get pointers to GL functions
	glGenFramebuffers                     = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
	glDeleteFramebuffers                  = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
	glBindFramebuffer                     = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
	glCheckFramebufferStatus              = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
	glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
	glGenerateMipmap                      = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
	glFramebufferTexture2D                = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
	glFramebufferRenderbuffer             = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
	glGenRenderbuffers                    = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
	glDeleteRenderbuffers                 = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
	glBindRenderbuffer                    = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
	glRenderbufferStorage                 = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
	glGetRenderbufferParameteriv          = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)wglGetProcAddress("glGetRenderbufferParameteriv");
	glIsRenderbuffer                      = (PFNGLISRENDERBUFFERPROC)wglGetProcAddress("glIsRenderbuffer");

	// check once again FBO extension
	if(glGenFramebuffers && glDeleteFramebuffers && glBindFramebuffer && glCheckFramebufferStatus &&
		glGetFramebufferAttachmentParameteriv && glGenerateMipmap && glFramebufferTexture2D && glFramebufferRenderbuffer &&
		glGenRenderbuffers && glDeleteRenderbuffers && glBindRenderbuffer && glRenderbufferStorage &&
		glGetRenderbufferParameteriv && glIsRenderbuffer)
	{
		fboUsed = true;
		// std::cout << "Video card supports GL_ARB_framebuffer_object." << std::endl;
	}
	else
	{
		fboUsed = false;
		// std::cout << "Video card does NOT support GL_ARB_framebuffer_object." << std::endl;
	}

	// 创建fbo对象。。。
	glGenTextures(1, &color_rboId);
	glBindTexture(GL_TEXTURE_2D, color_rboId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, targetWidth, targetHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);

	glGenRenderbuffers(1, &depth_stencil_rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_rboId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, targetWidth, targetHeight);
	
	// Attach depth buffer to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_rboId);
	// Also attach as a stencil
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_stencil_rboId);

	// Attach color buffer to FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_rboId, 0);

	// check FBO status
	fboUsed = checkFramebufferStatus();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);	
			
#else 
	fboUsed = true;

	fbo_log_i("fbo doInit {{ error = %d", glGetError());

	glGenTextures(1, &color_rboId);
	glBindTexture(GL_TEXTURE_2D, color_rboId);			

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, targetWidth, targetHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenFramebuffersOES(1, &fboId);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, fboId);

	glGenRenderbuffersOES(1, &depth_rboId);

	glBindRenderbufferOES(GL_RENDERBUFFER_OES, depth_rboId);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, targetWidth, targetHeight);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depth_rboId);			

	glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, color_rboId, 0);

	// check FBO status
	fboUsed = checkFramebufferStatus();

	glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
#endif

	// change renderTarget
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#if defined(ES_ANDROID) || defined(__APPLE__)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#else
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	fbo_log_i("fbo doInit }} error = %d", glGetError());
}

void OpenGLFrameBuffer::doDestroy()
{
#ifdef _WIN32
	if (fboId != 0)
	{
		glDeleteFramebuffers(1, &fboId);
		fboId = 0;
	}

	if (depth_stencil_rboId != 0)
	{
		glDeleteRenderbuffers(1, &depth_stencil_rboId);
		depth_stencil_rboId = 0;
	}

#else // ES_ANDROID
	if (fboId != 0)
	{
		glDeleteFramebuffersOES(1, &fboId);
		fboId = 0;
	}

	if (depth_rboId != 0)
	{
		glDeleteRenderbuffersOES(1, &depth_rboId);
		depth_rboId = 0;
	}
#endif		
	unsigned int deleteList[2] = {0};
	unsigned int idx = 0;

	if (color_rboId != 0)
	{
		deleteList[idx++] = color_rboId;
		color_rboId = 0;
	}
	if (textureId != 0)
	{
		deleteList[idx++] = textureId;
		textureId = 0;
	}	

	if (idx > 0)
	{
		glDeleteTextures(idx, deleteList);
	}
}

///////////////////////////////////////////////////////////////////////////////
// check FBO completeness
///////////////////////////////////////////////////////////////////////////////
bool OpenGLFrameBuffer::checkFramebufferStatus()
{
#ifdef _WIN32
	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE:
		fbo_log("%s\n", "Framebuffer complete.\n");
		return true;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		fbo_log("%s\n", "[ERROR] Framebuffer incomplete: Attachment is NOT complete.");
		return false;			
	default:
		fbo_log("%s\n", "[ERROR] Framebuffer incomplete: Unknown error.");
		return false;
	}
#else  //ES_ANDROID
	// check FBO status
	GLenum status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
	switch(status)
	{
	case GL_FRAMEBUFFER_COMPLETE_OES:
		fbo_log("%s\n", "Framebuffer complete.\n");
		return true;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_OES:
		fbo_log("%s\n", "[ERROR] Framebuffer incomplete: Attachment is NOT complete.");
		return false;			
	default:
		fbo_log("%s\n", "[ERROR] Framebuffer incomplete: Unknown error.");
		return false;
	}
#endif

	return false;
}

void OpenGLFrameBuffer::enterFBO()
{
	// set the rendering destination to FBO
#ifdef _WIN32
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
#else
	{
		ProfileActor temp("bindFBO");
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, fboId);
	}
#endif

}

// NOTE:
void OpenGLFrameBuffer::exitFBO()
{
/*
	// change renderTarget
	glGenTextures(1, &color_rboId);
	glBindTexture(GL_TEXTURE_2D, color_rboId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#if defined(ES_ANDROID) || defined(__APPLE__)
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#else
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

#if defined(ES_ANDROID) || defined(__APPLE__)
	glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, color_rboId, 0);
#else
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_rboId, 0);
#endif
	*/

#ifdef _WIN32
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind
#else
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0); // unbind
#endif

}
