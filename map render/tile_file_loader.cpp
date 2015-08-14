// @module:		map-render/tile_file_loader
// @file:		tile_file_loader.cpp
// @author:		peteryfren
// @date:		2013/3/2
// @version:	1.0

#include "tile_file_loader.h"
#include <fstream>
#include <string>
#include <sstream>

#include "road render/road_render.h"
#include "concave_polygon_render.h"
#include "road_atlas_texture.h"

#include <gl/glut.h>

#define USE_OPENGL_VECTOR_RENDERER

TileFileLoader::TileFileLoader()
	: _is_loaded(false)
	, _ogl_renderer(256, 256)
{}

TileFileLoader::~TileFileLoader()
{
}

// 原点位于左上角，Y下为正，X右为正
// top < bottom	
// left < right
void TileFileLoader::load_dat_file(const char* file_name)
{
	using namespace std;
	
	ifstream tile_file;
	tile_file.open(file_name, ifstream::in);

	if (!tile_file.good()) 
	{
		return;
	}

	_parse_region_layer(tile_file);

	_parse_road_layer(tile_file);

	_is_loaded = true;
}

int TileFileLoader::_parse_region_layer(std::ifstream& tile_file)
{
	using namespace std;

	string item;
	string line;

	_region_layer_count = 0;
	
	// 
	while ( getline(tile_file, line) )
	{
		if (line=="region {")
		{
			continue;
		}
		else if ( line== "}" )
		{
			_region_layer_count++;
			
			break;
		}

		MapRegionLayer& region_layer = _region_layers[_road_layer_count];

		istringstream str_cin(line);
		str_cin >> item;

		if (item=="color")
		{
			int r,g,b,a;
			str_cin >> r >> g >> b >> a; 

			region_layer.render_color.set_value(r,g,b,a);
		}
		else if ( item.compare(0, 6, "region")==0 )		//region		
		{
			Point point;
			Polygon polygon;
			while (str_cin >> point.x >> point.y)
			{
				polygon.contour.push_back( point );
			} // compute bbox

			region_layer.polygons.push_back( polygon );
		}
	}

	return 1;
}

int TileFileLoader::_parse_road_layer(std::ifstream& tile_file)
{
	using namespace std;

	string item;
	string line;

	_road_layer_count = 0;

	// 
	while ( getline(tile_file, line) )
	{
		if (line=="polyline {")
		{
			continue;
		}
		else if ( line== "}" )
		{
			_road_layer_count++;

			continue;
		}

		MapRoadLayer& road_layer = _road_layers[_road_layer_count];

		istringstream str_cin(line);
		str_cin >> item;

		if (item=="color")
		{
			int r,g,b,a;
			str_cin >> r >> g >> b >> a; 

			road_layer.render_color.set_value(r,g,b,a);
		}
		else if (item=="width")
		{
			str_cin >> road_layer.render_width;
		}
		else if ( item.compare(0, 8, "polyline")==0 )		//region		
		{
			Point point;
			cPolyline polyline;
			while (str_cin >> point.x >> point.y)
			{
				polyline.push_back( point );
			} // compute bbox

			road_layer.polylines.push_back( polyline );
		}
	}

	return 1;
}

void TileFileLoader::render(RoadAtlasTexture& road_texture)
{
	const int maxSize = 100;
	float contour_buffer[maxSize*2];
	
	for (int iter=0; iter < _region_layer_count; iter++)
	{
		MapRegionLayer& region_layer = _region_layers[iter];

		// render region
		for (int region_cursor=0; region_cursor!=region_layer.polygons.size(); region_cursor++)
		{
			Polygon& polygon = region_layer.polygons[region_cursor];

			int point_num = polygon.contour.size();

	#ifndef USE_OPENGL_VECTOR_RENDERER
			ConcavePolygonRender region_render;

			gtl::Box2i bbox;
			for (int iter=0; iter < polygon.contour.size(); iter++)
			{
				int pos = 2*iter;
				contour_buffer[pos] = polygon.contour[iter].x;
				contour_buffer[pos+1] = polygon.contour[iter].y;
				bbox.extendBy( gtl::Vec2i(polygon.contour[iter].x, polygon.contour[iter].y) );
			}

			region_render.render_handler(contour_buffer, point_num,
						_region.render_color, _region.render_color, bbox);
	#else
			/* std::vector<Point> temp_vec;
			for (int i=point_num-1; i >= 0; i--)
			{
				temp_vec.push_back( polygon.contour[i] );
			} */

			// region_layer.render_color.data()[3] = 200;
			_ogl_renderer.set_brush_color(region_layer.render_color);
			_ogl_renderer.render_polygon(&polygon.contour[0], point_num);

			// _ogl_renderer.render_polygon(&temp_vec[0], point_num);
	#endif
		}
	}

	std::vector<gtl::Vec3f> render_buffer;

	// render road
	for (int iter=0; iter != _road_layer_count; iter++)
	{
		MapRoadLayer& road_layer = _road_layers[iter];

		for (int poly_iter=0; poly_iter != road_layer.polylines.size(); poly_iter++)
		{
			cPolyline& polyline = road_layer.polylines[poly_iter];

			int point_num = polyline.size();

#ifndef USE_OPENGL_VECTOR_RENDERER

			for (int k=0; k<point_num; k++)
			{
				render_buffer.push_back( gtl::Vec3f(polyline[k].x, polyline[k].y, 0) );
			}

			RoadRender render;
			float tex_buf[4] = {0};

			if ( GET_SUCCESS==road_texture.query_tile_texture_coord(road_layer.render_color, tex_buf) )
			{
				// printf("%f, %f, %f, %f\n", tex_buf[0], tex_buf[1], tex_buf[2], tex_buf[3]);

				render.extrudeLines(render_buffer, road_layer.render_width/2, tex_buf);
				render.render(road_texture.get_texture_id());
			}

			render_buffer.clear();
#else
			// 描边效果
			_ogl_renderer.set_pen_style(OpenGLVectorRender::PEN_NORMAL, road_layer.render_width*3/4);
			_ogl_renderer.set_pen_color( Color4ub(128,128,128,255) );
			_ogl_renderer.render_polyline(&polyline[0], point_num);		

			_ogl_renderer.set_pen_style(OpenGLVectorRender::PEN_NORMAL, road_layer.render_width*2/3);
			_ogl_renderer.set_pen_color(road_layer.render_color);
			_ogl_renderer.render_polyline(&polyline[0], point_num);					
#endif
		}
	}

#ifdef USE_OPENGL_VECTOR_RENDERER
	_ogl_renderer.flush();
#endif
}