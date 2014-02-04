#version 120
#extension GL_EXT_geometry_shader4 : enable
void main(void)
{
	gl_Position = gl_PositionIn[0];
	EmitVertex();
	gl_Position = gl_PositionIn[1];
	EmitVertex();
	gl_Position = gl_PositionIn[2];
	EmitVertex();
	EndPrimitive();
	
	gl_Position = gl_PositionIn[0] + vec4(0.5, 0.25, 0.0, 0.0);
	EmitVertex();
	gl_Position = gl_PositionIn[1] + vec4(0.5, 0.25, 0.0, 0.0);
	EmitVertex();
	gl_Position = gl_PositionIn[2] + vec4(0.5, 0.25, 0.0, 0.0);
	EmitVertex();
	EndPrimitive();
}