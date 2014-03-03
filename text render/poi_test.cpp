#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glut.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "BezierLinkRender.h"

static float rotAngle = 0.0f;
static float skewAngle = 0.0f;

static float cameraZ = 7.0f;
const float MaxCZ = 12.0f;
const float MinCZ = 4.0f;

static double screenPos[3];

const float RotMax = 60.0f;
const float Delta = 2.0f;

int frameIdx = 0;
int frameNum = 100;

float			g_fWidth = 512;			// 窗体的宽度
float			g_fHeight = 392;		// 窗体的高度

void	init(void);				// 初始化opengl的环境
void	reshape(int w, int h);	// 该函数当窗体改变以后响应，重新设置视口
void	display(void);			// 渲染opengl内容
void	keyboard(unsigned char, int, int);	// 响应鼠标事件

void testRenderRotateLabel();	// 单独rotate
void testRenderSkewLabel();		// 单独skew
void testRenderLabel();			// 测试综合情况
void getMapPointInScreenPos(float x, float y, float z, double (&screen_pos)[3]);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(g_fWidth, g_fHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("MapTransformDemo");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}


void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1);
	glShadeModel(GL_SMOOTH);

	// glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/* Rotate about x-axis when "x" typed; rotate about y-axis
when "y" typed; "i" returns torus to original view */
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;

	case 'r':
		if (rotAngle < 360)
		{
			rotAngle += Delta;
		}
		break;
	case 'R':
		if (rotAngle > -360)
		{
			rotAngle -= Delta;
		}
		break;
	case 's':
		if (skewAngle < RotMax)
		{
			skewAngle += Delta;
		}
		break;
	case 'S':
		if (skewAngle > 0)
		{
			skewAngle -= Delta;
		}
		break;
	case 'm':
		if (skewAngle < RotMax)
		{
			skewAngle += Delta;
			rotAngle += Delta;
		}
		break;
	case 'M':
		if (skewAngle > 0)
		{
			skewAngle -= Delta;
			rotAngle -= Delta;
		}
		break;
	case 'z':
		if (cameraZ < MaxCZ)
		{
			cameraZ += 0.1f;
		}
		break;
	case 'Z':
		if (cameraZ > MinCZ)
		{
			cameraZ -= 0.1;
		}
		break;

	case ' ':
		cameraZ = 7.0f;
		rotAngle = skewAngle = 0.0f;
		break;
	}

	glutPostRedisplay();
}

void reshape(int w, int h)
{
	g_fWidth = w;
	g_fHeight = h;

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);       //回复原有的设置  
	glLoadIdentity();
	gluPerspective(30, g_fWidth / g_fHeight, 0.001, 100.0);
}

#define RENDER_LABEL(xsize, ysize, r,g,b)	\
	glScalef(xsize, ysize, 0.01f);	\
	glColor3f(r, g, b);	\
	glutSolidCube(1);	

void renderBaseMap()
{
	glPushMatrix();
	{
		getMapPointInScreenPos(1, 1, -1, screenPos);
		glTranslatef(0, 0, -1);		

		// printf("%f, %f\n", screenPos[0], screenPos[1]);

		glScalef(4, 6, 1);
		glColor3f(0.5, 0.5, 0.5);
		glutSolidCube(1);
	} glPopMatrix();
}

void renderAxisBoudingLabel(float (&arr)[3])
{
	glPushMatrix();
	{			
		glTranslatef(arr[0], arr[1], arr[2]);
		glRotatef(rotAngle, 0, 0, 1);
		glRotatef(skewAngle, 1, 0, 0);
		RENDER_LABEL(0.5f, 0.25f, 1, 0, 0);
	} glPopMatrix();
}

void renderCommonLabel(float (&arr)[3])
{
	glPushMatrix();
	{	
		glTranslatef(arr[0], arr[1], arr[2]);
		// RENDER_LABEL(0.5f, 0.25f, 0,0,1);
		RENDER_LABEL(1.0f, 0.5f, 0,0,1);
	} glPopMatrix();
}

