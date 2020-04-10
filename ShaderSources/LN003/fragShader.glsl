#version 400 core

out vec4 gl_FragColor;

in vec2 oTexCoord;
in vec3 oPos;
uniform sampler2D sTexture1;
uniform sampler2D sTexture2;

void main()
{
	if(abs(oPos.x) < 0.5f && abs(oPos.y) < 0.5f)
		gl_FragColor = texture(sTexture2, oTexCoord)*texture(sTexture1, oTexCoord);	
	else
		gl_FragColor = texture(sTexture1, oTexCoord);

	//gl_FragColor = vec4(1, 0, 0, 1);
}