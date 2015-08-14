//-----------------------------------------------------------------------------
//           Name: ogl_alpha_blending_texture.cpp
//         Author: Kevin Harris
//  Last Modified: 03/25/05
//    Description: This sample demonstrates how to perform alpha blending using
//                 the alpha channel of a standard .tga texture. For proper
//                 alpha blending, the sample uses a cull-mode sorting trick
//                 to ensure the sides of the textured cube get rendered in
//                 back-to-front order.
//
//   Control Keys: b - Toggle blending
//                 s - Toggle usage of cull-mode sorting trick
//                 Up Arrow - Move the test cube closer
//                 Down Arrow - Move the test cube away
//-----------------------------------------------------------------------------

#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "tga.h"
#include "resource.h"

#include "soil/soil.h"

//-----------------------------------------------------------------------------
// GLOBALS
//-----------------------------------------------------------------------------
HWND   g_hWnd      = NULL;
HDC	   g_hDC       = NULL;
HGLRC  g_hRC       = NULL;
GLuint g_textureID = -1;

bool g_bBlending = true;

float g_fDistance = -4.5f;
float g_fSpinX    = 0.0f;
float g_fSpinY    = 0.0f;

struct Vertex
{
	float tu, tv;
	float x, y, z;
};

int g_vertexCount = 4;
float g_quadWid = 0.4f;
float g_quadHgh = 2.0f;
float g_arrowSpeed = 0.01f;

float g_texSize = 64.0f;
float g_texUOff = 34.0f / g_texSize;
float g_texVOff = 1 - 49.0f / g_texSize;

Vertex g_quadVertices[] =
{
	{ 0.0f,1.0f, -g_quadWid/2,-g_quadHgh/2, 0.0f },
	{ g_texUOff,1.0f,  g_quadWid/2,-g_quadHgh/2, 0.0f },
	{ g_texUOff, g_texVOff,  g_quadWid/2, g_quadHgh/2, 0.0f },
	{ 0.0f,g_texVOff, -g_quadWid/2, g_quadHgh/2, 0.0f }
};

//-----------------------------------------------------------------------------
// PROTOTYPES
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void loadTexture(void);
void init(void);
void render(void);
void shutDown(void);

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
int WINAPI WinMain(	HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR     lpCmdLine,
				   int       nCmdShow )
{
	WNDCLASSEX winClass; 
	MSG        uMsg;

	memset(&uMsg,0,sizeof(uMsg));

	winClass.lpszClassName = "MY_WINDOWS_CLASS";
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = hInstance;
	winClass.hIcon	       = LoadIcon(hInstance, (LPCTSTR)IDI_OPENGL_ICON);
	winClass.hIconSm	   = LoadIcon(hInstance, (LPCTSTR)IDI_OPENGL_ICON);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = NULL;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;

	if( !RegisterClassEx(&winClass) )
		return E_FAIL;

	g_hWnd = CreateWindowEx( NULL, "MY_WINDOWS_CLASS", 
		"OpenGL - Texture UV Animation",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, 640, 480, NULL, NULL, hInstance, NULL );

	if( g_hWnd == NULL )
		return E_FAIL;

	ShowWindow( g_hWnd, nCmdShow );
	UpdateWindow( g_hWnd );

	init();

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
		else
			render();
	}

	shutDown();

	UnregisterClass( "MY_WINDOWS_CLASS", winClass.hInstance );

	return uMsg.wParam;
}

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc( HWND   hWnd, 
							UINT   msg, 
							WPARAM wParam, 
							LPARAM lParam )
{
	static POINT ptLastMousePosit;
	static POINT ptCurrentMousePosit;
	static bool bMousing;

	switch( msg )
	{
	case WM_CHAR:
		{
			switch( wParam )
			{
			case 'b':
			case 'B':
				g_bBlending = !g_bBlending;
				break;

			case 's':
			case 'S':
				break;
			}
		}
		break;

	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;

			case 38: // Up Arrow Key
				g_fDistance -= 0.1f;
				break;

			case 40: // Down Arrow Key
				g_fDistance += 0.1f;
				break;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			ptLastMousePosit.x = ptCurrentMousePosit.x = LOWORD (lParam);
			ptLastMousePosit.y = ptCurrentMousePosit.y = HIWORD (lParam);
			bMousing = true;
		}
		break;

	case WM_LBUTTONUP:
		{
			bMousing = false;
		}
		break;

	case WM_MOUSEMOVE:
		{
			ptCurrentMousePosit.x = LOWORD (lParam);
			ptCurrentMousePosit.y = HIWORD (lParam);

			if( bMousing )
			{
				g_fSpinX -= (ptCurrentMousePosit.x - ptLastMousePosit.x);
				g_fSpinY -= (ptCurrentMousePosit.y - ptLastMousePosit.y);
			}

			ptLastMousePosit.x = ptCurrentMousePosit.x;
			ptLastMousePosit.y = ptCurrentMousePosit.y;
		}
		break;

	case WM_SIZE:
		{
			int nWidth  = LOWORD(lParam); 
			int nHeight = HIWORD(lParam);
			glViewport(0, 0, nWidth, nHeight);

			glMatrixMode( GL_PROJECTION );
			glLoadIdentity();
			gluPerspective( 45.0, (GLdouble)nWidth / (GLdouble)nHeight, 0.1, 100.0);
		}
		break;

	case WM_CLOSE:
		{
			PostQuitMessage(0);	
		}
		break;

	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;

	default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
		break;
	}

	return 0;
}

