#include "controller.h"

#include <GL\glut.h>

#define MAX_CACHE_TRI_NUM	(300)

void make_vec2(vec2f_t& vec, float x, float y)
{
	vec.x = x; vec.y = y;
}	

void make_vec2(vec3f_t& vec, float x, float y, float z)
{
	vec.x = x; vec.y = y; vec.z = z;
}

void Controller::render_items(item_t* item_arr, int item_size)
{
	if (pvert_cache == 0)
	{
		pvert_cache = new vec3f_t[MAX_CACHE_TRI_NUM];		
	}

	vert_id = 0;

	for (int iter = 0; iter < item_size; iter++)
	{
		vec2f_t pos;
		make_vec2(pos, item_arr[iter].pos[0], item_arr[iter].pos[1]);
		
		vec2f_t size;
		make_vec2(size, item_arr[iter].size[0], item_arr[iter].size[1]);

		float height = item_arr[iter].height;

		vec3f_t p1, p2, p3, p4;


	}

	// 
	// draw triangles in cache.

}

void Controller::_flush_tri_cache()
{
	// float verts[] = {0,0,0, 1,0,0, 0,0,1};
	// glVertexPointer(3, GL_FLOAT, 0, verts);
	// glDrawArrays(GL_TRIANGLES, 0, 3);

	glVertexPointer(3, GL_FLOAT, 0, pvert_cache);
	glDrawArrays(GL_TRIANGLES, 0, vert_id / 3);
}

void Controller::_add_tri_to_cache(vec3f_t& p1, vec3f_t& p2, vec3f_t& p3)
{
	if (vert_id >= MAX_CACHE_TRI_NUM)
	{
		_flush_tri_cache();
	}
	
	pvert_cache[vert_id] = p1;
	pvert_cache[vert_id + 1] = p2;
	pvert_cache[vert_id + 2] = p3;

	vert_id += 3;
}

