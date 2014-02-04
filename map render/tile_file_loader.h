// @module:		map-render/tile_file_loader
// @file:		tile_file_loader.h
// @author:		peteryfren
// @date:		2013/3/2
// @version:	1.0

#ifndef TILE_FILE_LOADER_201303021658_H
#define TILE_FILE_LOADER_201303021658_H

#include "common.h"

#include "opengl_vector_render.h"

class RoadRender;
class ConcavePolygonRender;
class RoadAtlasTexture;

class TileFileLoader
{
	MapRegionLayer _region_layers[100];
	int _region_layer_count;

	MapRoadLayer _road_layers[25];
	int _road_layer_count;

	bool _is_loaded;

public:

	TileFileLoader();

	~TileFileLoader();
	
	void load_dat_file(const char* file_name);
	
	bool is_loaded() const { return _is_loaded; }

	void render(RoadAtlasTexture& road_texture);

private:

	int _parse_region_layer(std::ifstream& tile_file);

	int _parse_road_layer(std::ifstream& tile_file);

	OpenGLVectorRender _ogl_renderer;

private:

	typedef TileFileLoader _type;
	_type(const _type& obj);
	_type operator=(const _type& obj);
};

#endif // TILE_FILE_LOADER_201303021658_H