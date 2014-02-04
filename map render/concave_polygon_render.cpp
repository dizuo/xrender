// @module:		map-render/concave polygon render
// @file:		concave_polygon_render.h
// @author:		peteryfren
// @date:		2013/3/13
// @version:	1.0
// @desc:		render concave polygon with OpenGL stencil buffer test.
//				增加顶点顺序判断，判断前三个顶点叉乘结果

#include "concave_polygon_render.h"

#include <gl/glut.h>

ConcavePolygonRender::ConcavePolygonRender()
{}

ConcavePolygonRender::~ConcavePolygonRender()
{}

void ConcavePolygonRender::render_handler(float* vertex_list, int vertex_num,
										  const Color4ub& top_color, const Color4ub& bot_color,
										  const gtl::Box2i& bbox)
{

	float x0 = bbox.getMin().x();
	float x1 = bbox.getMax().x();

	float y0 = bbox.getMin().y();
	float y1 = bbox.getMax().y();

	// Draw to stencil
	glDisable (GL_BLEND);
	glEnable (GL_STENCIL_TEST);		// 开启模板测试

	glStencilMask (0x01);			// 开启模板缓冲区写入
	glStencilOp (GL_KEEP, GL_KEEP, GL_INVERT);	// 对通过模板测试的值 逐位反转
	glStencilFunc (GL_ALWAYS, 0, ~0);	// 禁用模板测试
	glColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);	// 禁止颜色缓冲区写入

	glColor4ubv(top_color.data());

	glVertexPointer(2, GL_FLOAT, 0, vertex_list);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertex_num);

	// Draw aliased off-pixels to real
	glColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);	// 开启颜色缓冲区写入
	glEnable (GL_BLEND);	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// 设置混合模式

	glStencilFunc (GL_EQUAL, 0x00, 0x01);	
	glStencilOp (GL_KEEP, GL_KEEP, GL_KEEP);  // 所有片段保持

	glEnable(GL_LINE_SMOOTH);
	glVertexPointer(2, GL_FLOAT, 0, vertex_list);	
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_LINE_STRIP, 0, vertex_num);

	glDisable (GL_LINE_SMOOTH);

	// Draw fill
	glStencilFunc (GL_EQUAL, 0x01, 0x01);		// 开启模板测试
	glStencilOp (GL_ZERO, GL_ZERO, GL_ZERO);	// 用0替换当前值

	float tileVertices[] = {x0,y0, x0,y1, x1,y0, x1,y1};
	unsigned char tileColors[16];
	int off_0 = 0;	int off_1 = 4;	int off_2 = 8;	int off_3 = 12;
	unsigned short tileIndices[] = {0,1,2, 1,3,2};
	for (int i=0; i<4; i++)
	{		
		tileColors[off_0 + i] = top_color.data()[i];
		tileColors[off_1 + i] = top_color.data()[i];
		tileColors[off_2 + i] = bot_color.data()[i];
		tileColors[off_3 + i] = bot_color.data()[i];
	}
	glVertexPointer(2, GL_FLOAT, 0, tileVertices);	
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, tileColors);		
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawElements(GL_TRIANGLES, sizeof(tileIndices)/sizeof(tileIndices[0]), GL_UNSIGNED_SHORT, tileIndices);
	glDisableClientState(GL_COLOR_ARRAY);

	glDisable (GL_STENCIL_TEST);

	glColor4f(1,1,1,1);
}
