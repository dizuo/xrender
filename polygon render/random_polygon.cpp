#include "random_polygon.h"

#include <ctime>  // random
#include <cstdlib>
#include <cmath>

RandomPolygon::RandomPolygon(unsigned int pointNum, int width, int height)
{
	srand( (unsigned)time( NULL ) );  

	vertexNum = pointNum;
	vertexList = new data_type[ 2*vertexNum ];

	// 随机初始化点  
	for (int i=0; i<vertexNum; i++)  
	{  
		vertexList[i].x = rand() % width;  
		vertexList[i].y = rand() % height;  
	} 

	_grahamScan();
}

RandomPolygon::~RandomPolygon()
{
	if (vertexList)
		delete[] vertexList;

	vertexList = NULL;
	vertexNum = 0;
}

void RandomPolygon::getPolygonContour(float* & contour, unsigned int& pointNum)
{
	pointNum = vertexNum;

	if (contour==NULL)
	{
		contour = new float[ 3*pointNum ];
	}

	for (int i=0; i<pointNum; i++)
	{
		contour[3*i + 0] = vertexList[i].x;
		contour[3*i + 1] = vertexList[i].y;
		contour[3*i + 2] = 0;
	}
}

/******************************************************************************  
判断三个点构成逆时针/顺时针方向 
r = multiply(begPnt, endPnt, nextPnt),得到(nextPnt-begPnt)*(endPnt-begPne)的叉积  
r>0: nextPnt在矢量<beg, end>的逆时针方向 
r=0: opspep三点共线  
r<0: ep在矢量<beg, end>的顺时针方向  
*******************************************************************************/   
double multiply(POINT begPnt,POINT endPnt,POINT nextPnt)   
{   
    return((nextPnt.x-begPnt.x)*(endPnt.y-begPnt.y) - (endPnt.x-begPnt.x)*(nextPnt.y-begPnt.y));   
}   
double distance(const POINT& pnt1, const POINT& pnt2)  
{  
    return sqrtf( (pnt2.x-pnt1.x) * (pnt2.x-pnt1.x) + (pnt2.y-pnt1.y) * (pnt2.y-pnt1.y) );  
}    

void RandomPolygon::_grahamScan()
{
	data_ptr PointSet = vertexList;
	int i,j,k=0,top=2;
	POINT tmp;   
	// 选取PointSet中y坐标最小的点PointSet[k]，如果这样的点有多个，则取最左边的一个   
	for(i=1; i<vertexNum; i++)   
		if ( PointSet[i].y<PointSet[k].y || (PointSet[i].y==PointSet[k].y) && (PointSet[i].x<PointSet[k].x) )   
			k=i;

	tmp = PointSet[0];   
	PointSet[0] = PointSet[k];   
	PointSet[k] = tmp; // 现在PointSet中y坐标最小的点在PointSet[0]   
	//  对顶点按照相对PointSet[0]的极角从小到大进行排序，  
	//  极角相同的按照距离PointSet[0]从近到远进行排序  
	for (i=1; i<vertexNum-1; i++)   
	{   
		k=i;   
		for (j=i+1; j<vertexNum; j++)   
			if ( multiply(PointSet[j], PointSet[k], PointSet[0])>0 ||  // 极角更小      
				(multiply(PointSet[j], PointSet[k], PointSet[0])==0) && /* 极角相等，距离更短 */    
				distance(PointSet[0], PointSet[j]) < distance(PointSet[0], PointSet[k]) )   
				k=j;

		tmp = PointSet[i];   
		PointSet[i] = PointSet[k];   
		PointSet[k] = tmp;   
	}  

}
