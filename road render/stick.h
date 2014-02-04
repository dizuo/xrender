#ifndef STICK_201301261205_H
#define STICK_201301261205_H

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

class Stick
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

private:

	VListType _vertexList;
	IListType _indexList;

public:

	Stick();
	~Stick();

	int extrudeLines(const PathType& path, float width);

	void render(unsigned int textureId);

private:

	Stick(const Stick& obj);
	Stick& operator=(const Stick& obj);

	void _generateTriangesIndices();

	void _generateTriangleTexCoord();

};

#endif	// STICK_201301261205_H