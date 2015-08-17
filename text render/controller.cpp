// -------------------------------------------------------------------------------------
// 没开启深度测试，面的相对关系会比较奇怪。
// -------------------------------------------------------------------------------------

#include "controller.h"

#include <GL\glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TEST 0

#define MAX_CACHE_TRI_NUM	(300)

void make_vec2(vec2f_t& vec, float x, float y)
{
	vec.x = x; vec.y = y;
}	

void make_vec3(vec3f_t& vec, float x, float y, float z)
{
	vec.x = x; vec.y = y; vec.z = z;
}

// -------------------------------------------------------------------------------------
// BatchRender
// -------------------------------------------------------------------------------------
void BatchRender::init()
{
	if (pvert_cache == 0)
	{
		pvert_cache = new vec3f_t[MAX_CACHE_TRI_NUM];		
	}
	vert_id = 0;
}

void BatchRender::flush_tri_cache()
{
	// float verts[] = {0,0,0, 1,0,0, 0,0,1};
	// glVertexPointer(3, GL_FLOAT, 0, verts);
	// glDrawArrays(GL_TRIANGLES, 0, 3);

	// printf("flush triangle cache\n");

	glColor4fv(color);

	glVertexPointer(3, GL_FLOAT, 0, pvert_cache);
	glDrawArrays(GL_TRIANGLES, 0, vert_id);

	vert_id = 0;
}

void BatchRender::add_tri_to_cache(vec3f_t& p1, vec3f_t& p2, vec3f_t& p3)
{
	if (vert_id >= MAX_CACHE_TRI_NUM)
	{
		flush_tri_cache();
	}

	pvert_cache[vert_id] = p1;
	pvert_cache[vert_id + 1] = p2;
	pvert_cache[vert_id + 2] = p3;

	vert_id += 3;
}

// -------------------------------------------------------------------------------------
// Controller.
// -------------------------------------------------------------------------------------

float Controller::get_rand_height()
{
#if TEST
	return 3.0f;
#endif

	float fmin = 1.0f;
	float fmax = 5.0f;

	int val = rand() / 100;
	return fmin + val / 100.0f * (fmax-fmin);
}

void Controller::product_items(item_t* & arr, int& size)
{
	srand(time(NULL));

	const float kStep = 2;	// Width of box.
	const float kRadius = kStep / 2.0f;

#if TEST
	// size = 1;
	// int xnum = 1;
	size = 4;
	int xnum = 2;
#else
	size = 6400;
	int xnum = 80;
#endif

	float left = -xnum/2 * kStep + kRadius;
	float top = -xnum/2 * kStep + kRadius;

	float margin_factor = 1.0f;

	if (arr)
		delete[] arr;

	arr = new item_t[size];
	int idx = 0;

	for (int k = 0; k < size; k++)
	{
		int seedx = k % xnum;
		int seedy = k / xnum;

		item_t& item = *(arr + idx);
		item.pos[0] = left + seedx * kStep;
		item.pos[1] = top + seedy * kStep;
		item.size[0] = item.size[1] = kStep * margin_factor;
		item.height = get_rand_height();

		idx += 1;
	}

}

void Controller::render_items(item_t* item_arr, int item_size)
{
	float top_c[4] = {0.9f, 0.9f, 0.9f, 1.0f};
	float even_c[4] = {0.8f, 0.8f, 0.8f, 1.0f};
	float odd_c[4] = {0.5f, 0.5f, 0.5f, 1.0f};
	
	top_render.init();
	even_render.init();
	odd_render.init();

	top_render.set_color(top_c);
	even_render.set_color(even_c);
	odd_render.set_color(odd_c);

	for (int iter = 0; iter < item_size; iter++)
	{
		vec2f_t pos;
		make_vec2(pos, item_arr[iter].pos[0], item_arr[iter].pos[1]);
		
		vec2f_t step;
		make_vec2(step, item_arr[iter].size[0] / 2.0f, item_arr[iter].size[1] / 2.0f);

		float height = item_arr[iter].height;

		vec3f_t p1, p2, p3, p4;
		make_vec3(p1, pos.v[0] - step.v[0], 0, pos.v[1] - step.v[1]);
		make_vec3(p2, pos.v[0] - step.v[0], 0, pos.v[1] + step.v[1]);
		make_vec3(p3, pos.v[0] + step.v[0], 0, pos.v[1] + step.v[1]);
		make_vec3(p4, pos.v[0] + step.v[0], 0, pos.v[1] - step.v[1]);

		vec3f_t t1 = p1, t2 = p2, t3 = p3, t4 = p4;
		t1.y = t2.y = t3.y = t4.y = height;

		// bottom
		// _add_tri_to_cache(p1, p2, p4);
		// _add_tri_to_cache(p2, p3, p4);

		// top
		top_render.add_tri_to_cache(t1, t2, t4);
		top_render.add_tri_to_cache(t2, t3, t4);

		// left
		even_render.add_tri_to_cache(p1, t1, t2);
		even_render.add_tri_to_cache(p1, t2, p2);

		// front
		odd_render.add_tri_to_cache(p2, p3, t3);
		odd_render.add_tri_to_cache(p2, t3, t2);
	
		// right
		even_render.add_tri_to_cache(p3, t4, t3);
		even_render.add_tri_to_cache(p3, p4, t4);

		// back
		odd_render.add_tri_to_cache(p1, p4, t4);
		odd_render.add_tri_to_cache(p1, t4, t1);
	}

	// 
	// draw triangles in cache.
	top_render.flush_tri_cache();
	even_render.flush_tri_cache();
	odd_render.flush_tri_cache();
}

