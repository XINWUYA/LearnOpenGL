#version 430 core

out vec4 FragColor;

vec3 LightColor = vec3(1.0);

void main()
{
	FragColor = vec4(LightColor, 1.0);
}