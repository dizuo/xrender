// @module:		map-render/road_composer
// @file:		road_composer.h
// @author:		peteryfren
// @date:		2013/3/12
// @version:	1.0

// @NOTE: 
//	1. 优化纹理贴图，alpha通道采用ps处理图像数据。
//	2. 描边效果需要两次绘制。

#ifndef ROAD_COMPOSER_201303121140_H
#define ROAD_COMPOSER_201303121140_H

#include "vec2.hpp"

#include "common.h"

#include <vector>

class RoadComposer
{	
public:
	typedef gtl::Vec2f VecType;

	enum
	{
		EXTRUDE_SUCCESS = 0,
		EXTRUDE_FAIL = 1
	};

	enum	// 折线每个LINE的参数定义
	{
		LINE_MIN_VERT_SIZE = 6,		// 前面部分每段 6个顶点
		LINE_MAX_VERT_SIZE = 10,	// 最后一段 10个顶点，right_cap占四个顶点
		LINE_MIN_TRIANGLE_SIZE = 4,		// 前面部分每段 4个三角形
		LINE_MAX_TRIANGLE_SIZE = 6,		// 最后一段6个三角形
		INDEX_SIZE_PER_TRIANGLE = 3		// 每个三角形3个索引
	};

	enum CAP_TYPE
	{
		CAP_LEFT = 0,
		CAP_RIGHT = 1,
		CAP_BOTH = 2
	};

public:

	static void evaluate_memory(int pnt_size, int& vertex_size, int& index_size);

	// framework
	static int extrude_lines(const Point* point_list, int point_num,
						int width, float (&tex_coord)[4], 
						V2F2F* vertex_page, int vertex_size,
						IType* index_page, int index_size,
						int index_off);

private:

	static int _generate_trianges_indices(int line_size, int index_size, 
									IType* index_page, int index_off=0);

	static void _generate_triangle_texCoord(int line_size, int vertex_size, 
									V2F2F* vertex_page, float (&tex_coord)[4] );

private:

	RoadComposer(const RoadComposer& obj);
	RoadComposer& operator=(const RoadComposer& obj);

};

#endif	// ROAD_COMPOSER_201303121140_H