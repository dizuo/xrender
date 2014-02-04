// @module:		map-render/batch_render
// @file:		batch_render.h
// @author:		peteryfren
// @date:		2013/3/10
// @version:	1.0

#ifndef BATCH_RENDER_201303101949_H
#define BATCH_RENDER_201303101949_H

// 任何绘制都是以GL_TRIANGLES形式，顶点、索引

#include "common.h"

struct RectRenderHandler
{
	void operator()(float* v_list, int v_cnt, unsigned short* i_list, int i_cnt, unsigned char* color);
};

struct V2FRenderHandler
{
	typedef V2F VecType;

	// draw with position
	void operator()(VecType* v_list, int v_cnt, unsigned short* i_list, int i_cnt, unsigned int);
};

struct V2F2FRenderHandler
{
	typedef V2F2F VecType;

	// draw with position, texture
	void operator()(VecType* v_list, int v_cnt, unsigned short* i_list, int i_cnt, unsigned int);
};

struct V2FPolygonRenderHandler
{
	typedef V2FPolygon VecType;

	void operator()(VecType* v_list, int v_cnt, unsigned char* color);
};

template<class VType, class RenderHandler>
class BatchRender
{
public:

	enum	// 批处理阈值
	{
		SECTION_SIZE = 2,
		INDEX_SIZE_EACH_SECTION = 6,
		BATCH_VERTEX_THRESHOLD = 1000,
		BATCH_INDEX_THRESHOLD = (BATCH_VERTEX_THRESHOLD/SECTION_SIZE-1)*INDEX_SIZE_EACH_SECTION,
	};

	BatchRender()
		: _vertex_cache(NULL)
		, _vertex_count(0)
		, _index_cache(NULL)
		, _index_count(0)
		, _texture_id(0)
	{
		_vertex_cache = new VType[BATCH_VERTEX_THRESHOLD];
		_index_cache = new IType[BATCH_INDEX_THRESHOLD];
	}

	~BatchRender()
	{
		SAFE_DELETE_ARRAY(_vertex_cache);
		SAFE_DELETE_ARRAY(_index_cache);
	}

	// 返回当前顶点个数
	int get_vertex_count() const { return _vertex_count; }
	int& get_vertex_count() { return _vertex_count; }

	int get_index_count() const { return _index_count; }
	int& get_index_count() { return _index_count; }

	// @vert_cnt：需要的顶点个数
	// @vertex_page：足够容纳所需顶点个数的内存空间
	// @max_page_size：返回内存空间的大小，防止越界访问
	int get_vertex_page(int vert_cnt, VType* & vertex_page, int& max_page_size)
	{
		reserve(vert_cnt);

		vertex_page = _vertex_cache + _vertex_count;
		max_page_size = BATCH_VERTEX_THRESHOLD - _vertex_count;

		return GET_SUCCESS;
	}

	// @indx_cnt：需要的索引个数
	// @index_page：足够容纳所需索引个数的内存空间
	// @max_page_size：返回内存空间的大小，防止越界访问
	int get_indice_page(int indx_cnt, IType* & index_page, int& max_page_size)
	{
		if (indx_cnt > BATCH_INDEX_THRESHOLD-_index_count)
		{
			return GET_FAIL;
		}

		index_page = _index_cache + _index_count;
		max_page_size = BATCH_INDEX_THRESHOLD - _index_count;

		return GET_SUCCESS;
	}

	// make sure vert_cnt memories.
	void reserve(int vert_cnt)
	{
		if ( !is_memory_enough(vert_cnt) )
		{
			do_flush();
		}
	}

	void do_flush() 
	{
		RenderHandler handler;
		handler(_vertex_cache, _vertex_count, _index_cache, _index_count, _texture_id);

		_index_count = 0;
		_vertex_count = 0;
	}

	bool is_memory_enough(int vert_cnt)
	{
		return (_vertex_count + vert_cnt <= BATCH_VERTEX_THRESHOLD);
	}

	void set_texture_id(unsigned int tex_id) 
	{
		_texture_id = tex_id;
	}
private:

	VType* _vertex_cache;
	IType* _index_cache;

	int _vertex_count;
	int _index_count;

	unsigned int _texture_id;
};

#endif // BATCH_RENDER_201303101949_H
