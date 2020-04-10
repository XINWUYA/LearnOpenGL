#version 430 core

uniform vec3 u_LightPos;
uniform float u_FarPlane;

in vec3 v2f_FragPos;

void main()
{
	float LightDistance = length(v2f_FragPos - u_LightPos);
	gl_FragDepth = LightDistance / u_FarPlane;
}