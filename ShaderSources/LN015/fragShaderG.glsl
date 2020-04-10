#version 440 core

out vec4 gl_FragColor;

in vec2 oTexCoord;
in vec3 oPos;
uniform sampler2D uTexture;

void main()
{
	gl_FragColor = texture(uTexture, oTexCoord) * vec4(0.0f, 1.0f, 0.0f, 1.0f);

	//gl_FragColor = vec4(1, 0, 0, 1);
}