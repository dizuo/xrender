// @ author : ren.yafee@gmail.com
// @ desc : generate random polygon
#ifndef RANDOM_POLYGON_201301291949_H
#define RANDOM_POLYGON_201301291949_H

struct POINT
{
	int x;
	int y;
};

class RandomPolygon
{
public:
	
	typedef POINT data_type;

	typedef data_type* data_ptr;

	RandomPolygon(unsigned int pointNum, int, int);

	~RandomPolygon();

	void getPolygonContour(float* & contour, unsigned int& pointNum);

private:
	
	void _grahamScan();

	data_ptr vertexList;
	unsigned int vertexNum;
};

#endif