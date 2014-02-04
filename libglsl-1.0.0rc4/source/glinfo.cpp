//*****************************************************************************
// GLInfo queries data from the current OpenGL implementation.
// If an extension is unavailable the value is set to -1.
//
// (incomplete, only supports extensions that are important to me)
//
//(c) 2003-2006 by Martin Christen. All Rights reserved.
//******************************************************************************/

#include "glInfo.h"
#include "GL/glew.h"
#include <iostream>

using namespace cwc;

//-----------------------------------------------------------------------------

glInfo::glInfo()
{
   InitVars();
}

//-----------------------------------------------------------------------------

glInfo::~glInfo()
{
}

//-----------------------------------------------------------------------------

bool glInfo::Init(void)
{
   InitVars();

   GLenum err = glewInit();

   if (GLEW_OK != err)
   {
      return false;
   }

   std::cout << "OpenGL Vendor: " << (char*) glGetString(GL_VENDOR) << "\n";
   std::cout << "OpenGL Renderer: " << (char*) glGetString(GL_RENDERER) << "\n";
   std::cout << "OpenGL Version: " << (char*) glGetString(GL_VERSION) << "\n";
   
   char* sShadingLanguage = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
   
   if (sShadingLanguage)
   {
      std::cout << "GLSL Version: " << sShadingLanguage << "\n\n";
   }
   
   // OpenGL 1.0

   glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &_nMaxAttribStackDepth);         
   glGetIntegerv(GL_MAX_CLIP_PLANES, &_nMaxClipPlanes);              
   glGetIntegerv(GL_MAX_EVAL_ORDER, &_nMaxEvalOrder);                
   glGetIntegerv(GL_MAX_LIGHTS, &_nMaxLights);                   
   glGetIntegerv(GL_MAX_LIST_NESTING, &_nMaxListNesting);              
   glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &_nMaxModelViewStackDepth);      
   glGetIntegerv(GL_MAX_NAME_STACK_DEPTH, &_nMaxNameStackDepth);           
   glGetIntegerv(GL_MAX_PIXEL_MAP_TABLE, &_nMaxPixelMapTable);            
   glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &_nMaxProjectionStackDepth);     
   glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_nMaxTextureSize);              
   glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, &_nMaxTextureStackDepth);        
   glGetIntegerv(GL_MAX_VIEWPORT_DIMS, _nMaxViewPortDims);         

   if (glewGetExtension("GL_EXT_color_matrix"))
   {
      glGetIntegerv(GL_MAX_COLOR_MATRIX_STACK_DEPTH,&_nMaxColorMatrixStackDepth);
   }
   
   if (glewGetExtension("GL_EXT_draw_range_elements"))
   {
      glGetIntegerv(GL_MAX_ELEMENTS_VERTICES,&_nMaxElementsVertices);
      glGetIntegerv(GL_MAX_ELEMENTS_INDICES,&_nMaxElementsIndices);
   }
   
   if (glewGetExtension("GL_EXT_texture3D"))
   {
      glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &_nMax3DTextureSize);
   }
   
   //************
   //OpenGL 1.2
   //************
   
   //************
   //OpenGL 1.3
   //************
   
   //************
   //OpenGL 1.4  
   //************ 
   
   //************
   // OpenGL 1.5
   //************
   
   //************
   // OpenGL 2.0
   //************   
   
   if (GLEW_VERSION_2_0)
   {
      glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,&_nMaxCombinedTextureImageUnits);    // the total number of hardware units that can be used to access texture maps from the vertex processor and the fragment processor combined. The minimum legal value is 2.
      glGetIntegerv(GL_MAX_DRAW_BUFFERS,&_nMaxDrawBuffers);                                  // the maximum number of buffers that can be simultaneously written into from within a fragment shader using the special output variable array gl_FragData. This constant effectively defines the size of the gl_FragData array.
      glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,&_nMaxFragmentUniformComponents);     // the number of components (i.e., floating-point values) that are available for fragment shader uniform variables. The minimum legal value is 64.
      glGetIntegerv(GL_MAX_TEXTURE_COORDS,&_nMaxTextureCoords);                              // the number of texture coordinate sets that are available. The minimum legal value is 2.
      glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS,&_nMaxTextureImageUnits);                     // the total number of hardware units that can be used to access texture maps from the fragment processor. The minimum legal value is 2.
      glGetIntegerv(GL_MAX_VARYING_FLOATS,&_nMaxVaryingFloats);                              // the number of floating-point variables avalable for varying variables. The minimum legal value is 32.
      glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&_nMaxVertexAttribs);                              // the number of active vertex attributes that are available. The minimum legal value is 16.
      glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&_nMaxVertexTextureImageUnits);        // the number of hardware units that can be used to access texture maps from the vertex processor. The minimum legal value is 0.
      glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS,&_nMaxVertexUniformComponents);         // the number of components (i.e., floating-point values) that are available for vertex shader uniform variables. The minimum legal value is 512.
   }

   //**************************************
   //** Shader Model 4 / Geometry Shader **
   //**************************************

   if (glewGetExtension("GL_EXT_geometry_shader4"))
   {
      glGetIntegerv(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT,&_nMaxGeometryTextureImageUnits); 
      glGetIntegerv(GL_MAX_GEOMETRY_VARYING_COMPONENTS_EXT,&_nMaxGeometryVaryingComponents);
      glGetIntegerv(GL_MAX_VERTEX_VARYING_COMPONENTS_EXT,&_nMaxVertexVaryingComponents);
      glGetIntegerv(GL_MAX_VARYING_COMPONENTS_EXT,&_nMaxVaryingComponents);
      glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT,&_nMaxGeometryUniformComponents); 
      glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&_nMaxGeomentryOutputVertices);
      glGetIntegerv(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_EXT,&_nMaxGeomentryTotalOutputComponents); 
   }

   if (glewGetExtension("GL_EXT_gpu_shader4"))
   {
   }
   
   return true;

}

