#include "stick.h"

#include <gl/glut.h>


Stick::Stick()
{}

Stick::~Stick()
{}

//==========================
//	NW	N			N	NE
//	  \	|			|  /
//		a-----------b
//	  /	|			|  \
//	SW	S			S	SE
//==========================
int Stick::extrudeLines(const PathType& path, float width)
{
	if (path.size() < 2)
	{
		return EXTRUDE_FAIL;
	}
	
	int pointSize = path.size();
	int lineSize = pointSize - 1;

	_indexList.resize( lineSize*IDEX_FACTOR );

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
		_vertexList.push_back( Vertex(b + SE) );
		_vertexList.push_back( Vertex(b + NE) );
	}

	_generateTriangleTexCoord();
	_generateTriangesIndices();

	return EXTRUDE_SUCCESS;	
}

void Stick::render(unsigned int textureId)
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

void Stick::_generateTriangesIndices()
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

	// CCW order
	for (GLushort v = 0; iter != _indexList.end(); v += 8) 
	{
		*iter++ = 0 + v; *iter++ = 2 + v; *iter++ = 1 + v;
		*iter++ = 2 + v; *iter++ = 3 + v; *iter++ = 1 + v;
		*iter++ = 2 + v; *iter++ = 4 + v; *iter++ = 3 + v;
		*iter++ = 4 + v; *iter++ = 5 + v; *iter++ = 3 + v;
		*iter++ = 4 + v; *iter++ = 6 + v; *iter++ = 5 + v;
		*iter++ = 6 + v; *iter++ = 7 + v; *iter++ = 5 + v;
	}
}

void Stick::_generateTriangleTexCoord()
{
	using namespace gtl;

	VListIterType iter = _vertexList.begin();
	
	float margin = 0.08;

	while (iter != _vertexList.end()) 
	{
		iter->texCoord = Vec2f(0 + margin, 0 + margin);		iter++;
		iter->texCoord = Vec2f(0 + margin, 1 - margin);		iter++;
		iter->texCoord = Vec2f(0.5, 0 + margin);			iter++;
		iter->texCoord = Vec2f(0.5, 1 - margin);			iter++;

		iter->texCoord = Vec2f(0.5, 0 + margin);			iter++;
		iter->texCoord = Vec2f(0.5, 1 - margin);			iter++;
		iter->texCoord = Vec2f(1 - margin, 0 + margin);		iter++;
		iter->texCoord = Vec2f(1 - margin, 1 - margin);		iter++;
	}
}

