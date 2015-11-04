#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include <GL\glut.h>

#include <time.h>

#include "controller.h"

vec3f_t camera, object, rot_vec;

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
	glutCreateWindow("PolyDemo");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

void init(void)
{
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// Add lights for scene.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	make_vec3(camera, 0, 0, 5);
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
	glClearColor(0.8, 0.8, 0.8, 1.0);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.x, camera.y, camera.z, 0, 0, 0, 0, 1, 0);
	//-----------------------------------------------------------------------------------------
	// 3D perspective
	//-----------------------------------------------------------------------------------------

	glDisable(GL_DEPTH_TEST);

	glPushMatrix();
	{
		glTranslatef(object.x, object.y, object.z);

		glRotatef(rot_vec.y, 0,1,0);
		glRotatef(rot_vec.x, 1,0,0);

		// 禁用颜色数组，启用顶点、纹理数组
		glDisableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_VERTEX_ARRAY);
		glColor4f(0.0f, 95.0f/255.0f, 165.0f/255.0f, 1.0f);
		float verts[] = {0,2,0, 0,0,0, 2,0,0};
		glVertexPointer(3, GL_FLOAT, 0, verts);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glEnableClientState(GL_COLOR_ARRAY);
		// float inner_verts[] = {0.5,1.5,0, 0.5,0.5,0, 1.5,0.5, 0};
		float inner_verts[] = {0.2,1.2,0, 0.2,0.2,0, 1.2,0.2, 0};

#if 0
		float xx_verts[18];		
		for (int k = 0; k < 3; k++)
		{
			xx_verts[6*k] = inner_verts[3*k ];
			xx_verts[6*k + 1] = inner_verts[3*k + 1];
			xx_verts[6*k + 2] = inner_verts[3*k + 2];

			xx_verts[6*k + 3] = verts[3*k];
			xx_verts[6*k + 4] = verts[3*k + 1];
			xx_verts[6*k + 5] = verts[3*k + 2];
		}
		glVertexPointer(3, GL_FLOAT, 0, xx_verts);
		
		float src_color[4] = {0.0f, 95.0f/255.0f, 165.0f/255.0f, 1.0f};
		float dst_color[4] = {1, 1, 1, 1};

		float xx_colors[24];
		for (int k = 0; k < 6; k++)
		{
			for (int t = 0; t < 4; t++)
			{
				xx_colors[4*k + t] = k%2==1 ? src_color[t] : dst_color[t];
			}
		}
		glColorPointer(4, GL_FLOAT, 0, xx_colors);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
#endif

		float xx_verts[24];
		for (int k = 0; k < 4; k++)
		{
			int newk = k % 3;

			xx_verts[6*k] = inner_verts[3*newk ];
			xx_verts[6*k + 1] = inner_verts[3*newk + 1];
			xx_verts[6*k + 2] = inner_verts[3*newk + 2];

			xx_verts[6*k + 3] = verts[3*newk];
			xx_verts[6*k + 4] = verts[3*newk + 1];
			xx_verts[6*k + 5] = verts[3*newk + 2];
		}
		glVertexPointer(3, GL_FLOAT, 0, xx_verts);

		float src_color[4] = {0.0f, 95.0f/255.0f, 165.0f/255.0f, 1.0f};
		float dst_color[4] = {1, 1, 1, 1};

		float xx_colors[32];
		for (int k = 0; k < 8; k++)
		{
			for (int t = 0; t < 4; t++)
			{
				xx_colors[4*k + t] = k%2==1 ? src_color[t] : dst_color[t];
			}
		}

		glColorPointer(4, GL_FLOAT, 0, xx_colors);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);

	
	} glPopMatrix();

	
	glutSwapBuffers();

	// glutPostRedisplay();

}
