#version 430 core

out vec4 gl_FragColor;

in vec2 oTexCoord;
in vec3 oNormal;
in vec3 oFragPos;

uniform sampler2D uTexture;

void main()
{
	vec4 Result = texture(uTexture, oTexCoord);
	gl_FragColor = Result;
}