//-----------------------------------------------------------------------------
// Name: loadTexture()
// Desc: 
//-----------------------------------------------------------------------------
void loadTexture( void )	
{
	unsigned char* buffer = NULL;
	int width = 0;
	int height = 0;
	int channel = 0;
	buffer = SOIL_load_image("road.png", &width, &height, &channel, 4);

	// tgaImageFile tgaImage;
	// tgaImage.load("arrow.tga");

	glGenTextures( 1, &g_textureID );

	glBindTexture( GL_TEXTURE_2D, g_textureID );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	
	free(buffer);

// 	glTexImage2D( GL_TEXTURE_2D, 0, tgaImage.m_texFormat, 
// 		tgaImage.m_nImageWidth, tgaImage.m_nImageHeight, 
// 		0, tgaImage.m_texFormat, GL_UNSIGNED_BYTE, 
// 		tgaImage.m_nImageData );
}

//-----------------------------------------------------------------------------
// Name: init()
// Desc: 
//-----------------------------------------------------------------------------
void init( void )
{
	GLuint PixelFormat;

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion   = 1;
	pfd.dwFlags    = PFD_DRAW_TO_WINDOW |PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 16;
	pfd.cDepthBits = 16;

	g_hDC = GetDC( g_hWnd );
	PixelFormat = ChoosePixelFormat( g_hDC, &pfd );
	SetPixelFormat( g_hDC, PixelFormat, &pfd);
	g_hRC = wglCreateContext( g_hDC );
	wglMakeCurrent( g_hDC, g_hRC );

	loadTexture();

	glClearColor( 0.35f, 0.53f, 0.7f, 1.0f );
	glEnable( GL_TEXTURE_2D );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 45.0f, 640.0f / 480.0f, 0.1f, 100.0f);
}

//-----------------------------------------------------------------------------
// Name: shutDown()
// Desc: 
//-----------------------------------------------------------------------------
void shutDown( void )
{
	glDeleteTextures( 1, &g_textureID );

	if( g_hRC != NULL )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( g_hRC );
		g_hRC = NULL;
	}

	if( g_hDC != NULL )
	{
		ReleaseDC( g_hWnd, g_hDC );
		g_hDC = NULL;
	}
}

//-----------------------------------------------------------------------------
// Name: render()
// Desc: 
//-----------------------------------------------------------------------------
void render( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, g_fDistance );
	glRotatef( -g_fSpinY, 1.0f, 0.0f, 0.0f );
	glRotatef( -g_fSpinX, 0.0f, 1.0f, 0.0f );

	if( g_bBlending == true )
	{
		//
		// Use the texture's alpha channel to blend it with whatever抯 already 
		// in the frame-buffer.
		//

		glDisable( GL_DEPTH_TEST );

		glEnable( GL_BLEND );
		
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );	 // 如果alpha值有效
		
		// glBlendFunc( GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR );	// 使用图像RGB抠图

		glBindTexture( GL_TEXTURE_2D, g_textureID );
	
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);	// 实际混合为淡红色值

		glInterleavedArrays( GL_T2F_V3F, 0, g_quadVertices );
		glDrawArrays( GL_QUADS, 0, g_vertexCount );
	}
	else
	{
		//
		// Render the cube, but do no blending...
		//

		glDisable( GL_BLEND );
		glEnable( GL_DEPTH_TEST );

		glBindTexture( GL_TEXTURE_2D, g_textureID );
		glInterleavedArrays( GL_T2F_V3F, 0, g_quadVertices );
		glDrawArrays( GL_QUADS, 0, g_vertexCount );
	}

	SwapBuffers( g_hDC );
}
