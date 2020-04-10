#version 430 core

layout(location = 0) in vec3 Pos;

uniform mat4 u_LightSpaceMat;
uniform mat4 u_ModelMat;

out vec3 v2f_FragPos;

void main()
{
	gl_Position = u_LightSpaceMat * u_ModelMat * vec4(Pos, 1.0f);
	v2f_FragPos = vec3(u_ModelMat * vec4(Pos, 1.0f));
}