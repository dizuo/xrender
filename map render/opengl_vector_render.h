// @module:		map-render/opengl vector render
// @file:		opengl_vector_render.h
// @author:		peteryfren
// @date:		2013/2/28
// @version:	1.0
// @desc:		采用设备坐标系，原点左上角，X向右，Y向下

#ifndef OPENGL_VECTOR_RENDER_201302282116_H
#define OPENGL_VECTOR_RENDER_201302282116_H

#include "common.h"

#include "batch_render.hpp"

#include "box2.hpp"

// forward declaration
class RoadAtlasTexture;
class ConcavePolygonRender;

class OpenGLVectorRender
{
public:

	enum PEN_STYLE
	{
		PEN_NORMAL = 0,
		PEN_DASHED = 1
	};

	enum
	{
		PEN_MIN_WIDTH = 2
	};
		
	OpenGLVectorRender(int width, int height);

	~OpenGLVectorRender();

	void set_pen_color(Color4ub color);
	void set_pen_style(PEN_STYLE style, int width);

	void set_brush_color(Color4ub color);

	void render_line(const Point& beg_pt, const Point& end_pt);	
	void render_polyline(Point* point_list, int point_num, bool have_cap=true);

	void render_polygon(Point* polygon_contour, int point_num);
	void render_rectangle(const Point& lt_corner, const Point& rb_corner);

	// void render_bitmap(Bitmap* bmp, const gtl::Box2i& bmp_area, const gtl::Box2i& window_area);

	void flush();

	void begin_render();		// enter fbo environment.
	
	void end_render();			// leave fbo environment.
	
	unsigned int get_rendered_texture();
	
private:

	int _pen_width;
	Color4ub _pen_color;
	Color4ub _brush_color;
	PEN_STYLE _pen_style;

	gtl::Box2i _window_rect;

	RoadAtlasTexture* _road_texture;
	
	BatchRender<V2F2F, V2F2FRenderHandler>* _road_batch_render;

	ConcavePolygonRender* _region_render;

private:

	typedef OpenGLVectorRender _type;

	_type(const _type& obj);
	
	_type operator=(const _type& obj);

};

#endif