//-----------------------------------------------------------------------------

void glInfo::InitVars()
{
   _nMaxAttribStackDepth = -1;         
   _nMaxClipPlanes = -1;               
   _nMaxEvalOrder = -1;                
   _nMaxLights = -1;                   
   _nMaxListNesting = -1;              
   _nMaxModelViewStackDepth = -1;      
   _nMaxNameStackDepth = -1;           
   _nMaxPixelMapTable = -1;            
   _nMaxProjectionStackDepth = -1;     
   _nMaxTextureSize = -1;              
   _nMaxTextureStackDepth = -1;        
   _nMaxViewPortDims[0] = -1;   
   _nMaxViewPortDims[1] = -1;       

   _nMaxColorMatrixStackDepth = -1;    
   _nMaxElementsIndices = -1;          
   _nMaxElementsVertices = -1;         
   _nMax3DTextureSize = -1;            

   _nMaxCombinedTextureImageUnits = -1;
   _nMaxDrawBuffers = -1;              
   _nMaxFragmentUniformComponents = -1;
   _nMaxTextureCoords = -1;            
   _nMaxTextureImageUnits = -1;        
   _nMaxVaryingFloats = -1;            
   _nMaxVertexAttribs = -1;            
   _nMaxVertexTextureImageUnits = -1;  
   _nMaxVertexUniformComponents = -1;  

   _nMaxGeometryTextureImageUnits = -1;  
   _nMaxGeometryVaryingComponents = -1; 
   _nMaxVertexVaryingComponents = -1;   
   _nMaxVaryingComponents = -1;        
   _nMaxGeometryUniformComponents = -1; 
   _nMaxGeomentryOutputVertices = -1;    
   _nMaxGeomentryTotalOutputComponents = -1;









}

//-----------------------------------------------------------------------------