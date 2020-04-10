#version 430 core

out vec4 gl_FragColor;

uniform vec3 uObjectColor;

void main()
{
	gl_FragColor = vec4(uObjectColor, 1);
}