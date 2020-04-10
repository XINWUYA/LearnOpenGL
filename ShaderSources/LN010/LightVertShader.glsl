#version 430 core

layout(location = 0) in vec3 Pos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(Pos, 1.0f);
}