#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glut.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>

static float rotAngle = 0.0f;
static float skewAngle = 0.0f;

const float RotMax = 40.0f;
const float Delta = 2.0f;

int frameIdx = 0;
int frameNum = 100;

float			g_fWidth = 512;			// 窗体的宽度
float			g_fHeight = 392;		// 窗体的高度

void	init(void);			// 初始化opengl的环境
void	reshape(int w,int h);	// 该函数当窗体改变以后响应，重新设置视口
void	display(void);			// 渲染opengl内容
void	keyboard(unsigned char, int, int);	// 响应鼠标事件

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glutInitWindowSize (g_fWidth, g_fHeight);
	glutInitWindowPosition (0, 0);
	glutCreateWindow("ModelRotate");
	init ();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc( keyboard );
	glutMainLoop();
	return 0;
}


void init (void)
{ 
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClearDepth(1);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
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
		if (rotAngle < RotMax)
		{
			rotAngle += Delta;
		}
		break;
	case 'R':
		if (rotAngle > 0)
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
	case 'z':
		if (skewAngle < RotMax)
		{
			skewAngle += Delta;
			rotAngle += Delta;
		}
		break;
	case 'Z':
		if (skewAngle > 0)
		{
			skewAngle -= Delta;
			rotAngle -= Delta;
		}
		break;

	}
	
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	g_fWidth = w;
	g_fHeight = h;

	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode (GL_PROJECTION);       //回复原有的设置  
    glLoadIdentity ();  
	gluPerspective(30, g_fWidth/g_fHeight, 0.001, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity ();             
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
}

void renderBaseMap()
{
	glPushMatrix();
	{
		glTranslatef(0, 0, -1);
		glScalef(3, 3, 1);
		glColor3f(1, 1, 1);
		glutSolidCube(1);
	} glPopMatrix();
}

void renderAxisBoudingLabel(float x, float y, float z)
{
	glPushMatrix();
	{
		glTranslatef(x, y, z);
		glRotatef(rotAngle, 0,0,1);
		glRotatef(skewAngle, 1,0,0);
		glScalef(0.5, 0.25, 0.01);
		glColor3f(1,0,0);
		glutSolidCube(1);
	} glPopMatrix();
}

void testRenderRotateLabel()
{
	const float angle = RotMax;

	glPushMatrix();
	{
		glRotatef(rotAngle, 0,0,-1);

		glPushMatrix();
		glScalef(0.5, 0.25, 0.01);
		glColor3f(1,0,0);
		glutSolidCube(1);
		glPopMatrix();

		glTranslatef(0.5, 1, 0);
		glRotatef(angle, 0,0,1);
		glScalef(0.5, 0.25, 0.01);
		glColor3f(1,0,0);
		glutSolidCube(1);
	} glPopMatrix();
}

void testRenderLabel()
{
	const float angle = RotMax;

	glPushMatrix();
	{
		glRotatef(skewAngle, -1,0,0);
		glRotatef(rotAngle, 0,0,-1);

		glPushMatrix();
		glScalef(0.5, 0.25, 0.01);
		glColor3f(1,0,0);
		glutSolidCube(1);
		glPopMatrix();

		glTranslatef(0.5, 1, 0);
		glRotatef(angle, 0,0,1);
		glRotatef(angle, 1,0,0);
		glScalef(0.5, 0.25, 0.01);
		glColor3f(1,0,0);
		glutSolidCube(1);
	} glPopMatrix();
}

void testRenderSkewLabel()
{
	const float angle = RotMax;

	glPushMatrix();
	{
		glRotatef(skewAngle, -1,0,0);

		glPushMatrix();
		glScalef(0.5, 0.25, 0.01);
		glColor3f(1,0,0);
		glutSolidCube(1);
		glPopMatrix();

		glTranslatef(0.5, 1, 0);
		glRotatef(angle, 1,0,0);
		glScalef(0.5, 0.25, 0.01);
		glColor3f(1,0,0);
		glutSolidCube(1);
	} glPopMatrix();
}


void display(void)
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glColor3f (1.0, 1.0, 1.0);  // 清屏幕颜色

	glPushMatrix();
	{
		// testRenderRotateLabel();	// 单独rotate
		// testRenderSkewLabel();	// 单独skew
		// testRenderLabel();			// 测试综合情况
	} glPopMatrix();
 
 	glPushMatrix();
 	{
 		glRotatef(skewAngle, -1, 0, 0);
 		glRotatef(rotAngle, 0,0, -1);
 
 		renderBaseMap();

		renderAxisBoudingLabel(1.0f, -0.5f, 0.7f);
		renderAxisBoudingLabel(-1.0f, 0.5f, -0.5f);
 
	} glPopMatrix();

/*
	float lineVerticesBuf[] = {0,0,0, 1,1,0, 0,0,0, -1,1,0};
	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, lineVerticesBuf);
	glDrawArrays(GL_LINES, 0, 4);
*/
	frameIdx = (frameIdx+1) % frameNum;

	glutSwapBuffers();	// 防止刷屏，采用双缓冲机制

	// glutPostRedisplay(); // 重复调用自身
}
