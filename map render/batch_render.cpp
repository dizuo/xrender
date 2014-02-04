#include "batch_render.hpp"

#include <GL/glut.h>

void RectRenderHandler::operator()(float* v_list, int v_cnt, 
								   unsigned short* i_list, int i_cnt, unsigned char* color)
{
	if (NULL==v_list || NULL==i_cnt || 0==v_cnt || 0==i_cnt)
	{
		return;
	}

	unsigned char c_list[16];

	for (int i=0; i<4; i+=4)
	{
		for (int k=0; k<4; k++) c_list[i*4 + k] = color[k];
	}

	glColorPointer(4, GL_UNSIGNED_BYTE, 0, c_list);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, v_list);	
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawElements(GL_TRIANGLES, i_cnt, GL_UNSIGNED_SHORT, i_list);
}

void V2FRenderHandler::operator ()(V2FRenderHandler::VecType *v_list, int v_cnt, 
								   unsigned short *i_list, int i_cnt,
								   unsigned int texture_id)
{
	if (NULL==v_list || NULL==i_cnt ||
		0==v_cnt || 0==i_cnt || 0==texture_id)
	{
		return;
	}

	glVertexPointer(2, GL_FLOAT, sizeof(VecType), v_list);
	glEnableClientState(GL_VERTEX_ARRAY);

	//Draw
	glDrawElements(GL_TRIANGLES, i_cnt, GL_UNSIGNED_SHORT, i_list);
}

void V2F2FRenderHandler::operator ()(V2F2FRenderHandler::VecType *v_list, int v_cnt,
									 unsigned short *i_list, int i_cnt,
									 unsigned int texture_id)
{
/*	int width = 100;	int height = 100;
	glBindTexture(GL_TEXTURE_2D, texture_id);
	float tileVertices[] = {0,0, 0,height, width,height, width,0}; 
	float tileTexVertices[] = {0,0, 0,1, 1,1, 1,0};
	glVertexPointer(2, GL_FLOAT, 0, tileVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, tileTexVertices);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindTexture(GL_TEXTURE_2D, 0); 
*/	
	if (NULL==v_list || NULL==i_cnt ||
		0==v_cnt || 0==i_cnt || 0==texture_id)
	{
		return;
	}

	glBindTexture(GL_TEXTURE_2D, texture_id);
	glVertexPointer(2, GL_FLOAT, sizeof(VecType), &v_list[0].xy[0]);
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(VecType), &v_list[0].uv[0]);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
	//Draw
	glDrawElements(GL_TRIANGLES, i_cnt, GL_UNSIGNED_SHORT, i_list);
	glBindTexture(GL_TEXTURE_2D, 0); 
}

// 批量绘制任意多边形
// Refer to ConcavePolygonRender::renderHandler
void V2FPolygonRenderHandler::operator ()(V2FPolygonRenderHandler::VecType *v_list, int v_cnt, 
										  unsigned char* color4)
{
	if (NULL==v_list || 0==v_cnt)
	{
		return;
	}

	glColor4ubv(color4);

	glVertexPointer(2, GL_FLOAT, sizeof(VecType), v_list);
	glEnableClientState(GL_VERTEX_ARRAY);

	//Draw
	glLineWidth(2.0);

	glDrawArrays(GL_LINE_STRIP, 0, v_cnt);

	glLineWidth(1.0);
}