#version 430 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec2 i_TexCoords;

out vec2 o_TexCoords;

void main()
{
	o_TexCoords = i_TexCoords;
	gl_Position = vec4(i_Pos, 1.0f);
}