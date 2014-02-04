// @module:		map-render/road_composer
// @file:		road_composer.h
// @author:		peteryfren
// @date:		2013/3/12
// @version:	1.0

#include "road_composer.h"

#include <gl/glut.h>

//==========================
//	NW	N			N	NE
//	  \	|			|  /
//		a-----------b
//	  /	|			|  \
//	SW	S			S	SE
//==========================

// 顶点位置分布
//	y============================
//	|	NW	N			N	NE
//	| 				 
//	|		a-----------b
//	| 				  
//	|	SW	S			S	SE
// (0,0)-------------------x
//===============================

void RoadComposer::evaluate_memory(int pnt_size, int& vertex_size, int& index_size)
{
	int line_size = pnt_size - 1;

	// 每个线段只绘制left_cap，最后一段加上right_cap
	// vertex_size = line_size*6 + 4;
	vertex_size = (line_size-1)*LINE_MIN_VERT_SIZE + LINE_MAX_VERT_SIZE;

	index_size = INDEX_SIZE_PER_TRIANGLE *
		( (line_size-1)*LINE_MIN_TRIANGLE_SIZE + LINE_MAX_TRIANGLE_SIZE );
}

int RoadComposer::extrude_lines(const Point* point_list, int point_num, int width, float (&tex_coord)[4], 
							  V2F2F* vertex_page, int vertex_size,
							  IType* index_page, int index_size,
							  int index_off)
{
	if (point_num < 2)
	{
		return EXTRUDE_FAIL;
	}
	int line_size = point_num - 1;
	
	int v_cursor = 0;

	for (int idx=0; idx < line_size; idx++)
	{
		const Point& p0 = point_list[idx];
		const Point& p1 = point_list[idx+1];
		
		VecType a(p0.x, p0.y);
		VecType b(p1.x, p1.y);

		VecType e = (b-a);
		e.normalize();
		e *= width;

		VecType N = VecType(-e.y(), e.x());
		VecType S = -N;
		VecType NE = N + e;
		VecType NW = N - e;

		VecType SW = -NE;
		VecType SE = -NW;

		if (v_cursor + LINE_MAX_VERT_SIZE > vertex_size)
		{
			return EXTRUDE_FAIL;			
		}

		ASSIGN_2F_ARRAY( vertex_page[v_cursor].xy, VecType(a + SW).getValue() );
		v_cursor++;

		ASSIGN_2F_ARRAY( vertex_page[v_cursor].xy, VecType(a + NW).getValue() );
		v_cursor++;

		ASSIGN_2F_ARRAY( vertex_page[v_cursor].xy, VecType(a + S).getValue() );
		v_cursor++;

		ASSIGN_2F_ARRAY( vertex_page[v_cursor].xy, VecType(a + N).getValue() );
		v_cursor++;

		ASSIGN_2F_ARRAY( vertex_page[v_cursor].xy, VecType(b + S).getValue() );
		v_cursor++;

		ASSIGN_2F_ARRAY( vertex_page[v_cursor].xy, VecType(b + N).getValue() );
		v_cursor++;

		if (idx==line_size-1)
		{
			// 最后一段增加尾部
			ASSIGN_2F_ARRAY( vertex_page[v_cursor].xy, VecType(b + S).getValue() );
			v_cursor++;

			ASSIGN_2F_ARRAY( vertex_page[v_cursor].xy, VecType(b + N).getValue() );
			v_cursor++;

			ASSIGN_2F_ARRAY( vertex_page[v_cursor].xy, VecType(b + SE).getValue() );
			v_cursor++;

			ASSIGN_2F_ARRAY( vertex_page[v_cursor].xy, VecType(b + NE).getValue() );
			v_cursor++;
		}
	}
	
	_generate_triangle_texCoord(line_size, vertex_size, vertex_page, tex_coord);

	return _generate_trianges_indices(line_size, index_size, index_page, index_off);;
}

