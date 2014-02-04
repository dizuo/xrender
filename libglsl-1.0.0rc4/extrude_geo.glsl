#version 120
#extension GL_EXT_geometry_shader4 : enable
void main(void)
{
	vec4 vert0 = gl_PositionIn[0];
	vec4 vert1 = gl_PositionIn[1];
	vec4 vert2 = gl_PositionIn[2];
	
	vec4 offset = vec4(-0.25, 0.25, 0.25, 0.0);
	
	vec4 vert3 = vert0 + offset;
	vec4 vert4 = vert1 + offset;
	vec4 vert5 = vert2 + offset;	

	gl_Position = vert0;
	EmitVertex();
	gl_Position = vert1;
	EmitVertex();
	gl_Position = vert2;
	EmitVertex();
	EndPrimitive();
	

	gl_Position = vert0 + offset;
	EmitVertex();
	gl_Position = vert1 + offset;
	EmitVertex();
	gl_Position = vert2 + offset;
	EmitVertex();
	EndPrimitive();

	gl_Position = vert3 + offset;
	EmitVertex();
	gl_Position = vert4 + offset;
	EmitVertex();
	gl_Position = vert5 + offset;
	EmitVertex();
	EndPrimitive();

	gl_Position = vert3;
	EmitVertex();
	gl_Position = vert1;
	EmitVertex();
	gl_Position = vert4;
	EmitVertex();
	EndPrimitive();
}
