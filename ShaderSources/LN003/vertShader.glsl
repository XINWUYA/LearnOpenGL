#version 400 core

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec2 Tex;

out vec2 oTexCoord;
out vec3 oPos;

void main()
{
	gl_Position = vec4(Pos, 1.0f);
	oTexCoord   = Tex;
	oPos = Pos;
}