#include "concave_polygon_render.h"

#include <gl/glut.h>

ConcavePolygonRender::ConcavePolygonRender()
{}

ConcavePolygonRender::~ConcavePolygonRender()
{}

void ConcavePolygonRender::renderHandler(float* vertexList, int vertexNum, 
										 float topColor[4], float botColor[4])
{
	float bbox[6];

	_calculateBoundingBox(vertexList, vertexNum, bbox);

	float x0 = bbox[0];
	float x1 = bbox[3];

	float y0 = bbox[1];
	float y1 = bbox[4];

	// Draw to stencil
	glDisable (GL_BLEND);
	glEnable (GL_STENCIL_TEST);
	glStencilMask (0x01);
	glStencilOp (GL_KEEP, GL_KEEP, GL_INVERT);
	glStencilFunc (GL_ALWAYS, 0, ~0);
	glColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glColor4fv(topColor);

	glVertexPointer(3, GL_FLOAT, 0, vertexList);	
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexNum);

	// Draw aliased off-pixels to real
	glColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glStencilFunc (GL_EQUAL, 0x00, 0x01);
	glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);

	glEnable(GL_LINE_SMOOTH);
	glVertexPointer(3, GL_FLOAT, 0, vertexList);	
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_LINE_STRIP, 0, vertexNum);

	glDisable (GL_LINE_SMOOTH);

	// Draw fill
	glStencilFunc (GL_EQUAL, 0x01, 0x01);
	glStencilOp (GL_ZERO, GL_ZERO, GL_ZERO);

	float tileVertices[] = {x0,y0, x0,y1, x1,y0, x1,y1};
	float tileColors[16];
	int off_0 = 0;	int off_1 = 4;	int off_2 = 8;	int off_3 = 12;
	unsigned short tileIndices[] = {0,1,2, 1,3,2};
	for (int i=0; i<4; i++)
	{		
		tileColors[off_0 + i] = topColor[i];
		tileColors[off_1 + i] = topColor[i];
		tileColors[off_2 + i] = botColor[i];
		tileColors[off_3 + i] = botColor[i];
	}
	glVertexPointer(2, GL_FLOAT, 0, tileVertices);	
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(4, GL_FLOAT, 0, tileColors);		
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawElements(GL_TRIANGLES, sizeof(tileIndices)/sizeof(tileIndices[0]), GL_UNSIGNED_SHORT, tileIndices);
	glDisableClientState(GL_COLOR_ARRAY);

	glDisable (GL_STENCIL_TEST);

	glColor4f(1,1,1,1);
}

void ConcavePolygonRender::_calculateBoundingBox(float* vertexList, int vertexNum,
												 float (&bbox)[6])
{
	int idx = 0;

	for (int k=0; k<3; k++)
	{
		// init minCorner
		bbox[k] = vertexList[k];
		// init maxCorner
		bbox[3+k] = vertexList[k];
	}

	for (int idx=1; idx<vertexNum; idx++)
	{
		float curX = vertexList[3*idx + 0];
		float curY = vertexList[3*idx + 1];

		bbox[0] = _min(curX, bbox[0]);
		bbox[1] = _min(curY, bbox[1]);

		bbox[3] = _max(curX, bbox[3]);
		bbox[4] = _max(curY, bbox[4]);
	}
}