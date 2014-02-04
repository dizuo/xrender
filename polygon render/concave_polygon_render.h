// @author : ren.yafee@gmail.com
// @desc : render concave polygon with OpenGL stencil buffer test.

#ifndef CONCAVE_POLYGON_RENDER_201301272128_H
#define CONCAVE_POLYGON_RENDER_201301272128_H

class ConcavePolygonRender
{
public:

	enum 
	{
		MAX_VERTEX_NUM = 100
	};

	ConcavePolygonRender();
	
	~ConcavePolygonRender();

	void renderHandler(float* vertexList, int vertexNum, float topColor[4], float botColor[4]);

private:

	void _calculateBoundingBox(float* vertexList, int vertexNum, float (&bbox)[6]);

	float _max(float a, float b) { return a > b ? a : b; }

	float _min(float a, float b) { return a > b ? b : a; }

};

#endif // CONCAVE_POLYGON_RENDER_201301272128_H