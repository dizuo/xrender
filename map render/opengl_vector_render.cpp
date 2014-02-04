// @module:		map-render/opengl vector render
// @file:		opengl_vector_render.h
// @author:		peteryfren
// @date:		2013/2/28
// @version:	1.0
// @desc:		采用设备坐标系，原点左上角，X向右，Y向下

#include "opengl_vector_render.h"

#include "batch_render.hpp"
#include "road_atlas_texture.h"
#include "road_composer.h"

#include "concave_polygon_render.h"

#include "box2.hpp"

namespace my_private
{
	void compute_bbox(Point* point_list, int point_num, int (&bbox)[4])
	{
		int idx = 0;
		
		bbox[0] = bbox[2] = point_list[0].x;
		bbox[1] = bbox[3] = point_list[0].x;
		
		for (int idx=1; idx<point_num; idx++)
		{
			int curX = point_list[idx].x;
			int curY = point_list[idx].y;

			bbox[0] = MIN(curX, bbox[0]);
			bbox[1] = MIN(curY, bbox[1]);

			bbox[2] = MAX(curX, bbox[2]);
			bbox[3] = MAX(curY, bbox[3]);
		}
	}

	bool filter_road(Point* point_list, int point_num, const gtl::Box2i& window_rect)
	{
		gtl::Box2i bbox;
		int bbox_buf[4];
		my_private::compute_bbox(point_list, point_num, bbox_buf);

		bbox.extendBy( gtl::Vec2i(bbox_buf[0], bbox_buf[1]) );
		bbox.extendBy( gtl::Vec2i(bbox_buf[2], bbox_buf[3]) );

		return window_rect.intersect( bbox );
	}

	static const int BUFFER_SIZE = 200;
	static float g_contour[BUFFER_SIZE];
}

OpenGLVectorRender::OpenGLVectorRender(int width, int height)
	: _road_texture(NULL)
	, _road_batch_render(NULL)
	, _pen_style(PEN_NORMAL), _pen_color(), _pen_width(2)
	, _brush_color(), _region_render(NULL)
{
	_window_rect.extendBy( gtl::Vec2i(0,0) );
	_window_rect.extendBy( gtl::Vec2i(width,height) );
}

OpenGLVectorRender::~OpenGLVectorRender()
{
	SAFE_DELETE_PTR( _road_texture );
	SAFE_DELETE_PTR( _road_batch_render );
	SAFE_DELETE_PTR( _region_render );
}

void OpenGLVectorRender::set_pen_color(Color4ub color)
{
	_pen_color = color;
}

void OpenGLVectorRender::set_pen_style(OpenGLVectorRender::PEN_STYLE style, int width)
{
	_pen_style = style;
	_pen_width = MAX(PEN_MIN_WIDTH, width);
}

void OpenGLVectorRender::set_brush_color(Color4ub color)
{
	_brush_color = color;
}

void OpenGLVectorRender::render_line(const Point& beg_pt, const Point& end_pt)
{
	Point point_list[2];
	point_list[0] = beg_pt;
	point_list[1] = end_pt;
	
	render_polyline(point_list, 2);
}

void OpenGLVectorRender::render_polyline(Point* point_list, int point_num, bool have_cap)
{
	if ( !my_private::filter_road(point_list, point_num, _window_rect) )
	{
		return;
	}
	
	// defer construct RoadAtlasTexture
	if (NULL==_road_texture)
	{
		_road_texture = new RoadAtlasTexture();
		float tex_buffer[4] = {0};
		_road_texture->query_tile_texture_coord( Color4ub(255,0,0,0), tex_buffer);
	}

	// defer construct BatchRender
	if (NULL==_road_batch_render)
	{
		_road_batch_render = new BatchRender<V2F2F, V2F2FRenderHandler>();
		_road_batch_render->set_texture_id( _road_texture->get_texture_id() );
	}

	int index_size = 0;
	int max_vertex_size = 0;
	V2F2F* vertex_page = NULL;

	int vertex_size = 0;
	int max_index_size = 0;
	IType* index_page = NULL;

	float tex_buf[4] = {0};

	RoadComposer::evaluate_memory(point_num, vertex_size, index_size);

	if ( GET_FAIL==_road_texture->query_tile_texture_coord(_pen_color, tex_buf) )
	{
		return;
	}
	
	if ( GET_FAIL==_road_batch_render->get_vertex_page(vertex_size, vertex_page, max_vertex_size) )
	{
		return;
	}

	// pass write memory to RoadRender
	if ( GET_FAIL==_road_batch_render->get_indice_page(index_size, index_page, max_index_size) 
		 || index_size > max_index_size 
		 || vertex_size > max_vertex_size )
	{
		return;
	}
	
	int ret = RoadComposer::extrude_lines( point_list, point_num, _pen_width, 
									tex_buf, vertex_page, vertex_size,
									index_page, index_size,
									_road_batch_render->get_vertex_count() );

	if (RoadComposer::EXTRUDE_SUCCESS==ret)
	{
		_road_batch_render->get_vertex_count() += vertex_size;
		_road_batch_render->get_index_count() += index_size;
	}	
}

// left < right, top < bottom
void OpenGLVectorRender::render_rectangle(const Point& lt_corner, const Point& rb_corner)
{
	// x0,y0, x0,y1, x1,y0, x1,y1
	//	0	2
	//	1	3
	float v_list[] = {lt_corner.x,lt_corner.y, lt_corner.x,rb_corner.y, 
		rb_corner.x,lt_corner.y, rb_corner.x, rb_corner.y};
	unsigned short i_list[] = {0,1,2, 1,3,2};		// CCW

	RectRenderHandler rect_handler;
	rect_handler(v_list, 4, i_list, sizeof(i_list)/sizeof(i_list[0]), _brush_color.data());
}

void OpenGLVectorRender::render_polygon(Point* polygon_contour, int point_num)
{
	if (NULL==_region_render)
	{
		_region_render = new ConcavePolygonRender;
	}

	gtl::Box2i region_rect;
	
	point_num = MIN(my_private::BUFFER_SIZE, point_num);

	for (int iter=0; iter < point_num; iter++)
	{
		int pos = 2*iter;
		my_private::g_contour[pos] = (float)polygon_contour[iter].x;
		my_private::g_contour[pos+1] = (float)polygon_contour[iter].y;

		region_rect.extendBy( gtl::Vec2i(polygon_contour[iter].x, polygon_contour[iter].y) );
	}

	// region_filter
	if ( !_window_rect.intersect(region_rect) )
	{
		return;
	}

	// For debug, render polygon in line mode.
	// V2FPolygonRenderHandler handler;
	// handler( (V2FPolygon*)my_private::g_contour, point_num, _brush_color.data() );

	_region_render->render_handler(my_private::g_contour, point_num,
	_brush_color, _brush_color, region_rect);
}

// 将缓存中所有数据绘制并清空缓存
void OpenGLVectorRender::flush()
{
	if (_road_batch_render)
	{
		_road_batch_render->do_flush();
	}
}