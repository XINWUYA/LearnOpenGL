#version 430 core

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

out vec2 oTexCoord;

void main()
{
	gl_Position = vec4(Pos, 1.0f);
	oTexCoord   = TexCoords;
}