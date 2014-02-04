// @module:		map-render/common
// @file:		common.h
// @author:		peteryfren
// @date:		2013/3/2
// @version:	1.0

#ifndef COMMON_201303021701_H
#define COMMON_201303021701_H

#include "box2.hpp"

#include "color.hpp"

#include <vector>

//=====================================================================
// for 2D
struct Point
{
	int x;
	int y;
};

struct Bitmap
{
	int width;
	int height;
	unsigned char* buffer;
};

//=====================================================================
// for opengl

struct V3F
{
	float xyz[3];
};

struct V2F
{
	float xy[2];
};

struct V2F2F
{
	float xy[2];
	float uv[2];
};

typedef unsigned short IType;

struct V2FPolygon
{
	float xy[2];
};

//=====================================================================
// for tile_file_loader

struct Polygon
{
	std::vector<Point> contour;
	gtl::Box2i bbox;
};

struct MapRegionLayer
{
	Color4ub render_color;
	std::vector<Polygon> polygons;
};

typedef std::vector<Point> cPolyline;

struct MapRoadLayer
{
	Color4ub render_color;
	int render_width;
	std::vector<cPolyline> polylines;
};

//=====================================================================
// for global

enum 
{
	GET_SUCCESS = 0,
	GET_FAIL = 1
};

#define SAFE_DELETE_ARRAY(arr) { if(arr) { delete[] arr; arr=0; } }

#define SAFE_DELETE_PTR(ptr) { if(ptr) { delete ptr; ptr=0; } }

#define ASSIGN_2F_ARRAY( left_ptr, right_ptr ) { *left_ptr = *right_ptr; *(left_ptr+1) = *(right_ptr+1); }

#define FILL_2F_ARRAY( ptr, a,b ) { *(ptr) = (a); *(ptr+1) = (b); }

#define ASSIGN_3F_ARRAY( left_ptr, right_ptr ) { *left_ptr = *right_ptr; *(left_ptr+1) = *(right_ptr+1); *(left_ptr+2) = *(right_ptr+2); }

#define ASSIGN_POINT( point, a, b ) { point.x = (a); point.y = (b); }

#define MAX(a, b) ( (a) > (b) ? (a) : (b) )

#define MIN(a, b) ( (a) > (b) ? (b) : (a) )

#endif // COMMON_201303021701_H