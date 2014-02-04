//*****************************************************************************
// GLInfo queries data from the current OpenGL implementation.
// If an extension is unavailable the value is set to -1.
//
// (incomplete, only supports extensions that are important to me)
//
//(c) 2003-2006 by Martin Christen. All Rights reserved.
//******************************************************************************/


#ifndef C_GLINFO_H
#define C_GLINFO_H

#include "GL/glew.h"

namespace cwc
{

   class glInfo
   {
   public: 
      glInfo();
      virtual ~glInfo();

      bool Init(void);

      // OpenGL 1.0
      GLint _nMaxAttribStackDepth;         //!< the maximum supported depth of the attribute stack
      GLint _nMaxClipPlanes;               //!< the maximum number of application-defined clipping planes
      GLint _nMaxEvalOrder;                //!< the maximum equation order supported by 1D and 2D evaluators.
      GLint _nMaxLights;                   //!< the maximum number of lights. 
      GLint _nMaxListNesting;              //!< the maximum recursion depth allowed during display-list traversal.
      GLint _nMaxModelViewStackDepth;      //!< the maximum supported depth of the modelview matrix stack.
      GLint _nMaxNameStackDepth;           //!< the maximum supported depth of the selection name stack.
      GLint _nMaxPixelMapTable;            //!< the maximum supported size of a glPixelMap lookup table.
      GLint _nMaxProjectionStackDepth;     //!< the maximum supported depth of the projection matrix stack.
      GLint _nMaxTextureSize;              //!< the maximum width or height of any texture image (without borders).
      GLint _nMaxTextureStackDepth;        //!< the maximum supported depth of the texture matrix stack.
      GLint _nMaxViewPortDims[2];          //!< (two values) the maximum supported width and height of the viewport.

      // OpenGL Extensions:
      GLint _nMaxColorMatrixStackDepth;    //!< the maximum supported depth of the color matrix stack. 
      GLint _nMaxElementsIndices;          //!< the recommended maximum number of indices that can efficiently be handled by glDrawRangeElementsEXT.
      GLint _nMaxElementsVertices;         //!< the recommended maximum number of vertices that can efficiently be handled by glDrawRangeElementsEXT.
      GLint _nMax3DTextureSize;            //!< the maximum width, height, or depth of any texture image (without borders).

      // OpenGL 2.0
      GLint _nMaxCombinedTextureImageUnits;  //!< the total number of hardware units that can be used to access texture maps from the vertex processor and the fragment processor combined. The minimum legal value is 2.
      GLint _nMaxDrawBuffers;                //!< the maximum number of buffers that can be simultaneously written into from within a fragment shader using the special output variable array gl_FragData. This constant effectively defines the size of the gl_FragData array.
      GLint _nMaxFragmentUniformComponents;  //!< the number of components (i.e., floating-point values) that are available for fragment shader uniform variables. The minimum legal value is 64.
      GLint _nMaxTextureCoords;              //!< the number of texture coordinate sets that are available. The minimum legal value is 2.
      GLint _nMaxTextureImageUnits;          //!< the total number of hardware units that can be used to access texture maps from the fragment processor. The minimum legal value is 2.
      GLint _nMaxVaryingFloats;              //!< the number of floating-point variables avalable for varying variables. The minimum legal value is 32.
      GLint _nMaxVertexAttribs;              //!< the number of active vertex attributes that are available. The minimum legal value is 16.
      GLint _nMaxVertexTextureImageUnits;    //!< the number of hardware units that can be used to access texture maps from the vertex processor. The minimum legal value is 0.
      GLint _nMaxVertexUniformComponents;    //!< the number of components (i.e., floating-point values) that are available for vertex shader uniform variables. The minimum legal value is 512.

      // Shader Model 4 / Geometry Shader
      GLint _nMaxGeometryTextureImageUnits;  //!< The maximum number of texture image units available to a geometry shader
      GLint _nMaxGeometryVaryingComponents;  //!< GL_MAX_GEOMETRY_VARYING_COMPONENTS_EXT 
      GLint _nMaxVertexVaryingComponents;    //!< GL_MAX_VERTEX_VARYING_COMPONENTS_EXT 
      GLint _nMaxVaryingComponents;          //!< GL_MAX_VARYING_COMPONENTS_EXT 
      GLint _nMaxGeometryUniformComponents;  //!< GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT 
      GLint _nMaxGeomentryOutputVertices;    //!< The total number of vertices
      GLint _nMaxGeomentryTotalOutputComponents; //!< Max product of the total number of vertices and the sum of all components of all active varying variables 

   private:
      void InitVars();
   };
}

#endif