#version 440 core

layout(location = 0) in vec3 Pos;
layout(location = 1) in vec2 Tex;

out vec2 oTexCoord;
out vec3 oPos;

layout(std140) uniform Matrices
{
	mat4 uProjection;
	mat4 uView;
};
uniform mat4 uModel;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(Pos, 1.0f);
	oTexCoord   = Tex;
	oPos = Pos;
}