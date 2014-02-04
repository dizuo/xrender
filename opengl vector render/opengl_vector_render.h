// @author: peteryfren
// @date: 2013/2/28
// @version 1.0

#ifndef OPENGL_VECTOR_RENDER_201302282116_H
#define OPENGL_VECTOR_RENDER_201302282116_H

struct Color4
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;
};

struct Point
{
	int x;
	int y;
};

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
		CACHE_THRESHOLD = 1000		// threshold
	};

	OpenGLVectorRender(int width, int height);

	~OpenGLVectorRender();

	void set_pen_style(PEN_STYLE style, int width);

	void set_pen_color(Color4 color);

	void set_brush_color(Color4 color);

	void render_line(Point beg_pt, Point end_pt);
	
	void render_polyline(Point* point_lisy, int point_num);

	void render_polygon(Point* polygon_contour, int point_num);
	
	void render_rectangle(Point lt_corner, Point rb_corner);

	void flush();

	void begin_render();

	void end_render();

	unsigned int get_rendered_texture();

private:

	void _deferred_render();		// —”≥Ÿ‰÷»æ
	
	Color4 _pen_color;
	Color4 _brush_color;
	PEN_STYLE _pen_style;

	short* _line_vertex_cache;
	int _line_vertex_num;
	int* _line_index_cache;
	int _line_index_num;

	short* _polygon_cache;
	int _polygon_vertex_num;
	int* _polygon_index_cache;
	int _polygon_index_num;
		
private:

	typedef OpenGLVectorRender _type;

	_type(const _type& obj);
	
	_type operator=(const _type& obj);

};

#endif