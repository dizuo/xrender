/*
SIMPLE GEOMETRY SHADER INSTANCING DEMO
by Martin Christen, christen@clockworkcoders.com
*/

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glApplication.h"
#include "glutWindow.h"
#include "glinfo.h"
#include "glsl.h"
#include <iostream>

using namespace std;

//-----------------------------------------------------------------------------
// Simple GLSL Vertex Shader:
const char vertex_prog[] = "#version 120\n"                 \
"void main(void)"                                           \
"{"                                                         \
"   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"\
"}";
//-----------------------------------------------------------------------------
// Simple GLSL Geometry Shader
const char geometry_prog[] = "#version 120\n"\
"#extension GL_EXT_geometry_shader4 : enable\n"\
"void main(void)"\
"{"\
"gl_Position = gl_PositionIn[0];"\
"EmitVertex();"\
"gl_Position = gl_PositionIn[1];"\
"EmitVertex();"\
"gl_Position = gl_PositionIn[2];"\
"EmitVertex();"\
"EndPrimitive();"\
"gl_Position = gl_PositionIn[0] + vec4(0.5, 0.25, 0.0, 0.0);"\
"EmitVertex();"\
"gl_Position = gl_PositionIn[1] + vec4(0.5, 0.25, 0.0, 0.0);"\
"EmitVertex();"\
"gl_Position = gl_PositionIn[2] + vec4(0.5, 0.25, 0.0, 0.0);"\
"EmitVertex();"\
"EndPrimitive();"\
"}";
// Simple GLSL Fragment Shader:
const char fragment_prog[] = "#version 120\n"\
"void main (void)"\
"{"\
" gl_FragColor = vec4 (0.0, 1.0, 0.0, 1.0);"\
"}";
//-----------------------------------------------------------------------------

class myApplication : public cwc::glApplication
{
public:
	virtual void OnInit() {cout << "Hello World!\n"; }
	virtual void OnExit() {cout << "Bye World\n"; }
};


class myWindow : public cwc::glutWindow
{
protected:
	cwc::glShaderManager   shader_manager;
	cwc::glShader*         shader;
	int _mx, _my;
	int _w, _h;
	bool bGeometryShader;

public:
	myWindow()
	{
		bGeometryShader = false;
		//pTexture = 0;
		_mx = 0;
		_my = 0;
		_w = 256;
		_h = 256;
	}

	virtual void OnRender(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (shader) shader->begin();

		// glutSolidTeapot(0.25);
		float size = 0.25f;
		float vertices[] = {0.0f, 0.0f, 0.0f, size, 0.0f, 0.0f, size, size, 0.0f};
//#define INSTANCE_MODE

#ifdef INSTANCE_MODE
		glBegin(GL_TRIANGLES); 
		for (int i = 0; i<3; i++)
		{
			glVertex3fv(vertices + 3*i);
		}
		glEnd();
#else
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glPushMatrix();
		glTranslatef(0.0f, -0.25f, 0.0f);
		glVertexPointer(3, GL_FLOAT, 0, vertices);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glPopMatrix();

		//glDisableClientState(GL_VERTEX_ARRAY);
#endif // INSTANCE_MODE


		if (shader) shader->end();

		glutSwapBuffers();
	}

	virtual void OnIdle()
	{
		//cwc::psleep(1);
	}

	virtual void OnInit()
	{
		//SetFullscreen(true);
		cwc::glInfo   oGLInfo;
		oGLInfo.Init();

		if (cwc::HasShaderModel4())
		{
			std::cout << "Shader Model 4 is supported!\n";
		}

		bGeometryShader = cwc::HasGeometryShaderSupport();

		if (bGeometryShader)
		{
			std::cout << "Geometry Shader is supported!!\n";
		}

		bool bOk = cwc::HasGLSLSupport();   // This MUST be called!!
		shader = 0;

		if (bOk)
		{
			shader_manager.SetInputPrimitiveType(GL_TRIANGLES);
			shader_manager.SetOutputPrimitiveType(GL_TRIANGLE_STRIP);
			shader_manager.SetVerticesOut(6);

			// shader = shader_manager.loadfromMemory(vertex_prog, geometry_prog, fragment_prog);
			shader = shader_manager.loadfromFile("extrude_vert.glsl", "extrude_geo.glsl", "extrude_frag.glsl");
		}

		glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);

	}

	virtual void OnResize(int w, int h)
	{
	}

	virtual void OnClose(void)
	{
	}

	virtual void OnMouseDown(int button, int x, int y)
	{
		Repaint();
	}    

	virtual void OnMouseUp(int button, int x, int y)
	{
	}

	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y)
	{ 
		Repaint();
	}

	virtual void OnKeyDown(int nKey, char cAscii)
	{       
		if (cAscii == 27) // 0x1b = ESC
		{
			Close(); // Close Window!
		} 
	};

	virtual void OnKeyUp(int nKey, char cAscii)
	{
		if (cAscii == 'f')
		{
			SetFullscreen(true);
		}
		else if (cAscii == 'w')
		{
			SetFullscreen(false);
		}     
	};
};

//-----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
#ifdef FREEIMAGE_LIB
	FreeImage_Initialise(); // only call if freeimage is linked statically
#endif

	myApplication*  pApp = new myApplication;
	myWindow* myWin = new myWindow();

	pApp->run();
	delete pApp;		// dead code!

#ifdef FREEIMAGE_LIB 
	FreeImage_DeInitialise(); // only call if freeimage is linked statically
#endif   
	return 0; 
}

//-----------------------------------------------------------------------------

