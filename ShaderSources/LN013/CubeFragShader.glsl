#version 430 core

out vec4 gl_FragColor;

in vec2 oTexCoord;

uniform sampler2D uTexture;
uniform sampler2D uTexture1;

void main()
{
	gl_FragColor = texture(uTexture, oTexCoord) * texture(uTexture1, oTexCoord);
}