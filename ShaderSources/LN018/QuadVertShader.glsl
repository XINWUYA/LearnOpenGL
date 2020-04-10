#version 430 core

layout(location = 0) in vec2 Pos;
layout(location = 1) in vec2 Tex;

out vec2 oTexCoord;

void main()
{
	gl_Position = vec4(Pos, 0.0f, 1.0f);
	oTexCoord   = Tex;
}