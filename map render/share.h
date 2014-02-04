#ifndef SHARE_201303141453_H
#define SHARE_201303141453_H

#include "opengl_vector_render.h"
#include "opengl_context.h"


// #include "data_type.h"

class Share
{
public:
	static OpenGLContext* get_context()
	{
		if (NULL==context)
		{
			context = new OpenGLContext(256, 256);
		}

		return context;
	}

	static void release_context()
	{
		if (context)
		{
			delete context;	context = NULL;
		}
	}

	static OpenGLVectorRender* get_render()
	{
		if (NULL==render)
		{
			render = new OpenGLVectorRender(256, 256);
		}

		return render;
	}

	static void release_render()
	{
		if (render)
		{
			delete render;	render = NULL;
		}
	}

/*
	static Color4ub transform_color(TXColor color_fill)
	{
		return Color4ub( TXColorGetR(color_fill),
			TXColorGetG(color_fill),
			TXColorGetB(color_fill),
			TXColorGetA(color_fill) );
	}*/


private:

	static OpenGLContext* context;
	static OpenGLVectorRender* render;
};

#endif	// SHARE_201303141453_H