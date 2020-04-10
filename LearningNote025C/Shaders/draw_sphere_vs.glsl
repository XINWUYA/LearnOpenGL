#version 430 core
layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec2 i_TexCoords;
layout(location = 2) in vec3 i_Normal;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

out vec3 o_WorldPos;
out vec2 o_TexCoords;
out vec3 o_Normal;

void main()
{
	o_WorldPos = vec3(u_Model * vec4(i_Pos, 1.0f));
	o_TexCoords = i_TexCoords;
	o_Normal = mat3(u_Model) * i_Normal;

	gl_Position = u_Projection * u_View * vec4(o_WorldPos, 1.0);
}