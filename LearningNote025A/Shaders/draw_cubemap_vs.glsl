#version 430 core
layout(location = 0) in vec3 i_Pos;

uniform mat4 u_Projection;
uniform mat4 u_View;

out vec3 o_WorldPos;

void main()
{
	o_WorldPos = i_Pos;

	vec4 ClipPos = u_Projection * mat4(mat3(u_View)) * vec4(i_Pos, 1.0);
	gl_Position = ClipPos.xyww;
}