#version 430 core

layout(location = 0) in vec3 Pos;

uniform mat4 u_LightSpaceMat;
uniform mat4 u_ModelMat;

void main()
{
	gl_Position = u_LightSpaceMat * u_ModelMat * vec4(Pos, 1.0f);
}