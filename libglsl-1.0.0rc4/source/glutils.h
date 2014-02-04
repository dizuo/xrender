//*****************************************************************************
// Some Utilities related to OpenGL (or not)
//
//(c) 2003-2006 by Martin Christen. All Rights reserved.
//******************************************************************************/


#ifndef C_GLUTILS_H
#define C_GLUTILS_H

// These functions here will be removed (or replaced) one day

namespace cwc
{
   void     drawCube(float size); //!< Draws a cube. \param size Length of side
   void     drawCubeNN(float size, float w, float h); //!< Draws cube using non-normalized texture-coordinates. \param size Length of side \param w width of rectangular texture \param h height of rectangular texture

   void     fseed(long k); //!< Initializes random seed. \param k The random seed.
   float    frand01(void); //!< returns a random number in [0,1]
   float    frand(void);   //!< returns a random number in [-1,1]

   double   Wtime(void);    //!< returns current time (tick count)
   double   Wclocks(void);  //!< returns clocks per tick

   bool     checkOpenGLExtension(char* extension); //!< checks if a certain OpenGL extension is available. \param Extension name of the OpenGL extension to be checked.
}


#endif