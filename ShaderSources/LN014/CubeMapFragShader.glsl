#version 430 core

out vec4 gl_FragColor;

in vec3 oTexCoord;

uniform samplerCube uCubeMap;

void main()
{
	vec4 Result = texture(uCubeMap, oTexCoord);
	gl_FragColor = Result;
}