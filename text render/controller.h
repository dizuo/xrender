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
void make_vec2(vec3f_t& vec, float x, float y, float z);

typedef struct Item
{
	float pos[2];
	float size[2];

	float height;
} item_t;

class Controller
{
public:
	
	Controller() : vert_id(0), pvert_cache(0)
	{}
	
	~Controller() 
	{
		if (pvert_cache)
			delete [] pvert_cache;
		pvert_cache = 0;
		vert_id = 0;
	}

	void render_items(item_t* arr, int size);

private:

	void _add_tri_to_cache(vec3f_t& p1, vec3f_t& p2, vec3f_t& p3);

	void _flush_tri_cache();

	vec3f_t* pvert_cache;
	int vert_id;

};

#endif