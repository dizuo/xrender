//*****************************************************************************
// Some Utilities related to OpenGL (or not)
//
//(c) 2003-2006 by Martin Christen. All Rights reserved.
//******************************************************************************/

#include "glutils.h"
#include "GL/glew.h"

#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#endif

#include <cmath>

using namespace cwc;

namespace cwc
{

   void drawCube(float size)
   {
      // vorne
      glBegin(GL_QUADS); 
      glTexCoord2f(0.0f, 1.0f);  glVertex3f(-1.0f*size, -1.0f*size, +1.0f*size);
      glTexCoord2f(1.0f, 1.0f);  glVertex3f(+1.0f*size, -1.0f*size, +1.0f*size);
      glTexCoord2f(1.0f, 0.0f);  glVertex3f(+1.0f*size, +1.0f*size, +1.0f*size);
      glTexCoord2f(0.0f, 0.0f);  glVertex3f(-1.0f*size, +1.0f*size, +1.0f*size);         
      glEnd();

      //rechts
      glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 1.0f);  glVertex3f(+1.0f*size, -1.0f*size, +1.0f*size);
      glTexCoord2f(1.0f, 1.0f);  glVertex3f(+1.0f*size, -1.0f*size, -1.0f*size); 
      glTexCoord2f(1.0f, 0.0f);  glVertex3f(+1.0f*size, +1.0f*size, -1.0f*size);
      glTexCoord2f(0.0f, 0.0f);  glVertex3f(+1.0f*size, +1.0f*size, +1.0f*size);         
      glEnd();

