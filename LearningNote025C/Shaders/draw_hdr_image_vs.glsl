#version 430 core
layout(location = 0) in vec3 i_Pos;

out vec3 o_WorldPos;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
	o_WorldPos = i_Pos;
	gl_Position = u_Projection * u_View * vec4(i_Pos, 1.0);
}