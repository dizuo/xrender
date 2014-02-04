// @NOTE: 底图描边效果：路段绘制两次。

#ifndef ROAD_RENDER_201303052151_H
#define ROAD_RENDER_201303052151_H

#include "../gtl/vec3.hpp"
#include "../gtl/vec2.hpp"

#include <vector>

struct Vertex
{
	Vertex()
		: position()
		, texCoord()
	{}

	Vertex(const gtl::Vec3f& pos)
		: position(pos)
		, texCoord()
	{}

	gtl::Vec3f position;
	gtl::Vec2f texCoord;
};

class RoadRender
{	
public:
	typedef gtl::Vec3f VecType;
	typedef std::vector<VecType> PathType;
	typedef PathType::iterator PathIterType;

	typedef std::vector<Vertex> VListType;
	typedef VListType::iterator VListIterType;
	
	typedef std::vector<unsigned int> IListType;
	typedef IListType::iterator IListIterType;

	enum
	{
		EXTRUDE_SUCCESS = 0,
		EXTRUDE_FAIL = 1
	};

	enum
	{
		VERT_FACTOR = 8,
		IDEX_FACTOR = 18
	};

	enum CAP_TYPE
	{
		CAP_LEFT,	// add left cap
		CAP_RIGHT,	// add right cap
		CAP_BOTH	// add both side cap
	};

private:

	VListType _vertexList;
	IListType _indexList;

public:

	RoadRender();
	~RoadRender();

	int extrudeLines(const PathType& path, float width);

	int extrudeLines(const PathType& path, float width, float (&tex_coord)[4]);

	void render(unsigned int textureId);

private:

	void _generateTriangesIndices(int segSize);

	void _generateTriangleTexCoord(int segSize);

	void _generateTriangleSepcialTexCoord(int segSize, float (&tex_coord)[4] );

private:

	RoadRender(const RoadRender& obj);
	RoadRender& operator=(const RoadRender& obj);

};

#endif	// STICK_201301261205_H