      //links
      glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 1.0f);  glVertex3f(-1.0f*size, +1.0f*size, +1.0f*size);
      glTexCoord2f(1.0f, 1.0f);  glVertex3f(-1.0f*size, +1.0f*size, -1.0f*size);
      glTexCoord2f(1.0f, 0.0f);  glVertex3f(-1.0f*size, -1.0f*size, -1.0f*size);
      glTexCoord2f(0.0f, 0.0f);  glVertex3f(-1.0f*size, -1.0f*size, +1.0f*size);       
      glEnd();

      //oben
      glBegin(GL_QUADS); 
      glTexCoord2f(0.0f, 1.0f);  glVertex3f(-1.0f*size, +1.0f*size, +1.0f*size); 
      glTexCoord2f(1.0f, 1.0f);  glVertex3f(+1.0f*size, +1.0f*size, +1.0f*size);
      glTexCoord2f(1.0f, 0.0f);  glVertex3f(+1.0f*size, +1.0f*size, -1.0f*size);
      glTexCoord2f(0.0f, 0.0f);  glVertex3f(-1.0f*size, +1.0f*size, -1.0f*size);          
      glEnd();

      //unten
      glBegin(GL_QUADS); 
      glTexCoord2f(0.0f, 0.0f);  glVertex3f(-1.0f*size, -1.0f*size, -1.0f*size); 
      glTexCoord2f(1.0f, 0.0f);  glVertex3f(+1.0f*size, -1.0f*size, -1.0f*size);
      glTexCoord2f(1.0f, 1.0f);  glVertex3f(+1.0f*size, -1.0f*size, +1.0f*size);
      glTexCoord2f(0.0f, 1.0f);  glVertex3f(-1.0f*size, -1.0f*size, +1.0f*size);       
      glEnd();

      //hinten
      glBegin(GL_QUADS); 
      glTexCoord2f(0.0f, 0.0f);  glVertex3f(-1.0f*size, +1.0f*size, -1.0f*size);
      glTexCoord2f(1.0f, 0.0f);  glVertex3f(+1.0f*size, +1.0f*size, -1.0f*size);
      glTexCoord2f(1.0f, 1.0f);  glVertex3f(+1.0f*size, -1.0f*size, -1.0f*size);
      glTexCoord2f(0.0f, 1.0f);  glVertex3f(-1.0f*size, -1.0f*size, -1.0f*size);    
      glEnd();   
   }

   //-----------

   void drawCubeNN(float size, float w, float h)
   {
      // vorne
      glBegin(GL_QUADS); 
      glTexCoord2f(0.0f, h);  glVertex3f(-1.0f*size, -1.0f*size, +1.0f*size);
      glTexCoord2f(w, h);  glVertex3f(+1.0f*size, -1.0f*size, +1.0f*size);
      glTexCoord2f(w, 0.0f);  glVertex3f(+1.0f*size, +1.0f*size, +1.0f*size);
      glTexCoord2f(0.0f, 0.0f);  glVertex3f(-1.0f*size, +1.0f*size, +1.0f*size);         
      glEnd();

      //rechts
      glBegin(GL_QUADS);
      glTexCoord2f(0.0f, h);  glVertex3f(+1.0f*size, -1.0f*size, +1.0f*size);
      glTexCoord2f(w, h);  glVertex3f(+1.0f*size, -1.0f*size, -1.0f*size); 
      glTexCoord2f(w, 0.0f);  glVertex3f(+1.0f*size, +1.0f*size, -1.0f*size);
      glTexCoord2f(0.0f, 0.0f);  glVertex3f(+1.0f*size, +1.0f*size, +1.0f*size);         
      glEnd();

      //links
      glBegin(GL_QUADS);
      glTexCoord2f(0.0f, h);  glVertex3f(-1.0f*size, +1.0f*size, +1.0f*size);
      glTexCoord2f(w, h);  glVertex3f(-1.0f*size, +1.0f*size, -1.0f*size);
      glTexCoord2f(w, 0.0f);  glVertex3f(-1.0f*size, -1.0f*size, -1.0f*size);
      glTexCoord2f(0.0f, 0.0f);  glVertex3f(-1.0f*size, -1.0f*size, +1.0f*size);       
      glEnd();

      //oben
      glBegin(GL_QUADS); 
      glTexCoord2f(0.0f, h);  glVertex3f(-1.0f*size, +1.0f*size, +1.0f*size); 
      glTexCoord2f(w, h);  glVertex3f(+1.0f*size, +1.0f*size, +1.0f*size);
      glTexCoord2f(w, 0.0f);  glVertex3f(+1.0f*size, +1.0f*size, -1.0f*size);
      glTexCoord2f(0.0f, 0.0f);  glVertex3f(-1.0f*size, +1.0f*size, -1.0f*size);          
      glEnd();

      //unten
      glBegin(GL_QUADS); 
      glTexCoord2f(0.0f, 0.0f);  glVertex3f(-1.0f*size, -1.0f*size, -1.0f*size); 
      glTexCoord2f(w, 0.0f);  glVertex3f(+1.0f*size, -1.0f*size, -1.0f*size);
      glTexCoord2f(w, h);  glVertex3f(+1.0f*size, -1.0f*size, +1.0f*size);
      glTexCoord2f(0.0f, h);  glVertex3f(-1.0f*size, -1.0f*size, +1.0f*size);       
      glEnd();

      //hinten
      glBegin(GL_QUADS); 
      glTexCoord2f(0.0f, 0.0f);  glVertex3f(-1.0f*size, +1.0f*size, -1.0f*size);
      glTexCoord2f(w, 0.0f);  glVertex3f(+1.0f*size, +1.0f*size, -1.0f*size);
      glTexCoord2f(w, h);  glVertex3f(+1.0f*size, -1.0f*size, -1.0f*size);
      glTexCoord2f(0.0f, h);  glVertex3f(-1.0f*size, -1.0f*size, -1.0f*size);    
      glEnd();
   }

   //-----------------------------------------------------------------------------
   // Random Numbers
   //-----------------------------------------------------------------------------
   void fseed(long k)
   {
      srand(k);
   }
   float frand(void)
   {
      return (float)rand()/(float)RAND_MAX;
   }

   float frand01(void)
   {
      return (float)(::abs(rand()))/(float)RAND_MAX;
   }
   //-----------------------------------------------------------------------------
   // Timer Utils
   //-----------------------------------------------------------------------------
   double Wtime(void)
   {
   #ifdef _WIN32
      LONGLONG tt;
      QueryPerformanceCounter((LARGE_INTEGER*)&tt);

      return double(tt);
   #else
      // if you dont compile under windows:
      return (double) clock();  // bugfix Oct-06-2006
   #endif
   }
   //-----------------------------------------------------------------------------
   double Wclocks(void)
   {
   #ifdef _WIN32
      LONGLONG frequency;
      QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

      return double(frequency);
   #else
      //if you dont compile under windows:
      return (double)CLOCKS_PER_SEC;
   #endif
   }
   //-----------------------------------------------------------------------------
   bool checkOpenGLExtension(char* extension)
   {
      return (GL_TRUE == glewGetExtension(extension));
   }
   //-----------------------------------------------------------------------------

} // namespace

