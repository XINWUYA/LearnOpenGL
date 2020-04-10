#version 430 core

out vec4 gl_FragColor;

uniform vec3 uObjectColor;
uniform vec3 uLightColor;

void main()
{
	gl_FragColor = vec4(uLightColor * uObjectColor, 1);
}