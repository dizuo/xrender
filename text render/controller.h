#ifndef CONTROLLER_H
#define CONTROLLER_H

typedef union Vec3f
{
	float v[3];
	struct 
	{
		float x;
		float y;
		float z;
	};
} vec3f_t;

typedef union Vec2f
{
	struct 
	{
		float x;
		float y;
	};
	float v[2];
} vec2f_t;

void make_vec2(vec2f_t& vec, float x, float y);
void make_vec3(vec3f_t& vec, float x, float y, float z);

typedef struct Item
{
	float pos[2];
	float size[2];

	float height;
} item_t;

class BatchRender
{
public:
	BatchRender() : vert_id(0), pvert_cache(0)
	{
		color[0] = 1.0f;
		color[1] = color[2] = 0.0f;
		color[3] = 1.0f;
	}

	~BatchRender() 
	{
		if (pvert_cache)
			delete [] pvert_cache;
		pvert_cache = 0;
		vert_id = 0;
	}

	void add_tri_to_cache(vec3f_t& p1, vec3f_t& p2, vec3f_t& p3);

	void flush_tri_cache();

	void init();

	void set_color(float c[4]) { 
		for (int k = 0; k < 4; k++) color[k] = c[k];
	}

private:

	vec3f_t* pvert_cache;
	int vert_id;

	float color[4];

};

class Controller
{
public:
	
	Controller()
	{}
	
	~Controller() 
	{
	}

	void product_items(item_t* & arr, int& size);

	void render_items(item_t* arr, int size);

	float get_rand_height();

private:

	BatchRender top_render;
	BatchRender odd_render;
	BatchRender even_render;

};

#endif