// 顶底编号
//	s===========================
//	|	1	3			5_7	9
//	|
//	|	0	2			4_6	8	
// (0,0)-------------------x
//==============================
int RoadComposer::_generate_trianges_indices(int line_size, int index_size,
											  IType* index_page, int index_off)
{
	int line_cnt = 0;
	IType* i_cursor = index_page;

	for (GLushort v = index_off; i_cursor < index_page+index_size; v += 6) 
	{
		if (i_cursor + LINE_MAX_VERT_SIZE > index_page+index_size)
		{
			return EXTRUDE_FAIL;
		}
		
		// CCW order
		//*i_cursor++ = 0 + v; *i_cursor++ = 2 + v; *i_cursor++ = 1 + v;
		//*i_cursor++ = 2 + v; *i_cursor++ = 3 + v; *i_cursor++ = 1 + v;
		//*i_cursor++ = 2 + v; *i_cursor++ = 4 + v; *i_cursor++ = 3 + v;
		//*i_cursor++ = 4 + v; *i_cursor++ = 5 + v; *i_cursor++ = 3 + v;

		// CW order
		*i_cursor++ = 0 + v; *i_cursor++ = 1 + v; *i_cursor++ = 2 + v;
		*i_cursor++ = 2 + v; *i_cursor++ = 1 + v; *i_cursor++ = 3 + v;
		*i_cursor++ = 2 + v; *i_cursor++ = 3 + v; *i_cursor++ = 4 + v;
		*i_cursor++ = 4 + v; *i_cursor++ = 3 + v; *i_cursor++ = 5 + v;

		if (line_size-1 == line_cnt)
		{
			// CCW
			//*i_cursor++ = 6 + v; *i_cursor++ = 8 + v; *i_cursor++ = 7 + v;
			//*i_cursor++ = 8 + v; *i_cursor++ = 9 + v; *i_cursor++ = 7 + v;	

			// CW
			*i_cursor++ = 6 + v; *i_cursor++ = 7 + v; *i_cursor++ = 8 + v;
			*i_cursor++ = 8 + v; *i_cursor++ = 7 + v; *i_cursor++ = 9 + v;	

			v += 4;
		}

		line_cnt++;
	}

	return EXTRUDE_SUCCESS;
}

// 纹理坐标分布
//=====u=========================
//max_y |
//		|
//mid_y	|		
//		|	
//min_y	| 				  
//	  (0,0)-------------------v
//		min_x	mid_x	max_x
//===============================
void RoadComposer::_generate_triangle_texCoord(int line_size, int vertex_size, 
											   V2F2F* vertex_page, float (&tex_coord)[4])
{
	int v_cursor = 0;
	int line_cnt = 0;

	float margin = 0;
	float min_x = tex_coord[0];
	float min_y = tex_coord[1];
	float max_x = tex_coord[2];
	float max_y = tex_coord[3];
	float mid_x = (min_x + max_x)/2.0f;
	float mid_y = (min_y + max_y)/2.0f;

	while (v_cursor != vertex_size) 
	{
		FILL_2F_ARRAY( vertex_page[v_cursor].uv, min_x + margin, min_y + margin );
		v_cursor++;
		FILL_2F_ARRAY( vertex_page[v_cursor].uv, min_x + margin, max_y - margin );
		v_cursor++;
		FILL_2F_ARRAY( vertex_page[v_cursor].uv, mid_x, min_y + margin );
		v_cursor++;
		FILL_2F_ARRAY( vertex_page[v_cursor].uv, mid_x, max_y - margin );
		v_cursor++;

		FILL_2F_ARRAY( vertex_page[v_cursor].uv, mid_x, min_y + margin );
		v_cursor++;
		FILL_2F_ARRAY( vertex_page[v_cursor].uv, mid_x, max_y - margin );
		v_cursor++;

		// 复用前面部分 半圆
		if ( line_size-1 == line_cnt )
		{
			FILL_2F_ARRAY( vertex_page[v_cursor].uv, mid_x + margin, min_y + margin );
			v_cursor++;
			FILL_2F_ARRAY( vertex_page[v_cursor].uv, mid_x + margin, max_y - margin );
			v_cursor++;

			FILL_2F_ARRAY( vertex_page[v_cursor].uv, min_x + margin, min_y + margin );
			v_cursor++;
			FILL_2F_ARRAY( vertex_page[v_cursor].uv, min_x + margin, max_y - margin );
			v_cursor++;
		}

		line_cnt++;
	}
}