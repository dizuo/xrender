#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glut.h>

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <time.h>

#include "controller.h"

static float rotAngle = 0.0f;
static float skewAngle = 0.0f;

vec3f_t camera, object, rot_vec;

item_t* g_seed_items = NULL;
int	g_seed_num = 0;

Controller g_ctrl;

float			g_fWidth = 500;			// 窗体的宽度
float			g_fHeight = 592;		// 窗体的高度

void	init(void);				// 初始化opengl的环境
void	reshape(int w, int h);	// 该函数当窗体改变以后响应，重新设置视口
void	display(void);			// 渲染opengl内容
void	keyboard(unsigned char, int, int);	// 响应鼠标事件

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(g_fWidth, g_fHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("WaveDemo");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

void load_seed_items(const char* fname)
{
	FILE* fp = fopen(fname, "r");
	if (!fp)
		return;

	char buffer[1024];
	g_seed_num = 0;
	int item_id = 0;
	const char* token = ", \t\n\r";

	while (fgets(buffer, 1024, fp))
	{
		if (g_seed_num == 0)
		{
			g_seed_num = atoi(buffer);

			if (g_seed_items)
				delete[] g_seed_items;
			
			g_seed_items = new item_t[g_seed_num];
			continue;
		}
		
		if (strlen(buffer) < 2)
			continue;

		if (!g_seed_items)
			continue;
		
		item_t& item = g_seed_items[item_id++];
		char* pch = strtok(buffer, token);
		item.pos[0] = atof(pch);

		pch = strtok(NULL, token);
		item.pos[1] = atof(pch);

		pch = strtok(NULL, token);
		item.size[0] = atof(pch);

		pch = strtok(NULL, token);
		item.size[1] = atof(pch);

		pch = strtok(NULL, token);
		item.height = atof(pch);

	}
}

void init(void)
{
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	// load_seed_items("seed_items.txt");
	g_ctrl.product_items(g_seed_items, g_seed_num);

	// glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Add lights for scene.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	make_vec3(camera, 0, 0, 200);
	make_vec3(object, 0, 0, 0);
	make_vec3(rot_vec, 0, 0, 0);

}

/* Rotate about x-axis when "x" typed; rotate about y-axis
when "y" typed; "i" returns torus to original view */
void keyboard(unsigned char key, int x, int y)
{
	const float delta = 1.3f;
	const float rot_delta = 2.0f;

	vec3f_t& ref = object;

	switch (key)
	{
	case 27:
		exit(0); break;
	case 'z':
		ref.z -= delta; break;
	case 'Z':
		ref.z += delta; break;
	case 'x':
		ref.x -= delta; break;
	case 'X':
		ref.x += delta; break;
	case 'y':
		ref.y -= delta; break;
	case 'Y':
		ref.y += delta; break;

	case '1':
		rot_vec.x += rot_delta; break;
	case '2':
		rot_vec.x -= rot_delta; break;
	case '3':
		rot_vec.y += rot_delta; break;
	case '4':
		rot_vec.y -= rot_delta; break;

	case ' ':
		object.x = object.y = object.z = 0;
		rot_vec.x = rot_vec.y = rot_vec.z = 0;
		break;

	case 'f':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
	case 'F':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;

	default:
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
	gluPerspective(60, g_fWidth / g_fHeight, 0.001, 330.0);
}

void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.x, camera.y, camera.z, 0, 0, 0, 0, 1, 0);
	//-----------------------------------------------------------------------------------------
	// 3D perspective
	//-----------------------------------------------------------------------------------------

	glPushMatrix();
	{
		glTranslatef(object.x, object.y, object.z);

		glRotatef(rot_vec.y, 0,1,0);
		glRotatef(rot_vec.x, 1,0,0);

		// 禁用颜色数组，启用顶点、纹理数组
		glDisableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);

		glColor4f(1, 0, 0, 0.3f);

#if 0
		float verts[] = {0,0,0, 1,0,0, 0,0,1};
		glVertexPointer(3, GL_FLOAT, 0, verts);
		glDrawArrays(GL_TRIANGLES, 0, 3);
#endif

		clock_t beg = clock();
		g_ctrl.render_items(g_seed_items, g_seed_num);
		clock_t end = clock();
		printf("cost = %d\n", end - beg);


	} glPopMatrix();

	
	glutSwapBuffers();

	// glutPostRedisplay();

}