void renderBezierRoad(float(&beg)[3], float(&end)[3])
{
	rctf bezier_rect;
#ifdef FLIP
	bezier_rect.xmin = min(beg[0], end[0]);
	bezier_rect.xmax = max(beg[0], end[0]);
	bezier_rect.ymin = min(beg[1], end[1]);
	bezier_rect.ymax = max(beg[1], end[1]);
#else
	bezier_rect.xmin = beg[0];
	bezier_rect.xmax = end[0];
	bezier_rect.ymin = beg[1];
	bezier_rect.ymax = end[1];
#endif

	float zoff = min(beg[2], end[2]);

	glPushMatrix();
	{
		glTranslatef(0.0f, 0.0f, zoff);
		glLineWidth(2.0f);
		BezierLinkRender::ui_draw_link_bezier(&bezier_rect);
		glLineWidth(1.0f);
	} glPopMatrix();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);  // 清屏幕颜色

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, cameraZ, 0, 0, 0, 0, 1, 0);
	//-----------------------------------------------------------------------------------------
	// 3D perspective
	//-----------------------------------------------------------------------------------------
	glPushMatrix();
	{
		// testRenderRotateLabel();	// 单独rotate
		// testRenderSkewLabel();	// 单独skew
		// testRenderLabel();			// 测试综合情况
	} glPopMatrix();

	float label_3dd_pos1[3] = {1.0f, -1.5f, -1.0f};
	float label_3dd_pos2[3] = {-1.5f, 2.5f, -1.0f};

	float label_3dc_pos1[3] = {-1.5f, -1.f, -1.0f};
	float label_3dc_pos2[3] = {1.0f, 1.0f, -1.0f};

	float road_beg[3] = {-1.5f, 1.0f, -1.0f};
	float road_end[3] = {1.0f, -1.0f, -1.0f};

	// 关闭了深度测试，相同深度的内容最后画的优先出现！
	glPushMatrix();
	{
		glRotatef(skewAngle, -1, 0, 0);
		glRotatef(rotAngle, 0, 0, -1);
		
		renderBaseMap();

		renderCommonLabel(label_3dc_pos2);
		renderCommonLabel(label_3dc_pos1);

		// renderBezierRoad(road_beg, road_end);
		renderBezierRoad(label_3dd_pos1, label_3dd_pos2);

		renderAxisBoudingLabel(label_3dd_pos1);
		renderAxisBoudingLabel(label_3dd_pos2);

	} glPopMatrix();

	//-----------------------------------------------------------------------------------------
	// 2D Ortho
	//-----------------------------------------------------------------------------------------
	float label_2ds_pos[2] = {g_fWidth - 50, 25};

	// 根据screenPos绘制2d dynamic label.
	glMatrixMode(GL_PROJECTION);       //回复原有的设置  
	glPushMatrix();		// save projection matrix.
		glLoadIdentity();
		gluOrtho2D(0, g_fWidth, 0, g_fHeight);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();		// save modelview matrix.
		glLoadIdentity();

		glPushMatrix();
		{
			glTranslatef(screenPos[0], screenPos[1], screenPos[2]);
			RENDER_LABEL(50, 25, 0,1,0);
		} glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(label_2ds_pos[0], label_2ds_pos[1], 0);
			RENDER_LABEL(50, 25, 1,1,0);
		} glPopMatrix();

		glPopMatrix();		// restore modelview matrix.
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();		// restore projection matrix.
	
	glutSwapBuffers();	// 防止刷屏，采用双缓冲机制

	// glutPostRedisplay(); // 重复调用自身
}

void getMapPointInScreenPos(float x, float y, float z, double (&screen_pos)[3])
{
	GLdouble modelMatrix[16];
	GLdouble projMatrix[16];
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
	gluProject(x, y, z, modelMatrix, projMatrix, viewport, &screen_pos[0], &screen_pos[1], &screen_pos[2]);		
}

void testRenderRotateLabel()
{
	const float angle = RotMax;

	glPushMatrix();
	{
		glRotatef(rotAngle, 0, 0, -1);

		glPushMatrix();
		glScalef(0.5, 0.25, 0.01);
		glColor3f(1, 0, 0);
		glutSolidCube(1);
		glPopMatrix();

		glTranslatef(0.5, 1, 0);
		glRotatef(angle, 0, 0, 1);
		glScalef(0.5, 0.25, 0.01);
		glColor3f(1, 0, 0);
		glutSolidCube(1);
	} glPopMatrix();
}

void testRenderLabel()
{
	const float angle = RotMax;

	glPushMatrix();
	{
		glRotatef(skewAngle, -1, 0, 0);
		glRotatef(rotAngle, 0, 0, -1);

		glPushMatrix();
		glScalef(0.5, 0.25, 0.01);
		glColor3f(1, 0, 0);
		glutSolidCube(1);
		glPopMatrix();

		glTranslatef(0.5, 1, 0);
		glRotatef(angle, 0, 0, 1);
		glRotatef(angle, 1, 0, 0);
		glScalef(0.5, 0.25, 0.01);
		glColor3f(1, 0, 0);
		glutSolidCube(1);
	} glPopMatrix();
}

void testRenderSkewLabel()
{
	const float angle = RotMax;

	glPushMatrix();
	{
		glRotatef(skewAngle, -1, 0, 0);

		glPushMatrix();
		glScalef(0.5, 0.25, 0.01);
		glColor3f(1, 0, 0);
		glutSolidCube(1);
		glPopMatrix();

		glTranslatef(0.5, 1, 0);
		glRotatef(angle, 1, 0, 0);
		glScalef(0.5, 0.25, 0.01);
		glColor3f(1, 0, 0);
		glutSolidCube(1);
	} glPopMatrix();
}

/*
float lineVerticesBuf[] = {0,0,0, 1,1,0, 0,0,0, -1,1,0};
glEnableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_TEXTURE_COORD_ARRAY);
glDisableClientState(GL_COLOR_ARRAY);
glVertexPointer(3, GL_FLOAT, 0, lineVerticesBuf);
glDrawArrays(GL_LINES, 0, 4);
*/