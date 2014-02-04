#include "road_render.h"

#include <gl/glut.h>


RoadRender::RoadRender()
{}

RoadRender::~RoadRender()
{}

//==========================
//	NW	N			N	NE
//	  \	|			|  /
//		a-----------b
//	  /	|			|  \
//	SW	S			S	SE
//==========================
int RoadRender::extrudeLines(const PathType& path, float width)
{
	float tex_buf[4] = {0.0f, 0.0f, 1.0f, 1.0f};
	
	return extrudeLines(path, width, tex_buf);
}

// tex_buf : [min_x, min_y, max_x, max_y]
int RoadRender::extrudeLines(const PathType& path, float width, float (&tex_coord)[4])
{
	if (path.size() < 2)
	{
		return EXTRUDE_FAIL;
	}

	int pointSize = path.size();
	int lineSize = pointSize - 1;

	// _indexList.resize( lineSize*IDEX_FACTOR );
	_indexList.resize( lineSize*4*3 + 6 );

	for (int idx=0; idx < lineSize; idx++)
	{
		const VecType& a = path[idx];
		const VecType& b = path[idx+1];

		VecType e = (b-a);
		e.normalize();
		e *= width;

		VecType N = VecType(-e.y(), e.x(), 0);
		VecType S = -N;
		VecType NE = N + e;
		VecType NW = N - e;

		VecType SW = -NE;
		VecType SE = -NW;

		_vertexList.push_back( Vertex(a + SW) );
		_vertexList.push_back( Vertex(a + NW) );
		_vertexList.push_back( Vertex(a + S) );
		_vertexList.push_back( Vertex(a + N) );

		_vertexList.push_back( Vertex(b + S) );
		_vertexList.push_back( Vertex(b + N) );

		if ( lineSize-1 == idx )		// 最后一段增加尾部
		{
			_vertexList.push_back( Vertex(b + SE) );
			_vertexList.push_back( Vertex(b + NE) );
		}

	}

	// _generateTriangleTexCoord(lineSize);
	_generateTriangleSepcialTexCoord(lineSize, tex_coord);

	_generateTriangesIndices(lineSize);

	return EXTRUDE_SUCCESS;
}

void RoadRender::render(unsigned int textureId)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &_vertexList[0].position[0]);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &_vertexList[0].texCoord[0]);

	glDrawElements(GL_TRIANGLES, _indexList.size(), GL_UNSIGNED_INT, &_indexList[0]);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void RoadRender::_generateTriangesIndices(int segSize)
{
	IListIterType iter = _indexList.begin();

	// CW order
	/*for (GLushort v = 0; iter != _indexList.end(); v += 8) 
	{
		*iter++ = 0 + v; *iter++ = 1 + v; *iter++ = 2 + v;
		*iter++ = 2 + v; *iter++ = 1 + v; *iter++ = 3 + v;
		*iter++ = 2 + v; *iter++ = 3 + v; *iter++ = 4 + v;
		*iter++ = 4 + v; *iter++ = 3 + v; *iter++ = 5 + v;
		*iter++ = 4 + v; *iter++ = 5 + v; *iter++ = 6 + v;
		*iter++ = 6 + v; *iter++ = 5 + v; *iter++ = 7 + v;
	}*/

	int segIdx = 0;
	// CCW order
	for (GLushort v = 0; iter != _indexList.end(); v += 6) 
	{
		*iter++ = 0 + v; *iter++ = 2 + v; *iter++ = 1 + v;
		*iter++ = 2 + v; *iter++ = 3 + v; *iter++ = 1 + v;
		*iter++ = 2 + v; *iter++ = 4 + v; *iter++ = 3 + v;
		*iter++ = 4 + v; *iter++ = 5 + v; *iter++ = 3 + v;

		if (segSize-1 == segIdx)
		{
			*iter++ = 4 + v; *iter++ = 6 + v; *iter++ = 5 + v;
			*iter++ = 6 + v; *iter++ = 7 + v; *iter++ = 5 + v;

			v += 2;
		}

		segIdx++;
	}
}

void RoadRender::_generateTriangleTexCoord(int segSize)
{
	using namespace gtl;

	VListIterType iter = _vertexList.begin();
	int segIdx = 0;
	float margin = 0.08;

	while (iter != _vertexList.end()) 
	{
		iter->texCoord = Vec2f(0 + margin, 0 + margin);		iter++;
		iter->texCoord = Vec2f(0 + margin, 1 - margin);		iter++;
		iter->texCoord = Vec2f(0.5, 0 + margin);			iter++;
		iter->texCoord = Vec2f(0.5, 1 - margin);			iter++;

		iter->texCoord = Vec2f(0.5, 0 + margin);			iter++;
		iter->texCoord = Vec2f(0.5, 1 - margin);			iter++;

		if ( segSize-1 == segIdx )
		{
			iter->texCoord = Vec2f(1 - margin, 0 + margin);		iter++;
			iter->texCoord = Vec2f(1 - margin, 1 - margin);		iter++;
		}

		segIdx++;
	}
}

void RoadRender::_generateTriangleSepcialTexCoord(int segSize, float (&tex_coord)[4])
{
	using namespace gtl;

	VListIterType iter = _vertexList.begin();
	int segIdx = 0;

	float margin = 0;
	float min_x = tex_coord[0];
	float min_y = tex_coord[1];
	float max_x = tex_coord[2];
	float max_y = tex_coord[3];
	float mid_x = (min_x + max_x)/2.0f;
	float mid_y = (min_y + max_y)/2.0f;

	while (iter != _vertexList.end()) 
	{
		iter->texCoord = Vec2f(min_x + margin, min_y + margin);		iter++;
		iter->texCoord = Vec2f(min_x + margin, max_y - margin);		iter++;
		iter->texCoord = Vec2f(mid_x, min_y + margin);			iter++;
		iter->texCoord = Vec2f(mid_x, max_y - margin);			iter++;

		iter->texCoord = Vec2f(mid_x, min_y + margin);			iter++;
		iter->texCoord = Vec2f(mid_x, max_y - margin);			iter++;

		if ( segSize-1 == segIdx )
		{
			iter->texCoord = Vec2f(min_x + margin, min_y + margin);		iter++;
			iter->texCoord = Vec2f(min_x + margin, max_y - margin);		iter++;
		}

		segIdx++;
	}
}

