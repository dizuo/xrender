// @module:		map-render/road_atlas_texture
// @file:		road_atlas_texture.cpp
// @author:		peteryfren
// @date:		2013/3/6
// @version:	1.0

#include "road_atlas_texture.h"

#include <gl/glut.h>
#include <math.h>

#include "common.h"

#include "road_texture_alpha.h"

int getSphereFactor(int i, int j, int width, int height)
{
	float s = (float)i / width;
	float t = (float)j / height;
	float radius = 0.5;

	float dis = sqrt( (s-0.5)*(s-0.5) + (t-0.5)*(t-0.5) );
	if( dis < radius )
	{
		// normalize: norm = dis / radius
		// inverse: 1 - norm
		// transform to color space.
		return (int)( (1-dis/radius)*255 );
	}
	return 0;
}
void makeTexture(GLubyte* buffer, int width, int height, unsigned char color[3])
{
	int i, j, c = 0;

	for (i = 0; i < height; i++) 
	{
		for (j = 0; j < width; j++) 
		{
			// c = getSphereFactor(i, j, width, height);			
			// c = c > 255 ? 255 : c;

			c = alpha_buffer[ i*width + j];		// 直接使用图像

			buffer[ (i*width+j)*4 + 0 ] = color[0];
			buffer[ (i*width+j)*4 + 1 ] = color[1];
			buffer[ (i*width+j)*4 + 2 ] = color[2];
			buffer[ (i*width+j)*4 + 3 ] = (GLubyte) c;
		}
	}
}

RoadAtlasTexture::RoadAtlasTexture()
: _texture_id(0)
{}

RoadAtlasTexture::~RoadAtlasTexture()
{}

void RoadAtlasTexture::generate_texture(unsigned char (&color)[3])
{
	GLubyte* tex_buffer = new GLubyte[4*TEX_WIDTH*TEX_HEIGHT];

	makeTexture(tex_buffer, TEX_WIDTH, TEX_HEIGHT, color);

	glGenTextures(1, &_texture_id);

	glBindTexture(GL_TEXTURE_2D, _texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// filtering 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// initialize texturing
	glTexImage2D(GL_TEXTURE_2D,0,4,TEX_WIDTH, TEX_HEIGHT,0,GL_RGBA, GL_UNSIGNED_BYTE, tex_buffer);

	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] tex_buffer;
	tex_buffer = 0;
}

void RoadAtlasTexture::_generate_tile(const Color4ub& c4, float (&tex_coord)[4])
{
	if (0==_texture_id)
	{
		glGenTextures(1, &_texture_id);

		glBindTexture(GL_TEXTURE_2D, _texture_id);

		GLubyte* tex_buffer = new GLubyte[4*TEX_WIDTH*TEX_HEIGHT];

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// filtering 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// initialize texturing
		glTexImage2D(GL_TEXTURE_2D,0,4,TEX_WIDTH, TEX_HEIGHT,0,GL_RGBA, GL_UNSIGNED_BYTE, tex_buffer);

		glBindTexture(GL_TEXTURE_2D, 0);

		delete[] tex_buffer; tex_buffer = NULL;
	}

	GLubyte* tex_buffer = new GLubyte[4*TILE_WIDTH*TILE_HEIGHT];
	GLubyte color[3] = {c4.red(), c4.green(), c4.blue()};
	makeTexture(tex_buffer, TILE_WIDTH, TILE_HEIGHT, color);

	TileNode node(c4, _tile_cache.size());

	_tile_cache.push_back(node);
	
	float offset_x = (TILE_WIDTH*1.0f/TEX_WIDTH);
	float offset_y = (TILE_HEIGHT*1.0f/TEX_HEIGHT);

	tex_coord[0] = (node.tile_no%TILE_X_SIZE) * offset_x;
	tex_coord[1] = (node.tile_no/TILE_X_SIZE) * offset_y;
	tex_coord[2] = tex_coord[0] + offset_x;
	tex_coord[3] = tex_coord[1] + offset_y;

	int pos_x = TILE_WIDTH * (node.tile_no%TILE_X_SIZE);
	int pos_y = TILE_HEIGHT * (node.tile_no/TILE_X_SIZE);

	// update 
	glBindTexture(GL_TEXTURE_2D, _texture_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, pos_x, pos_y, TILE_WIDTH, TILE_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, tex_buffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete[] tex_buffer; tex_buffer = NULL;
}

int RoadAtlasTexture::query_tile_texture_coord(const Color4ub& c4, float (&tex_coord)[4])
{
	std::vector<TileNode>::iterator iter = _tile_cache.begin();

	for ( ; iter != _tile_cache.end(); ++iter )
	{
		if ( (*iter).c4 == c4 )
		{
			break;
		}
	}
	if (iter != _tile_cache.end())
	{
		float offset_x = (TILE_WIDTH*1.0f/TEX_WIDTH);
		float offset_y = (TILE_HEIGHT*1.0f/TEX_HEIGHT);

		tex_coord[0] = ((*iter).tile_no%TILE_X_SIZE) * offset_x;
		tex_coord[1] = ((*iter).tile_no/TILE_X_SIZE) * offset_y;
		tex_coord[2] = tex_coord[0] + offset_x;
		tex_coord[3] = tex_coord[1] + offset_y;

		return GET_SUCCESS;
	}

	if ( is_full() )		// 已满
	{
		return GET_FAIL;
	}

	_generate_tile(c4, tex_coord);

	return GET_SUCCESS;
}