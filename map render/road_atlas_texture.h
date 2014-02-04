// @module:		map-render/road_atlas_texture
// @file:		road_atlas_texture.h
// @author:		peteryfren
// @date:		2013/3/6
// @version:	1.0

// @纹理优化: 创建一个512*512纹理，其中每个32*32瓦片代表一个道理纹理。
//			前16*32是道路帽子纹理，后16*32是道路纹理

#ifndef ROAD_ATLAS_TEXTURE_201303062206_H
#define ROAD_ATLAS_TEXTURE_201303062206_H

#include "color.hpp"

#include "vec2.hpp"
#include <vector>

struct TileNode 
{
	TileNode(const Color4ub& c, int no)
		: c4(c), tile_no(no)
	{}

	TileNode(const TileNode& node)
		: c4(node.c4), tile_no(node.tile_no)
	{}

	TileNode& operator= (const TileNode& node)
	{
		c4 = node.c4;
		tile_no = node.tile_no;

		return *this;
	}

	Color4ub c4;
	int tile_no;
};

class RoadAtlasTexture
{
	unsigned int _texture_id;
	std::vector< TileNode > _tile_cache;

public:

	enum 
	{
		TEX_WIDTH = 512,
		TEX_HEIGHT = 512,
 		TILE_WIDTH = 32,
 		TILE_HEIGHT = 32,
		TILE_X_SIZE = TEX_WIDTH / TILE_WIDTH,
		TILE_Y_SIZE = TEX_HEIGHT / TILE_HEIGHT
	};

	RoadAtlasTexture();

	~RoadAtlasTexture();

	void generate_texture(unsigned char (&color)[3]);

	int query_tile_texture_coord(const Color4ub& c4, float (&tex_coord)[4]);

	unsigned int get_texture_id() { return _texture_id; }

	bool is_full() const { return _tile_cache.size() == (TEX_WIDTH/TILE_WIDTH)*(TEX_HEIGHT/TILE_HEIGHT); }
	
private:

	void _generate_tile(const Color4ub& c4, float (&tex_coord)[4]);
};

#endif // ROAD_TEXTURE_201303062206_H