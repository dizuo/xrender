// @module:		map-render/concave polygon render
// @file:		concave_polygon_render.h
// @author:		peteryfren
// @date:		2013/3/13
// @version:	1.0
// @desc:		render concave polygon with OpenGL stencil buffer test.

#ifndef CONCAVE_POLYGON_RENDER_201301272128_H
#define CONCAVE_POLYGON_RENDER_201301272128_H

#include "box2.hpp"
#include "color.hpp"

class ConcavePolygonRender
{
public:
	enum 
	{
		MAX_VERTEX_NUM = 100
	};

	ConcavePolygonRender();
	
	~ConcavePolygonRender();

	void render_handler(float* vertex_list, int vertex_num, const Color4ub& top_color,
		const Color4ub& bot_color, const gtl::Box2i& bbox);

};

#endif // CONCAVE_POLYGON_RENDER_201301272128_H